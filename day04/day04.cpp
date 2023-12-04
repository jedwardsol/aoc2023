#include <print>
#include <set>
#include <spanstream>

#include "include/codeAnalysis.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/stringstuff.h"


struct Card
{
    int             count{};
    std::set<int>   winners;
    std::set<int>   numbers;
};

std::set<int> extract(std::string_view numbers)
{
    std::set<int>     set;
    std::ispanstream    stream{ numbers };

    int     i;

    while(stream >> i)
    {
        set.insert(i);
    }

    return set;
}


Card parse(std::string_view  line)
{
    auto [discard, useful] = splitIn2(line, ':');
    auto [winners, numbers] = splitIn2(useful, '|');

    return
    {
        1,
        extract(winners),
        extract(numbers)
    };
}


std::vector<Card> parse()
{
    std::vector<Card>   cards;

    for(auto const& line : getDataLines())
    {
        cards.push_back(parse(line));
    }

    return cards;
}


int score(Card const& card)
{
    int score{};

    for(auto number : card.numbers)
    {
        if(card.winners.contains(number))
        {
            if(score)
            {
                score *= 2;
            }
            else
            {
                score = 1;
            }
        }
    }

    return score;
}

int part1(std::vector<Card> const& cards)
{
    int total{};

    for(auto& card : cards)
    {
        total += score(card);
    }

    return total;
}


int main()
try
{
    auto cards = parse();

    std::print("Part 1 : {}\n", part1(cards));

}
catch(std::exception const& e)
{
    std::print("{}", e.what());
}


#include <sstream>

// --------------------------
std::istringstream testInput{
R"(
)" };
