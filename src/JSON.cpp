#include <JSON.h>
#include <algorithm>
using namespace std;
JSONNode::JSONNode()
{
    nodes = {};
    strings = {};
    ints = {};
    nodeLists = {};
    stringLists = {};
    intLists = {};
}
template<>
std::string JSONNode::getProperty<std::string>(std::string propertyName) 
{
    return strings[propertyName];
}
template<>
int JSONNode::getProperty<int>(std::string propertyName) 
{
    return ints[propertyName];
}
template<>
JSONNode JSONNode::getProperty<JSONNode>(std::string propertyName) 
{
    return nodes[propertyName];
}
template<>
std::vector<std::string> JSONNode::getProperty<std::vector<std::string>>(std::string propertyName) 
{
    return stringLists[propertyName];
}
template<>
std::vector<int> JSONNode::getProperty<std::vector<int>>(std::string propertyName) 
{
    return intLists[propertyName];
}
template<>
std::vector<JSONNode> JSONNode::getProperty<std::vector<JSONNode>>(std::string propertyName) 
{
    return nodeLists[propertyName];
}
istream& operator>>(istream& in, JSONNode& node)
{
    bool inList = false, first = true, inString = false, inKey = true, inInt = false;
    string currentProperty = "", currentValueString = "";
    int currentInt = 0;
    while (true)
    {
        char c = in.get();
        if (inString)
        {
            if (c == '\"')
            {
                inString = false;
                if (!inKey)
                {
                    if (inList)
                    {
                        if (node.stringLists.find(currentProperty) == node.stringLists.end())
                        {
                            node.stringLists.emplace(
                                std::pair<std::string, std::vector<std::string>>(currentProperty, {})
                            );
                        }
                        node.stringLists[currentProperty].emplace_back(currentValueString);
                    }
                    else
                    {
                        node.strings.emplace(
                            std::pair<std::string, std::string>(currentProperty, currentValueString)
                        );
                    }
                }
            }
            else if (inKey) currentProperty.push_back(c);
            else currentValueString.push_back(c);
        }
        else if (isspace(c)) continue;
        else if (c >= '0' && c <= '9')
        {
            currentInt = currentInt * 10 + (int)(c - '0');
            inInt = true;
        }
        else if (c == ':')
        {
            inKey = false;
        }
        else if (c == ',')
        {
            if (inList)
            {
                currentValueString = "";
                if (inInt)
                {
                    if (node.intLists.find(currentProperty) == node.intLists.end())
                    {
                        node.intLists.emplace(
                            std::pair<std::string, std::vector<int>>(currentProperty, {})
                        );
                    }
                    node.intLists[currentProperty].emplace_back(currentInt);
                }
                currentInt = 0;
            }
            else
            {
                inKey = true;
                if (inInt)
                {
                    node.ints[currentProperty] = currentInt;
                }
                inInt = false;
                currentProperty = "";
                currentValueString = "";
                currentInt = 0;
            }
        }
        else if (!first && c == '{')
        {
            JSONNode newNode;
            in >> newNode;
            if (inList)
            {
                if (node.nodeLists.find(currentProperty) == node.nodeLists.end())
                {
                    node.nodeLists.emplace(
                        std::pair<std::string, std::vector<JSONNode>>(currentProperty, {})
                    );
                }
                node.nodeLists[currentProperty].emplace_back(newNode);
            }
            else
            {
                node.nodes.emplace(std::pair<std::string, JSONNode>(currentProperty, newNode));
            }
        }
        else if (c == '[')
        {
            inList = true;
        }
        else if (c == ']')
        {
            if (inInt)
            {
                if (node.intLists.find(currentProperty) == node.intLists.end())
                {
                    node.intLists.emplace(
                        std::pair<std::string, std::vector<int>>(currentProperty, {})
                    );
                }
                node.intLists[currentProperty].emplace_back(currentInt);
            }
            inList = false;
        }
        else if (c == '\"')
        {
            inString = true;
        }
        else if (c == '}')
        {
            if (inList)
            {
                currentValueString = "";
                if (inInt)
                {
                    if (node.intLists.find(currentProperty) == node.intLists.end())
                    {
                        node.intLists.emplace(
                            std::pair<std::string, std::vector<int>>(currentProperty, {})
                        );
                    }
                    node.intLists[currentProperty].emplace_back(currentInt);
                }
                currentInt = 0;
            }
            else
            {
                inKey = true;
                if (inInt)
                {
                    node.ints[currentProperty] = currentInt;
                }
                inInt = false;
                currentProperty = "";
                currentValueString = "";
                currentInt = 0;
            }
            break;
        }
        first = false;
    }
    return in;
}