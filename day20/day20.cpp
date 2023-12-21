#include <print>
#include <map>
#include <variant>
#include <queue>
#include <ranges>
#include <numeric>

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

    /* part 2

       rx is fed only from jq
       jq is a conjunction fed by 

        &jq -> rx

        &gt -> jq
        &vr -> jq
        &nl -> jq
        &lr -> jq


        jq will send a low when      it receives a high and all inputs from all sources are high

        jq receives

            3888 0 vr
            3888 1 lr
            3888 0 nl
            3888 0 gt
            3888 0 lr
            3888 0 lr
            3888 0 lr
            3888 0 lr
            3888 0 lr
            3888 0 lr
            3888 0 lr

            3906 1 vr
            3906 0 lr
            3906 0 nl
            3906 0 gt
            3906 0 vr
            3906 0 vr
            3906 0 vr
            3906 0 vr
            3906 0 vr
            3906 0 vr
            3906 0 vr

            3910 0 vr
            3910 0 lr
            3910 0 nl
            3910 1 gt
            3910 0 vr
            3910 0 gt
            3910 0 gt
            3910 0 gt
            3910 0 gt
            3910 0 gt
            3910 0 gt
            3910 0 gt
            3910 0 gt


        ie.  multiple messages from each source each tick.

        But the highs are in the 1st 4.  So assume that in some tick we'll see

            ???? 1 vr
            ???? 1 lr
            ???? 1 nl
            ???? 1 gt
            ???? 0 vr
            ???? 0 lr
            ???? 0 nl
            ???? 0 gt


            lr 1st @ 3888
            vr 1st @ 3906
            gt 1st @ 3910
            nl 1st @ 4002
            lr 2nd @ 7777.  Period 3889
            vr 2nd @ 7813.  Period 3907
            gt 2nd @ 7821.  Period 3911
            nl 2nd @ 8005.  Period 4003

        

    */


    for(int i=0;;i++)
    {
        addPulse({ Pulse::Type::low, "Button", "broadcaster"});

        while(not queue.empty())
        {
            auto pulse = queue.front();
            queue.pop();

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
                                exit(1);
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


#include <sstream>

// --------------------------
std::istringstream testInput{
R"(broadcaster -> a
%a -> inv, con
&inv -> b
%b -> con
&con -> output
)"};
