#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <map>
#include <queue>
#include <set>
#include <stdexcept>
#include <string>

#include "ArgumentParser.h"


/**
    Define a parameter to parse.

    @name  Identifier for the parameter. If the name starts with a dash ("-"),
    it is interpreted as a named parameter (e.g. "./your_program --fileName
    input.txt"), otherwise it is added to the positional parameter list.
    @optional  If the parameter is required, parseArguments() will fail if
    it isn't present.
    @numValues  How many values the parameter should get. If this is zero, the
    named parameter is interpreted as a boolean flag.
*/
void ArgumentParser::addArgument(std::string name, bool optional, unsigned int numValues)
{
    bool isNamed = name.find("-") == 0;

    Argument argument{name, optional, numValues};

    // FIXME Currently, if the same parameter is added twice or more, only
    // the first one is stored and the rest are ignored. We should return an
    // error message instead / throw an exception / whatever is appropriate.
    if (isNamed)
    {
        m_namedArgs.insert(name);
        m_arguments.insert(std::make_pair(name, argument));
    }
    else
    {
        if (numValues == 0)
        {
            throw std::invalid_argument("Positional argument must take at least one value!");
        }
        m_positionalArgs.push_back(name);
        m_arguments.insert(std::make_pair(name, argument));
    }
}


/**
    This method pops all values belonging to the named argument from the
    queue (and returns false if that cannot be done).
*/
bool ArgumentParser::_parseValues(std::string argName, std::queue<std::string> &args)
{
    Argument argument = m_arguments.find(argName)->second;
    unsigned int numValues = argument.getNumValues();

    std::pair<std::string, std::vector<std::string>> parsedValues =
        std::make_pair(argName, std::vector<std::string>{});

    std::vector<std::string> &values = parsedValues.second;

    while (!args.empty() && numValues > 0)
    {
        values.push_back(args.front());
        args.pop();
        --numValues;
    }

    if (numValues > 0)
    {
        // Could not read enough values, error
        return false;
    }

    // FIXME what if named arg is given multiple times? What should be the
    // expected behaviour?
    m_parsedValues.insert(parsedValues);

    return true;
}


bool ArgumentParser::_checkAllRequiredParamsGiven()
{
    return true;
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
    bool valid = true;
    // TODO split into functions

    // Convert the whole thing to a queue of strings instead, so we don't
    // have to pass argc around
    std::queue<std::string> args;
    for (int i = 1; i < argc; ++i)
    {
        args.push(std::string{argv[i]});
    }

    // First, parse named args
    while (valid && !args.empty() && m_namedArgs.find(args.front()) != m_namedArgs.end())
    {
        std::string argName = args.front();
        args.pop();
        valid = _parseValues(argName, args);
    }

    // Then, parse positional args
    for (const auto &argName : m_positionalArgs)
    {
        if (!valid || args.empty())
        {
            break;
        }

        valid = _parseValues(argName, args);
    }

    // Finally, check that all required params have been specified
    // TODO
    valid = _checkAllRequiredParamsGiven();

    if (!valid)
    {
        std::string programName{argv[0]};
        printUsageMessage(programName);
    }

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


bool ArgumentParser::hasValue(std::string arg) const
{
    return m_parsedValues.find(arg) != m_parsedValues.end();
}


const std::vector<std::string> ArgumentParser::getParsedKeys() const
{
    return m_parsedKeys;
}


namespace {
    inline std::string &trimDashesFromStart(std::string &s)
    {
        unsigned int eraseUntil = 0;
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



void ArgumentParser::printUsageMessage(std::string programName) const
{
    std::cout << "Usage: " << programName;

    for (const auto &name : m_namedArgs)
    {
        Argument argument = m_arguments.find(name)->second;
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
