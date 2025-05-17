cmake_minimum_required(VERSION 3.28)
include(FetchContent)

set(ProjectName numberconverter)
project(${ProjectName} LANGUAGES C)

set(CMAKE_C_STANDARD 99)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB APP_SOURCE_FILES src/*.cpp src/*.hpp src/*.c src/*.h)
add_executable(${ProjectName} ${APP_SOURCE_FILES})

target_link_libraries(${ProjectName})

# Setting ASSETS_PATH
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/") # Set the asset path macro to the absolute path on the dev machine
else()
  target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="./assets/") # Set the asset path macro in release mode to a relative path that assumes the assets folder is in the same directory as the game executable
endif()