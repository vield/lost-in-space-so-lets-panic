
CC=g++
CFLAGS=-Wextra -Wall -Werror -std=c++14

anagrams:
	$(CC) $(CFLAGS) main.cpp anagram.cpp ArgumentParser.cpp -I . -o anagrams

test:
	$(CC) $(CFLAGS) ArgumentParser.cpp test.cpp -I . -o tests
	./tests

clean:
	rm -f anagrams tests
