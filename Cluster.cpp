//
// Created by Pichau on 31/08/2021.
//

#include "Cluster.h"

Cluster::Cluster(int id) {
    this->id = id;
    this->next_cluster = nullptr;
}

Cluster::~Cluster() {

}

int Cluster::getId() const {
    return id;
}

void Cluster::setId(int id) {
    Cluster::id = id;
}

const vector<Node *> &Cluster::getListNodes() const {
    return list_nodes;
}

void Cluster::setListNodes(const vector<Node *> &listNodes) {
    list_nodes = listNodes;
}

Cluster *Cluster::getNextCluster() const {
    return next_cluster;
}

void Cluster::setNextCluster(Cluster *nextCluster) {
    next_cluster = nextCluster;
}

void Cluster::insert_node(Node *node){
    list_nodes.push_back(node);
}

Node *Cluster::getNode(int id_node) {
    return list_nodes[id_node];
}