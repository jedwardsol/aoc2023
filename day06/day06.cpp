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


/*
    L = length of race  (ms)
    d = distance traveled (mm)
    t = time button is pressed for (ms)

        d =  t(L-t)  
        d =  -t² + Lt

    R = record in race
    race is won if

        -t² + Lt > R

        -t² + Lt - R > 0

*/

//  roots =  (-B ±  √(B² - 4AC)) / 2A
std::pair<int64_t,int64_t>  roots(int64_t A, int64_t B, int64_t C)
{
    auto discriminant   = std::sqrt( (B * B) - ( 4 * A * C) );

    auto root1          = (-B +  discriminant) / (2 * A);
    auto root2          = (-B -  discriminant) / (2 * A);

    auto roots          = std::minmax(root1,root2);             // sort

    return  
    { 
        static_cast<int64_t>(std::floor(roots.first)),          // time of last loss at beginning
        static_cast<int64_t>(std::ceil(roots.second)),          // time of first loss at end
    };

}

auto wins(int64_t time, int64_t record)
{
    auto times = roots (-1,  time, -record);

    return (times.second - times.first)-1;
}


int main()
try
{
    auto part1{1ll};

    for(auto &race : races)
    {
        part1*=wins(race.time, race.record);
    }

    std::print("Part 1 : {}\n",part1);       // 170000 

    // part 2 :  single race where the numbers are all mushed together

    //Time:        35     69     68     87
    //Distance:   213   1168   1086   1248
    auto time    =          35'696'887ll;
    auto record  = 213'116'810'861'248ll;
    std::print("Part 2 : {}\n",wins(time,record));        
}
catch(std::exception const &e)
{
    std::print("{}",e.what());
}


