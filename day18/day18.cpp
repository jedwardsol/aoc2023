#include <print>
using std::print;
#include <cassert>
#include <queue>


#include "include/codeAnalysis.h"
#include "include/thrower.h"
#include "include/getdata.h"



namespace Part1
{
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

void part1()
{
    auto const edges                 = parse();
    auto const [topLeft,bottomRight] = limits(edges);
    auto const height                = (bottomRight.row - topLeft.row)+3;
    auto const width                 = (bottomRight.col - topLeft.col)+3;

    auto const offset                = Vector{topLeft.row - 1, topLeft.col - 1 };

    auto       lagoon                = Grid<uint32_t>  {width,height,0};

    digTrench(lagoon,edges,Pos{0,0} - offset);

    digInside(lagoon);
//  printLagoon(lagoon);

    auto part1 = (width * height) - std::ranges::count(lagoon.rawData(),0);

    print("Part 1 : {}\n",part1);
}

}



namespace Part2
{
#include "include/posVector-XY.h"


struct Boundary
{
    int                 length{};
    std::vector<Pos>    vertices;
};


auto parse()
{
    auto                lines   = getDataLines();

    Boundary            little;
    Boundary            large;
    
    Pos                 littleWalk{0,0};
    Pos                 largeWalk{0,0};

    std::regex          matcher{R"((.) (\d+) \(#(.+)\))" };        // D 4 (#290881)


    for(auto const &line : lines)
    {
        std::smatch matches;
        std::regex_match(line, matches, matcher);

        assert(matches.size()==4);

        {
            Vector      littleVec;
            auto        littleDir = *(matches[1].first);

            auto        littleLen = std::stoi(matches[2]);

            if(littleDir=='U') littleVec = {-littleLen, 0};
            if(littleDir=='D') littleVec = {+littleLen, 0};
            if(littleDir=='L') littleVec = { 0,-littleLen};
            if(littleDir=='R') littleVec = { 0,+littleLen};


            littleWalk += littleVec;

            little.length += littleLen;
            little.vertices.push_back(littleWalk);
        }

        {
            Vector      largeVec;
            auto        largeLen = std::stoi(matches[3],nullptr,16) / 16;
            auto        largeDir = std::stoi(matches[3],nullptr,16) % 16;
            
            if(largeDir==3) largeVec = {-largeLen, 0};
            if(largeDir==1) largeVec = {+largeLen, 0};
            if(largeDir==2) largeVec = { 0,-largeLen};
            if(largeDir==0) largeVec = { 0,+largeLen};

            largeWalk += largeVec;

            large.length += largeLen;
            large.vertices.push_back(largeWalk);
        }
    }

    return std::make_pair(little,large);
}

int64_t     area(std::vector<Pos>   const &vertices)
{
    int64_t     sum{};

    for(int i=0; i<vertices.size(); i++)
    {
        int i1= (i+1) % vertices.size();

        sum += static_cast<int64_t>(vertices[i].x)  * vertices[i1].y;
        sum -= static_cast<int64_t>(vertices[i1].x) * vertices[i].y;
    }

    return std::abs(sum/2);
}


int64_t interior(int64_t area, int  boundaryPoints)
{
/*

    area =  interiorPoints + BoundaryPoints/2 - 1;

    =>

    area - BoundaryPoints/2 + 1 =  interiorPoints ;

*/

    return area - (boundaryPoints/2) + 1;

}


void part2()
{
    auto [little,large]=parse();

    auto    littleArea = area(little.vertices);
    auto    largeArea  = area(large.vertices);

    auto    littleInterior = interior(littleArea,little.length);
    auto    largeInterior  = interior(largeArea, large.length);


    print("Part 1 : {}\n",littleInterior + little.length);
    print("Part 2 : {}\n",largeInterior  + large.length);
}
}

int main()
try
{
    Part1::part1();
    Part2::part2();
 

}
catch(std::exception const &e)
{
    std::print("{}",e.what());
}


#include <sstream>

// --------------------------
std::istringstream testInput{
R"(R 6 (#70c710)
D 5 (#0dc571)
L 2 (#5713f0)
D 2 (#d2c081)
R 2 (#59c680)
D 2 (#411b91)
L 5 (#8ceee2)
U 2 (#caa173)
L 1 (#1b58a2)
U 2 (#caa171)
R 2 (#7807d2)
U 3 (#a77fa3)
L 2 (#015232)
U 2 (#7a21e3)
)"};
