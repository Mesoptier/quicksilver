#include <numeric>
#include "SimpleGraph.h"
#include "SimpleEstimator.h"
//#include "prettyprint.hpp"

SimpleEstimator::SimpleEstimator(std::shared_ptr<SimpleGraph> &g) {
    graph = g;
}

void SimpleEstimator::prepare() {
    card_labels.resize(graph->getNoLabels());
    card_labels_in.resize(graph->getNoLabels());
    card_labels_out.resize(graph->getNoLabels());

    std::unordered_set<uint32_t> distinct_labels_in;
    std::unordered_set<uint32_t> distinct_labels_out;

    for (uint32_t vertex = 0; vertex != graph->getNoVertices(); vertex++) {
        const auto &adj = graph->adj[vertex];
        const auto &reverse_adj = graph->reverse_adj[vertex];

        for (const auto &edge : adj) {
            auto label = edge.first;

            card_labels[label]++;
            distinct_labels_out.insert(label);
        }
        for (const auto &edge : reverse_adj) {
            auto label = edge.first;

            distinct_labels_in.insert(label);
        }

        for (const auto &label : distinct_labels_in) {
            card_labels_in[label]++;
        }
        for (const auto &label : distinct_labels_out) {
            card_labels_out[label]++;
        }

        distinct_labels_in.clear();
        distinct_labels_out.clear();
    }

    //std::cout << card_labels << std::endl;
    //std::cout << card_labels_in << std::endl;
    //std::cout << card_labels_out << std::endl;
}

cardStat SimpleEstimator::estimate(RPQTree *q) {
    const auto edges = reduceQueryTree(q);

    uint32_t noOut = 0;
    uint32_t noIn = 0;
    uint32_t noPaths = graph->getNoEdges();

    for (size_t i = 0; i != edges.size(); i++) {
        const auto &edge = edges[i];
        bool reverse = edge.substr(edge.size() - 1) == "-";
        uint32_t label =
            static_cast<uint32_t>(std::stoi(edge.substr(0, edge.size() - 1)));

        if (i == 0) {
            noOut = card_labels_out[label];
        }
        if (i == edges.size() - 1) {
            noIn = card_labels_in[label];
        }

        float propPaths = (float) card_labels[label] / graph->getNoEdges();
        noPaths += noPaths * propPaths;
    }

    return cardStat {noOut, noPaths, noIn};
}

std::vector<std::string> SimpleEstimator::reduceQueryTree(RPQTree *q) {
    std::vector<std::string> vec;
    reduceQueryTree(q, vec);
    return vec;
}

void SimpleEstimator::reduceQueryTree(
    RPQTree *q,
    std::vector<std::string> &vec
) {
    if (q->isLeaf()) {
        vec.push_back(q->data);
    } else {
        reduceQueryTree(q->left, vec);
        reduceQueryTree(q->right, vec);
    }
}
