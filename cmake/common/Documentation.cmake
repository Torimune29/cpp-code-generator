# --------------------------------------------------------------------------------
#                         Documentation (no change needed).
# --------------------------------------------------------------------------------
# Add a make target 'doc' to generate API documentation with Doxygen.
# You should set options to your liking in the file 'Doxyfile.in'.
find_package(Doxygen)
if(DOXYGEN_FOUND AND CMAKE_BUILD_TYPE STREQUAL "Release")
    include(FetchContent)
    FetchContent_Populate(
      doxygen-awesome-css
      GIT_REPOSITORY https://github.com/jothepro/doxygen-awesome-css
      GIT_TAG        v2.0.3
      QUIET
      SUBBUILD_DIR ${FETCHCONTENT_BASE_DIR}/doxygen-awesome-css-subbuild
      SOURCE_DIR ${FETCHCONTENT_BASE_DIR}/doxygen-awesome-css-src
      BINARY_DIR ${FETCHCONTENT_BASE_DIR}/doxygen-awesome-css-build
    )
    file(COPY ${doxygen-awesome-css_SOURCE_DIR}/doxygen-awesome-sidebar-only.css DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})
    file(COPY ${doxygen-awesome-css_SOURCE_DIR}/doxygen-awesome.css DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "${BoldMagenta}Generating API documentation with Doxygen (open ./html/index.html to view).${ColourReset}" VERBATIM
    )
endif()

