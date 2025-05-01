cmake_minimum_required(VERSION 3.15)
project(exchange_poc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(/usr/local/include)
link_directories(/usr/local/lib)

add_executable(exchange_poc src/main.cpp)

target_link_libraries(exchange_poc
    PRIVATE
        quickfix
        pthread
)
