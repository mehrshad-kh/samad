default:
	gcc -Wall src/main.c -o obj/main.o -c
	gcc -Wall src/shared.c -o obj/shared.o -c
	gcc -Wall src/admin.c -o obj/admin.o -c
	gcc -Wall src/student.c -o obj/student.o -c
	gcc -Wall src/callback.c -o obj/callback.o -c
	gcc -Wall src/utility.c -o obj/utility.o -c
	gcc -Wall src/types.c -o obj/types.o -c
	# gcc -Wall src/mklib/mklib.c -o obj/mklib.o -c
	gcc -Wall obj/main.o obj/shared.o obj/admin.o obj/student.o obj/callback.o \
	obj/utility.o obj/types.o obj/mklib.o obj/leak_detector_c.o obj/sqlite3.o \
	-o samad-cli
	./samad-cli
