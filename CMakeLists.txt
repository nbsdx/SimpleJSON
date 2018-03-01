cmake_minimum_required(VERSION 2.8)

project(SimpleJson)

add_library(SimpleJSON json.cpp)

foreach (example
        array
        init
        iter
        json
        load
        prim
        )
    add_executable(SimpleJson_${example}_example examples/${example}_example.cpp)
    target_link_libraries(SimpleJson_${example}_example SimpleJSON)
endforeach()
