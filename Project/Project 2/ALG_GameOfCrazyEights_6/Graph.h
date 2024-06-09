/* 
 * File:   Graph.h
 * Author: Draylend Chow
 *
 * Created on June 8, 2024, 8:09 PM
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <map>
#include <vector>

using namespace std;

struct Edge{
    int data;
    Edge *lnkNext;
    Edge *lnkPrev;
    
    Edge(Edge *next, Edge *prev, int val) : lnkNext(next), lnkPrev(prev), data(val) {}
};

class Graph {
    private:
        map<int, vector<Edge>> graph;
    public:
        void insertEdge(Edge newEdge) {this->graph.end()->second.push_back(newEdge);}
};


#endif /* GRAPH_H */

