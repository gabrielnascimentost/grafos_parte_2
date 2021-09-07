#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <utility>
#include <tuple>
#include <iomanip>
#include <stdlib.h>
#include <chrono>
#include "Graph.h"
#include "Node.h"

using namespace std;

void buildOutputFile(Graph *graph, ofstream &output_file){
    if (graph != nullptr){
        Node *aux_node = graph->getFirstNode();
        Edge *aux_edge;
        int size_graph = graph->getOrder();

        output_file << "graph {" << endl;
        for (int i = 0; i < size_graph; i++){
            aux_edge = aux_node->getFirstEdge();
            int aux = 0;
            while ((aux < (aux_node->getOutDegree() + aux_node->getInDegree())) && aux_edge != nullptr){
                output_file << "\t" << aux_node->getId() << " -- " << aux_edge->getTargetId() << ";" << endl;
                aux_edge = aux_edge->getNextEdge();
                aux++;
            }
            aux_node = aux_node->getNextNode();
        }
        output_file << "}" << endl;
    }
}



Graph* readInstance(ifstream& input_file, int directed, int weightedEdge, int weightedNode, int is_clusters){

    //Variáveis para auxiliar na criação dos nós no Grafo
    int idNodeSource;
    int idNodeTarget;
    Graph* graph;
    int order = 0;

    if(is_clusters == false){
        //Pegando a ordem do grafo
        input_file >> order;
        //Criando objeto grafo
        graph = new Graph(order, directed, weightedEdge, weightedNode);
    }else{
        string id_cluster;
        int id_node = 0;
        graph = new Graph(order, directed, weightedEdge, weightedNode, true);
        while (getline(input_file, id_cluster)){
            if (id_cluster == "" ||id_cluster[0] == '\r'){
                break;
            }
            graph->insertNode(id_node, stoi(id_cluster));
            id_node++;
        }
    }

    //Leitura de arquivo
    if(!graph->getWeightedEdge() && !graph->getWeightedNode()){
        while(input_file >> idNodeSource >> idNodeTarget) {
            graph->insertEdge(idNodeSource, idNodeTarget, 0);
        }
    }else if(graph->getWeightedEdge() && !graph->getWeightedNode() ){
        float edgeWeight;
        while(input_file >> idNodeSource >> idNodeTarget >> edgeWeight) {
            graph->insertEdge(idNodeSource, idNodeTarget, edgeWeight);
        }
    }else if(graph->getWeightedNode() && !graph->getWeightedEdge()){
        float nodeSourceWeight, nodeTargetWeight;
        while(input_file >> idNodeSource >> nodeSourceWeight >> idNodeTarget >> nodeTargetWeight) {
            graph->insertEdge(idNodeSource, idNodeTarget, 0);
            graph->getNode(idNodeSource)->setWeight(nodeSourceWeight);
            graph->getNode(idNodeTarget)->setWeight(nodeTargetWeight);
        }
    }else if(graph->getWeightedNode() && graph->getWeightedEdge()){
        float nodeSourceWeight, nodeTargetWeight, edgeWeight;
        while(input_file >> idNodeSource >> nodeSourceWeight >> idNodeTarget >> nodeTargetWeight) {
            graph->insertEdge(idNodeSource, idNodeTarget, edgeWeight);
            graph->getNode(idNodeSource)->setWeight(nodeSourceWeight);
            graph->getNode(idNodeTarget)->setWeight(nodeTargetWeight);
        }
    }
    return graph;
}

int menu(){

    int selecao;

    cout << "MENU" << endl;
    cout << "------------------Parte 2--------------" << endl;
    cout << "[1] Algoritmo Guloso" << endl;
    cout << "[2] Algoritmo Guloso Randomizado " << endl;
    cout << "[3] Algoritmo Guloso Randomizado Reativo" << endl;
    cout << "[0] Sair" << endl;
    cout << "----------------------------------------" << endl;
    cout << "Digite a opcao desejada: ";
    cin >> selecao;

    return selecao;

}

void selecionar(int selecao, Graph* graph, ofstream& output_file){

    switch (selecao) {


        case 1:{
            time_t start, end;

            time(&start);
            Graph *aux_graph;
            aux_graph = graph->greed();
            time(&end);

            double time_ex = double(end - start);
            cout << "Built on: " << fixed;
            cout << time_ex << setprecision(5);
            cout << " seconds " << endl;

            cout << "Writing data on output file....";
            buildOutputFile(aux_graph, output_file);
            system("pause");
            break;
        }

        case 2:{
            time_t start, end;
            float alpha = 0.01;
            cout << "Digite o valor de alpha (de 0.01 a 0.09): ";
            cin >> alpha;

            if(!(alpha >= 0.01 && alpha <= 0.09)){
                cout << "Alpha out of range" << endl << endl;
                alpha = 1.00;
            }

            time(&start);
            Graph *aux_graph;
            aux_graph = graph->greedRandom(alpha);
            time(&end);

            double time_ex = double(end - start);
            cout << "Built on: " << fixed;
            cout << time_ex << setprecision(5);
            cout << " seconds " << endl;

            cout << "Writing data on output file....";
            buildOutputFile(aux_graph, output_file);
            system("pause");
            break;
        }

        case 3:{
            time_t start, end;
            int num_alphas = 0;
            cout << "Digite a quantidade alphas a serem usados: ";
            cin >> num_alphas;
            if(!(num_alphas > 0 && num_alphas < 1000)){
                cout << "Error. Size out of bound" << endl << endl;
                break;
            }
            float *alphas = new float[num_alphas];

            for (int i = 0; i < num_alphas; i++)
            {
                cout << "Alpha [" << i + 1 << "]: ";
                cin >> alphas[i];
                if(!(alphas[i] >= 0.009 && alphas[i] <= 0.09)){
                    cout << "Alpha out of range" << endl << endl; //se o usuário digitar um valor fora do range permitido, atribuo um valor padrão
                    alphas[i] = 1.00;
                }

            }

            time(&start);
            Graph *aux_graph;
            aux_graph = graph->greedRactiveRandom(alphas, num_alphas);
            time(&end);

            double time_ex = double(end - start);
            cout << "Built on: " << fixed;
            cout << time_ex << setprecision(5);
            cout << " seconds " << endl;

            cout << "Writing data on output file....";
            buildOutputFile(aux_graph, output_file);
            system("pause");
            break;
        }

        case 0:{
            cout << "Bye bye!" << endl;
            return;
        }

        default:
        {
            cout << " Error!!! invalid option!!" << endl;
        }

    }
}

int mainMenu(ofstream& output_file, Graph* graph){

    int selecao = 1;

    while(selecao != 0){
        system("clear");
        selecao = menu();
        if(output_file.is_open())
            selecionar(selecao, graph, output_file);
        else
            cout << "Unable to open the output_file" << endl;
        output_file << endl;
    }
    return 0;
}



int main(int argc, char const *argv[]) {

    //Verificação se todos os parâmetros do programa foram entrados
    if (argc != 7) {
        cout << "ERROR: Expecting: ./<program_name> <input_file> <output_file> <directed> <weighted_edge> <weighted_node> <cluster>" << endl;
        return 1;
    }

    string program_name(argv[0]);
    string input_file_name(argv[1]);

    string instance;
    if(input_file_name.find("v") <= input_file_name.size()){
        string instance = input_file_name.substr(input_file_name.find("v"));
        cout << "Running " << program_name << " with instance " << instance << " ... " << endl;
    }

    //Abrindo arquivo de entrada
    ifstream input_file;
    ofstream output_file;
    input_file.open(argv[1], ios::in);
    output_file.open(argv[2], ios::out | ios::trunc);

    Graph* graph;

    if(input_file.is_open()){
        cout << "Open file: " << argv[1];
        graph = readInstance(input_file, atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]));

    }else {
        cout << "Unable to open: " << argv[1];
    }

    mainMenu(output_file, graph);



    //Fechando arquivo de entrada
    input_file.close();

    //Fechando arquivo de saída
    output_file.close();

    return 0;
}

