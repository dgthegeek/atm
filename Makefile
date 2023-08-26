objects = src/main.o src/system.o src/session.o src/auth.o src/utils.o sqlite-amalgamation-3420000//sqlite3.o src/conf.o

atm : $(objects)
	cc -o atm $(objects)
	
main.o : src/header.h
kbd.o : src/header.h
command.o : src/header.h
display.o : src/header.h
insert.o : src/header.h
search.o : src/header.h
files.o : src/header.h
utils.o : src/header.h
sqlite3.o : src/header.h
conf.o : src/header.h
session.o : src/header.h


clean :
	rm -f $(objects)