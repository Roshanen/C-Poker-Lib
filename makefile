build:
	gcc main.c pokerlib.c pokerstat.c utils.c -o main.exe

run: build
	./main.exe