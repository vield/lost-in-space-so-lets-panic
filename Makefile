
CC=g++
CFLAGS=-Wextra -Wall -Werror

anagrams:
	$(CC) $(CFLAGS) main.cpp anagram.cpp arguments.cpp tree.cpp -I . -o anagrams

clean:
	rm -f anagrams
