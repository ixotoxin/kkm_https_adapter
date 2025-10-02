set(LIB_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src/library/lib")
set(LIB_SOURCE_FILES "${LIB_SOURCE_DIR}/errexp.cpp" "${LIB_SOURCE_DIR}/datetime.cpp" "${LIB_SOURCE_DIR}/text.cpp")

set(MAIN_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src/library/main")
set(MAIN_SOURCE_FILES "${MAIN_SOURCE_DIR}/varop.cpp")

set(LOG_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src/library/log")
set(LOG_SOURCE_FILES "${LOG_SOURCE_DIR}/core.cpp" "${LOG_SOURCE_DIR}/varop.cpp")

set(KKM_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src/library/kkm")

set(
    KKM_SOURCE_FILES
    "${KKM_SOURCE_DIR}/connparams.cpp"
    "${KKM_SOURCE_DIR}/callparams.cpp"
    "${KKM_SOURCE_DIR}/device.cpp"
    "${KKM_SOURCE_DIR}/impex.cpp"
    "${KKM_SOURCE_DIR}/varop.cpp"
)

set(CONFIG_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src/library/config")
set(CONFIG_SOURCE_FILES "${CONFIG_SOURCE_DIR}/core.cpp" "${CONFIG_SOURCE_DIR}/varop.cpp")

include_directories("${PROJECT_SOURCE_DIR}/src/library" BEFORE)
