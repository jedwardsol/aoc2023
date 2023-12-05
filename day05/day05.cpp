#include <print>

#include "include/codeAnalysis.h"
#include "include/thrower.h"
#include "include/getdata.h"


struct Seed
{
    int64_t     seed;
    int64_t     soil;
    int64_t     fertilizer;
    int64_t     water;
    int64_t     light;
    int64_t     temperature;
    int64_t     humidity;
    int64_t     location;
};

struct Range
{
    int64_t     destinationStart;
    int64_t     sourceStart;
    int64_t     length;
};



auto readSeeds(std::vector<std::string> const &lines)
{
    std::vector<Seed>       seeds;

    return seeds;
}

auto readRanges(std::vector<std::string> const &lines,std::string_view  mapName)
{
    std::vector<Range>       ranges;

    return ranges;
}


int main()
try
{
    auto    lines=getDataLines();

    auto    seeds                =readSeeds(lines);
    auto    seed2soil            =readRanges(lines,"seed-to-soil map:");
    auto    soil2fertilizer      =readRanges(lines,"soil-to-fertilizer map:");
    auto    fertilizer2water     =readRanges(lines,"fertilizer-to-water map:");
    auto    water2light          =readRanges(lines,"water-to-light map:");
    auto    light2temperature    =readRanges(lines,"light-to-temperature map:");
    auto    temperature2humidity =readRanges(lines,"temperature-to-humidity map:");
    auto    humidity2location    =readRanges(lines,"humidity-to-location map:");


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
