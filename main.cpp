#include <iostream>

#include "anagram.h"
#include "arguments.h"
#include "tree.h"


int main(int argc, char *argv[])
{
    Options options;
    getProgramOptions(&options, argc, argv);

    Tree tree{options.fileName};
    findAnagrams(tree, options.anagram);

    return 0;
}
