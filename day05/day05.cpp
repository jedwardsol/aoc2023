#include <print>
#include <spanstream>
#include <string_view>

using namespace std::literals;

#include "include/codeAnalysis.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/stringstuff.h"
#include "include/stopwatch.h"


struct Range
{
    int64_t     destinationStart;
    int64_t     sourceStart;
    int64_t     length;
};

using Map = std::vector<Range>;

struct Almanac
{
    Map     seed2soil;
    Map     soil2fertilizer;
    Map     fertilizer2water;
    Map     water2light;
    Map     light2temperature;
    Map     temperature2humidity;
    Map     humidity2location;
};


auto readRanges(std::vector<std::string> const &lines, std::string_view  mapName)
{
    auto start = std::ranges::find(lines, mapName);

    if (start == lines.end())
    {
        throw_runtime_error("No section");
    }

    start++;

    Map     ranges;

    while (start != lines.end() && *start != ""sv)
    {
        std::ispanstream    stream{ *start };
        Range               range;

        stream >> range.destinationStart >> range.sourceStart >> range.length;

        ranges.push_back(range);

        start++;
    }

    return ranges;
}


int64_t     convert(int64_t     attribute, Map const &map)
{
    for (auto &range : map)
    {
        if (attribute >= range.sourceStart
            && attribute < range.sourceStart + range.length)
        {
            return range.destinationStart + (attribute - range.sourceStart);
        }
    }

    return attribute;
}

void part1(std::string_view line, Almanac &almanac)  // seeds: 28965817 302170009
{
    auto [header, numbers] = splitIn2(line, ':');

    int64_t             seed{};
    int64_t             minLocation{ std::numeric_limits<int64_t>::max() };

    std::ispanstream    stream{ numbers };

    while (stream >> seed)
    {
        auto soil = convert(seed, almanac.seed2soil);
        auto fertilizer = convert(soil, almanac.soil2fertilizer);
        auto water = convert(fertilizer, almanac.fertilizer2water);
        auto light = convert(water, almanac.water2light);
        auto temperature = convert(light, almanac.light2temperature);
        auto humidity = convert(temperature, almanac.temperature2humidity);
        auto location = convert(humidity, almanac.humidity2location);

        minLocation = std::min(location, minLocation);
    }

    std::print("Part 1  :   {}\n", minLocation);
}


void part2(std::string_view line, Almanac &almanac)  // seeds: 28965817 302170009
{
    auto [header, numbers] = splitIn2(line, ':');

    int64_t             start{};
    int64_t             length{};
    int64_t             minLocation{ std::numeric_limits<int64_t>::max() };
    int64_t             count{};

    std::ispanstream    stream{ numbers };

    Stopwatch           sw;

    while (stream >> start >> length)
    {
        for (auto seed = start; seed < start + length; seed++)
        {
            auto soil = convert(seed, almanac.seed2soil);
            auto fertilizer = convert(soil, almanac.soil2fertilizer);
            auto water = convert(fertilizer, almanac.fertilizer2water);
            auto light = convert(water, almanac.water2light);
            auto temperature = convert(light, almanac.light2temperature);
            auto humidity = convert(temperature, almanac.temperature2humidity);
            auto location = convert(humidity, almanac.humidity2location);

            minLocation = std::min(location, minLocation);
            count++;
        }
    }

    std::print("Part 2  :   {}   ({} seeds in {} seconds)\n", minLocation, count, sw.seconds());

    // Part 2  :   79004094   (1975502102 seeds in 133.305586 seconds)
    // can definitely be cleverer!
}


int main()
try
{
    auto    lines = getDataLines();

    Almanac almanac
    {
        .seed2soil = readRanges(lines,"seed-to-soil map:"),
        .soil2fertilizer = readRanges(lines,"soil-to-fertilizer map:"),
        .fertilizer2water = readRanges(lines,"fertilizer-to-water map:"),
        .water2light = readRanges(lines,"water-to-light map:"),
        .light2temperature = readRanges(lines,"light-to-temperature map:"),
        .temperature2humidity = readRanges(lines,"temperature-to-humidity map:"),
        .humidity2location = readRanges(lines,"humidity-to-location map:"),
    };

    part1(lines[0], almanac);
    part2(lines[0], almanac);
}
catch (std::exception const &e)
{
    std::print("{}", e.what());
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
)" };
