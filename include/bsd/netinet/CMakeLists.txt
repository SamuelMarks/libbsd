get_filename_component(lib_name "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
set(LIBRARY_NAME "${PROJECT_NAME}_${lib_name}")

set(Header_Files "ip_icmp.h")
source_group("Header Files" FILES "${Header_Files}")

add_library("${LIBRARY_NAME}" INTERFACE "${Header_Files}")

include(GNUInstallDirs)
#target_include_directories(
#        "${LIBRARY_NAME}"
#        INTERFACE
#        "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include}>"
#        "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>"
#        "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>"
#        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
#)

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
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/${lib_name}")
