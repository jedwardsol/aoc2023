#include <print>
using std::print;
#include <cassert>
#include <queue>


#include "include/codeAnalysis.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/posVector-RC.h"


using Lagoon = Grid<uint32_t>;

struct Edge
{
    Vector      dir;
    int         length;
    uint32_t    colour;
};

auto parse()
{
    auto    lines   = getDataLines();
    auto    edges   = std::vector<Edge>{};
    

    std::regex  matcher{R"((.) (\d+) \(#(.+)\))" };        // D 4 (#290881)


    for(auto const &line : lines)
    {
        std::smatch matches;
        std::regex_match(line, matches, matcher);

        assert(matches.size()==4);

        Vector      dir;
        auto        c = *(matches[1].first);

        if(c=='U') dir = {-1, 0};
        if(c=='D') dir = {+1, 0};
        if(c=='L') dir = { 0,-1};
        if(c=='R') dir = { 0,+1};

        
        edges.emplace_back( dir,
                            std::stoi(matches[2]),
                            std::stoi(matches[3],nullptr,16));
    }

    return edges;
}


auto limits(std::vector<Edge> const &edges)
{
    Pos topLeft    {0,0};
    Pos bottomRight{0,0};

    Pos walk       {0,0};
    for(auto edge : edges)
    {
        for(int i=0;i<edge.length;i++)
        {
            walk += edge.dir;

            topLeft.row = std::min(topLeft.row,walk.row);
            topLeft.col = std::min(topLeft.col,walk.col);

            bottomRight.row = std::max(bottomRight.row,walk.row);
            bottomRight.col = std::max(bottomRight.col,walk.col);
        }
    }

    return std::make_pair(topLeft,bottomRight);
}


void printLagoon(Lagoon const &lagoon)
{
    for(int row = 0; row < lagoon.height; row++)
    {
        for(int col = 0; col< lagoon.width; col++)
        {
            char c{' '};

            if(lagoon[row][col] != 0)        c='#';
            if(lagoon[row][col] == 0xffffff) c='=';

            print("{}", c);
        }
        print("\n");
    }
}

void digTrench(Lagoon &lagoon, std::vector<Edge> const &edges, Pos pos)
{
    lagoon[pos] = 0xffffff;     // does get covered in colour at the end     

    for(auto edge : edges)
    {
        for(int i=0;i<edge.length;i++)
        {
            pos += edge.dir;
            lagoon[pos] = edge.colour;
        }
    }
}


void digInside(Lagoon &lagoon)
{
    Pos     inside{lagoon.height/2, 0};

    while(lagoon[inside] == 0)
    {
        inside.col++;
    }

    while(lagoon[inside] != 0)
    {
        inside.col++;
    }


    std::queue<Pos> flood;

    flood.push(inside);


    while(not flood.empty())
    {
        inside = flood.front();
        flood.pop();

        for(auto dir : { Vector{-1,0}, Vector{+1,0}, Vector{0,-1},Vector{0,1}})
        {
            if(lagoon[inside+dir] == 0)
            {
                lagoon[inside+dir] = 0xffffff;
                flood.push(inside+dir);
            }
        }
    }
}


int main()
try
{
    auto const edges                 = parse();
    auto const [topLeft,bottomRight] = limits(edges);
    auto const height                = (bottomRight.row - topLeft.row)+3;
    auto const width                 = (bottomRight.col - topLeft.col)+3;

    auto const offset                = Vector{topLeft.row - 1, topLeft.col - 1 };

    auto       lagoon                = Grid<uint32_t>  {width,height,0};

    digTrench(lagoon,edges,Pos{0,0} - offset);

    digInside(lagoon);
    printLagoon(lagoon);

    auto part1 = (width * height) - std::ranges::count(lagoon.rawData(),0);

    print("Part 1 : {}\n",part1);

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
