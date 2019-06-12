project(libbz2)

add_library(libbz2 STATIC
    src/blocksort.c
    src/bzlib.c
    src/compress.c
    src/crctable.c
    src/decompress.c
    src/huffman.c
    src/randtable.c
)
