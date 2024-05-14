cmake_minimum_required(VERSION 3.17)
project(farrago)

set(CMAKE_CXX_STANDARD 20)

add_executable(create_object
        example/create_object.cpp
        )

target_include_directories(create_object PUBLIC
        .
        ./include
        )

add_executable(create_singleton
        example/create_singleton.cpp
        )

target_include_directories(create_singleton PUBLIC
        .
        ./include
        )
