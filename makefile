
db: db.c list.o tree.o utils.o
	gcc db.c list.o tree.o utils.o -o db -g

tester: tester.c list.o tree.o
	gcc tester.c list.o tree.o -o tester -g

utils_db.o: utils.o tree.o list.o utils_db.c utils_db.h
	gcc utils.o tree.o list.o db_utils.c -c -g

utils.o: utils.c utils.h
	gcc utils.c -Wall -c -g

tree.o: tree.c tree.h
	gcc tree.c -Wall -c -g

list.o: list.c list.h
	gcc list.c -Wall -c -g

run: db
	./db

debug: db
	gdb db

valgrind: db
	valgrind --leak-check=full ./db

runtester: tester
	./tester

debugtester: tester
	gdb tester

valgrindtester: tester
	valgrind --leak-check=full ./tester

clean:
	rm common.h.gch list.o tree.o utils.o db_utils.o a.out tester utils.h.gch db_utils.h.gch list.h.gch tree.h.gch db_utils.c~ db_utils.h~ -f

.PHONY: run debug clean valgrind runtester debugtester valgrindtester
