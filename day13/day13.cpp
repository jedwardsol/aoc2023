#include <print>
using std::print;
#include <bitset>

#include "include/codeAnalysis.h"
#include "include/thrower.h"
#include "include/getdata.h"

struct Grid
{
    std::vector<std::bitset<32>>   rows;
    std::vector<std::bitset<32>>   cols;
};


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
            grid.rows.emplace_back();

            if(rowCount==0)
            {
                for(int i=0;i<line.size();i++)
                {
                    grid.cols.emplace_back();
                }
            }



            for(int colCount=0;colCount<line.size();colCount++)
            {
                if(line[colCount]=='#')
                {
                    grid.rows[rowCount].set(colCount);
                    grid.cols[colCount].set(rowCount);
                }
            }

            rowCount++;
        }
    }

    return grids;
}



int main()
try
{
    auto grids = readGrids();

    print("Part 1 : {}",0);

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
