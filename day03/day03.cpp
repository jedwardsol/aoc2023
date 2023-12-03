#include <print>
#include <map>

#include "include/codeAnalysis.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/posVector-RC.h"
#include "include/int.h"



using Symbols = std::map<Pos, char>;

struct Number
{
    int number;
    int len;
};

using Numbers = std::map<Pos, Number>;


auto parse()
{
    Symbols     symbols;
    Numbers     numbers;

    auto const  lines = getDataLines();

    for(int row = 0; row < isize(lines); row++)
    {
        auto& line = lines[row];

        for(int col = 0; col < isize(line); )
        {
            if(std::isdigit(line[col]))
            {
                char const  *start{ &line[col] };
                char        *end{};

                numbers[{row, col}] = {std::strtol(start, &end, 10), static_cast<int>(end - start)};

                col += static_cast<int>(end - start);
            }
            else if(line[col]!='.')
            {
                symbols[{row, col}] = line[col];
                col++;
            }
            else
            {
                col++;
            }
        }
    }

    return std::make_pair(symbols, numbers);
}

// sum numbers adjacent to a symbol
auto isAdjacent(Symbols const &symbols, Pos pos, int len)
{
    for(int row=pos.row-1; row <= pos.row+1;row++)
    {
        for(int col=pos.col-1; col <= pos.col+len;col++)
        {
            if(symbols.contains({row,col}))
            {
                return true;
            }
        }
    }

    return false;
}


auto part1(Symbols const &symbols, Numbers const &numbers)
{
    int sum{};

    for(auto [pos,number] : numbers)
    {
//      std::print("{} : {}\n",number.number,isAdjacent(symbols,pos,number.len));
        if(isAdjacent(symbols,pos,number.len))
        {
            
            sum+=number.number;            
        }
    }

    return sum;
}


int main()
try
{
    auto [symbols, numbers] = parse();

    std::print("Part 1 = {}\n", part1(symbols,numbers));

}
catch(std::exception const& e)
{
    std::print("{}", e.what());
}


#include <sstream>

// --------------------------
std::istringstream testInput{
R"(467..114..
...*......
..35..633.
......#...
617*......
.....+.58.
..592.....
......755.
...$.*....
.664.598..
)" };
