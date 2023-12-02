#include <print>
#include <array>
#include <cassert>
#include "include/codeAnalysis.h"
#include "include/thrower.h"
#include "include/getdata.h"


int valueOf1(std::string const &line)
{
    auto first = line.find_first_of("0123456789");
    auto last  = line.find_last_of("0123456789");

    return (line[first] - '0') * 10 + (line[last] - '0');
}

int valueOf2(std::string line)
{
    std::array<std::string_view,20>     words
    {
        "zero","one","two","three","four","five","six","seven","eight","nine",
        "0","1","2","3","4","5","6","7","8","9"
    };

    std::array<uint64_t, 20>     forward{};         // unsigned so npos is big
    std::array< int64_t, 20>     backward{};        //   signed so npos is -ve

    for(int i=0;i<20;i++)
    {
        forward [i] = line.find(words[i]);
        backward[i] = line.rfind(words[i]);
    }

    auto first = std::distance(forward.begin(),  std::ranges::min_element(forward))  % 10;
    auto last  = std::distance(backward.begin(), std::ranges::max_element(backward)) % 10;

    assert(first >=1 && first <= 9);
    assert(last  >= 1 && last <= 9);

    int result = first * 10 + last;

    return result;
}


int main()
try
{
    int part1Sum{};
    int part2Sum{};

    for(auto const &line : getDataLines())
    {
        part1Sum += valueOf1(line);
        part2Sum += valueOf2(line);
    }


    std::print("Part 1 : {}\n", part1Sum);
    std::print("Part 2 : {}\n", part2Sum);

}
catch(std::exception const &e)
{
    std::print("{}",e.what());
}


#include <sstream>

// --------------------------
std::istringstream testInput{
R"(two1nine
eightwothree
abcone2threexyz
xtwone3four
4nineeightseven2
zoneight234
7pqrstsixteen)"};
