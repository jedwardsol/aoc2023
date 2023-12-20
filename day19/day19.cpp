#include <print>
#include <map>
#include <spanstream>
#include <algorithm>
#include <cassert>

#include "include/codeAnalysis.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/int.h"
#include "include/stringstuff.h"


struct Rule
{
    enum class Condition
    {
        less, greater,always
    };

    int             attribute;          // offset into Part vector
    Condition       condition;
    int             value;
    std::string     trueResult;
};

using Rules = std::vector<Rule>;


struct Workflow
{
    std::string         name;
    Rules               rules;

//    std::string         defaultResult;
};

using Workflows = std::map<std::string, Workflow>;


using Part  = std::vector<int>;      // x m a s

using Parts = std::vector<Part>;



namespace Parse
{


auto parsePart(std::string  const &line)            
{
    auto regex   = R"(\{x=(\d+),m=(\d+),a=(\d+),s=(\d+)\})";                  // {x=494,m=380,a=686,s=2820}

    return numbersFromRegex(line,regex,4);
}


auto parseWorkflow(std::string_view line) -> Workflows::value_type        // qhp{m<484:R,m>597:A,s<2378:R,A}
{
    auto    [name,ruleSv] = splitIn2(line,'{');

    ruleSv.remove_suffix(1);    


    auto rules = split(ruleSv,",");

    auto lastRule =  std::string{rules.back()};
    rules.pop_back();


    Workflow    workflow
    {
        std::string{name},
        {},
    };


    for(auto rule : rules)          // m<484:R
    {
        char        variableName{};
        char        condition{};
        int         value{};
        char        dummy{};
        std::string result;

        std::ispanstream    stream{rule};

        stream >> variableName >> condition >> value >> dummy >> result;

        int         attribute{};

        if(variableName == 'x') attribute = 0;
        if(variableName == 'm') attribute = 1;
        if(variableName == 'a') attribute = 2;
        if(variableName == 's') attribute = 3;


        Rule    rule
        {
            attribute,
            condition == '<' ? Rule::Condition::less : Rule::Condition::greater,
            value,
            result
        };

        workflow.rules.push_back(rule);
    }

    workflow.rules.emplace_back(0, Rule::Condition::always, 0, lastRule);



    return { workflow.name, workflow};
}



auto parse()
{
    auto lines      = getDataLines();
    auto doPart     = bool{false};
    auto workflows  = Workflows{};
    auto parts      = Parts{};

    for(auto &line : lines)
    {
        if(line.empty())
        {
            doPart=true;
        }
        else if(doPart)
        {
            parts.push_back(parsePart(line));
        }
        else
        {
            workflows.insert(parseWorkflow(line));
        }
    }

    return std::make_pair(workflows,parts);
}


}




namespace Part1
{

auto evaluate(Workflow const &workflow, Part const &part)
{

    for(auto &rule : workflow.rules)
    {
        if(rule.condition == Rule::Condition::always)
        {
            return rule.trueResult;
        }
        else if(rule.condition == Rule::Condition::less)
        {
            if( part[rule.attribute] < rule.value)
            {
                return rule.trueResult;
            }
        }
        else 
        {
            if( part[rule.attribute] > rule.value)
            {
                return rule.trueResult;
            }
        }
    }

    std::unreachable();

}


bool accept(Workflows &workflows, Part const &part)
{
    auto workflow = "in"s;

//  std::print("{:4}\n  {}\n",part[0],workflow);

    while(    workflow != "A"
          &&  workflow != "R")
    {
        assert(workflows.contains(workflow));

        workflow = evaluate(workflows[workflow], part);

//      std::print("  {}\n",workflow);
    }

    return workflow == "A";
}


int part1(Workflows &workflows,Parts const &parts)
{
    auto        sum{0};

    for(auto &part : parts)
    {
        if(accept(workflows,part))
        {
            sum +=     std::ranges::fold_left(part, 0, std::plus{});              
        }
    }

    return sum;
}

}


int main()
try
{
    auto [workflows,parts] = Parse::parse();

    std::print("Part 1 : {}",Part1::part1(workflows, parts));



}
catch (std::exception const &e)
{
    std::print("{}", e.what());
}


#include <sstream>

// --------------------------
std::istringstream testInput{
R"(px{a<2006:qkq,m>2090:A,rfg}
pv{a>1716:R,A}
lnx{m>1548:A,A}
rfg{s<537:gd,x>2440:R,A}
qs{s>3448:A,lnx}
qkq{x<1416:A,crn}
crn{x>2662:A,R}
in{s<1351:px,qqz}
qqz{s>2770:qs,m<1801:hdj,R}
gd{a>3333:R,R}
hdj{m>838:A,pv}

{x=787,m=2655,a=1222,s=2876}
{x=1679,m=44,a=2067,s=496}
{x=2036,m=264,a=79,s=2244}
{x=2461,m=1339,a=466,s=291}
{x=2127,m=1623,a=2188,s=1013}
)" };
