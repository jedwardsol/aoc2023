#pragma once

#include "thrower.h"
#include <fstream>
#include <string>
#include <string_view>
#include <sstream>
#include <spanstream>
#include <filesystem>
#include <source_location>
#include <ranges>
namespace fs=std::filesystem;
using namespace std::literals;

#include "stringstuff.h"

struct TestData{};
using TestInput=TestData;
extern std::istringstream testInput;


[[nodiscard]] inline auto getDataFileName(std::source_location const &sourceLocation = std::source_location::current())
{
    return fs::path{sourceLocation.file_name()}.replace_extension(".txt");
}




///--- As a stream

[[nodiscard]] inline auto getDataStream(std::source_location const &sourceLocation = std::source_location::current())
{
    auto dataFilename = getDataFileName(sourceLocation);

    std::ifstream file{dataFilename};

    if(!file)
    {
        throw_system_error(dataFilename.string());
    }

    return file;
}


[[nodiscard]] inline auto &getDataStream(TestData)
{
    return testInput;
}


///--- As a single line

[[nodiscard]] inline auto getDataLine(TestData)
{
    extern std::istringstream testInput;

    return testInput.str();
}

[[nodiscard]] inline auto getDataLine(std::source_location const &sourceLocation = std::source_location::current())
{
    auto file{getDataStream(sourceLocation)};

    std::string line;

    std::getline(file,line);

    return line;
}


///--- As a vector of lines

[[nodiscard]] inline auto getDataLines(TestData)
{
    std::vector<std::string>    data;
    std::string line;

    while(std::getline(testInput,line))
    {
        data.push_back(line);
    }

    return data;
}


[[nodiscard]] inline auto getDataLines(std::source_location const &sourceLocation = std::source_location::current())
{
    auto file{getDataStream(sourceLocation)};

    std::vector<std::string>    data;
    std::string                 line;


    while(std::getline(file,line))
    {
        data.push_back(line);
    }

    return data;
}




///--- As a vector of ints  (each number on its own line)

[[nodiscard]] inline auto getDataInts(std::vector<std::string> const &lines)
{
    return   lines 
           | std::views::transform([](auto &s) {return std::stoi(s);})
           | std::ranges::to<std::vector>();
}

[[nodiscard]] inline auto getDataInts(TestData)
{
    return getDataInts(getDataLines(TestData{}));    
}


[[nodiscard]] inline auto getDataInts(std::source_location const &sourceLocation = std::source_location::current())
{
    return getDataInts(getDataLines(sourceLocation));    
}



///--- As a vector of vector of ints  (each line a list of numbers)

[[nodiscard]] inline auto getDataGridOfInts(std::vector<std::string> const &lines)
{
    std::vector<std::vector<int64_t>> data;

    for(std::string_view line : lines)
    {
        data.push_back(  std::views::split(line," "sv)
                       | std::views::transform([](auto const &rng) { return rangeToSV(rng);})
                       | std::views::transform([](auto const &sv)  { return stoll(sv);})
                       | std::ranges::to<std::vector>());
    }

    return data;
}

[[nodiscard]] inline auto getDataGridOfInts(TestData)
{
    return getDataGridOfInts(getDataLines(TestData{}));    
}


[[nodiscard]] inline auto getDataGridOfInts(std::source_location const &sourceLocation = std::source_location::current())
{
    return getDataGridOfInts(getDataLines(sourceLocation));    
}

