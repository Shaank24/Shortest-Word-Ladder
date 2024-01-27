build:
	rm -f demo.exe
	gcc main.c -o test.exe

run: 
	./test.exe

valgrind:
	valgrind -s --tool=memcheck --leak-check=yes --track-origins=yes ./test.exe

clean:
	rm -f ./test.exe  