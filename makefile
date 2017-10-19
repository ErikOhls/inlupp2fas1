
db: db.c list.o tree.o utils.o db_aux.o
	gcc db.c list.o tree.o utils.o db_aux.o -o db -g

tester: tester.c list.o tree.o
	gcc tester.c list.o tree.o -o tester -g

ltest: test_list.c list.c
	gcc test_list.c list.c -Wall -o ltest -g -lcunit

ttest: test_tree.c tree.c
	gcc test_tree.c tree.c -Wall -o ttest -g -lcunit

db_aux.o: db_aux.c
	gcc db_aux.c -c -g

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

runltest: ltest
	./ltest

runttest: ttest
	./ttest

clean:
	rm db ltest ttest common.h.gch list.o tree.o utils.o db_aux.o a.out tester utils.h.gch db_aux.h.gch list.h.gch tree.h.gch db_aux.c~ db_aux.h~ -f

.PHONY: run debug clean valgrind runtester debugtester valgrindtester runltest runttest
