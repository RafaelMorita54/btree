BINARY = main.run

all: compile

compile:
	@gcc -o main.run *.c -Wall -I .

valgrind:
	valgrind --tool=memcheck --leak-check=full  --track-origins=yes --show-leak-kinds=all --show-reachable=yes ./$(BINARY)

run:
	@./main.run

zip:
	@zip -r runCodes.zip *.c *.h Makefile


# MAIN = main.c
# UTIL = btree.c
# BINARY = main
# NAME = btree

# all:
# 	gcc -Wall -Werror -g $(UTIL) $(MAIN) -o $(BINARY)

# run:
# 	./$(BINARY)

# valgrind:
# 	valgrind --tool=memcheck --leak-check=full  --track-origins=yes --show-leak-kinds=all --show-reachable=yes ./$(BINARY)

# zip:
# 	@zip -r $(NAME).zip *

# clean:
# 	@rm *.o

# objlib:
# 	gcc -Wall -pedantic -Werror -Wextra -c -fPIC $(UTIL) $(ITEM)

# lib:
# 	ar -rcs lib_util.a *.o