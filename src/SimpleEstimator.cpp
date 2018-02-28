//
// Created by Nikolay Yakovets on 2018-02-01.
//

#include <numeric>
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

    totalOut = std::accumulate(cardOut.begin(), cardOut.end(), (uint32_t) 0);
    totalPaths = std::accumulate(cardPaths.begin(), cardPaths.end(), (uint32_t) 0);
    totalIn = std::accumulate(cardIn.begin(), cardIn.end(), (uint32_t) 0);
}

cardStat SimpleEstimator::estimate(RPQTree *q) {
    uint32_t noOut = totalOut;
    uint32_t noPaths = totalPaths;
    uint32_t noIn = totalIn;

    auto edges = reduceQueryTree(q);

    for (const auto &edge : edges) {
        bool reverse = edge.substr(edge.size() - 1) == "-";
        uint32_t edgeLabel =
            static_cast<uint32_t>(std::stoi(edge.substr(0, edge.size() - 1)));

        float propOut = (float)cardOut[edgeLabel] / totalOut;
        noOut *= propOut;

        float propPaths = (float)cardPaths[edgeLabel] / totalPaths;
        noPaths += noPaths * propPaths;

        float propIn = (float)cardIn[edgeLabel] / totalIn;
        noIn *= propIn;
    }

    return cardStat { noOut, noPaths, noIn };
}

std::vector<std::string> SimpleEstimator::reduceQueryTree(RPQTree *q) {
    std::vector<std::string> vec;
    reduceQueryTree(q, vec);
    return vec;
}

void SimpleEstimator::reduceQueryTree(RPQTree *q, std::vector<std::string> &vec) {
    if (q->isLeaf()) {
        vec.push_back(q->data);
    } else {
        reduceQueryTree(q->left, vec);
        reduceQueryTree(q->right, vec);
    }
}
