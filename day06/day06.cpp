#include <print>

#include "include/codeAnalysis.h"
#include "include/thrower.h"


struct Race
{
    int time;
    int record;
};


//Time:        35     69     68     87
//Distance:   213   1168   1086   1248

Race const races[]
{
    {35, 213},
    {69,1168},
    {68,1086},
    {87,1248}
};


//Time:      7  15   30
//Distance:  9  40  200

Race const testRaces[]
{
    {7,9},
    {15,40},
    {30,200},
};



int main()
try
{
    {
        auto part1{1};

        for(auto &race : races)
        {
            auto wins{0};

            for(int i=0;i<race.time;i++)
            {
                auto distance = i * (race.time-i);
            
                if(distance > race.record)
                {
                    wins++;
                }
            }

            part1*=wins;

        }


        std::print("Part 1 : {}\n",part1);        
    }


//Time:        35     69     68     87
//Distance:   213   1168   1086   1248

    // bah humbug to maths,  just do it ..
    {
        auto wins{0};

        auto time    =          35'696'887ll;
        auto record  = 213'116'810'861'248ll;

        for(auto i=0;i<time;i++)
        {
            auto distance = i * (time-i);
            
            if(distance > record)
            {
                wins++;
            }
        }

        std::print("Part 2 : {}\n",wins);        
    }
}
catch(std::exception const &e)
{
    std::print("{}",e.what());
}


