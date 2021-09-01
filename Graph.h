/**************************************************************************************************
 * Implementation of the TAD Graph
**************************************************************************************************/

#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED
#include "Node.h"
#include "Cluster.h"
#include <fstream>
#include <stack>
#include <list>

using namespace std;

class Graph{

    //Atributes
    private:
        int order;
        int number_edges;
        bool directed;
        bool weighted_edge;
        bool weighted_node;
        bool is_clusters;
        Node* first_node;
        Node* last_node;
        Cluster *first_cluster;
        int clusters;

    public:
        //Constructor
        Graph(int order, bool directed, bool weighted_edge, bool weighted_node);
        Graph(int order, bool directed, bool weighted_edge, bool weighted_node, bool is_clusters);
        //Destructor
        ~Graph();
        //Getters
        int getOrder();
        int getNumberEdges();
        bool getDirected();
        bool getWeightedEdge();
        bool getWeightedNode();
        Node* getFirstNode();
        Node* getLastNode();
        //Other methods
        void insertNode(int id);
        void insertNode(int id,int id_cluster);
        void insertEdge(int id, int target_id, float weight);
        void removeNode(int id);
        bool searchNode(int id);
        Node* getNode(int id);
        Cluster *insertCluster(int id_cluster);
        Cluster *getCluster(int id_cluster);
        void aux_insert_edge(vector<Edge *> *list_edge, Node *start_node, Graph *graph, bool visited_clusters[]);

        //methods phase1
        float greed();
        float greedRandom();
        float greedRactiveRandom();
    private:
        //Auxiliar methods

};

#endif // GRAPH_H_INCLUDED
