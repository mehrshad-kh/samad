default:
	gcc -Wall src/main.c -o obj/main.o -c
	gcc -Wall src/shared.c -o obj/shared.o -c
	gcc -Wall src/admin.c -o obj/admin.o -c
	gcc -Wall src/student.c -o obj/student.o -c
	gcc -Wall src/callback.c -o obj/callback.o -c
	gcc -Wall src/utility.c -o obj/utility.o -c
	gcc -Wall src/types.c -o obj/types.o -c
	gcc -Wall obj/main.o obj/shared.o obj/admin.o \
	obj/student.o obj/callback.o obj/utility.o obj/types.o \
	obj/mklib.o obj/leak_detector_c.o obj/sqlite3.o \
	-o samad-cli
	./samad-cli

debug:
	gcc -g -Wall src/main.c -o dbg/main-dbg.o -c
	gcc -g -Wall src/shared.c -o dbg/shared-dbg.o -c
	gcc -g -Wall src/admin.c -o dbg/admin-dbg.o -c
	gcc -g -Wall src/student.c -o dbg/student-dbg.o -c
	gcc -g -Wall src/callback.c -o dbg/callback-dbg.o -c
	gcc -g -Wall src/utility.c -o dbg/utility-dbg.o -c
	gcc -g -Wall src/types.c -o dbg/types-dbg.o -c
	gcc -g -Wall dbg/main-dbg.o dbg/shared-dbg.o dbg/admin-dbg.o \
	dbg/student-dbg.o dbg/callback-dbg.o dbg/utility-dbg.o dbg/types-dbg.o \
	dbg/mklib-dbg.o dbg/leak_detector_c_dbg.o dbg/sqlite3-dbg.o \
	-o samad-cli-dbg

