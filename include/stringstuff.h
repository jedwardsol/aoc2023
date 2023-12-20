#pragma once
#include <tuple>
#include <charconv>
#include <string>
#include <string_view>
#include <regex>
#include <ranges>
#include "thrower.h"

[[nodiscard]] inline auto splitIn2(std::string_view  string, char delimiter)
{
    auto pos = string.find(delimiter);
    return std::make_pair(string.substr(0,pos),string.substr(pos+1));
}

[[nodiscard]] inline auto splitIn2(std::string_view  string, std::string_view delimiter)
{
    auto pos = string.find(delimiter);
    return std::make_pair(string.substr(0,pos),string.substr(pos+delimiter.size()));
}



inline auto  rangeToSV(auto &&range)
{
    return std::string_view{range.begin(), range.end()};
}

inline auto  rangeToStr(auto &&range)
{
    return std::string{range.begin(), range.end()};
}


inline auto split(std::string_view string, std::string_view  delim)
{
    std::vector<std::string_view>  result;     

    for(auto const &element : std::views::split(string,delim))
    {
        result.emplace_back(rangeToSV(element));
    }

    return result;
}




[[nodiscard]] inline int stoi(std::string_view  string)
{
    int i{};
    auto result = std::from_chars(string.data(),string.data()+string.size(),i);

    if(result.ec != std::errc{})
    {
        throw_runtime_error("stoi from " + std::string{string});
    }

    return i;
}


[[nodiscard]] inline int64_t stoll(std::string_view  string)
{
    int64_t i{};
    auto result = std::from_chars(string.data(),string.data()+string.size(),i);

    if(result.ec != std::errc{})
    {
        throw_runtime_error("stoi from " + std::string{string});
    }

    return i;
}



// returns an int and adjusts the view to consume the characters
[[nodiscard]] inline int stoi_c(std::string_view  &string)
{
    int i{};
    auto result = std::from_chars(string.data(),string.data()+string.size(),i);

    if(result.ec != std::errc{})
    {
        throw_runtime_error("stoi from " + std::string{string});
    }

    string.remove_prefix(result.ptr-string.data());

    return i;
}



[[nodiscard]] inline auto numbersFromRegex(std::string const &line, std::string const &regex,  int numExpected)
{
    std::regex  matcher{regex};
    std::smatch matches;

    std::regex_match(line, matches, matcher);

    if(matches.size() != numExpected+1)
    {
        throw_runtime_error("no match " + line);
    }

    std::vector<int> numbers(numExpected);

    std::transform(matches.begin()+1,matches.end(), numbers.begin(), [](std::string const &string){return std::stoi(string);});
    
    return numbers;
}
