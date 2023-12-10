#include <print>
#include <cassert>
#include <bit>

#include "include/codeAnalysis.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/posVector-RC.h"
#include "include/int.h"


enum Dir : uint8_t
{
    U = 0b0001,
    L = 0b0010,
    D = 0b0100,
    R = 0b1000,
};

namespace Move
{
    Vector      U{-1, 0};
    Vector      L{ 0,-1};
    Vector      D{ 1, 0};
    Vector      R{ 0, 1};
};


enum  Pipe : uint8_t
{
    empty = 0,    //  .
    LR    = L|R,    //  -
    UD    = U|D,    //  |
    UR    = U|R,    //  L
    UL    = U|L,    //  J
    DR    = D|R,    //  F
    DL    = D|L,    //  7
    start = 0b10000000,
};


bool operator&(Pipe lhs, Dir rhs) noexcept
{
    return   static_cast<uint8_t>(lhs) 
           & static_cast<uint8_t>(rhs);
}

Pipe &operator|=(Pipe &lhs, Dir rhs)
{
    lhs = static_cast<Pipe>(static_cast<uint8_t>(lhs)  | static_cast<uint8_t>(rhs));

    return lhs;
}



Vector  next(Vector current, Pipe pipe)
{
    struct 
    {
        Vector      current;
        Pipe        pipe;
        Vector      next;
    } moves[]
    {
        {Move::U, Pipe::UD, Move::U},
        {Move::U, Pipe::DL, Move::L},
        {Move::U, Pipe::DR, Move::R},

        {Move::L, Pipe::LR, Move::L},
        {Move::L, Pipe::UR, Move::U},
        {Move::L, Pipe::DR, Move::D},

        {Move::D, Pipe::UD, Move::D},
        {Move::D, Pipe::UL, Move::L},
        {Move::D, Pipe::UR, Move::R},

        {Move::R, Pipe::LR, Move::R},
        {Move::R, Pipe::UL, Move::U},
        {Move::R, Pipe::DL, Move::D},
    };

    for(auto const &move : moves)
    {
        if(   current == move.current 
           && pipe    == move.pipe)
        {
            return move.next;
        }
    }

    throw_runtime_error("no move");
}


struct Tile
{
    Pipe    pipe;
    int     distance;
};


auto getPipes()
{
    auto pipe = [](char c) -> Tile
    {
        switch(c)
        {
            case 'S': return {Pipe::start,0};
            case '.': return {Pipe::empty,0};
            case '-': return {Pipe::LR,0};
            case '|': return {Pipe::UD,0};
            case 'L': return {Pipe::UR,0};
            case 'J': return {Pipe::UL,0};
            case 'F': return {Pipe::DR,0};
            case '7': return {Pipe::DL,0};
            default:  throw_runtime_error("bad input");
        }
    };


    auto const   lines = getDataLines();
    auto const   rows = isize(lines);
    auto const   cols = isize(lines[0]);

    Grid<Tile>   pipes{cols,rows};
    Pos          startPos{};

    for(int row=0;row<rows;row++)
    {
        for(int col=0;col<cols;col++)
        {
            pipes[row][col] = pipe(lines[row][col]);

            if(pipes[row][col].pipe == Pipe::start)
            {
                startPos = Pos{row,col};
            }
        }
    }

    // deduce what pipe is at the start  (assumes it isn't at the edge)

    Pipe        startPipe{Pipe::empty};    
    Vector      startDir{};

    if( pipes[ startPos + Move::U ].pipe & Dir::D)  { startPipe |= Dir::U; startDir = Move::U;}
    if( pipes[ startPos + Move::L ].pipe & Dir::R)  { startPipe |= Dir::L; startDir = Move::L;}
    if( pipes[ startPos + Move::D ].pipe & Dir::U)  { startPipe |= Dir::D; startDir = Move::D;}
    if( pipes[ startPos + Move::R ].pipe & Dir::L)  { startPipe |= Dir::R; startDir = Move::R;}

    pipes[startPos].pipe = startPipe;

    assert(std::popcount(static_cast<uint8_t>(startPipe)) == 2);

    return std::make_tuple(pipes,startPos, startDir);
}


int main()
try
{
    auto [pipes,startPos, startDir] = getPipes();


    Pos      walkPos{startPos};
    Vector   walkDir{startDir};

    int     steps{0};

    do
    {
        walkPos += walkDir;

        walkDir = next(walkDir, pipes[walkPos].pipe);

        steps++;

        if(walkPos != startPos)
        {
            pipes[walkPos].distance = steps;
        }

    }while(walkPos!=startPos);

    std::print("Part 1 : {}\n",steps/2);

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
