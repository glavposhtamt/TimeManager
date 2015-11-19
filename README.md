Install SQLite3 in Ubuntu:

sudo apt-get install sqlite3 libsqlite3-dev

Compile gcc:

cc -o time main.c function.c -g -Wall -lsqlite3 -D_XOPEN_SOURCE=600
