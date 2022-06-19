# --------------------------------------------------------------------------------
#                         Notice (no change needed).
# --------------------------------------------------------------------------------
# Add a make target 'notice' to generate API documentation with Doxygen.
# 'make notice' searches LICENCE in dependent library repository and generate NOTICE.
if(CMAKE_BUILD_TYPE STREQUAL "Release")
add_custom_target(notice
    rm -rf NOTICE && echo "This project uses code from the following third-party projects, listed here\\nwith the full text of their respective licenses.\\n" >> NOTICE && find ${CMAKE_CURRENT_BINARY_DIR}/_deps -name "LICENSE*" | xargs -I {} sh -c "echo '\\n--\\n' >> NOTICE && basename \$(dirname {}) | sed -e 's/\\-src//g' >> NOTICE && echo '\\n---\\n' >> NOTICE && cat {} >> NOTICE"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "${BoldMagenta}Generating NOTICE documentation.${ColourReset}" VERBATIM
)
endif()
