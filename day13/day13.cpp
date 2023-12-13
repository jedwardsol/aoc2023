#include <print>
using std::print;
#include <bitset>
#include <cassert>

#include "include/codeAnalysis.h"
#include "include/thrower.h"
#include "include/getdata.h"

struct Grid
{
    std::vector<std::string>   rows;
    std::vector<std::string>   cols;
};

namespace Parse
{

auto readGrids()
{
    std::vector<Grid>   grids;
    Grid                grid{};
    int                 rowCount{};

    for(auto const &line : getDataLines())
    {
        if(line.empty())
        {
            grids.push_back(std::move(grid));
            grid      = Grid{};
            rowCount  = 0;
        }
        else
        {

            if(grid.cols.empty())
            {
                for(int i=0;i<line.size();i++)
                {
                    grid.cols.emplace_back();
                }
            }

            grid.rows.push_back(line);
            
            for(int colCount=0;colCount<line.size();colCount++)
            {
                grid.cols[colCount].push_back(line[colCount]);
            }

        }
    }

    return grids;
}
}



namespace Part1
{

int score(Grid const &grid)
{
    // return column  if a vertical line of symmetry
    // return 100*row if a horizontal line of symmetry


    return 1;
}


int part1(std::vector<Grid> const &grids)
{
    int total{};    

    for(auto &grid : grids)
    {
        total += score(grid);
    }

    return total;
}


}



int main()
try
{
    auto grids = Parse::readGrids();

    assert(grids.size()==100);

    print("Part 1 : {}",Part1::part1(grids));

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
