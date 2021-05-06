main.o: main.c lapcounter.c lapcounter.h geometry.c geometry.h
	@echo "Compiling"
	@gcc main.c lapcounter.c geometry.c -o main.o -lm -O3
	@echo "Compiled"