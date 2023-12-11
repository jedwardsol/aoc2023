#include <print>
#include <map>

#include "include/codeAnalysis.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/posVector-RC.h"
#include "include/int.h"


struct Number
{
    int     number{};
    int     len{};
    bool    nextToSymbol{};
};

struct Symbol
{
    char                symbol{};
    std::vector<int>    adjcentNumbers{};
};


using Symbols = std::map<Pos, Symbol>;
using Numbers = std::map<Pos, Number>;


auto parse()
{
    Symbols     symbols;
    Numbers     numbers;

    auto const  lines = getDataLines();

    for (int row = 0; row < isize(lines); row++)
    {
        auto &line = lines[row];

        for (int col = 0; col < isize(line); )
        {
            if (std::isdigit(line[col]))
            {
                char const *start{ &line[col] };
                char *end{};

                numbers[{row, col}] = { std::strtol(start, &end, 10), static_cast<int>(end - start) };

                col += static_cast<int>(end - start);
            }
            else if (line[col] != '.')
            {
                symbols[{row, col}] = { line[col] };
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


void findAdjacencies(Symbols &symbols, Numbers &numbers)
{
    for (auto &[pos, number] : numbers)
    {
        for (int row = pos.row - 1; row <= pos.row + 1; row++)
        {
            for (int col = pos.col - 1; col <= pos.col + number.len; col++)
            {
                if (symbols.contains({ row,col }))
                {
                    number.nextToSymbol = true;
                    symbols[{row, col}].adjcentNumbers.push_back(number.number);
                }
            }
        }
    }
}


// sum numbers adjacent to a symbol
auto part1(Numbers const &numbers)
{
    int sum{};

    for (auto [pos, number] : numbers)
    {
        if (number.nextToSymbol)
        {
            sum += number.number;
        }
    }

    return sum;
}


// gears are '*' next to 2 numbers.  return sum of products of gears' neighbours
auto part2(Symbols const &symbols, Numbers const &numbers)
{
    int sum{};

    for (auto &[pos, symbol] : symbols)
    {
        if (symbol.symbol == '*'
            && symbol.adjcentNumbers.size() == 2)
        {
            sum += symbol.adjcentNumbers[0]
                * symbol.adjcentNumbers[1];
        }
    }

    return sum;
}




int main()
try
{
    auto [symbols, numbers] = parse();

    findAdjacencies(symbols, numbers);

    std::print("Part 1 = {}\n", part1(numbers));
    std::print("Part 2 = {}\n", part2(symbols, numbers));

}
catch (std::exception const &e)
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
