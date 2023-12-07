#include <print>
#include <array>
#include <compare>
#include <map>
#include <cassert>

#include "include/codeAnalysis.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/stringstuff.h"



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


};





namespace Part1
{

std::map<char,int>  faceValues
{
    {'2',2},    {'3',3},    {'4',4},    {'5',5},    {'6',6},    {'7',7},    {'8',8},    
    {'9',9},    {'T',10},   {'J',11},   {'Q',12},   {'K',13},    {'A',14},
};



auto classify(std::string cards)
{
    std::ranges::sort(cards);

    int             inARow[6]{};

    int             count{1}; 
    char            previous{cards[0]};

    for(char card : std::ranges::drop_view{cards, 1})
    {
        if(card == previous)
        {
            count++;
        }
        else
        {
            inARow[count]++;
            count=1;
        }
        previous=card;
    }
    inARow[count]++;

// ---

    if(inARow[5])
    {
        return Hand::Type::FiveOfAKind;
    }
    else if(inARow[4])
    {
        return Hand::Type::FourOfAKind;
    }
    else if(inARow[3] && inARow[2])
    {
        return Hand::Type::FullHouse;
    }
    else if(inARow[3])
    {
        return Hand::Type::ThreeOfAKind;
    }
    else if(inARow[2] == 2)
    {
        return Hand::Type::TwoPair;
    }
    else if(inARow[2] == 1)
    {
        return Hand::Type::Pair;
    }
    else
    {
        return Hand::Type::HighCard;
    }
}


Hand  makeHand(std::string_view  cards, int bid) 
{
    assert(cards.size()==5);

    Hand    hand{classify(std::string{cards}), {}, bid};

    std::ranges::transform(cards,hand.cards.begin(), [](char card){return faceValues[card];});

    return hand;
}


}


namespace Part2
{
std::map<char,int>  faceValues
{
    {'J',1},    {'2',2},    {'3',3},    {'4',4},    {'5',5},    {'6',6},    {'7',7},
    {'8',8},    {'9',9},    {'T',10},   {'Q',12},   {'K',13},   {'A',14},
};


auto classify(std::string cards)
{
    std::ranges::sort(cards);

    auto            jokerCount = std::ranges::count(cards,'J');

    int             inARow[6]{};

    int             count{1}; 
    char            previous{cards[0]};

    for(char card : std::ranges::drop_view{cards, 1})
    {
        if(card == previous)
        {
            count++;
        }
        else
        {
            inARow[count]++;
            count=1;
        }
        previous=card;
    }
    inARow[count]++;

// ---

    if(inARow[5])
    {
        // irrelevent whether they're jokers or not.
        return Hand::Type::FiveOfAKind;
    }
    else if(inARow[4])
    {
        if(jokerCount)                  // either 1 or 4.       JJJJX -> XXXXX <- JXXXX
        {
            assert(jokerCount == 1 || jokerCount == 4);
            return Hand::Type::FiveOfAKind;
        }
        else
        {
            return Hand::Type::FourOfAKind;
        }
    }
    else if(inARow[3] && inARow[2])    
    {
        if(jokerCount)                  // either 2 or 3.       JJJXX -> XXXXX < JJXXX
        {
            assert(jokerCount == 2 || jokerCount == 3);
            return Hand::Type::FiveOfAKind;
        }
        else
        {
            return Hand::Type::FullHouse;
        }
    }
    else if(inARow[3])
    {
        if(jokerCount)                  // either 3 or 1.       JXXXY -> XXXXY <- JJJXY
        {
            assert(jokerCount == 1 || jokerCount == 3);
            return Hand::Type::FourOfAKind;
        }
        else
        {
            return Hand::Type::ThreeOfAKind;
        }
    }
    else if(inARow[2] == 2)
    {
        if(jokerCount == 2)             //                      JJXXY -> XXXXY  
        {
            return Hand::Type::FourOfAKind;
        }
        else if(jokerCount == 1)        //                      JXXYY -> XXXYY  
        {
            return Hand::Type::FullHouse;
        }
        else
        {
            return Hand::Type::TwoPair;
        }
    }
    else if(inARow[2] == 1)
    {
        if(jokerCount)                  //                      JJXYZ -> XXXYZ <  JXXYZ     
        {
            return Hand::Type::ThreeOfAKind;
        }
        else
        {
            return Hand::Type::Pair;
        }
    }
    else
    {
        if(jokerCount)                  //                      JWXYZ -> WWXYZ
        {
            return Hand::Type::Pair;
        }
        else
        {
            return Hand::Type::HighCard;

        }
    }
}


Hand  makeHand(std::string_view  cards, int bid) 
{
    assert(cards.size()==5);

    Hand    hand{classify(std::string{cards}), {}, bid};

    std::ranges::transform(cards,hand.cards.begin(), [](char card){return faceValues[card];});

    return hand;
}

}

int main()
try
{
    std::vector<Hand>       hands1;
    std::vector<Hand>       hands2;

    for(auto const &line : getDataLines())
    {
        auto [cards,bid] = splitIn2(line,' ');  //266QJ 288

        hands1.push_back(Part1::makeHand(cards, stoi(bid)));
        hands2.push_back(Part2::makeHand(cards, stoi(bid)));
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
