# For UNIX

CC = gcc
CFLAGS = -Wall
DEPS_H = parse.h cmd.h arbre.h
DEPS_O = parse.o cmd.o arbre.o
EXEC = termfm


all : $(EXEC)

%.o: %.c $(DEPS_H)
	$(CC) $(CFLAGS) -c $<

termfm : main.o $(DEPS_O)
	$(CC) $(CFLAGS) -o $@ $^

clean :
	@echo "clean starts"
	rm -rf $(EXEC) *.o
	@echo "clean completed"
