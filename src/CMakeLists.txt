set(LIBRARY_NAME "${PROJECT_NAME}")

set(Header_Files
        "arc4random.h"
        "chacha_private.h"
        "local-elf.h"
        "local-link.h")
source_group("Header Files" FILES "${Header_Files}")

set(Source_Files
        "arc4random.c"
        "arc4random_uniform.c"
        "bsd_getopt.c"
        "closefrom.c"
        "dehumanize_number.c"
        "err.c"
        "expand_number.c"
        "explicit_bzero.c"
        "fgetwln.c"
        "fmtcheck.c"
        "fparseln.c"
        "freezero.c"
        "getbsize.c"
        "getentropy.c"
        "heapsort.c"
        "humanize_number.c"
        "inet_net_pton.c"
        "merge.c"
        "nlist.c"

        "progname.c"

        "radixsort.c"
        "reallocarray.c"
        "reallocf.c"
        "recallocarray.c"

        "stringlist.c"
        "strlcat.c"
        "strlcpy.c"
        "strnstr.c"
        "strtoi.c"
        "strtonum.c"
        "strtou.c"
        "timeconv.c"
        "wcslcat.c"
        "wcslcpy.c")
source_group("Source Files" FILES "${Source_Files}")

if (NOT MSVC
        AND NOT CMAKE_SYSTEM_NAME STREQUAL "Windows"
        AND NOT CMAKE_SYSTEM_NAME MATCHES "MINGW.*")
    # No one seems to have an impl of this for Windows
    list(APPEND Source_Files
            "getpeereid.c"
            "md5.c"
            "pidfile.c"
            "pwcache.c"
            "setmode.c"
            "setproctitle.c"
            "setproctitle_ctor.c"

            # These can have Windows impl but need to work on them
            "fgetln.c"          # TODO
            "flopen.c"          # TODO
            "fpurge.c"          # TODO
            "funopen.c"         # TODO
            "readpassphrase.c"  # TODO
            "unvis.c"           # TODO
            "vis.c"             # TODO
    )
    if (NOT APPLE)
        list(APPEND Source_Files "strmode.c")
    endif (NOT APPLE)
endif ()

if (MSVC OR CYGWIN OR CMAKE_SYSTEM_NAME STREQUAL "Windows"
        OR CMAKE_SYSTEM_NAME MATCHES "MINGW.*")
    list(APPEND Header_Files "arc4random_win.h")
    list(APPEND Source_Files "getentropy_win.c")
    if (NOT CYGWIN)
        list(APPEND Header_Files "${CMAKE_SOURCE_DIR}/include/bsd/win_shim/elf.h")
    endif (NOT CYGWIN)
    add_compile_definitions("-DLIBBSD_NETBSD_VIS")
elseif (LINUX)
    list(APPEND Header_Files "arc4random_linux.h")
    list(APPEND Source_Files "getentropy_linux.c")
elseif (APPLE)
    list(APPEND Source_Files "getentropy_osx.c")
else ()
    list(APPEND Header_Files "arc4random_unix.h")

    if (BSD)
        list(APPEND Source_Files "getentropy_bsd.c")
    elseif (CMAKE_SYSTEM_NAME STREQUAL "AIX")
        list(APPEND Source_Files "getentropy_aix.c")
    elseif (CMAKE_SYSTEM_NAME STREQUAL "SunOS" OR CMAKE_SYSTEM_NAME STREQUAL "Solaris")
        list(APPEND Source_Files "getentropy_solaris.c")
    elseif (CMAKE_SYSTEM_NAME STREQUAL "HP-UX")
        list(APPEND Source_Files "getentropy_hpux.c")
    else ()  # GNU Hurd
        list(APPEND Source_Files "getentropy_hurd.c")
    endif ()
endif ()

add_library("${LIBRARY_NAME}" "${Header_Files}" "${Source_Files}")

if (MINGW)
    if (DEFINED ENV{SYSTEM_BIN_DIR})
        set(SYSTEM_BIN_DIR "$ENV{SYSTEM_BIN_DIR}")
    else ()
        get_filename_component(SYSTEM_BIN_DIR "${CMAKE_C_COMPILER}" DIRECTORY)
        if (SYSTEM_BIN_DIR MATCHES ".*JetBrains.*")
            if (EXISTS "C:/usr/msys64/mingw64/bin")
                set(SYSTEM_BIN_DIR "C:/usr/msys64/mingw64/bin")
            elseif (EXISTS "C:/msys64/mingw64/bin")
                set(SYSTEM_BIN_DIR "C:/usr/msys64/mingw64/bin")
            else ()
                message(FATAL_ERROR "Unable to find real dir of MinGW")
            endif ()
        endif ()
    endif ()
    get_filename_component(SYSTEM_ROOT_DIR "${SYSTEM_BIN_DIR}" DIRECTORY)
    list(APPEND CMAKE_PREFIX_PATH "${SYSTEM_ROOT_DIR}")
endif ()

include("${CMAKE_SOURCE_DIR}/cmake/modules/FindPCRE2.cmake")
#find_package(PCRE2 CONFIG REQUIRED)

include(GNUInstallDirs)

target_include_directories(
        "${LIBRARY_NAME}"
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
        "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

set(installable_libs
        "${PROJECT_NAME}_headers"
        "${PROJECT_NAME}_netinet"
        "${PROJECT_NAME}_sys")
target_link_libraries("${LIBRARY_NAME}" "${PCRE2_LIBRARIES}" "${PROJECT_NAME}_headers" ${installable_libs})

list(APPEND installable_libs "${LIBRARY_NAME}")

if (CMAKE_SYSTEM_NAME STREQUAL "Windows"
            OR CMAKE_SYSTEM_NAME MATCHES "MINGW.*")
else ()
    target_link_libraries("${LIBRARY_NAME}" m)
endif ()
set_target_properties(
        "${LIBRARY_NAME}"
        PROPERTIES
        LINKER_LANGUAGE
        C
)

include(GenerateExportHeader)
set(_export_file "${CMAKE_BINARY_DIR}/${LIBRARY_NAME}_export.h")
generate_export_header("${LIBRARY_NAME}" EXPORT_FILE_NAME "${_export_file}")

#################
# Install rules #
#################

include(GNUInstallDirs)

# setup the version numbering
set_property(TARGET "${LIBRARY_NAME}" PROPERTY VERSION "${${PROJECT_NAME}_VERSION}")
set_property(TARGET "${LIBRARY_NAME}" PROPERTY SOVERSION "${${PROJECT_NAME}_VERSION_MAJOR}")

if (TARGET "${DEPENDANT_LIBRARY}")
    list(APPEND installable_libs "${DEPENDANT_LIBRARY}")
endif ()
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.19")
    list(APPEND installable_libs "${PROJECT_NAME}_compiler_flags")
endif ()
install(TARGETS ${installable_libs}
        EXPORT "${LIBRARY_NAME}Targets"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
install(EXPORT "${LIBRARY_NAME}Targets" DESTINATION "${CMAKE_INSTALL_DATADIR}/${LIBRARY_NAME}")

install(FILES ${Header_Files} "${_export_file}"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
