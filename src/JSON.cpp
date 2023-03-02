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
bool JSONNode::getProperty<bool>(std::string propertyName) 
{
    return bools[propertyName];
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
template<>
std::vector<bool> JSONNode::getProperty<std::vector<bool>>(std::string propertyName) 
{
    return boolLists[propertyName];
}
template<>
void JSONNode::setProperty<int>(std::string propertyName, int i)
{
    ints[propertyName] = i;
}
template<>
void JSONNode::setProperty<string>(std::string propertyName, string i)
{
    strings[propertyName] = i;
}
template<>
void JSONNode::setProperty<JSONNode>(std::string propertyName, JSONNode i)
{
    nodes[propertyName] = i;
}
template<>
void JSONNode::setProperty<bool>(std::string propertyName, bool i)
{
    bools[propertyName] = i;
}
ostream& operator<<(ostream& out, JSONNode node)
{
    out << '{';
    bool first = true;
    for (auto item : node.strings)
    {
        if (!first) out << ',';
        out << '\"';
        out << item.first << "\":\"";
        out << item.second << '\"';
        first = false;
    }
    for (auto item : node.ints)
    {
        if (!first) out << ',';
        out << '\"';
        out << item.first << "\":";
        out << item.second;
        first = false;
    }
    for (auto item : node.nodes)
    {
        if (!first) out << ',';
        out << '\"';
        out << item.first << "\":";
        out << item.second;
        first = false;
    }
    for (auto item : node.bools)
    {
        if (!first) out << ',';
        out << '\"';
        out << boolalpha << item.first << "\":";
        out << item.second;
        first = false;
    }
    for (auto item : node.stringLists)
    {
        if (!first) out << ',';
        out << '\"';
        out << item.first << "\":[";
        for (size_t i = 0; i < item.second.size(); i++)
        {
            out << "\"" << item.second[i] << "\"";
            if (i != (item.second.size() - 1)) out << ",";
        }
        out << "]";
        first = false;
    }
    for (auto item : node.intLists)
    {
        if (!first) out << ',';
        out << '\"';
        out << item.first << "\":[";
        for (size_t i = 0; i < item.second.size(); i++)
        {
            out << item.second[i];
            if (i != (item.second.size() - 1)) out << ",";
        }
        out << "]";
        first = false;
    }
    for (auto item : node.nodeLists)
    {
        if (!first) out << ',';
        out << '\"';
        out << item.first << "\":[";
        for (size_t i = 0; i < item.second.size(); i++)
        {
            out << item.second[i];
            if (i != (item.second.size() - 1)) out << ",";
        }
        out << "]";
        first = false;
    }
    for (auto item : node.boolLists)
    {
        if (!first) out << ',';
        out << '\"';
        out << item.first << "\":[";
        for (size_t i = 0; i < item.second.size(); i++)
        {
            out << boolalpha << item.second[i];
            if (i != (item.second.size() - 1)) out << ",";
        }
        out << "]";
        first = false;
    }
    out << '}';
    return out;
}
istream& operator>>(istream& in, JSONNode& node)
{
    bool inList = false, first = true, inString = false, inKey = true, inInt = false, inBool = false;
    bool currentBool = false;
    char lastC = 0;
    string currentProperty = "", currentValueString = "";
    int currentInt = 0;
    while (true)
    {
        char c = in.get();
        if (inString)
        {
            if (c == '\"' && lastC != '\\')
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
                else if (inBool)
                {
                    if (node.boolLists.find(currentProperty) == node.boolLists.end())
                    {
                        node.boolLists[currentProperty].push_back(currentBool);
                    }
                }
                currentInt = 0;
                currentBool = false;
            }
            else
            {
                inKey = true;
                if (inInt)
                {
                    node.ints[currentProperty] = currentInt;
                }
                if (inBool)
                {
                    node.bools[currentProperty] = currentBool;
                }
                inInt = false;
                inBool = false;
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
            if (inBool)
            {
                if (node.boolLists.find(currentProperty) == node.boolLists.end())
                {
                    node.boolLists.emplace(
                        std::pair<std::string, std::vector<bool>>(currentProperty, {})
                    );
                }
                node.boolLists[currentProperty].push_back(currentBool);
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
        else if (c == 't')
        {
            inBool = true;
            currentBool = true;
        }
        else if (c == 'f')
        {
            inBool = true;
            currentBool = false;
        }
        lastC = c;
        first = false;
    }
    return in;
}