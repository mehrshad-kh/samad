default:
	gcc src/main.c -o obj/main.o -c
	gcc src/samad-lib.c -o obj/samad-lib.o -c
	gcc src/student.c -o obj/student.o -c
	gcc src/callback.c -o obj/callback.o -c
	gcc src/utilities.c -o obj/utilities.o -c
	gcc src/types.c -o obj/types.o -c
	gcc src/mklib/mklib.c -o obj/mklib.o -c
	gcc obj/main.o obj/samad-lib.o obj/student.o obj/callback.o\
	obj/utilities.o obj/types.h obj/mklib.o obj/sqlite3.o -o main
	./main
