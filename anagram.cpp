#include <iostream>
#include <algorithm>
#include <string>

#include "tree.h"


void findAnagrams(Tree &tree, std::string anagram)
{
    std::string sortedAnagram = anagram;
    std::sort(sortedAnagram.begin(), sortedAnagram.end());

    Node *current = tree.getRoot();
    bool anagramsFound = true;

    for (const auto &c : sortedAnagram)
    {
        if (current->hasChild(c))
        {
            current = &current->getChild(c);
        }
        else
        {
            anagramsFound = false;
            break;
        }
    }

    if (anagramsFound)
    {
        for (const auto &word : current->getWords())
        {
            std::cout << word << std::endl;
        }
    }
    else
    {
        // This is not very useful for machine parsing...
        std::cout << "No anagrams found." << std::endl;
    }
}
