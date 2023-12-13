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



std::vector<int>    findReflections(std::vector<std::string> const &lines)
{
    std::vector<int>    reflections;

    for(int i=0;i<lines.size()-1;i++)
    {
        if(lines[i] == lines[i+1])
        {
            if(trueReflection(lines,i))
            {
                reflections.push_back(i);
            }
        }
    }

    return reflections;
}


std::optional<int> score(Grid const &grid)
{
    // return column  if a vertical line of symmetry
    // return 100*row if a horizontal line of symmetry

    auto reflections = findReflections(grid.cols);

    if(!reflections.empty())
    {
        assert(reflections.size()==1);
        return reflections[0]+1;                // 1-based indexing
    }

    reflections = findReflections(grid.rows);

    if(!reflections.empty())
    {
        assert(reflections.size()==1);
        return (reflections[0]+1) * 100;        // 1-based indexing
    }


    throw_runtime_error("No reflection");
}


int part1(std::vector<Grid> const &grids)
{
    int total{};    

    for(auto &grid : grids)
    {
        total += score(grid).value();
    }

    return total;
}

}



namespace Part2
{


std::optional<int> score(Grid &grid)
{
    auto const part1Score = Part1::score(grid);


    for(int row = 0; row< grid.rows.size(); row++)
    {
        for(int col = 0; col< grid.rows[row].size(); col++)
        {
            auto originalChar = grid.rows[row][col];

            auto newChar      = (originalChar== '#') ? '.' : '#';

            grid.cols[col][row] = newChar;
            auto reflections = Part1::findReflections(grid.cols);
            grid.cols[col][row] = originalChar;

            for(auto reflection : reflections)
            {
                auto score = reflection + 1;

                if(score != part1Score)
                {
                    return score;
                }
            }


            grid.rows[row][col] = newChar;
            reflections = Part1::findReflections(grid.rows);
            grid.rows[row][col] = originalChar;

            for(auto reflection : reflections)
            {
                auto score = (reflection + 1)*100;

                if(score != part1Score)
                {
                    return score;
                }
            }
        }
    }

    return std::nullopt;
}



int part2(std::vector<Grid> &grids)
{
    int total{};    

    for(auto &grid : grids)
    {
        total += score(grid).value();
    }

    return total;
}




}



int main()
try
{
    auto grids = Parse::readGrids();

    assert(grids.size()==100 || grids.size()==2);

    print("Part 1 : {}\n",Part1::part1(grids));
    print("Part 1 : {}\n",Part2::part2(grids));

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
