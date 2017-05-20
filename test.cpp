#include <iostream>
#include <string>

#include "ArgumentParser.h"


/**
    Simple test code that requires a manual review of the results.

    Only tests cases relevant to the anagram solver (this is not
    yet a general-purpose argument parser).
*/

void testArguments(std::string description, int argc, char *argv[])
{
    // Initialise parser as it would be initialised for the anagram solver
    ArgumentParser parser;
    parser.addArgument("word");
    parser.addArgument("--filename", true);

    // Print header
    std::cout << "\n" << description << "\n";
    for (size_t i = 0; i < description.length(); ++i)
    {
        std::cout << "-";
    }
    std::cout << "\n";
    std::cout << "parser.parseArguments(" << argc << ", {";
    for (int i = 0; i < argc; ++i)
    {
        std::cout << "\"" << argv[i] << "\"";
        if (i != argc - 1) std::cout << ", ";
    }
    std::cout << "});\n\n---\n";

    // Run test
    // This may also print other stuff
    parser.parseArguments(argc, argv);
    std::cout << "---\n";

    // Print variables
    std::cout << "word: ";
    auto wordValue = parser.getValue("word");
    for (size_t i = 0; i < wordValue.size(); ++i)
    {
        std::cout << wordValue.at(i);
        if (i != wordValue.size() - 1) std::cout << ", ";
    }
    std::cout << "\n";

    std::cout << "--filename: ";
    auto fileNameValue = parser.getValue("--filename");
    for (size_t i = 0; i < fileNameValue.size(); ++i)
    {
        std::cout << wordValue.at(i);
        if (i != fileNameValue.size() - 1) std::cout << ", ";
    }
    std::cout << "\n";

    std::cout << "\n";
}

int main()
{
    char programName[] = "./programName";

    char *argv_noArguments[] = {programName};
    testArguments(
        "Called without arguments",
        1, argv_noArguments
    );

    char word[] = "lost in space";
    char *argv_justWord[] = {programName, word};
    testArguments(
        "Called with just the word",
        2, argv_justWord
    );

    char fileNameOpt[] = "--filename";
    char fileName[] = "/path/to/file.txt";
    char *argv_wordAndFileName[] = {programName, word, fileNameOpt, fileName};
    testArguments(
        "Called with word and filename",
        4, argv_wordAndFileName
    );

    char *argv_fileNameAndWord[] = {programName, fileNameOpt, fileName, word};
    testArguments(
        "Called with filename and word",
        4, argv_fileNameAndWord
    );

    return 0;
}
