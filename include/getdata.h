#pragma once

#include "thrower.h"
#include <fstream>
#include <deque>
#include <string>
#include <sstream>
#include <filesystem>
#include <source_location>
namespace fs=std::filesystem;


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




///--- As a vector of ints

[[nodiscard]] inline auto getDataInts(TestData)
{
    auto lines = getDataLines(TestData{});

    std::vector<int> data;

    for(auto const &line : lines)
    {
        data.push_back(std::stoi(line));
    }

    return data;
}


[[nodiscard]] inline auto getDataInts(std::source_location const &sourceLocation = std::source_location::current())
{
    auto lines = getDataLines(sourceLocation);

    std::vector<int> data;

    for(auto const &line : lines)
    {
        data.push_back(std::stoi(line));
    }

    return data;

}

