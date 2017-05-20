#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>


class Argument
{
private:
    std::string m_longOptionName;
    std::string m_varName;
    bool m_optional;
    unsigned int m_numValues;

    std::string getVarName(std::string longOptionName) const;
public:
    Argument(std::string longOptionName, bool optional, unsigned int numValues):
        m_longOptionName{longOptionName},
        m_optional{optional},
        m_numValues{numValues}
    {
        m_varName = getVarName(longOptionName);
    }
    bool isOptional() const
    { return m_optional; }
    std::string getLongOptionName() const
    { return m_longOptionName; }
    std::string getVarName() const
    { return m_varName; }
    unsigned int getNumValues() const
    { return m_numValues; }
};


class ArgumentParser
{
private:
    // FIXME this should probably be a set of sorts, not a vector
    std::vector<std::string> m_parsedKeys;
    std::map<std::string, std::vector<std::string>> m_parsedValues;
    std::vector<std::string> m_positionalArgs;
    std::set<std::string> m_namedArgs;
    std::map<std::string, Argument> m_arguments;

    std::string capitalize(const std::string argName) const;
    bool _checkAllRequiredParamsGiven();
    bool _parseValues(std::string argName, std::queue<std::string> &args);
public:
    ArgumentParser()
    {
        this->addArgument("--help", true);
    }
    void addArgument(
        std::string name,
        bool optional=false,
        unsigned int numValues=1
    );
    ArgumentParser& parseArguments(int argc, char *argv[]);
    bool hasValue(std::string arg) const;
    const std::vector<std::string> getValue(std::string arg) const;
    const std::vector<std::string> getParsedKeys() const;
    void printUsageMessage(std::string programName) const;
};
