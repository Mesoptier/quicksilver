/*
 * SimpleGraph.h
 *
 *  Created on: 20 Apr 2018
 *      Author: Wilco
 */

#ifndef NOTSIMPLEGRAPH_H_
#define NOTSIMPLEGRAPH_H_

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <regex>
#include <fstream>
#include "Graph.h"

class SimpleGraph: public Graph {
public:
	std::unordered_map<uint32_t, std::vector<std::pair<uint32_t,uint32_t>>> adj;
    std::unordered_map<uint32_t, std::vector<std::pair<uint32_t,uint32_t>>> reverse_adj; // vertex adjacency list

    SimpleGraph() : V(0), L(0) {};
    ~SimpleGraph() = default;
    explicit SimpleGraph(uint32_t n);

    void setNoVertices(uint32_t n);
    void setNoLabels(uint32_t noLabels) ;
    void addEdge(uint32_t from, uint32_t to, uint32_t edgeLabel) override ;
    uint32_t getNoDistinctEdges() const override ;
    uint32_t getNoEdges() const override ;
    void readFromContiguousFile(const std::string &fileName) override ;
    uint32_t getNoLabels() const override ;
    uint32_t getNoVertices() const override ;



protected:
    uint32_t V;
    uint32_t L;
};

#endif /* NOTSIMPLEGRAPH_H_ */
