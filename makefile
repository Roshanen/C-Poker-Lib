build:
	gcc main.c pokerlib.c pokerstat.c utils.c -o main.exe

run: build
	./main.exe

buildtest:
	gcc test.c pokerlib.c pokerstat.c utils.c -o main.exe

test: buildtest
	./main.exe