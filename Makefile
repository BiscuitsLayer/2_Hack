all:	b l r

b:
	g++ -c -ggdb3 main.cpp
l:
	g++ main.o -o main -lsfml-graphics -lsfml-window -lsfml-system
r:
	./main