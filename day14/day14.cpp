#include <print>
using std::print;
#include <map>

#include "include/codeAnalysis.h"
#include "include/thrower.h"
#include "include/getdata.h"

#include "include/posVector-RC.h"
#include "include/int.h"
#include "include/hash.h"

using Dish = Grid<char>;

namespace Parse
{

auto readDish()
{
    auto lines = getDataLines();

    Dish    dish{isize(lines[0]), isize(lines)};

    for(int i=0;i<dish.height;i++)
    {
        std::ranges::copy(lines[i], dish[i].begin());
    }

    return dish;
}


void printDish(Dish const &dish)
{
    for(auto row =0;row < dish.height;row++)
    {
        for(auto col=0;col < dish.width;col++)
        {
            print("{}",dish[row][col]);
        }
        print("\n");
    }
    print("\n");
}

}

namespace Tilt
{

void rollStone(Dish  &tilted, Pos start, Vector dir)
{

    if(   tilted[start] == '.'
        || tilted[start] == '#')
    {
        return;
    }

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


auto tiltNorthOrWest(Dish const &dish, Vector dir)
{
    auto tilted = dish;

    for(auto row =0;row < tilted.height;row++)
    {
        for(auto col=0;col < tilted.width;col++)
        {
            rollStone(tilted, {row,col}, dir);
        }
    }

    return tilted;
}


auto tiltSouthOrEast(Dish const &dish, Vector dir)
{
    auto tilted = dish;

    for(auto row =tilted.height-1;row >=0 ;row--)
    {
        for(auto col=tilted.width-1;col >= 0 ;col--)
        {
            rollStone(tilted, {row,col}, dir);
        }
    }

    return tilted;
}




auto tiltNorth(Dish const &dish)
{
    return tiltNorthOrWest(dish,{-1,0});
}

auto tiltWest(Dish const &dish)
{
    return tiltNorthOrWest(dish,{0,-1});
}


auto tiltSouth(Dish const &dish)
{
    return tiltSouthOrEast(dish,{+1,0});
}

auto tiltEast(Dish const &dish)
{
    return tiltSouthOrEast(dish,{0,+1});
}


}



auto loadNorth(Dish const &dish)
{
    int     load{};

    for(auto row =0;row < dish.height;row++)
    {
        for(auto col=0;col < dish.width;col++)
        {
            if(   dish[row][col] == 'O')
            {
                load += dish.height - row;
            }
        }
    }

    return load;
}


auto hash(Dish const &dish)
{
    size_t  hash{};

    hash_combine(hash,dish.rawData());

    return hash;

}


int main()
try
{
    auto const dish = Parse::readDish();                                                                                              

    {
        auto tilted     = Tilt::tiltNorth(dish);

        auto load       = loadNorth(tilted);

        print("Part 1 : {}\n",load);
    }


    auto                    tilted = dish;
    std::map<size_t,int>    hashes;                 //  hash -> cycle when hash 1st seen

    constexpr int           totalCycles{1'000'000'000};
    bool                    skipped{false};

    for(int cycle=0;cycle<totalCycles;cycle++)                        
    {
        tilted = Tilt::tiltNorth(tilted);    
        tilted = Tilt::tiltWest(tilted);    
        tilted = Tilt::tiltSouth(tilted);    
        tilted = Tilt::tiltEast(tilted);    

        if(!skipped)
        {
            auto hash = ::hash(tilted);        

            if(hashes.contains(hash))
            {
                auto loopLength    = cycle - hashes[hash];
                auto cyclesLeft    = totalCycles - cycle;
                auto loopsToSkip   = cyclesLeft / loopLength;
                auto cyclesToSkip  = loopsToSkip * loopLength;

                print("@ cycle {} \n",cycle);
                print(" found loop of length {} cycles \n",loopLength);
                print(" skipping {} cycles\n",cyclesToSkip);

                cycle += cyclesToSkip;
                skipped=true;
            }
            else
            {
                hashes[hash]=cycle;
            }
        }
    }

    auto load       = loadNorth(tilted);

    print("Part 2 : {}\n",load);




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
