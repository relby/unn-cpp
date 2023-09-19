#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <vector>
#include <iterator>
#include <sstream>
#include <algorithm>
#include <numeric>

const std::string COMMAND_PREFIX = "!";

const std::string QUIT_COMMAND = COMMAND_PREFIX + "quit";
const std::string HELLO_COMMAND = COMMAND_PREFIX + "hello";
const std::string LIST_COMMAND = COMMAND_PREFIX + "list";
const std::string GET_COMMAND = COMMAND_PREFIX + "get";
const std::string ADD_COMMAND = COMMAND_PREFIX + "add";

std::vector<std::string> split(const std::string& s, const std::string& separator)
{
    std::vector<std::string> output;
    std::string::size_type prev_pos = 0, pos = 0;
    while((pos = s.find(separator, pos)) != std::string::npos)
    {
        std::string substring(s.substr(prev_pos, pos-prev_pos));
        output.push_back(substring);
        prev_pos = ++pos;
    }
    output.push_back(s.substr(prev_pos, pos-prev_pos)); // Last word
    return output;
}

std::string join(const std::vector<std::string>& strings, const std::string& delim)
{
    if (strings.empty()) {
        return std::string();
    }
 
    return std::accumulate(strings.begin() + 1, strings.end(), strings[0],
        [&delim](std::string x, std::string y) {
            return x + delim + y;
        }
    );
}

#endif // COMMON_H

