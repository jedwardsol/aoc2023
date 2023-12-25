#include <print>
#include <cassert>
#include "include/codeAnalysis.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/posVector-XYZ.h"

/*
            y
            ^
            |       o
            |      /
            |     /
------------+----/-------->x
            |   /
            |  /
            | /
            |/
           c|

*/

struct Trajectory2D
{
    Pos<int64_t>     start;
    Vector<int64_t>  dir;

    // y = mx + c
    double          m;
    double          c;


    Trajectory2D(Pos<int64_t> start, Vector<int64_t> dir) : start{start}, 
                                                            dir  {dir},  
                                                            m    { static_cast<double>(dir.dy) / dir.dx },
                                                            c    { start.y - (m * start.x) }
    {
        assert( dir.dx != 0);
    }


    std::optional<double> xIntercept(Trajectory2D  const &other) const
    {
        //  m1x + c1 = m2x + c2
        //  x(m1-m2) = c2-c1
        //  x= (c2-c1)/(m1-m2)

        auto x =  (other.c - c) / (m - other.m);

        if(std::isfinite(x))
        {
            return x ;
        }
        else 
        {
            return std::nullopt;
        }
    }

    std::optional<double> yIntercept(Trajectory2D  const &other) const
    {
        auto x =  xIntercept(other);

        if(!x)
        {
            return std::nullopt;
        }

        return m*x.value()+c;
    }

    std::optional<double> tIntercept(Trajectory2D  const &other) const
    {
        auto x =  xIntercept(other);

        if(!x)
        {
            return std::nullopt;
        }

        return  (x.value() - start.x) / dir.dx;
    }



};



template <>
struct std::formatter<Trajectory2D> : std::formatter<std::string> 
{
    auto format(Trajectory2D const &p, std::format_context& ctx) const 
    {
        return std::formatter<std::string>::format(std::format("{}, {}, {} @ {}, {}, {}", p.start.x,p.start.y, p.start.z,  p.dir.dx,p.dir.dy,p.dir.dz), ctx);
    }
};


int part1(std::vector<Trajectory2D> const &trajectories,int64_t lower, int64_t upper)
{
    int intercepts{};

    for(int i=0;i<trajectories.size();i++)
    {
        for(int j=i+1;j<trajectories.size();j++)
        {
            auto x  = trajectories[i].xIntercept(trajectories[j]);
            auto y  = trajectories[i].yIntercept(trajectories[j]);
            auto t1 = trajectories[i].tIntercept(trajectories[j]);
            auto t2 = trajectories[j].tIntercept(trajectories[i]);

//          std::print("{}\n{}\n {},{} : {}&{} : ", trajectories[i],trajectories[j],x.value_or(0),y.value_or(0),t1.value_or(0),t2.value_or(0));

            if(x && y && t1 && t2)
            {
                if(   x.value() >= lower
                   && x.value() <= upper
                   && y.value() >= lower
                   && y.value() <= upper
                   && t1 > 0
                   && t2 > 0)
                {
                    intercepts++;
                    std::print("yes\n\n");
                }
                else
                {
                    std::print("no\n\n");
                }
            }
        }
    }

    return intercepts;
}


int main()
try
{
    std::vector<Trajectory2D>  trajectories;

//    for(auto const &line : getDataLines(TestData{}))
    for(auto const &line : getDataLines())
    {
        auto regex   = R"((\d+), (\d+), (\d+) @ (-?\d+), (-?\d+), (-?\d+))";         // 248315803897794, 386127890875011, 326651351825022 @ -89, -119, 32
        auto numbers = bigNumbersFromRegex(line,regex,6);

        auto trajectory = Trajectory2D{Pos   {numbers[0],numbers[1],numbers[2]},
                                       Vector{numbers[3],numbers[4],numbers[5]}};
        
        trajectories.push_back(trajectory);
    }



//    std::print("Part 1 : {}\n",part1(trajectories,7,27));
    std::print("Part 1 : {}\n",part1(trajectories,200'000'000'000'000,400'000'000'000'000));
}
catch(std::exception const &e)
{
    std::print("{}",e.what());
}


#include <sstream>

// --------------------------
std::istringstream testInput{
R"(19, 13, 30 @ -2, 1, -2
18, 19, 22 @ -1, -1, -2
20, 25, 34 @ -2, -2, -4
12, 31, 28 @ -1, -2, -1
20, 19, 15 @ 1, -5, -3
)"};
