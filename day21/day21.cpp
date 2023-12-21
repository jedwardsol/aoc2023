#include <print>
#include <queue>
#include <cassert>

#include "include/codeAnalysis.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/int.h"
#include "include/posVector-RC.h"


auto readGarden()
{
    auto lines = getDataLines();

    Grid<char>    garden{isize(lines[0]), isize(lines)};

    for(int i=0;i<garden.height;i++)
    {
        std::ranges::copy(lines[i], garden[i].begin());
    }

    return garden;
}


int main()
try
{
    struct Step
    {
        Pos pos;
        int distance;
    };


    auto                garden = readGarden();
    Grid<int>           steps{garden.width, garden.height, -1};    
    std::queue<Step>    fringe;

    assert(garden[65][65] == 'S');
    fringe.push({Pos{65,65},0});

    Vector dirs[]
    {
        {-1, 0},
        {+1, 0},
        { 0,-1},
        { 0,+1},
    };


    while(not fringe.empty())
    {
        auto step = fringe.front();
        fringe.pop();

        if(steps[step.pos] != -1)
        {
            continue;
        }

        steps[step.pos] = step.distance;

        for(auto dir:dirs)
        {
            auto newPos      = step.pos      + dir;
            auto newDistance = step.distance + 1;

            if(    garden.inGrid(newPos)
               &&  garden[newPos] == '.'
               &&  steps[newPos] == -1)
            {
                fringe.push({newPos, newDistance});
            }
        }
    }


    auto reachable = [](int steps)
    { 
        return steps <= 64
               && steps %2 == 0;
    };
    
    auto part1 = std::ranges::count_if(steps.rawData(), reachable);

    std::print("Part 1 : {}\n",part1);

    return 0;
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
