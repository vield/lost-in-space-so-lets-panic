#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <stdexcept>
#include <map>
#include <string>
#include <set>

#include "ArgumentParser.h"


/**
    Define a parameter to parse.

    @name   Identifier for the parameter. If the name starts with a dash ("-"),
    it is interpreted as a named parameter (e.g. "./your_program --fileName
    input.txt"), otherwise it is added to the positional parameter list.
*/
void ArgumentParser::addArgument(std::string name, bool optional)
{
    bool isNamed = name.find("-") == 0;

    std::vector<std::string> emptyVector;
    m_parsedValues.insert(std::pair<std::string, std::vector<std::string>>(name, emptyVector));

    if (isNamed)
    {
        m_namedArgs.insert(name);
        Argument argument{name, optional};
        m_namedArgsSettings.insert(std::pair<std::string, Argument>(
            name, argument
        ));
    }
    else
    {
        m_positionalArgs.push_back(name);
    }
}


/**
    Parse arguments.

    If the given arguments are not valid (e.g. not all required arguments
    were given), this will print the usage message, but the program will
    currently not terminate and no exception will be thrown.

    TODO: Exceptions or exit(errorCode) as options for the ArgumentParser.

    @argc  Number of arguments given.
    @argv  Arguments. First one is the path to the program or the program name.
           Any arguments following that will be parsed.
*/
ArgumentParser& ArgumentParser::parseArguments(int argc, char *argv[])
{
    // Skip program name
    int current = 1;

    // At the end, we'll check if all required arguments were received
    bool valid = true;

    // Currently, all params are expected to take exactly one argument.
    // FIXME Add option to specify 0, 5, '+', '*' etc arguments

    bool inNamedArgs = true;

    int nextPosArg = 0;

    // Parse named arguments
    while (current < argc)
    {
        // Check if next arg is named
        std::string currentParam{argv[current]};
        // If we're doing positional args, no named args may follow any more
        inNamedArgs = inNamedArgs && m_namedArgs.find(currentParam) != m_namedArgs.end();

        // If parsing named args
        if (inNamedArgs)
        {

            std::cout << "Processing argument:\n";
            std::cout << argv[current] << " " << argv[current+1] << "\n";


            m_parsedKeys.push_back(currentParam);
            ++current;
            if (current >= argc)
            {
                std::cout << "named\n";
                valid = false;
                break;
            }
            std::vector<std::string> values;
            std::string currentVal = std::string{argv[current]};
            values.push_back(currentVal);
            m_parsedValues.insert(
                std::pair<std::string, std::vector<std::string>>(
                    currentParam, values
                )
            );
            ++current;

        }
        // If parsing positional args
        else
        {

            if (nextPosArg >= m_positionalArgs.size())
            {
                std::cout << "bleep\n";
                valid = false;
                break;
            }

            std::string posArgName = m_positionalArgs.at(nextPosArg);
            m_parsedKeys.push_back(posArgName);

            std::cout << "Processing argument:\n";
            std::cout << posArgName << " = " << currentParam << "\n";

            std::vector<std::string> values;
            values.push_back(currentParam);
            m_parsedValues.insert(
                std::pair<std::string, std::vector<std::string>>(
                    posArgName,
                    values
                )
            );
            ++nextPosArg;
            ++current;
        }
    }

    // Check if all positional args were found
    if (nextPosArg != m_positionalArgs.size())
    {
        std::cout << "pos args missing\n";
        valid = false;
    }

    // Check if all required args were included
    /*
    for (const auto &namedArg : m_namedArgs)
    {
        if (!m_namedArgsSettings.find(namedArg)->second->optional)
        {
            if
        }
    }*/

    if (!valid)
        printUsageMessage(argc, argv);

    return *this;
}


const std::vector<std::string> ArgumentParser::getValue(std::string arg) const
{
    auto it = m_parsedValues.find(arg);

    if (it == m_parsedValues.end())
    {
        throw std::invalid_argument("No value for " + arg + " found!");
    }

    return it->second;
}


const bool ArgumentParser::hasValue(std::string arg) const
{
    return false;
}


const std::vector<std::string> ArgumentParser::getParsedKeys() const
{
    return m_parsedKeys;
}


namespace {
    inline std::string &trimDashesFromStart(std::string &s)
    {
        int eraseUntil = 0;
        while (eraseUntil < s.length() &&
               s[eraseUntil] == '-')
        {
            ++eraseUntil;
        }
        s.erase(0, eraseUntil);
        return s;
    }
}


std::string ArgumentParser::capitalize(const std::string argName) const
{
    std::string capitalizedStr = argName;
    capitalizedStr = trimDashesFromStart(capitalizedStr);

    std::transform(capitalizedStr.begin(), capitalizedStr.end(), capitalizedStr.begin(), toupper);

    return capitalizedStr;
}


std::string Argument::getVarName(const std::string argName) const
{
    std::string capitalizedStr = argName;
    capitalizedStr = trimDashesFromStart(capitalizedStr);

    std::transform(capitalizedStr.begin(), capitalizedStr.end(), capitalizedStr.begin(), toupper);

    return capitalizedStr;
}



void ArgumentParser::printUsageMessage(int argc, char *argv[]) const
{
    char *programName = argv[0];

    std::cout << "Usage: " << programName;

    for (const auto &name : m_namedArgs)
    {
        Argument argument = m_namedArgsSettings.find(name)->second;
        bool optional = argument.isOptional();

        std::cout << " " << (optional ? "[" : "");
        std::cout << argument.getLongOptionName();
        std::cout << " " << argument.getVarName();
        std::cout << (optional ? "]" : "");
    }

    for (const auto &posArg : m_positionalArgs)
    {
        std::cout << " " << capitalize(posArg);
    }

    std::cout << "\n";
}



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
    for (int i = 0; i < description.length(); ++i)
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
    for (int i = 0; i < wordValue.size(); ++i)
    {
        std::cout << wordValue.at(i);
        if (i != wordValue.size() - 1) std::cout << ", ";
    }
    std::cout << "\n";

    std::cout << "--filename: ";
    auto fileNameValue = parser.getValue("--filename");
    for (int i = 0; i < fileNameValue.size(); ++i)
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
