#ifndef QS_SIMPLEESTIMATOR_H
#define QS_SIMPLEESTIMATOR_H

#include "Estimator.h"
#include "SimpleGraph.h"

class SimpleEstimator : public Estimator {

    std::shared_ptr<SimpleGraph> graph;
    std::map<uint32_t, uint32_t> entry_counts;
    std::map<uint32_t, std::map<uint32_t, float>> nested_degrees;

public:
    explicit SimpleEstimator(std::shared_ptr<SimpleGraph> &g);
    ~SimpleEstimator() = default;

    void prepare() override;
    cardStat estimate(RPQTree *q) override;

private:
    std::vector<std::string> reduceQueryTree(RPQTree *q);
    void reduceQueryTree(RPQTree *q, std::vector<std::string> &vec);

};

#endif //QS_SIMPLEESTIMATOR_H
