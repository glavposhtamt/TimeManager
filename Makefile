# Makefile for Time project

tdb: libsql.so main.o function.o
	cc -o tdb main.o function.o -L. -lsql -Wl,-rpath,.

main.o: main.c
	cc -c main.c -Wall -g

libsql.so: wrapper.o
	cc -shared -o libsql.so wrapper.o -lsqlite3

function.o: function.c
	cc -c -fPIC function.c -Wall -g -D_XOPEN_SOURCE=600

wrapper.o: wrapper.c
	cc -c -fPIC wrapper.c -Wall -g

clean:
	rm -f *.o *.so tdb
