#include <print>
#include <set>
using std::print;
#include <ranges>
#include <spanstream>

#include "include/codeAnalysis.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/posVector-RC.h"
#include "include/stopwatch.h"
#include "include/stringstuff.h"


struct Row
{
    std::string         springs;
    std::vector<int>    groups;
};

namespace Parse
{


auto getRows()
{
    std::vector<Row>    rows;

    for(auto const &line : getDataLines())
    {
        auto [springs,groups] = splitIn2(line,' ');

        Row     row{std::string{springs}};

        int                 group;
        std::ispanstream    stream{groups};

        while(stream >> group)
        {
            row.groups.push_back(group);
            stream.ignore(/*comma*/);
        }

        rows.push_back(std::move(row));
    }


    return rows;
}

}


namespace Part1
{

auto countGroups(std::string_view   springs)
{
    std::vector<int>    groups;

    char        previous='.';
    int         length{};

    for(auto c: springs)
    {
        if(c == previous)
        {
            length++;
        }
        else
        {
            if(previous == '#')
            {
                groups.push_back(length);
            }

            length=1;
        }

        previous=c;
    }

    if(previous == '#')
    {
        groups.push_back(length);
    }


    return groups;
}



int countArrangements(Row &row)
{
    auto unknown = row.springs.find('?');

    if(unknown == std::string::npos)
    {
        auto groups = countGroups(row.springs);
    
        return groups == row.groups;
    }
    else
    {
        int arrangements{};

        row.springs[unknown] = '#';
        arrangements += countArrangements(row);

        row.springs[unknown] = '.';
        arrangements += countArrangements(row);

        row.springs[unknown] = '?';
        return arrangements;
    }
}


}



int main()
try
{
    auto rows = Parse::getRows();

    int  part1{};

    Stopwatch   sw;

    for(auto row : rows)
    {
        part1 += Part1::countArrangements(row);
    }

    print("Part 1 : {} in {:.1f}s\n",part1,sw.seconds());       // 7506 in 1.4s

}
catch(std::exception const &e)
{
    std::print("{}",e.what());
}


#include <sstream>

// --------------------------
std::istringstream testInput{
R"(???.### 1,1,3
.??..??...?##. 1,1,3
?#?#?#?#?#?#?#? 1,3,1,6
????.#...#... 4,1,1
????.######..#####. 1,6,5
?###???????? 3,2,1)"};
