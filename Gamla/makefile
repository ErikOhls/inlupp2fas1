

db: db.c db_utils.o utils.o tree.o list.o
	gcc db_utils.o utils.o tree.o list.o db.c -o db -g

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

clean:
	rm list.o tree.o utils.o db_utils.o utils.h.gch db_utils.h.gch list.h.gch tree.h.gch db_utils.c~ db_utils.h~ -f

.PHONY: run debug clean 
