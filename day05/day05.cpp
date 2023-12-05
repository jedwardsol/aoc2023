#include <print>
#include <spanstream>
#include <string_view>

using namespace std::literals;

#include "include/codeAnalysis.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/stringstuff.h"

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

using Map = std::vector<Range>;


auto readSeeds(std::string_view line)  // seeds: 28965817 302170009
{
    std::vector<Seed>       seeds;

    auto [header,numbers] = splitIn2(line,':');

    std::ispanstream    stream{numbers};
    Seed                seed{};

    while(stream >> seed.seed)
    {
        seeds.push_back(seed);
    }

    return seeds;
}

auto readRanges(std::vector<std::string> const &lines,std::string_view  mapName)
{
    auto start = std::ranges::find(lines,mapName);

    if(start == lines.end())
    {
        throw_runtime_error("No section");
    }

    start++;

    Map     ranges;

    while(start != lines.end() && *start != ""sv)
    {
        std::ispanstream    stream{*start};
        Range               range;
        
        stream >> range.destinationStart >> range.sourceStart >> range.length;

        ranges.push_back(range);

        start++;
    }

    return ranges;
}


int64_t     convert(int64_t     attribute, Map const &map)
{
    for(auto range : map)
    {
        if(    attribute >= range.sourceStart
           &&  attribute  < range.sourceStart+range.length)
        {
            return range.destinationStart + (attribute-range.sourceStart);
        }
    }

    return attribute;
}


int main()
try
{
    auto    lines                = getDataLines();

    auto    seeds                = readSeeds (lines[0]);
    auto    seed2soil            = readRanges(lines,"seed-to-soil map:");
    auto    soil2fertilizer      = readRanges(lines,"soil-to-fertilizer map:");
    auto    fertilizer2water     = readRanges(lines,"fertilizer-to-water map:");
    auto    water2light          = readRanges(lines,"water-to-light map:");
    auto    light2temperature    = readRanges(lines,"light-to-temperature map:");
    auto    temperature2humidity = readRanges(lines,"temperature-to-humidity map:");
    auto    humidity2location    = readRanges(lines,"humidity-to-location map:");


    int64_t     minLocation{std::numeric_limits<int64_t>::max()};

    for(auto &seed : seeds)
    {
        seed.soil           = convert(seed.seed,        seed2soil);
        seed.fertilizer     = convert(seed.soil,        soil2fertilizer     );
        seed.water          = convert(seed.fertilizer,  fertilizer2water    );
        seed.light          = convert(seed.water,       water2light         );
        seed.temperature    = convert(seed.light,       light2temperature   );
        seed.humidity       = convert(seed.temperature, temperature2humidity);
        seed.location       = convert(seed.humidity,    humidity2location   );

        minLocation = std::min(seed.location,minLocation);
    }

    std::print("Part 1 : {}\n",minLocation);


}
catch(std::exception const &e)
{
    std::print("{}",e.what());
}


#include <sstream>

// --------------------------
std::istringstream testInput{
R"(seeds: 79 14 55 13

seed-to-soil map:
50 98 2
52 50 48

soil-to-fertilizer map:
0 15 37
37 52 2
39 0 15

fertilizer-to-water map:
49 53 8
0 11 42
42 0 7
57 7 4

water-to-light map:
88 18 7
18 25 70

light-to-temperature map:
45 77 23
81 45 19
68 64 13

temperature-to-humidity map:
0 69 1
1 0 69

humidity-to-location map:
60 56 37
56 93 4
)"};
