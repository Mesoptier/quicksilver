//
// Created by Nikolay Yakovets on 2018-02-01.
//

#ifndef QS_SIMPLEESTIMATOR_H
#define QS_SIMPLEESTIMATOR_H
#include <memory>
#include "Estimator.h"
#include "SimpleGraph.h"

class SimpleEstimator : public Estimator {

    std::shared_ptr<SimpleGraph> graph;

    std::vector<uint32_t> card_labels;
    std::vector<uint32_t> card_labels_in;
    std::vector<uint32_t> card_labels_out;

public:
    explicit SimpleEstimator(std::shared_ptr<SimpleGraph> &g);
    ~SimpleEstimator() = default;

    void prepare() override ;
    cardStat estimate(RPQTree *q) override ;

private:
    std::vector<std::string> reduceQueryTree(RPQTree *q);
    void reduceQueryTree(RPQTree *q, std::vector<std::string> &vec);

};


#endif //QS_SIMPLEESTIMATOR_H
