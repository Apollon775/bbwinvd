CC=gcc
CFLAGS=
LIBS=-L/usr/lib/mysql -lmysqlclient
INCLUDE=-I/usr/include/mysql -I./include/
OBJECT=include/query.c include/connection.c
TARGET=release/bbwinvd
SRC=src/server.c

.PHONY=all bbwinvcl

server.o: ${SRC}
	${CC} -c ${INCLUDE} ${SRC} 

include: ${OBJECT}
	${CC} -c ${INCLUDE} ${OBJECT}

bbwinvcl: server.o
	${CC} -o ${TARGET} *.o ${LIBS}

clean: bbwinvcl
	rm server.o