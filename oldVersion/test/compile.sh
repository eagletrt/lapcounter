gcc ../src/lapcounter.c \
    ../src/utils/vector/vector.c \
    ../src/utils/vector/point/point.c \
    test.c \
    worldmap/worldmap.c \
    -lm \
    -o test.out