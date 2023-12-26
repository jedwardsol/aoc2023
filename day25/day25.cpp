#include <print>
#include <map>
#include <vector>
#include <set>
#include <ranges>
#include <cassert>
#include <queue>
#include <algorithm>

#include "include/codeAnalysis.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/stringstuff.h"

using Graph = std::map<std::string, std::set<std::string>>;

using Edge = std::pair<std::string,std::string>;

Graph readGraph()
{
    Graph   graph;

    //  pcf: vqq glr rpx qcm

    for(auto const &line : getDataLines())
    {
        auto [start, ends] = splitIn2(line, ": ");

        for(auto end : std::views::split(ends," "sv) | std::views::transform([](auto const &rng) { return rangeToStr(rng);}))
        {
            assert(start.size()==3);
            assert(end.size()==3);
            assert(start!=end);

            graph[ std::string{start} ].insert( std::string{end});
            graph[ std::string{end}   ].insert( std::string{start});
        }
    }

    return graph;
}



auto pathToFurthest(Graph const &graph, std::string start)
{
    std::queue<std::string>                 fringe;
    std::map<std::string,std::string>       paths;
    std::string                             last;

    fringe.push(start);
    paths[start]=start;

    while(not fringe.empty())
    {
        auto next = fringe.front();
        fringe.pop();

        last=next;

        auto const &connections = graph.at(next);

        for(auto &neighbour : connections)
        {
            if(not paths.contains(neighbour))
            {
                fringe.push(neighbour);
                paths[neighbour]=next;
            }
        }
    }


    std::vector<std::string>    path;

    do
    {
        path.push_back(last);
        last = paths[last];
    } while(last!=start);

    std::vector<Edge>    edges;

    for(auto [a,b] : path | std::views::adjacent<2>)
    {
        edges.emplace_back( std::min(a,b), std::max(a,b));
    }

    return edges;
}



auto connectedNodes(Graph const &graph, std::string start)
{
    std::queue<std::string>                 fringe;
    std::set<std::string>                   visited;

    fringe.push(start);
    visited.insert(start);

    while(not fringe.empty())
    {
        auto next = fringe.front();
        fringe.pop();

        auto const &connections = graph.at(next);

        for(auto &neighbour : connections)
        {
            if(not visited.contains(neighbour))
            {
                fringe.push(neighbour);
                visited.insert(neighbour);
            }
        }
    }

    return visited;
}




auto busiestEdge(std::map<Edge,int> const &edges)
{
    using Element = std::map<Edge,int>::value_type;

    return std::ranges::max_element( edges , {} , &Element::second)->first;
}


int main()
try
{
    auto graph     = readGraph();
    auto edgeCount = std::map<Edge,int>{};


    for(int remove = 0;remove < 3;remove++)
    {
        for(auto [node,_] : graph)
        {
            auto edges = pathToFurthest(graph,node);

            for(auto edge : edges)
            {
                edgeCount[edge]++;
            }
        }

        auto busiestEdge = ::busiestEdge(edgeCount);

        print("removing {}-{}\n",busiestEdge.first,busiestEdge.second);

        graph[busiestEdge.first].erase(busiestEdge.second);
        graph[busiestEdge.second].erase(busiestEdge.first);
    }

    auto oneHalf = connectedNodes(graph, graph.begin()->first);
        
    auto size1 = oneHalf.size();
    auto size2 = graph.size() - oneHalf.size();


    std::print("Part 1 : {} * {} = {}",size1,size2,size1*size2);
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
