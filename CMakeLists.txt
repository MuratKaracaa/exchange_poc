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

FetchContent_Declare(
    ankerl_unordered_dense
    GIT_REPOSITORY https://github.com/martinus/unordered_dense.git
    GIT_TAG        main
)
FetchContent_MakeAvailable(ankerl_unordered_dense)

file(GLOB_RECURSE SOURCE_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(exchange_poc ${SOURCE_FILES})

target_include_directories(exchange_poc
    PRIVATE
        ${quickfix_SOURCE_DIR}/src/C++
        ${quickfix_SOURCE_DIR}/src/C++/fix42
        ${quickfix_BINARY_DIR}/src/C++
        ${concurrentqueue_SOURCE_DIR}
        ${ankerl_unordered_dense_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(exchange_poc
    PRIVATE
        quickfix
)