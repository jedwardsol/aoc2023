#include <print>
#include <map>
#include <cassert>
#include <numeric>

#include "include/codeAnalysis.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/stringstuff.h"


struct MapElement
{
    std::string left;
    std::string right;
};

using Map = std::map<std::string, MapElement>;



auto parse()
{
    auto    lines      = getDataLines();
    auto    directions = lines[0];
    Map     map;

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

    return std::make_pair(directions,map);
}

// how long to walk from AAA to ZZZ
auto part1(std::string_view     directions, Map &map)
{
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

    return step;
}



// each ghost walks from xxA to yyZ
// they all start at a different xxA at the same time.  How long until they're all on their yyZ at the same time
//  find each ghosts time to get to yyZ the 1st and 2nd time.  

struct Ghost
{
    std::string     start;
    std::string     firstEnd;
    int64_t         timeToFirstEnd;

    std::string     secondEnd;
    int64_t         timeToSecondEnd;

    int64_t         period;
};

auto collectGhost(std::string_view     directions, Map &map, std::string const &start)
{
    std::string     pos=start;
    int64_t         step{};

    Ghost ghost{start};

    while(step < 1'000'000'000)
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

        if(pos[2]=='Z')
        {
            if(ghost.timeToFirstEnd == 0)
            {
                ghost.firstEnd        = pos;
                ghost.timeToFirstEnd  = step;
            }
            else 
            {
                ghost.secondEnd       = pos;
                ghost.timeToSecondEnd = step;

                assert(ghost.firstEnd == ghost.secondEnd);          // it'll be harder if it goes   xxA  ->  yyZ  -> wwZ -> xxA  ->  yyY  -> wwZ ->
                ghost.period = ghost.timeToSecondEnd - ghost.timeToFirstEnd;

                assert(ghost.timeToFirstEnd == ghost.period);       // this doesn't have to be true - but seems to be for my data.  That makes the whole thing a lot easier

                return ghost;
            }
        }
    }

    throw_runtime_error("ghost didn't loop");
}

auto collectGhosts(std::string_view     directions, Map &map)
{
    std::vector<Ghost>  ghosts;

    for(auto &[start,_] : map)
    {
        if(start[2] == 'A')
        {
            ghosts.push_back(collectGhost(directions,map,start));
        }
    }

    return ghosts;
}



auto part2(std::string_view     directions, Map &map)
{
    auto ghosts = collectGhosts(directions,map);

    // Since the time to the 1st end happens to be the same as the period,   we just need to find the LCM of all the periods

    int64_t lcm = 1;

    for(auto &ghost : ghosts)
    {
        lcm = std::lcm(lcm, ghost.period);
    }

    return lcm;           //  18'024'643'846'273
}




int main()
try
{
    auto [directions,map]   = parse();

    std::print("Part 1 : {}\n",part1(directions,map));
    std::print("Part 2 : {}\n",part2(directions,map));
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
