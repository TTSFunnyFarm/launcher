if (WIN32)
    project(libbz2)
else()
    project(bz2)
endif()

include_directories(include)

add_library(${CMAKE_PROJECT_NAME} STATIC
    src/blocksort.c
    src/bzlib.c
    src/compress.c
    src/crctable.c
    src/decompress.c
    src/huffman.c
    src/randtable.c
)
