#include <print>
using std::print;
#include "include/codeAnalysis.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/posVector-RC.h"
#include <cassert>

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


int main()
try
{
    auto edges = parse();

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

    const int height = (bottomRight.row - topLeft.row)+3;
    const int width  = (bottomRight.col - topLeft.col)+3;

    Vector  offset{topLeft.row - 1, topLeft.col - 1 };

    Grid<uint32_t>  lagoon{width,height,0};

    auto pos = Pos{0,0} - offset;


    lagoon[pos] = 0xffffff;        

    for(auto edge : edges)
    {
        for(int i=0;i<edge.length;i++)
        {
            pos += edge.dir;
            lagoon[pos] = edge.colour;
        }
    }

    for(int row = 0; row < lagoon.height; row++)
    {
        for(int col = 0; col< lagoon.width; col++)
        {
            print("{}", lagoon[row][col] ? '#' : ' ');
        }
        print("\n");
    }


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
