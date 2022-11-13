#ifndef JSON_H
#define JSON_H
#include <istream>
#include <vector>
#include <map>
class JSONNode
{
private:
    std::map<std::string, JSONNode> nodes;
    std::map<std::string, std::string> strings;
    std::map<std::string, int> ints;
    std::map<std::string, std::vector<JSONNode>> nodeLists;
    std::map<std::string, std::vector<std::string>> stringLists;
    std::map<std::string, std::vector<int>> intLists;
public:
    JSONNode();
    template<class T>
    T getProperty(std::string propertyName);
    template<class T>
    void setProperty(std::string propertyName, T t);
    friend std::istream& operator>>(std::istream& in, JSONNode& info);
    friend std::ostream& operator<<(std::ostream& out, JSONNode info);
};
#endif