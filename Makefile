
CC=g++
CFLAGS=-Wextra -Wall -Werror -std=c++14

anagrams:
	$(CC) $(CFLAGS) main.cpp anagram.cpp arguments.cpp tree.cpp -I . -o anagrams

clean:
	rm -f anagrams
