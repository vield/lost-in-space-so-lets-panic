#include <iostream>
#include <cstdlib>
#include <string>

#include "arguments.h"


std::string askUserForFileName()
{
    return "Asking user for a filename is not implemented yet.";
}


std::string askUserForAnagram()
{
    return "Asking user for anagram word is not implemented yet.";
}



void getProgramOptions(Options *options, int argc, char *argv[])
{
    if (argc == 1)
    {
        options->fileName = askUserForFileName();
        options->anagram = askUserForAnagram();
    }
    else if (argc == 2)
    {
        // Use system word file in '/usr/share/dict/words'
        options->fileName = "/usr/share/dict/words";
        // Interpret input from user as the word to form anagrams from
        options->anagram = argv[1];
    }
    else if (argc == 3)
    {
        // Interpret input as anagram word; file path
        options->anagram = argv[1];
        options->fileName = argv[2];
    }
    else
    {
        std::cout
            << "Usage: ./anagrams [<phrase to find anagrams for> [<file name>]]"
            << std::endl;
        exit(1);
    }
}
