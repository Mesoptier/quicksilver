//
// Created by Nikolay Yakovets on 2018-02-01.
//

#include "SimpleGraph.h"
#include "SimpleEstimator.h"

SimpleEstimator::SimpleEstimator(std::shared_ptr<SimpleGraph> &g){

    // works only with SimpleGraph
    graph = g;
}

void SimpleEstimator::prepare() {
    std::vector<std::unordered_set<uint32_t>> setOut;
    std::vector<std::unordered_set<uint32_t>> setIn;

    setOut.resize(graph->getNoLabels());
    cardOut.resize(graph->getNoLabels());

    cardPaths.resize(graph->getNoLabels());

    setIn.resize(graph->getNoLabels());
    cardIn.resize(graph->getNoLabels());

    for (uint32_t label = 0; label != graph->getNoLabels(); label++) {
        cardPaths[label] = 0;
    }

    for (uint32_t vertex = 0; vertex != graph->getNoVertices(); vertex++) {
        auto adj = graph->adj[vertex];
        auto reverse_adj = graph->reverse_adj[vertex];

        for (uint32_t i = 0; i != adj.size(); i++) {
            auto pair = adj[i];

            setOut[pair.first].insert(vertex);
            cardPaths[pair.first]++;
        }

        for (uint32_t i = 0; i != reverse_adj.size(); i++) {
            auto pair = reverse_adj[i];

            setIn[pair.first].insert(vertex);
        }
    }

    for (uint32_t label = 0; label != graph->getNoLabels(); label++) {
        cardOut[label] = setOut[label].size();
        cardIn[label] = setIn[label].size();
    }
}

cardStat SimpleEstimator::estimate(RPQTree *q) {
    uint32_t edgeLabel = std::stoi(q->data.substr(0, q->data.size() - 1));

    uint32_t noOut = cardOut[edgeLabel];
    uint32_t noPaths = cardPaths[edgeLabel];
    uint32_t noIn = cardIn[edgeLabel];

    return cardStat {noOut, noPaths, noIn};
}