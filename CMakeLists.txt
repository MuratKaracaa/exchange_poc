cmake_minimum_required(VERSION 3.15)
project(exchange_poc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    QuickFIX
    GIT_REPOSITORY https://github.com/quickfix/quickfix.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(QuickFIX)

FetchContent_Declare(
    ConcurrentQueue
    GIT_REPOSITORY https://github.com/cameron314/concurrentqueue.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(ConcurrentQueue)

add_executable(exchange_poc src/main.cpp)

# Adjusted include paths after inspecting the actual structure
target_include_directories(exchange_poc
    PRIVATE
        ${quickfix_SOURCE_DIR}/src/C++
        ${quickfix_BINARY_DIR}/src/C++
        ${concurrentqueue_SOURCE_DIR}
)

# Link against the QuickFIX library
target_link_libraries(exchange_poc
    PRIVATE
        quickfix
)