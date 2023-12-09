#pragma once

#include "thrower.h"
#include <fstream>
#include <deque>
#include <string>
#include <sstream>
#include <spanstream>
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




///--- As a vector of ints  (each number on its own line)

[[nodiscard]] inline auto getDataInts(std::vector<std::string> const &lines)
{
    std::vector<int> data;

    for(auto const &line : lines)
    {
        data.push_back(std::stoi(line));
    }

    return data;
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

    for(auto const &line : lines)
    {
        std::vector<int64_t>    ints;
        std::ispanstream        stream(line);

        std::copy(std::istream_iterator<int64_t>(stream), 
                  std::istream_iterator<int64_t>{},           
                  std::back_inserter( ints ) );

        data.push_back(ints);
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

