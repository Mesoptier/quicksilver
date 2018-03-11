#include <numeric>
#include "SimpleGraph.h"
#include "SimpleEstimator.h"
//#include "prettyprint.hpp"

SimpleEstimator::SimpleEstimator(std::shared_ptr<SimpleGraph> &g){

    // works only with SimpleGraph
    graph = g;
}

void SimpleEstimator::prepare() {
    cardOut.resize(graph->getNoLabels());
    cardPaths.resize(graph->getNoLabels());
    cardIn.resize(graph->getNoLabels());

    for (uint32_t label = 0; label != graph->getNoLabels(); label++) {
        cardPaths[label] = 0;
    }

    for (uint32_t vertex = 0; vertex != graph->getNoVertices(); vertex++) {
        auto adj = graph->adj[vertex];
        auto reverse_adj = graph->reverse_adj[vertex];

        for (uint32_t i = 0; i != adj.size(); i++) {
            auto pair = adj[i];

            cardOut[pair.first]++;
            cardPaths[pair.first]++;
        }

        for (uint32_t i = 0; i != reverse_adj.size(); i++) {
            auto pair = reverse_adj[i];

            cardIn[pair.first]++;
        }
    }

    totalPaths = std::accumulate(cardPaths.begin(), cardPaths.end(), (uint32_t) 0);
}

cardStat SimpleEstimator::estimate(RPQTree *q) {
    uint32_t noOut = graph->getNoEdges();
    uint32_t noPaths = totalPaths;
    uint32_t noIn = graph->getNoEdges();

    auto edges = reduceQueryTree(q);

    for (const auto &edge : edges) {
        bool reverse = edge.substr(edge.size() - 1) == "-";
        uint32_t edgeLabel =
            static_cast<uint32_t>(std::stoi(edge.substr(0, edge.size() - 1)));

        float propOut = (float)cardOut[edgeLabel] / graph->getNoEdges();
        noOut *= propOut;

        float propPaths = (float)cardPaths[edgeLabel] / totalPaths;
        noPaths += noPaths * propPaths;

        float propIn = (float)cardIn[edgeLabel] / graph->getNoEdges();
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
