#include <print>
#include <queue>

#include "include/codeAnalysis.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/posVector-RC.h"
#include "include/int.h"

using HeatMap           = Grid<int8_t>;

constexpr auto veryHot  = std::numeric_limits<int>::max();


enum Dir

{
    up,down,left,right
};


struct Block
{
    int                     heat;
    std::array<bool,4>      visitedFrom;
};

using LowestTemp  = Grid<Block>;


struct Candidate
{
    Pos                     pos{};
    int                     heat;
    Dir                     arrivingFrom;



    friend auto operator<(Candidate const &lhs,Candidate const &rhs)
    {
        return lhs.heat > rhs.heat;
    }
};


using Fringe  = std::priority_queue<Candidate>;






namespace Parse
{

auto readHeatMap()
{
    auto lines = getDataLines();

    HeatMap    heatMap{isize(lines[0]), isize(lines)};

    for(int i=0;i<heatMap.height;i++)
    {
        std::ranges::transform(lines[i],heatMap[i].begin(), [](char c){return c - '0';});
    }

    return heatMap;
}

}



auto getDestinations(Candidate const &here, HeatMap const &heatMap)
{
    struct Destination
    {
        Pos     pos;
        int     heatGain;
        Dir     arrivingFrom;
    };

    std::vector<Destination>   destinations;


    auto emplace = [&](Pos destination, int accumulatedHeat, Dir arrivingFrom)
    {
        if(destination == Pos{3,0})__debugbreak();
        destinations.emplace_back(destination, accumulatedHeat, arrivingFrom );
    };

    if(     here.arrivingFrom == up
       ||   here.arrivingFrom == down)
    {
        int accumulatedHeat{};
        for(int col=1;col<=3;col++)
        {
            auto destination = here.pos + Vector{0,col};

            if(heatMap.inGrid(destination))
            {
                accumulatedHeat += heatMap[destination];
                emplace(destination, accumulatedHeat, right );
            }
        }

        accumulatedHeat=0;
        for(int col=-1;col>=-3;col--)
        {
            auto destination = here.pos + Vector{0,col};

            if(heatMap.inGrid(destination))
            {
                accumulatedHeat += heatMap[destination];
                emplace(destination, accumulatedHeat, left );
            }
        }
    }
    else 
    {
        int accumulatedHeat{};
        for(int row=1;row<=3;row++)
        {
            auto destination = here.pos + Vector{row,0};

            if(heatMap.inGrid(destination))
            {
                accumulatedHeat += heatMap[destination];
                emplace(destination, accumulatedHeat, down );
            }
        }

        accumulatedHeat=0;
        for(int row=-1;row>=-3;row--)
        {
            int accumulatedHeat{};
            auto destination = here.pos + Vector{row,0};

            if(heatMap.inGrid(destination))
            {
                accumulatedHeat += heatMap[destination];
                emplace(destination, accumulatedHeat, up );
            }
        }
    }


    return destinations;
}




int shortestPath(HeatMap  const &heatMap)
{
    auto  const     end              = Pos{heatMap.width-1, heatMap.height-1};
    auto            fringe           = Fringe{};
    auto            lowestTemp       = LowestTemp{heatMap.width, heatMap.height, {veryHot,{}}};

    lowestTemp[0][0]={0,{true,true,true,true}};
    fringe.push({{0,0},0, right});
    fringe.push({{0,0},0, down});

    while(   lowestTemp[end].heat == veryHot
          && not fringe.empty())
    {
        auto const candidate=fringe.top();
        fringe.pop();

        if(candidate.pos == Pos{3,0}) __debugbreak();

        lowestTemp[candidate.pos].heat                                = std::min(candidate.heat,lowestTemp[candidate.pos].heat);
        lowestTemp[candidate.pos].visitedFrom[candidate.arrivingFrom] = true;

        auto destinations = getDestinations(candidate, heatMap);

        for(auto destination : destinations)
        {
            if(! lowestTemp[destination.pos].visitedFrom[candidate.arrivingFrom])
            {
                auto newHeat = candidate.heat + destination.heatGain;

                if(newHeat < lowestTemp[destination.pos].heat)
                {
                    lowestTemp[destination.pos].heat = newHeat;

                    fringe.push({destination.pos, lowestTemp[destination.pos].heat});
                }
            }
        }
    }


    for(int r = 0;r <lowestTemp.height; r++)
    {
        auto row = lowestTemp[r];

        for(auto e : row)
        {
            std::print("{:3} ",e.heat);
        }
        std::print("\n");


    }



    return lowestTemp[end].heat;
}






int main()
try
{
    auto const heatMap = Parse::readHeatMap();

    std::print ("Part 1 : {}\n",shortestPath(heatMap));
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
