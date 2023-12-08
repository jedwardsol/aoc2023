#include "include/codeAnalysis.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/stringstuff.h"

#include <spanstream>
#include <print>
#include <vector>

struct Round
{
    int red{};
    int green{};
    int blue{};
};

struct Game
{
    std::vector<Round>  rounds;
};


auto readRound(std::string_view line)       // 1 red, 3 blue, 11 green
{
    Round round{};

    for (auto ball : std::views::split(line, ","))
    {
        int         count;
        std::string colour;

        std::ispanstream{ ball } >> count >> colour;

        if(colour == "red")
        {
            round.red = count;
        }
        else if(colour == "green")
        {
            round.green=count;
        }
        else if (colour == "blue")
        {
            round.blue=count;
        }
        else
        {
            throw_runtime_error("bad colour");
        }
    }

    return round;
}


auto readGame(std::string_view line)
{
    Game    game;

    auto [gameText, roundText] = splitIn2(line, ':');

    auto rounds = split(roundText, ";");

    for (auto round : rounds)
    {
        game.rounds.push_back(readRound(round));
    }

    return game;
}

auto readGames()
{
    std::vector<Game>   games;

    for (auto const& line : getDataLines())
    {
        games.push_back(readGame(line));
    }

    return games;
}



int sumValidGames(std::vector<Game> const &games,  Round const &target)
{
    int sum{};

    for(auto i=0u;i<games.size();i++)
    {
        auto &game =games[i];

        bool valid{true};

        for(auto &round : game.rounds)
        {
            if(   round.red   > target.red
               || round.green > target.green
               || round.blue  > target.blue)
            {
                valid=false;
                break;
            }
        }

        if(valid)
        {
            sum+=(i+1);
        }
    }

    return sum;
}



int64_t sumGamePowers(std::vector<Game> const& games)
{
    int64_t sum{};

    for (auto const &game : games)
    {
        Round minimum{};

        for (auto& round : game.rounds)
        {
            minimum.red   = std::max(minimum.red,round.red);
            minimum.blue  = std::max(minimum.blue, round.blue);
            minimum.green = std::max(minimum.green, round.green);
        }

        sum+= minimum.red*minimum.green*minimum.blue;            
    }

    return sum;
}



int main()
try
{
    auto games = readGames();

    std::print("part 1 : {}\n", sumValidGames(games,{ 12 , 13 , 14 }));
    std::print("part 2 : {}\n", sumGamePowers(games));
}
catch(std::exception const &e)
{
    std::print("{}",e.what());
}


#include <sstream>

// --------------------------
std::istringstream testInput{
R"(
)"};