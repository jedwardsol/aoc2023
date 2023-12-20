#include <print>
#include <map>
#include <variant>
#include <queue>
#include <ranges>

template<class... Ts> 
struct Overload : Ts... 
{ 
    using Ts::operator()...; 
};

#include "include/codeAnalysis.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/stringstuff.h"

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



    for(auto &line : getDataLines())
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
    auto system         = Parse::parse();
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

//      print("{} - {} -> {}\n",pulse.source,static_cast<int>(pulse.type),pulse.destination);

        queue.push(pulse);
    };


    for(int i=0;;i++)
    {
        addPulse({ Pulse::Type::low, "Button", "broadcaster"});

        while(not queue.empty())
        {
            auto pulse = queue.front();
            queue.pop();

            // this is one of those ones where you can map out the data and work out what the answer is 
            if(   pulse.type == Pulse::Type::low
               && pulse.destination == "rx")
            {
                std::print("Part 2 : {}\n",i+1);
                return 1;
            }

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
                conjunction.sources[pulse.source] = pulse.type;


                if(std::ranges::contains(conjunction.sources | std::views::values, Pulse::Type::low))
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


#include <sstream>

// --------------------------
std::istringstream testInput{
R"(broadcaster -> a
%a -> inv, con
&inv -> b
%b -> con
&con -> output
)"};
