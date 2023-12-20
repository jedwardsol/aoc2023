#include <print>
#include <map>
#include <variant>
#include <queue>

template<class... Ts> 
struct Overload : Ts... 
{ 
    using Ts::operator()...; 
};

#include "include/codeAnalysis.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/stringstuff.h"

using ModuleName = std::string;

struct Pulse
{
    enum class Type
    {
        low, high
    };

    Type                                type;
    ModuleName                          source;
    ModuleName                          destination;
};


struct FlipFlop
{
    enum class State
    {
        off,on
    };

    ModuleName                          name;
    State                               state;
    std::vector<ModuleName>             destinations;
};


struct Conjunction 
{
    ModuleName                          name;
    std::map<ModuleName,Pulse::Type>    sources;
    std::vector<ModuleName>             destinations;
};


struct Broadcaster
{
    ModuleName                          name;
    std::vector<ModuleName>             destinations;
};


using Module = std::variant<FlipFlop,Conjunction,Broadcaster>;

using Pulses = std::queue<Pulse>;


namespace Parse
{

auto parse()
{
    std::map<ModuleName, Module>    modules;


    auto get = [](std::string_view destinations)     
    {
        return   std::views::split(destinations,", "sv)
               | std::views::transform([](auto const &rng) { return rangeToStr(rng);})
               | std::ranges::to<std::vector>();
    };



    for(auto &line : getDataLines(TestData{}))
    {
        auto [name, destinations] = splitIn2(line," -> ");

        if(name == "broadcaster")
        {
            modules[std::string{name}] = Broadcaster{ std::string{name}, get(destinations) };
        }
        else if(name[0] == '%')
        {
            name.remove_prefix(1);
            modules[std::string{name}] = FlipFlop { std::string{name}, FlipFlop::State::off, get(destinations) };
        }
        else if(name[0] == '&')
        {
            name.remove_prefix(1);
            modules[std::string{name}] = Conjunction { std::string{name}, {} , get(destinations) };
        }
    }


    auto fill = [&](auto &module)
    {
        for(auto &destination : module.destinations)
        {
            auto &destinationModule = modules[destination];

            auto conjunction        = std::get_if<Conjunction>(&destinationModule);

            if(conjunction)
            {
                conjunction->sources[module.name] = Pulse::Type::low;
            }
        }
    };

    for(auto &[name, module] : modules)
    {
        std::visit(Overload{fill},module);
    }


    return modules;
}


}



int main()
try
{
    auto  system = Parse::parse();


}
catch(std::exception const &e)
{
    std::print("{}",e.what());
}


#include <sstream>

// --------------------------
std::istringstream testInput{
R"(broadcaster -> a
%a -> inv, con
&inv -> b
%b -> con
&con -> output
)"};
