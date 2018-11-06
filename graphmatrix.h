#ifndef GRAPHMATRIX_H
#define GRAPHMATRIX_H

class GraphBoost;

class GraphMatrix
{
    int vNumber;    //Количество вершин
    bool** graph;   //Граф
    int t;          //Время выполнения
    int proc;       //Количество процессоров
    int* maxSizes;  //Максимальный размер независимого множества для каждого процессора
    unsigned long long* setNumbers; //Номер множества

public:
    GraphMatrix(int N);
    GraphMatrix(GraphBoost& graphB);
    void printGraph();
    void run(int proc);
    void printResult();

private:
    void findMaxIndependentSet(bool* vector, unsigned long long l, unsigned long long r, unsigned long long* setNumber, int* maxSize);


    friend class GraphBoost;
};


#endif // GRAPHMATRIX_H
