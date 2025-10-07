file(GLOB_RECURSE LIB_SOURCE_FILES "${PROJECT_SOURCE_DIR}/src/library/lib/*.cpp")
file(GLOB_RECURSE MAIN_SOURCE_FILES "${PROJECT_SOURCE_DIR}/src/library/main/*.cpp")
file(GLOB_RECURSE LOG_SOURCE_FILES "${PROJECT_SOURCE_DIR}/src/library/log/*.cpp")
file(GLOB_RECURSE KKM_SOURCE_FILES "${PROJECT_SOURCE_DIR}/src/library/kkm/*.cpp")
file(GLOB_RECURSE CONF_SOURCE_FILES "${PROJECT_SOURCE_DIR}/src/library/config/*.cpp")

include_directories("${PROJECT_SOURCE_DIR}/src/library" BEFORE)
