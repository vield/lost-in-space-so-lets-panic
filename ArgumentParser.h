#include <map>
#include <set>
#include <string>
#include <vector>


class Argument
{
private:
    std::string m_longOptionName;
    std::string m_varName;
    bool m_optional;

    std::string getVarName(std::string longOptionName) const;
public:
    Argument(std::string longOptionName, bool optional):
        m_longOptionName{longOptionName},
        m_optional{optional}
    {
        m_varName = getVarName(longOptionName);
    }
    bool isOptional() const
    { return m_optional; }
    std::string getLongOptionName()
    { return m_longOptionName; }
    std::string getVarName()
    { return m_varName; }
};


class ArgumentParser
{
private:
    std::vector<std::string> m_parsedKeys;
    std::map<std::string, std::vector<std::string>> m_parsedValues;
    std::vector<std::string> m_positionalArgs;
    std::set<std::string> m_namedArgs;
    std::map<std::string, Argument> m_namedArgsSettings;

    std::string capitalize(const std::string argName) const;
public:
    ArgumentParser()
    {
        this->addArgument("--help", true);
    }
    void addArgument(std::string name, bool optional=false);
    ArgumentParser& parseArguments(int argc, char *argv[]);
    const bool hasValue(std::string arg) const;
    const std::vector<std::string> getValue(std::string arg) const;
    const std::vector<std::string> getParsedKeys() const;
    void printUsageMessage(int argc, char *argv[]) const;
};
