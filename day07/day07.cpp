#include <print>
#include <array>
#include <compare>
#include <map>
#include <cassert>

#include "include/codeAnalysis.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/stringstuff.h"

std::map<char,int>  faceValues
{
    {'2',2},
    {'3',3},
    {'4',4},
    {'5',5},
    {'6',6},
    {'7',7},
    {'8',8},
    {'9',9},
    {'T',10},
    {'J',11},
    {'Q',12},
    {'K',13},
    {'A',14},
};


struct Hand
{
    enum class Type
    {
        HighCard,
        Pair,
        TwoPair,
        ThreeOfAKind,
        FullHouse,
        FourOfAKind,
        FiveOfAKind,        
    };

    Type                type;
    std::array<int,5>   cards;
    int                 bid;

    friend auto operator<=>(Hand const&,Hand const&)=default;


    Hand(std::string_view  view, int bid) : bid{bid}
    {
        assert(view.size()==5);

        std::ranges::transform(view,cards.begin(), [](char card){return faceValues[card];});

        int             counts[6]{};

        std::string     s{view};
        char            previous{};
        int             count{}; 

        std::ranges::sort(s);

        for(auto c:s)
        {
            if(c == previous)
            {
                count++;
            }
            else
            {
                counts[count]++;
                count=1;
            }
            previous=c;
        }
        counts[count]++;

        if(counts[5])
        {
            type = Type::FiveOfAKind;
        }
        else if(counts[4])
        {
            type = Type::FourOfAKind;
        }
        else if(counts[3] && counts[2])
        {
            type = Type::FullHouse;
        }
        else if(counts[3])
        {
            type = Type::ThreeOfAKind;
        }
        else if(counts[2] == 2)
        {
            type = Type::TwoPair;
        }
        else if(counts[2] == 1)
        {
            type = Type::Pair;
        }
        else
        {
            type = Type::HighCard;
        }
    }

};


int main()
try
{
    std::vector<Hand>       hands;
    for(auto const &line : getDataLines())
    {
        auto [cards,bid] = splitIn2(line,' ');  //266QJ 288

        hands.emplace_back(cards, stoi(bid));
    }

    std::ranges::sort(hands);

    int64_t     total{};

    for(int rank=0;rank < hands.size();rank++)
    {
        total += (rank+1) * hands[rank].bid;
    }

    std::print("Part 1 : {}\n",total);

}
catch(std::exception const &e)
{
    std::print("{}",e.what());
}


#include <sstream>

// --------------------------
std::istringstream testInput{
R"(32T3K 765
T55J5 684
KK677 28
KTJJT 220
QQQJA 483
)"};
