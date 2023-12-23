#include <print>
#include <algorithm>

#include "include/codeAnalysis.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/stringstuff.h"

struct End
{
    int x,y,z;

    friend bool operator==(End const &lhs,End const &rhs)
    {
        return lhs.z == rhs.z;
    }

    friend auto operator<=>(End const &lhs,End const &rhs)
    {
        return lhs.z <=> rhs.z;
    }
};

struct Brick
{
    End  ends[2];

    friend bool operator== (Brick const &lhs,Brick const &rhs)=default;
    friend auto operator<=>(Brick const &lhs,Brick const &rhs)=default;

};



int main()
try
{
    std::vector<Brick>  bricks;

    for(auto const &line : getDataLines())
    {
        auto regex   = R"((\d+),(\d+),(\d+)~(\d+),(\d+),(\d+))";           // 3,7,269~3,7,270
        auto numbers = numbersFromRegex(line,regex,6);

        auto  brick = Brick{End{numbers[0],numbers[1],numbers[2]},
                            End{numbers[3],numbers[4],numbers[5]}};
        
        if(brick.ends[1] < brick.ends[0])
        {
            std::swap(brick.ends[1],brick.ends[0]);
        }

        bricks.push_back(brick);
    }


    std::ranges::sort(bricks);

    std::print("Part 1 : {}\n",1);
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
