#include <print>
using std::print;
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
    Vector      U{ -1, 0 };
    Vector      L{ 0,-1 };
    Vector      D{ 1, 0 };
    Vector      R{ 0, 1 };
};


enum  class Pipe : uint8_t
{
    empty = 0,    //  .
    LR = L | R,    //  -
    UD = U | D,    //  |
    UR = U | R,    //  L
    UL = U | L,    //  J
    DR = D | R,    //  F
    DL = D | L,    //  7
    start = 0b10000000,
};


bool operator&(Pipe lhs, Dir rhs) noexcept
{
    return   static_cast<uint8_t>(lhs)
        & static_cast<uint8_t>(rhs);
}

Pipe &operator|=(Pipe &lhs, Dir rhs)
{
    lhs = static_cast<Pipe>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));

    return lhs;
}


struct Tile
{
    Pipe    pipe;
    bool    realPipe;
    bool    inside;
};


namespace Parse
{

    auto getPipes()
    {
        auto pipe = [](char c) -> Pipe
            {
                switch (c)
                {
                case 'S': return Pipe::start;
                case '.': return Pipe::empty;
                case '-': return Pipe::LR;
                case '|': return Pipe::UD;
                case 'L': return Pipe::UR;
                case 'J': return Pipe::UL;
                case 'F': return Pipe::DR;
                case '7': return Pipe::DL;
                default:  throw_runtime_error("bad input");
                }
            };


        auto const   lines = getDataLines();
        auto const   rows = isize(lines);
        auto const   cols = isize(lines[0]);

        Grid<Tile>   pipes{ cols,rows };
        Pos          startPos{};

        for (int row = 0; row < rows; row++)
        {
            for (int col = 0; col < cols; col++)
            {
                pipes[row][col] = { pipe(lines[row][col]), false, false };

                if (pipes[row][col].pipe == Pipe::start)
                {
                    startPos = Pos{ row,col };
                }
            }
        }

        // deduce what pipe is at the start  (assumes it isn't at the edge)

        Pipe        startPipe{ Pipe::empty };
        Vector      startDir{};

        if (pipes[startPos + Move::U].pipe & Dir::D) { startPipe |= Dir::U; startDir = Move::U; }
        if (pipes[startPos + Move::L].pipe & Dir::R) { startPipe |= Dir::L; startDir = Move::L; }
        if (pipes[startPos + Move::D].pipe & Dir::U) { startPipe |= Dir::D; startDir = Move::D; }
        if (pipes[startPos + Move::R].pipe & Dir::L) { startPipe |= Dir::R; startDir = Move::R; }

        pipes[startPos].pipe = startPipe;

        assert(std::popcount(static_cast<uint8_t>(startPipe)) == 2);

        return std::make_tuple(pipes, startPos, startDir);
    }

}



namespace Part1
{


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

        for (auto const &move : moves)
        {
            if (current == move.current
                && pipe == move.pipe)
            {
                return move.next;
            }
        }

        throw_runtime_error("no move");
    }


    int walkPipe(Grid<Tile> &pipes, Pos const &startPos, Vector const &startDir)
    {
        Pos      walkPos{ startPos };
        Vector   walkDir{ startDir };

        int     steps{ 0 };

        pipes[walkPos].realPipe = true;

        do
        {
            walkPos += walkDir;

            walkDir = next(walkDir, pipes[walkPos].pipe);

            steps++;

            pipes[walkPos].realPipe = true;

        } while (walkPos != startPos);

        return steps;
    }


}



namespace Part2
{

    bool isUpCorner(Pipe pipe)
    {
        return   pipe == Pipe::UL
               || pipe == Pipe::UR;
    }

    bool isDownCorner(Pipe pipe)
    {
        return    pipe == Pipe::DL
               || pipe == Pipe::DR;
    }

    bool isCorner(Pipe pipe)
    {
        return    isUpCorner(pipe)
               || isDownCorner(pipe);
    }



    int countInside(std::span<Tile>  row)
    {
        int     insideCount{ 0 };
        bool    inside{ false };
        int     col{ 0 };


        while (col < row.size())
        {

            // step over this range of non-pipes
            while (col < row.size()
                && row[col].realPipe == false)
            {
                if (inside)
                {
                    insideCount++;
                    row[col].inside = true;
                }

                col++;
            }


            // check for end of row
            if (col == row.size())
            {
                break;
            }

            // hit a real vertical pipe  or a real corner
            if (row[col].pipe == Pipe::UD)
            {
                // vertical pipe : toggle state,  and don't count
                inside = !inside;
                col++;
            }
            else
            {
                assert(isCorner(row[col].pipe));

                auto cornerPhase = isUpCorner(row[col].pipe);

                // corner pipe : maintain state,  walk to end of section (the next corner)
                do
                {
                    col++;
                } while (!isCorner(row[col].pipe));

                if (isUpCorner(row[col].pipe) != cornerPhase)
                {
                    // counts as a vertical pipe 
                    inside = !inside;
                }

                col++;
            }
        }

        return insideCount;
    }

}



int main()
try
{
    SetConsoleOutputCP(CP_UTF8);

    auto [pipes, startPos, startDir] = Parse::getPipes();

    auto steps = Part1::walkPipe(pipes, startPos, startDir);             // marks the real pipe


    std::print("Part 1 : {}\n", steps / 2);

    int     insideCount{};

    for (int row = 0; row < pipes.height; row++)
    {
        insideCount += Part2::countInside(pipes[row]);
    }

    std::print("Part 2 : {}\n", insideCount);


    for (int row = 0; row < pipes.height; row++)
    {
        for (int col = 0; col < pipes.width; col++)
        {
            if (pipes[row][col].realPipe)
            {
                switch (pipes[row][col].pipe)
                {
                case Pipe::UD:
                    print("│");
                    break;

                case Pipe::LR:
                    print("─");
                    break;

                case Pipe::UR:
                    print("└");
                    break;

                case Pipe::UL:
                    print("┘");
                    break;


                case Pipe::DR:
                    print("┌");
                    break;

                case Pipe::DL:
                    print("┐");
                    break;
                }
            }
            else if (pipes[row][col].inside)
            {
                std::print("I");
            }
            else
            {
                std::print(" ");
            }
        }
        std::print("\n");
    }



}
catch (std::exception const &e)
{
    std::print("{}", e.what());
}


#include <sstream>

// --------------------------
std::istringstream testInput{
R"(
)" };
