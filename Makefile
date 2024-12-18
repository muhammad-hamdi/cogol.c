INCLUDE=-I./raylib/include
LINK=-L./raylib/lib
LIBS=-lraylib -lgdi32 -lwinmm

main:
	gcc $(INCLUDE) $(LINK) -o cogol main.c $(LIBS)