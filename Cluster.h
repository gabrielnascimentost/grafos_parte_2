//
// Created by Pichau on 31/08/2021.
//

#ifndef GRAFOS20211_CLUSTER_H
#define GRAFOS20211_CLUSTER_H

#include "vector"
#include "Node.h"

class Cluster {

    private:
        int id;
        vector<Node *> list_nodes;
        Cluster *next_cluster;

    public:
        Cluster(int id);

        virtual ~Cluster();

        int getId() const;

        void setId(int id);

        const vector<Node *> &getListNodes() const;

        void setListNodes(const vector<Node *> &listNodes);

        Cluster *getNextCluster() const;

        void setNextCluster(Cluster *nextCluster);

        void insert_node(Node *node);
};


#endif //GRAFOS20211_CLUSTER_H
