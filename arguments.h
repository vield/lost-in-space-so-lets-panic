#ifndef ARGUMENTS_H_
#define ARGUMENTS_H_

#include <string>


std::string askUserForFileName();

std::string askUserForAnagram();

struct Options
{
    std::string fileName;
    std::string anagram;
};

void getProgramOptions(Options *options, int argc, char *argv[]);

#endif
