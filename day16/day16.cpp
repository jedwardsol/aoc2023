#include <print>
using std::print;
#include <algorithm>
#include <execution>
#include <map>
#include <set>

#include "include/codeAnalysis.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/posVector-RC.h"
#include "include/int.h"
#include "include/stopwatch.h"

using Cave      = Grid<char>;

using Visit     = std::set<Vector>;

using Visited   = Grid<Visit>;


constexpr Vector    up        {-1, 0};
constexpr Vector    right     { 0,+1};
constexpr Vector    down      {+1, 0};
constexpr Vector    left      { 0,-1};

constexpr Vector    upDown    {-1,-1};
constexpr Vector    leftRight {-2,-2};


namespace Parse
{

auto readCave()
{
    auto lines = getDataLines();

    Cave    cave{isize(lines[0]), isize(lines)};

    for(int i=0;i<cave.height;i++)
    {
        std::ranges::copy(lines[i], cave[i].begin());
    }

    return cave;
}

}


struct Tile
{
    Vector  dir;
    char    tile;

    friend auto operator<=>(Tile const&,Tile const&)=default;
};

std::map<Tile, Vector>  nextDir
{
    {   {  up,    '.'  }    , up}, 
    {   {  up,    '|'  }    , up}, 
    {   {  up,    '-'  }    , leftRight}, 
    {   {  up,    '/'  }    , right} ,
    {   {  up,    '\\' }    , left}, 

    {   {  down,  '.'  }    , down}, 
    {   {  down,  '|'  }    , down}, 
    {   {  down,  '-'  }    , leftRight}, 
    {   {  down,  '/'  }    , left} ,
    {   {  down,  '\\' }    , right}, 

    {   {  right, '.'  }    , right}, 
    {   {  right, '|'  }    , upDown}, 
    {   {  right, '-'  }    , right}, 
    {   {  right, '/'  }    , up} ,
    {   {  right, '\\' }    , down}, 

    {   {  left,  '.'  }    , left}, 
    {   {  left,  '|'  }    , upDown}, 
    {   {  left,  '-'  }    , left}, 
    {   {  left,  '/'  }    , down} ,
    {   {  left,  '\\' }    , up}, 
};


void traceBeam(Cave const &cave,Visited &visited,Pos const startPos,Vector const startDir)
{
    Pos     pos{startPos};
    Vector  dir{startDir};


    do
    {
        pos += dir;

        if(not cave.inGrid(pos))
        {
            return;
        }

        auto [_,inserted] = visited[pos].insert(dir);

        if(!inserted)
        {
            // entering a loop
            return;
        }

        dir = nextDir[ { dir, cave[pos] }  ];

        if(dir == upDown)
        {
            traceBeam(cave,visited, pos, up);
            traceBeam(cave,visited, pos, down);
            return;
        }
        else if(dir == leftRight)
        {
            traceBeam(cave,visited, pos, left);
            traceBeam(cave,visited, pos, right);
            return;
        }

    } while(cave.inGrid(pos));
}

auto traceBeam(Cave const &cave,Pos const startPos,Vector const startDir)
{
    Visited visited{cave.height,cave.width};               

    traceBeam(cave,visited,startPos,startDir);
    
    return std::ranges::count_if(visited.rawData(), [](auto const &visit) {return not visit.empty();});

}

auto part1(Cave const &cave)
{
    return traceBeam(cave, {0,-1}, right);
}


auto part2(Cave const &cave)
{
    int64_t best{};

    for(int row = 0; row < cave.height;row++)
    {
        best = std::max(best,traceBeam(cave, {row,-1},         right));
        best = std::max(best,traceBeam(cave, {row,cave.width}, left));
    }

    for(int col = 0; col < cave.width;col++)
    {
        best = std::max(best,traceBeam(cave, {-1,col},         down));
        best = std::max(best,traceBeam(cave, {cave.height,col},up));
    }

    return best;
}



auto part2Parallel(Cave const &cave)
{
    struct Beam
    {
        Pos         start;
        Vector      direction;
        int64_t     result;
    };

    std::vector<Beam>   beams;

    for(int row = 0; row < cave.height;row++)
    {
        beams.emplace_back(Pos{row,-1},         right,0);
        beams.emplace_back(Pos{row,cave.width}, left, 0);
    }

    for(int col = 0; col < cave.width;col++)
    {
        beams.emplace_back(Pos{-1,col},         down,0);
        beams.emplace_back(Pos{cave.height,col},up,  0);
    }

    auto traceBeam = [&](Beam &beam)
    {
        beam.result = ::traceBeam(cave, beam.start, beam.direction);
    };

    std::for_each(std::execution::par, beams.begin(), beams.end(), traceBeam);

    return std::ranges::max_element(beams, {}, &Beam::result)->result;
}




int main()
try
{
    auto cave = Parse::readCave();

    print("Part 1 : {}\n",part1(cave));

    auto sw    = Stopwatch{};
//  auto best  = ::part2(cave);             // 9064 in 687.7296ms
    auto best  = ::part2Parallel(cave);     // 9064 in 74.2142ms

    print("Part 2 : {} in {}ms\n",best, sw.milliseconds());
}
catch(std::exception const &e)
{
    std::print("{}",e.what());
}


#include <sstream>

// --------------------------
std::istringstream testInput{
R"(.|...\....
|.-.\.....
.....|-...
........|.
..........
.........\
..../.\\..
.-.-/..|..
.|....-|.\
..//.|....
)"};
