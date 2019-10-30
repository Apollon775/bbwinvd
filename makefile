CC=gcc
CFLAGS=
ROOT='pwd'
LIBS=`mariadb_config --libs`
INCLUDE=-I/usr/include/mariadb -I${ROOT}/include/
OBJECT=include/query.c include/daemon.c
TARGET=release/bbwinvd
SRC=src/server.c

.PHONY: bbwinvd include debug bbwinvddebug


include: ${OBJECT}
	${CC} -c ${INCLUDE} ${OBJECT}

server.o: ${SRC}
	${CC} -c ${INCLUDE} ${SRC} 

bbwinvd: server.o include
	${CC} -o ${TARGET} *.o ${LIBS} ${INCLUDE} 

bbwinvddebug: server.o include
	${CC} -g -o ${TARGET} *.o ${LIBS} ${INCLUDE} 
	
release:  bbwinvd clean
	echo 'release build wurde gebaut'
	
debug:  bbwinvddebug clean

clean: include server.o
	rm *.o
