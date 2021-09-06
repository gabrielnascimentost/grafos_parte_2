#include "Graph.h"
#include "Node.h"
#include "Edge.h"
#include <iostream>
#include <fstream>
#include <stack>
#include <queue>
#include <list>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <float.h>
#include <iomanip>
#include <bits/stdc++.h>

using namespace std;

/**************************************************************************************************
 * Defining the Graph's methods
**************************************************************************************************/

// Constructor
Graph::Graph(int order, bool directed, bool weighted_edge, bool weighted_node)
{

    this->order = order;
    this->directed = directed;
    this->weighted_edge = weighted_edge;
    this->weighted_node = weighted_node;
    this->is_clusters = false;
    this->first_node = this->last_node = nullptr;
    this->first_cluster = nullptr;
    this->clusters = 0;
}

Graph::Graph(int order, bool directed, bool weighted_edge, bool weighted_node, bool is_clusters)
{

    this->order = order;
    this->directed = directed;
    this->weighted_edge = weighted_edge;
    this->weighted_node = weighted_node;
    this->is_clusters = is_clusters;
    this->first_node = this->last_node = nullptr;
    this->first_cluster = nullptr;
    this->number_edges = 0;
    this->clusters = 0;
}

// Destructor
Graph::~Graph()
{

    Node *next_node = this->first_node;

    while (next_node != nullptr){
        next_node->removeAllEdges();
        Node *aux_node = next_node->getNextNode();
        delete next_node;
        next_node = aux_node;
    }
}

// Getters
int Graph::getOrder()
{

    return this->order;
}
int Graph::getNumberEdges()
{

    return this->number_edges;
}
//Function that verifies if the graph is directed
bool Graph::getDirected()
{

    return this->directed;
}
//Function that verifies if the graph is weighted at the edges
bool Graph::getWeightedEdge()
{

    return this->weighted_edge;
}

//Function that verifies if the graph is weighted at the nodes
bool Graph::getWeightedNode()
{

    return this->weighted_node;
}


Node *Graph::getFirstNode()
{

    return this->first_node;
}

Node *Graph::getLastNode()
{

    return this->last_node;
}

// Other methods
/*
    The outdegree attribute of nodes is used as a counter for the number of edges in the graph.
    This allows the correct updating of the numbers of edges in the graph being directed or not.
*/

Cluster *Graph::insertCluster(int id_cluster) {
    Cluster *aux_cluster = new Cluster(id_cluster);
    if (first_cluster == nullptr){
        first_cluster = aux_cluster;
    }else{
        aux_cluster->setNextCluster(first_cluster);
        first_cluster = aux_cluster;
    }
    clusters++;
    return aux_cluster;
}

Cluster *Graph::getCluster(int id_cluster){
    Cluster *aux_cluster = first_cluster;
    while (aux_cluster != nullptr && aux_cluster->getId() != id_cluster)
        aux_cluster = aux_cluster->getNextCluster();
    return aux_cluster;
}

void Graph::insertNode(int id, int id_cluster){
    Node *node = new Node(id, id_cluster);
    Cluster *aux_cluster = getCluster(id_cluster);

    if (aux_cluster == nullptr){
        aux_cluster = insertCluster(id_cluster);
    }
    aux_cluster->insert_node(node);

    if (first_node == nullptr){
        first_node = last_node = node;
    }else{
        last_node->setNextNode(node);
        last_node = node;
    }
    order++;
}

void Graph::insertNode(int id){
    Node *node = new Node(id);

    if (first_node == nullptr){
        first_node = last_node = node;
    }else{
        last_node->setNextNode(node);
        last_node = node;
    }
    order++;
}

void Graph::insertEdge(int id, int target_id, float weight){
    Node *start_node = nullptr;
    Node *target_node = nullptr;
    start_node = getNode(id);

    if (start_node != nullptr){
        target_node = getNode(target_id);
        if (target_node != nullptr){
            start_node->insertEdge(target_id, weight);
            if (directed){
                start_node->incrementOutDegree();
                target_node->incrementInDegree();
            }else{
                start_node->incrementInDegree();
                target_node->insertEdge(id, weight);
                target_node->incrementOutDegree();
            }
        }
    }
    number_edges++;
}

void Graph::removeNode(int id){

}

bool Graph::searchNode(int id){
    Node *node = first_node;
    while (node != nullptr){
        if (node->getId() == id){
            return true;
        }
        node = node->getNextNode();
    }
    return false;
}

Node *Graph::getNode(int id){
    Node *node = first_node;
    while (node != nullptr && node->getId() != id){
        node = node->getNextNode();
    }
    return node;
    
}

void Graph::aux_insert_edge(vector<Edge *> *list_edge, Node *start_node, Graph *graph, bool visited_clusters[]) {
    Edge *edge = graph->getNode(start_node->getId())->getFirstEdge();
    while (edge != nullptr){
        if (!visited_clusters[graph->getNode(edge->getTargetId())->getIdCluster() - 1])
            list_edge->push_back(edge);
        edge = edge->getNextEdge();
    }
}

bool Graph::isClusters() const {
    return is_clusters;
}

void Graph::setIsClusters(bool isClusters) {
    is_clusters = isClusters;
}

Graph* Graph::buildEmptyTree(){
    Graph *aux =  new Graph(0, directed, weighted_edge, weighted_node, is_clusters);
    return aux;
}

bool myFunctionToCompareEdges(Edge *a, Edge *b){
    return a->getWeight() < b->getWeight();
}


Graph* Graph::greed() {

    Graph *pagmg = nullptr;
    Graph *aux_pagm = nullptr;
    Cluster *cluster;
    vector<Edge *> edges;
    Edge *aux_edge = nullptr;
    Node *aux_node = nullptr;

    int minimal_cost = INT_MAX;
    int aux_cost = 0;
    bool clusters_visited[clusters];

    for (cluster = first_cluster; cluster != nullptr; cluster = cluster->getNextCluster()){
        aux_pagm = buildEmptyTree();
        aux_cost = 0;

        for (int i = 0; i < clusters; i++) {
            clusters_visited[i] = false;
        }

        clusters_visited[cluster->getId() - 1] = true;

        for (int i = 0; i < cluster->getListNodes().size(); i++){
            Edge *edge = this->getNode(cluster->getNode(i)->getId())->getFirstEdge();
            while (edge != nullptr){
                if (!clusters_visited[this->getNode(edge->getTargetId())->getIdCluster() - 1])
                    edges.push_back(edge);
                edge = edge->getNextEdge();
            }
        }

        for (int i = 1; i < clusters; i++){
            for (aux_node = aux_pagm->getFirstNode(); aux_node != nullptr; aux_node = aux_node->getNextNode()){
                aux_insert_edge(&edges, aux_node, this, clusters_visited);
            }

            aux_edge = edges[0];

            for (int i = 1; i < edges.size(); i++){
                if (edges[i]->getWeight() < aux_edge->getWeight())
                    aux_edge = edges[i];
            }

            if (!aux_pagm->searchNode(aux_edge->getStartId()))
                aux_pagm->insertNode(aux_edge->getStartId());

            if (!aux_pagm->searchNode(aux_edge->getTargetId()))
                aux_pagm->insertNode(aux_edge->getTargetId());

            aux_cost += aux_edge->getWeight();
            clusters_visited[this->getNode(aux_edge->getTargetId())->getIdCluster() - 1] = true;
            aux_pagm->insertEdge(aux_edge->getStartId(), aux_edge->getTargetId(), aux_edge->getWeight());

            edges.clear();
        }

        if (aux_cost < minimal_cost){
            if (pagmg != nullptr)
                delete pagmg;
            pagmg = aux_pagm;
            minimal_cost = aux_cost;
        }
        else
            delete aux_pagm;
    }
    std::cout << "Minimal cost of tree: " << minimal_cost << endl;
    return pagmg;
}

Graph* Graph::greedRandom(float alpha) {
    Graph *pagmg = nullptr;
    Graph *aux_pagm = nullptr;

    Cluster *cluster;
    vector<Edge *> edges;
    Edge *aux_edge = nullptr;
    Node *aux_node = nullptr;

    int minimal_cost = INT_MAX;
    int aux_cost = 0;
    float max_weight_edge = 0;
    bool clusters_visited[clusters];

    srand(time(NULL));

    for (cluster = first_cluster; cluster != nullptr; cluster = cluster->getNextCluster()){
        aux_pagm = buildEmptyTree();
        aux_cost = 0;

        for (int i = 0; i < clusters; i++) {
            clusters_visited[i] = false;
        }

        clusters_visited[cluster->getId() - 1] = true;

        for (int i = 0; i < cluster->getListNodes().size(); i++){
            Edge *edge = this->getNode(cluster->getNode(i)->getId())->getFirstEdge();
            while (edge != nullptr){
                if (!clusters_visited[this->getNode(edge->getTargetId())->getIdCluster() - 1])
                    edges.push_back(edge);
                edge = edge->getNextEdge();
            }
        }

        for (int i = 1; i < clusters; i++){
            for (aux_node = aux_pagm->getFirstNode(); aux_node != nullptr; aux_node = aux_node->getNextNode()){
                aux_insert_edge(&edges, aux_node, this, clusters_visited);
            }
            sort(edges.begin(), edges.end(), myFunctionToCompareEdges);
            max_weight_edge = edges.back()->getWeight() + alpha * (edges.front()->getWeight() - edges.back()->getWeight());

            for (int i = 1; i < edges.size(); i++){
                if (edges[i]->getWeight() == max_weight_edge){
                    aux_edge = edges[rand()%(i+1)];
                    break;
                }
                if(edges[i]->getWeight() > max_weight_edge){
                    aux_edge = edges[rand() % i];
                    break;
                }
            }

            if (!aux_pagm->searchNode(aux_edge->getStartId()))
                aux_pagm->insertNode(aux_edge->getStartId());

            if (!aux_pagm->searchNode(aux_edge->getTargetId()))
                aux_pagm->insertNode(aux_edge->getTargetId());

            aux_cost += aux_edge->getWeight();
            clusters_visited[this->getNode(aux_edge->getTargetId())->getIdCluster() - 1] = true;
            aux_pagm->insertEdge(aux_edge->getStartId(), aux_edge->getTargetId(), aux_edge->getWeight());

            edges.clear();
        }

        if (aux_cost < minimal_cost){
            if (pagmg != nullptr)
                delete pagmg;
            pagmg = aux_pagm;
            minimal_cost = aux_cost;
        }
        else
            delete aux_pagm;
    }
    std::cout << "Minimal cost of tree: " << minimal_cost << endl;
    return pagmg;
}

Graph *Graph::greedRactiveRandom() {
    
}