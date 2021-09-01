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
    this->number_edges = 0;
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
}

// Destructor
Graph::~Graph()
{

    Node *next_node = this->first_node;

    while (next_node != nullptr)
    {

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
    while (aux_cluster != nullptr && aux_cluster->getId() != id_cluster){
        aux_cluster = aux_cluster->getNextCluster();
    }
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

void Graph::insertEdge(int id, int target_id, float weight){
    Node *start_node, *target_node;
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

void Graph::aux_insert_edge(vector<Edge *> *list_edge, Node *start_node, Graph *graph, bool *visited_clusters) {
    Edge *edge = graph->getNode(start_node->getId())->getFirstEdge();
    while (edge != nullptr){
        if (!visited_clusters[graph->getNode(edge->getTargetId())->getIdCluster() - 1])
            list_edge->push_back(edge);
        edge = edge->getNextEdge();
    }
}


