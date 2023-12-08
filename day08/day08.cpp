#include <print>
#include <map>
#include <cassert>

#include "include/codeAnalysis.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/stringstuff.h"


struct MapElement
{
    std::string left;
    std::string right;
};

std::map<std::string, MapElement>   map;



int main()
try
{
    auto lines      = getDataLines();
    auto directions = lines[0];
    

    lines.erase(lines.begin());
    lines.erase(lines.begin());

    std::regex  matcher{R"((...) = \((...), (...)\))" };        // GSV = (PKP, RMP)


    for(auto const &line : lines)
    {
        std::smatch matches;
        std::regex_match(line, matches, matcher);

        assert(matches.size()==4);

        map[matches[1]] = {matches[2],matches[3]};
    }


    std::string     pos="AAA";
    auto            step=0;

    while(pos != "ZZZ")
    {
        auto &element = map[pos];
        auto  turn    = directions[step % directions.size()];

        if(turn == 'L')
        {
            pos = element.left;
        }
        else        
        {
            pos = element.right;
        }

        step++;
    }

    std::print("Part 1 : {}\n",step);
}
catch(std::exception const &e)
{
    std::print("{}",e.what());
}


#include <sstream>

// --------------------------
std::istringstream testInput{
R"(
)"};
