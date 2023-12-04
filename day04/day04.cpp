#include <print>
#include <set>
#include <spanstream>

#include "include/codeAnalysis.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/stringstuff.h"
#include "include/int.h"

struct Card
{
    int64_t         count{};
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


int part1Score(Card const& card)
{
    std::set<int> intersection;

    std::ranges::set_intersection(card.winners, card.numbers, std::inserter(intersection, intersection.begin()));

    if(intersection.size() == 0)
    {
        return 0;
    }
    else
    {
        return 1 << (intersection.size() - 1);
    }
}

int part1(std::vector<Card> const& cards)
{
    int total{};

    for(auto& card : cards)
    {
        total += part1Score(card);
    }

    return total;
}

auto part2Score(Card const& card)
{
    std::set<int> intersection;

    std::ranges::set_intersection(card.winners, card.numbers, std::inserter(intersection, intersection.begin()));

    return intersection.size();
}


auto part2(std::vector<Card>& cards)
{

    for(int i = 0; i < isize(cards); i++)
    {
        auto& card = cards[i];
        auto  winners = part2Score(card);

        for(int j = 1; j <= winners && (i + j) < cards.size(); j++)
        {
            cards[i+j].count += card.count;
        }
    }

    auto totalCards = std::ranges::fold_left(cards | std::views::transform(&Card::count), 0, std::plus{});              // ranges accumulate with projection

    return totalCards;
}




int main()
try
{
    auto cards = parse();

    std::print("Part 1 : {}\n", part1(cards));
    std::print("Part 2 : {}\n", part2(cards));

}
catch(std::exception const& e)
{
    std::print("{}", e.what());
}


#include <sstream>

// --------------------------
std::istringstream testInput{
R"(Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53
Card 2: 13 32 20 16 61 | 61 30 68 82 17 32 24 19
Card 3:  1 21 53 59 44 | 69 82 63 72 16 21 14  1
Card 4: 41 92 73 84 69 | 59 84 76 51 58  5 54 83
Card 5: 87 83 26 28 32 | 88 30 70 12 93 22 82 36
Card 6: 31 18 13 56 72 | 74 77 10 23 35 67 36 11
)" };
