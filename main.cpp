#include <iostream>

#include "anagram.h"
#include "ArgumentParser.h"
#include "tree.h"


int main(int argc, char *argv[])
{
    ArgumentParser parser;
    parser.addArgument("--filename");
    parser.addArgument("word");
    parser.parseArguments(argc, argv);
    
    Tree tree{parser.getValue("--filename").at(0)};
    findAnagrams(tree, parser.getValue("word").at(0));

    return 0;
}
