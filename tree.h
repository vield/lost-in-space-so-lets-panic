#ifndef TREE_H_
#define TREE_H_

#include <iostream>
#include <algorithm>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <vector>


class Node
{
private:
    std::map<char, Node> m_children;
    std::set<std::string> m_words;
public:
    Node& getOrCreateChild(char c)
    {
        std::map<char, Node>::iterator it = m_children.find(c);
        if (it != m_children.end())
        {
            return it->second;
        }
        Node newNode;
        this->m_children.insert(std::pair<char, Node>(c, newNode));
        return m_children.find(c)->second;
    }
    bool hasChild(char c)
    {
        return m_children.find(c) != m_children.end();
    }
    Node& getChild(char c)
    {
        // Fails if child doesn't exist
        return m_children.find(c)->second;
    }
    std::map<char, Node> getChildren()
    {
        return m_children;
    }
    void addWord(std::string word)
    {
        m_words.insert(word);
    }
    std::set<std::string> getWords()
    {
        return m_words;
    }
};


class Tree
{
private:
    Node m_root;
public:
    /**
     * Initialise word tree from given file.
     */
    Tree(std::string fileName)
    {
        std::string line;
        std::ifstream dictFile(fileName);

        // Will throw an exception if opening/reading/closing fails
        while (getline(dictFile, line))
        {
            std::string sortedLine = line;
            std::sort(sortedLine.begin(), sortedLine.end());

            Node *current = &m_root;
            for (auto c: sortedLine)
            {
                current = &current->getOrCreateChild(c);
            }
            current->addWord(line);
        }
        dictFile.close();

    }

    Node* getRoot()
    {
        return &m_root;
    }
};


Tree readFile(std::string fileName);

#endif
