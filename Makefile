
CC=g++
CFLAGS=-Wextra -Wall -Werror -std=c++14

anagrams:
	$(CC) $(CFLAGS) main.cpp anagram.cpp arguments.cpp -I . -o anagrams

test:
	$(CC) -std=c++14 ArgumentParser.cpp -I . -o argTest
	./argTest

clean:
	rm -f anagrams
