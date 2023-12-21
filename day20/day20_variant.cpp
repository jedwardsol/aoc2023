#include <print>
#include <map>
#include <variant>
#include <queue>
#include <ranges>
#include <numeric>
#include <string_view>
using namespace std::literals;

template<class... Ts> 
struct Overload : Ts... 
{ 
    using Ts::operator()...; 
};

#include "include/codeAnalysis.h"
#include "include/thrower.h"
#include "include/stringstuff.h"


namespace Variant
{

using ModuleName = std::string;         // we're going to do a boat-load of copies!

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
    State                               state{};
    std::vector<ModuleName>             destinations;
};


struct Source
{
    Pulse::Type                         lastSeen;
    int                                 firstHigh{};
    int                                 secondHigh{};
    int                                 period{};
};


struct Conjunction 
{
    ModuleName                          name;
    std::map<ModuleName,Source>         sources;
    std::vector<ModuleName>             destinations;
};


struct Broadcaster
{
    ModuleName                          name;
    std::vector<ModuleName>             destinations;
};


// might be better with inheritence. I'm going off std::variant.
using Module = std::variant<FlipFlop,Conjunction,Broadcaster>;

using Pulses = std::queue<Pulse>;


namespace Parse
{

auto parse(std::vector<std::string>   const lines)
{
    std::map<ModuleName, Module>    modules;


    auto get = [](std::string_view destinations)     
    {
        return   std::views::split(destinations,", "sv)
               | std::views::transform([](auto const &rng) { return rangeToStr(rng);})
               | std::ranges::to<std::vector>();
    };



    for(auto &line : lines)
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

    // need to tell Conjunctions about all the module that feed them

    auto fill = [&](auto &module)
    {
        for(auto &destination : module.destinations)
        {
            auto &destinationModule = modules[destination];

            auto conjunction        = std::get_if<Conjunction>(&destinationModule);

            if(conjunction)
            {
                conjunction->sources[module.name] = Source{Pulse::Type::low,0,0};
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


}
using namespace Variant;




void withVariant    (std::vector<std::string>   const lines)
try
{
    auto system         = Parse::parse(lines);
    auto queue          = Pulses{};
    auto lowPulseCount  = int{};
    auto highPulseCount = int{};

    auto addPulse   =  [&](Pulse pulse)
    {
        if(pulse.type == Pulse::Type::low)
        {
            lowPulseCount++;
        }
        else
        {
            highPulseCount++;
        }

        queue.push(pulse);
    };



    for(int i=0;;i++)
    {
        addPulse({ Pulse::Type::low, "Button", "broadcaster"});

        while(not queue.empty())
        {
            auto pulse = queue.front();
            queue.pop();


            auto &destinationModule = system[pulse.destination];

            auto broadcaster = [&](Broadcaster &broadcaster)
            {
                for(auto &destination : broadcaster.destinations)
                {
                    addPulse({pulse.type, broadcaster.name,destination});
                }
            };

            auto flipflop = [&](FlipFlop &flipflop)
            {
                if(pulse.type == Pulse::Type::low)
                {
                    if(flipflop.state == FlipFlop::State::off)
                    {
                        flipflop.state = FlipFlop::State::on;

                        for(auto &destination : flipflop.destinations)
                        {
                            addPulse({Pulse::Type::high, flipflop.name,destination});
                        }
                    }
                    else
                    {
                        flipflop.state = FlipFlop::State::off;

                        for(auto &destination : flipflop.destinations)
                        {
                            addPulse({Pulse::Type::low, flipflop.name,destination});
                        }
                    }
                }
            };

            auto conjunction= [&](Conjunction &conjunction)
            {
                conjunction.sources[pulse.source].lastSeen = pulse.type;


                if(conjunction.name == "jq")
                {
                    if(pulse.type == Pulse::Type::high)
                    {
                        if(conjunction.sources[pulse.source].firstHigh == 0)
                        {
                            conjunction.sources[pulse.source].firstHigh = i;
                            print("{} 1st @ {}\n",pulse.source,i);
                        }
                        else if(conjunction.sources[pulse.source].secondHigh == 0)
                        {
                            conjunction.sources[pulse.source].secondHigh = i;
                            conjunction.sources[pulse.source].period    = i - conjunction.sources[pulse.source].firstHigh;
                            print("{} 2nd @ {}.  Period {} \n",pulse.source,i,conjunction.sources[pulse.source].period);


                            if(not std::ranges::contains(conjunction.sources | std::views::values, 0, &Source::period))
                            {
                                int64_t lcm = 1;

                                for(auto &[_, source] : conjunction.sources)
                                {
                                    lcm = std::lcm(lcm, source.period);
                                }

                                std::print("Part 2 : {}\n",lcm);   //  237'878'264'003'759
                                throw_runtime_error("Done");
                            }
                        }
                    }
                }


                if(std::ranges::contains(conjunction.sources | std::views::values, Pulse::Type::low, &Source::lastSeen))
                {
                    for(auto &destination : conjunction.destinations)
                    {
                        addPulse({Pulse::Type::high, conjunction.name,destination});
                    }
                }
                else // all high
                {
                    for(auto &destination : conjunction.destinations)
                    {
                        addPulse({Pulse::Type::low, conjunction.name,destination});
                    }
                }
            };

            std::visit( Overload{broadcaster,flipflop,conjunction},destinationModule);
        }

        if(i==999)
        {
            std::print("Part 1 : {}*{}={}\n",lowPulseCount,highPulseCount,lowPulseCount*highPulseCount);
        }

    }


}
catch(std::exception const &e)
{
    std::print("{}",e.what());
}


