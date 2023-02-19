#pragma once
#include <vector>
#include <string>
#include <regex>

namespace tudumper::utility {

    /*!
     * 分割字符串
     * @param str
     * @param regex
     * @return
     * @example
     *  static const std::regex regex(R" ", std::regex::optimize);
     */
    std::vector<std::string> split(std::string const& str, const std::regex& regex);
    std::vector<std::string> split(std::string const& s, std::string const& delimiter);
}
