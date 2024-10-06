build:
	gcc main.c pokerlib.c -o main.exe

run: build
	./main.exe