#include <numeric>
#include "SimpleGraph.h"
#include "SimpleEstimator.h"
//#include "prettyprint.hpp"

SimpleEstimator::SimpleEstimator(std::shared_ptr<SimpleGraph> &g) {
    graph = g;
}

void SimpleEstimator::prepare() {
    // entry_counts:    out_edge -> count
    // nested_counts:   in_edge -> out_edge -> count
    // nested_degrees:  in_edge -> out_edge -> count

    std::map<uint32_t, std::map<uint32_t, uint32_t>> nested_counts;

    for (uint32_t vertex = 0; vertex != graph->getNoVertices(); vertex++) {
        //for (uint32_t vertex = 0; vertex != 10; vertex++) {
        auto out_edges = graph->adj[vertex];
        auto in_edges = graph->reverse_adj[vertex];

        for (const auto &out_pair : out_edges) {
            auto out_edge = out_pair.first;

            entry_counts[out_edge]++;

            for (const auto &in_pair : in_edges) {
                auto in_edge = in_pair.first;

                nested_counts[in_edge][out_edge]++;
            }
        }
    }

    for (uint32_t in_edge = 0; in_edge != graph->getNoLabels(); in_edge++) {
        for (uint32_t out_edge = 0; out_edge != graph->getNoLabels();
             out_edge++) {
            float degree = (float) nested_counts[in_edge][out_edge]
                / entry_counts[out_edge];
            nested_degrees[in_edge][out_edge] = degree;
        }
    }

    //std::cout << entry_counts << std::endl;
    //std::cout << nested_degrees << std::endl;
}

cardStat SimpleEstimator::estimate(RPQTree *q) {
    uint32_t noPaths = 0;

    auto edges = reduceQueryTree(q);
    uint32_t prev_edge = 0;
    uint32_t prev_count = 0;

    for (size_t i = 0; i != edges.size(); i++) {
        auto edge_def = edges[i];
        bool reverse = edge_def.substr(edge_def.size() - 1) == "-";
        auto edge = static_cast<uint32_t>(
            std::stoi(edge_def.substr(0, edge_def.size() - 1))
        );

        if (i == 0) {
            prev_count = noPaths = entry_counts[edge];
        } else {
            auto degree = nested_degrees[prev_edge][edge];
            auto count = static_cast<uint32_t>(prev_count * degree);

            //std::cout << std::endl << count;
            prev_count = count;

            noPaths = static_cast<uint32_t>(noPaths * degree);
        }

        prev_edge = edge;
    }

    return cardStat {1, noPaths, 1};
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
