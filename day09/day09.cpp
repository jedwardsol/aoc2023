#include <print>
#include <numeric>

#include "include/codeAnalysis.h"
#include "include/thrower.h"
#include "include/getdata.h"


auto adjacentDifference(std::vector<int64_t> const &line)
{
    auto nextLine{line};

    std::adjacent_difference(line.begin(),line.end(),nextLine.begin());

    nextLine.erase(nextLine.begin());
    return nextLine;
}


int64_t nextValue(std::vector<int64_t> const &line)
{
    if(std::ranges::count(line,0) == line.size())
    {
        return 0;
    }

    auto differences = adjacentDifference(line);

    return line.back() + nextValue(differences);
}


int64_t previousValue(std::vector<int64_t> const &line)
{
    if(std::ranges::count(line,0) == line.size())
    {
        return 0;
    }

    auto differences = adjacentDifference(line);

    return line.front() - previousValue(differences);
}



int main()
try
{
    auto    data=getDataGridOfInts();
    auto    part1{0ll};
    auto    part2{0ll};

    for(auto line : data)
    {
        part1 += nextValue    (line);
        part2 += previousValue(line);
    }


    std::print("Part 1 : {}\n",part1);
    std::print("Part 2 : {}\n",part2);

}
catch(std::exception const &e)
{
    std::print("{}",e.what());
}


#include <sstream>

// --------------------------
std::istringstream testInput{
R"(0 3 6 9 12 15
1 3 6 10 15 21
10 13 16 21 30 45
)"};
