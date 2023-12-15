#include <print>
#include <algorithm>
#include <numeric>
using std::print;

#include "include/codeAnalysis.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/stringstuff.h"


struct Hash
{
    auto operator()(std::string_view step) const
    {
        uint8_t     hash{};

        for(auto c: step)
        {
            hash = (hash + c) * 17;
        }

        return hash;
    }
};

int main()
try
{
    auto const line  = getDataLine();

    auto       steps =  std::views::split(line,","sv)
                      | std::views::transform([](auto const &rng) { return rangeToSV(rng);})
                      | std::ranges::to<std::vector>();

    auto       part1 = std::ranges::fold_left(steps | std::views::transform(Hash{}) , 0, std::plus<>{});

    print("Part1 : {}\n",part1);        


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
