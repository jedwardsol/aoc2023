#include <print>
using std::print;
#include <algorithm>
#include <numeric>
#include <cassert>

#include "include/codeAnalysis.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/stringstuff.h"


struct Hash
{
    auto operator()(std::string_view s) const
    {
        uint8_t     hash{};

        for(auto c: s)
        {
            hash = (hash + c) * 17;
        }

        return hash;
    }
};


struct Lens
{
    std::string         label;
    int                 focalLength;
};

struct Box
{
    std::vector<Lens>   lenses;

    auto find(std::string  const &label)
    {
        return std::ranges::find(lenses, label, &Lens::label);
    }

    void add(std::string  const &label, int focalLength)
    {
        auto existing = find(label);

        if(existing != lenses.end())
        {
            existing->focalLength = focalLength;
        }
        else
        {
            lenses.emplace_back(label,focalLength);
        }
    }

    void remove(std::string const &label)
    {
        auto existing = find(label);

        if(existing != lenses.end())
        {
            lenses.erase(existing);
        }
    }

};

using Boxes = std::array<Box,256>;


void apply(std::string_view step,Boxes &boxes)
{
    auto    command = step.find_first_of("-=");
    assert(command != step.npos);

    auto    label   = std::string{step.substr(0,command)};
    auto    box     = Hash{}(label);

    if(step[command]=='=')
    {
        auto    number  = stoi(step.substr(command+1));

        boxes[box].add(label,number);
    }
    else
    {
        assert(step[command]=='-');

        boxes[box].remove(label);
    }
}


int main()
try
{
    auto const line  = getDataLine();

    auto       steps =  std::views::split(line,","sv)
                      | std::views::transform([](auto const &rng) { return rangeToSV(rng);})
                      | std::ranges::to<std::vector>();


    print("Part1 : {}\n",std::ranges::fold_left(steps | std::views::transform(Hash{}) , 0, std::plus<>{}));        


    Boxes       boxes{};

    for(auto step : steps)
    {
        apply(step,boxes);
    }


    int     totalPower{};

    for(int box = 0;box < boxes.size(); box++)
    {
        auto &lenses = boxes[box].lenses;
    
        for(int lens = 0; lens < lenses.size(); lens++)
        {
            totalPower +=  (box+1) * (lens+1) * lenses[lens].focalLength;
        }
    }

    print("Part 2 : {}\n", totalPower);            

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
