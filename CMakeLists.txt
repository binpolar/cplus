# The submodule's files are already there, we're just adding build rules
add_library(cplus_wrapper STATIC
    src/cplus_hashmap.c
    src/cplus_heap.c
    src/cplus_utils.c
)

target_include_directories(cplus_wrapper PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Private includes for compilation only
target_include_directories(cplus_wrapper PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)