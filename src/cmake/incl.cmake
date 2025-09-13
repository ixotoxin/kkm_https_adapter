set(EXPORT_CONFIG_INCL "${PROJECT_BINARY_DIR}/_Include_${CMAKE_BUILD_TYPE}")

configure_file("${PROJECT_SOURCE_DIR}/src/cmake/options.h.in" "${EXPORT_CONFIG_INCL}/cmake/options.h")
configure_file("${PROJECT_SOURCE_DIR}/src/cmake/variables.h.in" "${EXPORT_CONFIG_INCL}/cmake/variables.h")

include_directories("${EXPORT_CONFIG_INCL}" BEFORE)
