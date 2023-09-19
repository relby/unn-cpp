#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <sstream>

void parse_socket(
    sockaddr_in *socket_addr,
    std::string& host,
    std::string& port
) {
    host = inet_ntoa(socket_addr->sin_addr);
    port = std::to_string(ntohs(socket_addr->sin_port));
}

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
