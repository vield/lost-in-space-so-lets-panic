# Lost in Space - So Let's Panic
Anagram solver in C++.

Usage:

    ./anagrams --filename /usr/share/dict/words altering
    alerting
    altering
    integral
    relating
    triangle


## Requirements

The anagram solver is self-contained in that it doesn't rely on any external
libraries -- just download the code, compile it and run.

* `make`
* `g++` with support for C++14
* A dictionary file with one dictionary word per line (e.g. the one in
  `/usr/share/dict/words`, if that exists on your system)


## FIXME list

* Use `const` more wherever appropriate.
* Make it not leak memory.
* Add tests to uncover more bugs.
* Make interactive UI work (currently it's only possible to pass in parameters on the command line).


## New feature ideas

* Support for multi-word anagrams.
* Allow user to choose how many words the anagram should contain or how long the words should be (at most, at least).
