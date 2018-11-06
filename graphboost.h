#ifndef GRAPHBOOST_H
#define GRAPHBOOST_H

#include <boost/graph/adjacency_list.hpp>

using namespace boost;
using namespace std;

typedef adjacency_list<vecS, vecS, undirectedS, property<vertex_name_t, vector<boost::tuple<bool,bool>> > > Graph;
typedef property_map<Graph, vertex_name_t>::type Map;

class GraphMatrix;

class GraphBoost
{
    int vNumber;            //количество вершин
    vector<int> vSetCount;  //количество вершин в каждом множестве
    int t;                  //Время выполнения
    Graph graph;            //Граф
    Map map;                //Ассоциативный массив, где ключ = вершина

public:
    GraphBoost(int N);
    GraphBoost(GraphMatrix& graphM);
    void printGraph();
    void run(int proc);
    void printResult();

private:
    void findMaxIndependentSet(graph_traits<Graph>::vertex_iterator startVertex, int i);
    void findThread(int first, int second);

    friend class GraphMatrix;
};

#endif // GRAPHBOOST_H
