find_package(cli11 QUIET)
if(NOT cli11_FOUND)
  message(STATUS "Fetching cli11")

  include(FetchContent)
  FetchContent_Populate(
    cli11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11
    GIT_TAG        v2.2.0
    QUIET
    SUBBUILD_DIR ${FETCHCONTENT_BASE_DIR}/CLI11-subbuild
    SOURCE_DIR ${FETCHCONTENT_BASE_DIR}/CLI11-src
    BINARY_DIR ${FETCHCONTENT_BASE_DIR}/CLI11-build
 )
 # for inculde.
 target_include_directories(${APP_NAME} SYSTEM PRIVATE ${cli11_SOURCE_DIR}/include)

endif()