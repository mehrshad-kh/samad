default:
	gcc src/main.c -o obj/main.o -c
	gcc src/shared.c -o obj/shared.o -c
	gcc src/admin.c -o obj/admin.o -c
	gcc src/student.c -o obj/student.o -c
	gcc src/callback.c -o obj/callback.o -c
	gcc src/utility.c -o obj/utility.o -c
	gcc src/types.c -o obj/types.o -c
	gcc src/mklib/mklib.c -o obj/mklib.o -c
	gcc obj/main.o obj/shared.o obj/admin.o obj/student.o obj/callback.o \
	obj/utility.o obj/types.o obj/mklib.o obj/sqlite3.o -o main
	./main
