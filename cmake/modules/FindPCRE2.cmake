# - Find pcre (Perl Compatible Regular Expressions)
# Find the native PCRE2 headers and libraries.
#
# PCRE2_INCLUDE_DIRS    - where to find pcre.h, etc.
# PCRE2_LIBRARIES   - List of libraries when using pcre.
# PCRE2_FOUND   - True if pcre found.
include(FindPackageHandleStandardArgs)
message(STATUS "my FindPCRE")
set(SEARCH_PATH_FOR_INCLUDE "")
set(SEARCH_PATH_FOR_LIBRARY "")
if (DEFINED VCPKG_INSTALLED_DIR)
	foreach (p ${VCPKG_CMAKE_FIND_ROOT_PATH})
		list(APPEND SEARCH_PATH_FOR_INCLUDE "${p}/include")
		list(APPEND SEARCH_PATH_FOR_LIBRARY "${p}/lib")
	endforeach (p ${VCPKG_CMAKE_FIND_ROOT_PATH})
endif (DEFINED VCPKG_INSTALLED_DIR)
list(APPEND SEARCH_PATH_FOR_INCLUDE
		"/usr/include"
		"/usr/local/include"
		"/opt/local/include"
		"/sw/include"
		"${CMAKE_INCLUDE_PATH}"
		"${CMAKE_INSTALL_PREFIX}/include"
)
list(APPEND SEARCH_PATH_FOR_LIBRARY
		"/usr/lib"
		"/usr/lib64"
		"/usr/local/lib"
		"/usr/local/lib64"
		"/opt/local/lib"
		"/sw/lib"
		"${CMAKE_LIBRARY_PATH}"
		"${CMAKE_INSTALL_PREFIX}/lib"
)
if(PCRE2_LIBRARIES AND PCRE2_INCLUDE_DIRS)
	# in cache already
	set(PCRE2_FOUND TRUE)
else()
	find_path(PCRE2_INCLUDE_DIR
			NAMES
			pcre2.h
			PATHS
			${SEARCH_PATHS_FOR_INCLUDE})

	# Look for the library.
	find_library(PCRE2_LIBRARY
			NAMES
			libpcre2.a
			pcre2-8
			libpcre2-posix
			PATHS
			${SEARCH_PATHS_FOR_LIBRARY})

	if(PCRE2_INCLUDE_DIR AND PCRE2_LIBRARY)
		# learn pcre2 version
		file(STRINGS ${PCRE2_INCLUDE_DIR}/pcre2.h PCRE2_VERSION_MAJOR
				REGEX "#define[ ]+PCRE2_MAJOR[ ]+[0-9]+")
		string(REGEX MATCH " [0-9]+" PCRE2_VERSION_MAJOR ${PCRE2_VERSION_MAJOR})
		string(STRIP "${PCRE2_VERSION_MAJOR}" PCRE2_VERSION_MAJOR)

		file(STRINGS ${PCRE2_INCLUDE_DIR}/pcre2.h PCRE2_VERSION_MINOR
				REGEX "#define[ ]+PCRE2_MINOR[ ]+[0-9]+")
		string(REGEX MATCH " [0-9]+" PCRE2_VERSION_MINOR ${PCRE2_VERSION_MINOR})
		string(STRIP "${PCRE2_VERSION_MINOR}" PCRE2_VERSION_MINOR)

		set(PCRE2_VERSION ${PCRE2_VERSION_MAJOR}.${PCRE2_VERSION_MINOR})
	endif()

	set(PCRE2_INCLUDE_DIRS ${PCRE2_INCLUDE_DIR})
	set(PCRE2_LIBRARIES ${PCRE2_LIBRARY})
	mark_as_advanced(PCRE2_INCLUDE_DIRS PCRE2_LIBRARIES)

	# Handle the QUIETLY and REQUIRED arguments and set PCRE2_FOUND to TRUE if all listed variables are TRUE.
	find_package_handle_standard_args(PCRE2 FOUND_VAR PCRE2_FOUND
			REQUIRED_VARS PCRE2_LIBRARY PCRE2_INCLUDE_DIR
			VERSION_VAR PCRE2_VERSION)
endif()
