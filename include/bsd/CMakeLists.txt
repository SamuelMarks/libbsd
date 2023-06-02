set(LIBRARY_NAME "${PROJECT_NAME}_headers")

set(Header_Files "bitstring.h"
        "bsd.h"
        "err.h"
        "getopt.h"
        "grp.h"
        "inttypes.h"
        "libutil.h"
        "md5.h"
        "nlist.h"
        "pwd.h"
        "readpassphrase.h"
        "stdio.h"
        "stdlib.h"
        "string.h"
        "stringlist.h"
        "timeconv.h"
        "unistd.h"
        "vis.h"
        "wchar.h")
source_group("Header Files" FILES "${Header_Files}")

add_library("${LIBRARY_NAME}" INTERFACE "${Header_Files}")

include(GNUInstallDirs)
target_include_directories(
        "${LIBRARY_NAME}"
        INTERFACE
        #"$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR/include}>"
        "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
#                                  C:\Users\samue\repos\libbsd\include\bsd\err.h
message(STATUS "CMAKE_SOURCE_DIR/include = ${CMAKE_SOURCE_DIR}/include")

add_subdirectory("netinet")
add_subdirectory("sys")
target_link_libraries(
        "${LIBRARY_NAME}"
        INTERFACE
        "${PROJECT_NAME}_netinet"
        "${PROJECT_NAME}_sys"
)

set_target_properties(
        "${LIBRARY_NAME}"
        PROPERTIES
        LINKER_LANGUAGE
        C
)

#################
# Install rules #
#################

include(GNUInstallDirs)

# setup the version numbering
set_property(TARGET "${LIBRARY_NAME}" PROPERTY VERSION "${${PROJECT_NAME}_VERSION}")
set_property(TARGET "${LIBRARY_NAME}" PROPERTY SOVERSION "${${PROJECT_NAME}_VERSION_MAJOR}")

install(FILES ${Header_Files}
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}")
