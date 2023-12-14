#include <print>
using std::print;
#include "include/codeAnalysis.h"
#include "include/thrower.h"
#include "include/getdata.h"

#include "include/posVector-RC.h"
#include "include/int.h"

auto readGrid()
{
    auto lines = getDataLines();

    Grid<char>  grid{isize(lines[0]), isize(lines)};

    for(int i=0;i<grid.height;i++)
    {
        std::ranges::copy(lines[i], grid[i].begin());
    }

    return grid;
}


void printGrid(Grid<char> const &grid)
{
    for(auto row =0;row < grid.height;row++)
    {
        for(auto col=0;col < grid.width;col++)
        {
            print("{}",grid[row][col]);
        }
        print("\n");
    }
    print("\n");
}

auto tiltNorth(Grid<char> const &grid)
{
    auto tilted = grid;

    for(auto row =0;row < tilted.height;row++)
    {
        for(auto col=0;col < tilted.width;col++)
        {
            Pos     start{row,col};

            if(   tilted[start] == '.'
               || tilted[start] == '#')
            {
                continue;
            }

            Vector  dir{-1,0};
            Pos     end{start};
            
            do
            {
                end+=dir;
            } while(   tilted.inGrid(end)
                    && tilted[end] == '.');

            end-=dir;

            tilted[start]='.';
            tilted[end]='O';
        }
    }

    return tilted;
}



auto loadNorth(Grid<char> const &grid)
{
    int     load{};

    for(auto row =0;row < grid.height;row++)
    {
        for(auto col=0;col < grid.width;col++)
        {
            if(   grid[row][col] == 'O')
            {
                load += grid.height - row;
            }
        }
    }

    return load;
}



int main()
try
{
    auto const grid = readGrid();

    auto tilted     = tiltNorth(grid);

//    printGrid(grid);
//    printGrid(tilted);

    auto load       = loadNorth(tilted);

    print("Part 1 : {}\n",load);


}
catch(std::exception const &e)
{
    std::print("{}",e.what());
}


#include <sstream>

// --------------------------
std::istringstream testInput{
R"(O....#....
O.OO#....#
.....##...
OO.#O....O
.O.....O#.
O.#..O.#.#
..O..#O..O
.......O..
#....###..
#OO..#....
)"};
