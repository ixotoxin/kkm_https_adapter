set(
    XTXN_PUB_DEFS
        WIN32_LEAN_AND_MEAN
        NOMINMAX
        ASIO_STANDALONE
        ASIO_NO_WIN32_LEAN_AND_MEAN
        ASIO_NO_NOMINMAX
        ASIO_HAS_THREADS
        ASIO_HAS_STD_COROUTINE
        ASIO_HAS_CO_AWAIT
        JSON_USE_IMPLICIT_CONVERSIONS=0
)

if (BUILD_SEPARATED)
    set(
        XTXN_PRI_DEFS
            EXTERNAL_LIB_VARIABLES
            EXTERNAL_MAIN_VARIABLES
            EXTERNAL_LOG_VARIABLES
            EXTERNAL_KKM_VARIABLES
            EXTERNAL_CONFIG_VARIABLES
    )
else ()
    set(XTXN_PRI_DEFS)
    include_directories("${PROJECT_SOURCE_DIR}/src/kkmop")
    include_directories("${PROJECT_SOURCE_DIR}/src/kkmjl")
endif ()
