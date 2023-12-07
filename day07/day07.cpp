#include <print>
#include <array>
#include <compare>
#include <map>
#include <cassert>

#include "include/codeAnalysis.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/stringstuff.h"

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



namespace Part1
{

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



auto classify(std::string_view view)
{
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
        return Type::FiveOfAKind;
    }
    else if(counts[4])
    {
        return Type::FourOfAKind;
    }
    else if(counts[3] && counts[2])
    {
        return Type::FullHouse;
    }
    else if(counts[3])
    {
        return Type::ThreeOfAKind;
    }
    else if(counts[2] == 2)
    {
        return Type::TwoPair;
    }
    else if(counts[2] == 1)
    {
        return Type::Pair;
    }
    else
    {
        return Type::HighCard;
    }
}




struct Hand
{
    Type                type;
    std::array<int,5>   cards;
    int                 bid;

    friend auto operator<=>(Hand const&,Hand const&)=default;


    Hand(std::string_view  view, int bid) : bid{bid}, type{classify(view)}
    {
        assert(view.size()==5);

        std::ranges::transform(view,cards.begin(), [](char card){return faceValues[card];});

    }
};

}



int main()
try
{
    std::vector<Part1::Hand>       hands1;
    std::vector<Part1::Hand>       hands2;

    for(auto const &line : getDataLines())
    {
        auto [cards,bid] = splitIn2(line,' ');  //266QJ 288

        hands1.emplace_back(cards, stoi(bid));
        hands2.emplace_back(cards, stoi(bid));
    }

    std::ranges::sort(hands1);
    std::ranges::sort(hands2);

    int64_t     total1{};
    int64_t     total2{};

    for(int rank=0;rank < hands1.size();rank++)
    {
        total1 += (rank+1) * hands1[rank].bid;
        total2 += (rank+1) * hands2[rank].bid;
    }

    std::print("Part 1 : {}\n",total1);
    std::print("Part 2 : {}\n",total2);

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
