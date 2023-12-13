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

bool    trueReflection(std::vector<std::string> const &lines,  int index)
{
    auto back = index;
    auto fore = index+1;


    while(   back >= 0
          && fore < lines.size())
    {
        if(lines[back] != lines[fore])
        {
            return false;
        }

        back--;
        fore++;
    }

    return true ;
}


std::optional<int>  findReflection(std::vector<std::string> const &lines)
{

    for(int i=0;i<lines.size()-1;i++)
    {
        if(lines[i] == lines[i+1])
        {
            if(trueReflection(lines,i))
            {
                return i;
            }
        }
    }

    return std::nullopt;
}


int score(Grid const &grid)
{
    // return column  if a vertical line of symmetry
    // return 100*row if a horizontal line of symmetry

    auto reflection = findReflection(grid.cols);

    if(reflection)
    {
        return reflection.value()+1;                // 1-based indexing
    }

    reflection = findReflection(grid.rows);

    if(reflection)
    {
        return (reflection.value()+1) * 100;        // 1-based indexing
    }


    throw_runtime_error("No reflection");
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

    assert(grids.size()==100 || grids.size()==2);

    print("Part 1 : {}",Part1::part1(grids));

}
catch(std::exception const &e)
{
    std::print("{}",e.what());
}


#include <sstream>

// --------------------------
std::istringstream testInput{
R"(#.##..##.
..#.##.#.
##......#
##......#
..#.##.#.
..##..##.
#.#.##.#.

#...##..#
#....#..#
..##..###
#####.##.
#####.##.
..##..###
#....#..#

)"};
