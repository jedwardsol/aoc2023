#include <print>
#include <map>
#include <queue>
#include <ranges>
#include <numeric>
#include <memory>
#include <functional>

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
    int                                 buttonPress;
};

using Pulses = std::queue<Pulse>;

using Transmitter = std::function<void(Pulse)>;



struct PlainModule
{
    PlainModule(std::string name) : name{std::move(name)}
    {}

    PlainModule(std::string name, std::vector<ModuleName> destinations) : name{std::move(name)}, destinations{std::move(destinations)}
    {}


    ModuleName                          name;
    std::vector<ModuleName>             destinations;
    
    virtual void setSource(std::string const &name)
    {
    }

    virtual void receive(Pulse const &pulse, Transmitter transmitter) 
    {
    }


    void broadcast(Transmitter transmitter, Pulse::Type type)
    {
        for(auto &destination : destinations)
        {
            transmitter({type, name,destination});
        }
    }
};


struct Broadcaster : PlainModule
{
    using PlainModule::PlainModule;

    void receive(Pulse const &pulse, Transmitter transmitter) override
    {
        broadcast(transmitter, pulse.type);
    }


};

struct FlipFlop : PlainModule
{
    using PlainModule::PlainModule;

    enum class State
    {
        off,on
    };

    State                               state{State::off};



    void receive(Pulse const &pulse, Transmitter transmitter) override
    {

        if(pulse.type == Pulse::Type::low)
        {
            if(state == FlipFlop::State::off)
            {
                state = FlipFlop::State::on;

                broadcast(transmitter, Pulse::Type::high);
            }
            else
            {
                state = FlipFlop::State::off;

                broadcast(transmitter, Pulse::Type::low);
            }
        }
    }
};


struct Conjunction  : PlainModule
{
    using PlainModule::PlainModule;

    struct Source  
    {
        Pulse::Type                         lastSeen;
        int                                 firstHigh{};
        int                                 secondHigh{};
        int                                 period{};
    };

    std::map<ModuleName,Source>         sources;


    void setSource(std::string const &sourceName) override
    {
        sources[sourceName] = Source{Pulse::Type::low,0,0};
    }


    void receive(Pulse const &pulse, Transmitter transmitter) override
    {
        sources[pulse.source].lastSeen = pulse.type;

        part2(pulse);


        if(std::ranges::contains(sources | std::views::values, Pulse::Type::low, &Source::lastSeen))
        {
            broadcast(transmitter,Pulse::Type::high);
        }
        else // all high
        {
            broadcast(transmitter,Pulse::Type::low);
        }
    }


    void part2(Pulse const &pulse)
    {
        if(name == "jq")
        {
            if(pulse.type == Pulse::Type::high)
            {
                if(sources[pulse.source].firstHigh == 0)
                {
                    sources[pulse.source].firstHigh = pulse.buttonPress;
                    print("{} 1st @ {}\n",pulse.source,pulse.buttonPress);
                }
                else if(sources[pulse.source].secondHigh == 0)
                {
                    sources[pulse.source].secondHigh = pulse.buttonPress;
                    sources[pulse.source].period     = pulse.buttonPress - sources[pulse.source].firstHigh;
                    print("{} 2nd @ {}.  Period {} \n",pulse.source,pulse.buttonPress,sources[pulse.source].period);


                    if(not std::ranges::contains(sources | std::views::values, 0, &Source::period))
                    {
                        int64_t lcm = 1;

                        for(auto &[_, source] : sources)
                        {
                            lcm = std::lcm(lcm, source.period);
                        }

                        std::print("Part 2 : {}\n",lcm);   //  237'878'264'003'759
                        throw_runtime_error("Done");
                    }
                }
            }
        }
    }


};


using Module  = std::unique_ptr<PlainModule>;
using Modules = std::map<ModuleName, Module>;




namespace Parse
{

auto parse(std::vector<std::string>   const lines)
{
    Modules     modules;


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
            modules[std::string{name}] = std::make_unique<Broadcaster>(std::string{name}, get(destinations) );
        }
        else if(name[0] == '%')
        {
            name.remove_prefix(1);
            modules[std::string{name}] = std::make_unique<FlipFlop>   (std::string{name}, get(destinations) );
        }
        else if(name[0] == '&')
        {
            name.remove_prefix(1);
            modules[std::string{name}] = std::make_unique<Conjunction>(std::string{name}, get(destinations) );
        }
    }

    modules["rx"] = std::make_unique<PlainModule>("rx");

    // need to tell Conjunctions about all the module that feed them

    for(auto &[name, module] : modules)
    {

        for(auto &destination : module->destinations)
        {
            auto &destinationModule = modules[destination];

            destinationModule->setSource(name);
        }
    }

    return modules;
}


}





void withInheritance (std::vector<std::string>   const lines)
try
{
    auto system         = Parse::parse(lines);
    auto queue          = Pulses{};
    auto lowPulseCount  = int{};
    auto highPulseCount = int{};

    int  buttonPress{};

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

        pulse.buttonPress = buttonPress;

        queue.push(pulse);
    };


    for(buttonPress=0;;buttonPress++)
    {
        addPulse({ Pulse::Type::low, "Button", "broadcaster"});

        while(not queue.empty())
        {
            auto pulse = queue.front();
            queue.pop();

            auto &destinationModule = system[pulse.destination];

            destinationModule->receive(pulse,addPulse);    
        }

        if(buttonPress==999)
        {
            std::print("Part 1 : {}*{}={}\n",lowPulseCount,highPulseCount,lowPulseCount*highPulseCount);
        }

    }


}
catch(std::exception const &e)
{
    std::print("{}",e.what());
}

