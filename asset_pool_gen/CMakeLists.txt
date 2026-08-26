cmake_minimum_required(VERSION 3.5.0)

project(asset_pool_gen VERSION 0.1.0 LANGUAGES C CXX)

add_definitions(-DPLATFORM_BUILD_DESKTOP)

# Mooncake
set(MOONCAKE_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/../components/mooncake")
set(MOONCAKE_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/mooncake")

if(NOT EXISTS "${MOONCAKE_SOURCE_DIR}/CMakeLists.txt")
    message(FATAL_ERROR
        "Mooncake CMakeLists.txt was not found: ${MOONCAKE_SOURCE_DIR}/CMakeLists.txt"
    )
endif()

add_subdirectory(
    "${MOONCAKE_SOURCE_DIR}"
    "${MOONCAKE_BINARY_DIR}"
)

# App layer
file(GLOB_RECURSE APP_LAYER_SRCS
    "${CMAKE_CURRENT_LIST_DIR}/../main/utils/assets/*.c"
    "${CMAKE_CURRENT_LIST_DIR}/../main/utils/assets/*.cpp"
)

set(APP_LAYER_INCS
    "${CMAKE_CURRENT_LIST_DIR}/../main/utils/assets"
)

add_library(app_layer ${APP_LAYER_SRCS})

target_include_directories(app_layer PUBLIC
    ${APP_LAYER_INCS}
)

target_link_libraries(app_layer PUBLIC
    mooncake
)

add_executable(asset_pool_gen
    main.cpp
)

target_link_libraries(asset_pool_gen PUBLIC
    app_layer
)