#include <print>
#include <set>
using std::print;
#include <ranges>

#include "include/codeAnalysis.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/posVector-RC.h"


using Galaxies = std::set<Pos>;


auto readGalaxies()
{
    auto const lines    = getDataLines();
    auto       galaxies = Galaxies{};

    for(int row = 0;row <lines.size();row++)
    {
        for(int col=0;col<lines[row].size();col++)
        {
            if(lines[row][col] == '#')
            {
                galaxies.emplace(row,col);
            }
        }
    }

    return galaxies;
}


auto findDarkMatter(Galaxies const &galaxies)
{
    auto numRows   = std::ranges::max_element(galaxies,{}, &Pos::row)->row;
    auto numCols   = std::ranges::max_element(galaxies,{}, &Pos::col)->col;

    auto emptyRows = std::views::iota(0,numRows) | std::ranges::to<std::set>();
    auto emptyCols = std::views::iota(0,numCols) | std::ranges::to<std::set>();

    for(auto &galaxy : galaxies)
    {
        emptyRows.erase(galaxy.row);
        emptyCols.erase(galaxy.col);
    }

    return std::make_pair(emptyRows,emptyCols);
}


int main()
try
{
    auto galaxies              = readGalaxies();
    auto [emptyRows,emptyCols] = findDarkMatter(galaxies);


    auto total{0ll};        
    // double count for now
    for(auto galaxy1 : galaxies)
    {
        for(auto galaxy2 : galaxies)
        {
            auto  plainDist = (galaxy2 - galaxy1).manhattan();

            auto  rowBounds = std::minmax(galaxy1.row, galaxy2.row);                
            auto  colBounds = std::minmax(galaxy1.col, galaxy2.col);                

            auto  numEmptyRows = std::distance ( std::ranges::lower_bound(emptyRows,rowBounds.first), std::ranges::lower_bound(emptyRows,rowBounds.second));
            auto  numEmptyCols = std::distance ( std::ranges::lower_bound(emptyCols,colBounds.first), std::ranges::lower_bound(emptyCols,colBounds.second));

            total += plainDist + numEmptyRows + numEmptyCols;
        }
    }

    print("Part 1 : {}\n",total/2);


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
