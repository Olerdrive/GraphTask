#include "graphmatrix.h"

#include <iostream>
#include <boost/thread.hpp>
#include "graphboost.h"

unsigned GetTickCount();

GraphMatrix::GraphMatrix(int N): vNumber(N), t(0), proc(0)
{
    graph = new bool*[vNumber];
    for(int i = 0; i < N; ++i)
    {
        graph[i] = new bool[vNumber];
    }

    //Генерируем граф
    srand(time(NULL));
    for(int i = 0; i < vNumber; ++i)
    {
        for(int j = i+1; j < vNumber; ++j)
        {
            if(rand() % 10 < 6) //60%
            {
                graph[i][j] = true;
                graph[j][i] = true;
            }
        }
    }
}

void GraphMatrix::printGraph()
{
    int prevI = 0;
    for(int i = 0; i < vNumber; ++i)
    {
        for(int j = i+1; j < vNumber; ++j)
        {
            if (graph[i][j])
            {
                if (prevI != i) cout << endl;
                cout << "(" << i << "," << j << ") "; //выводит рёбра
                prevI = i;
            }
        }
    }
    cout << endl << endl;
}

void GraphMatrix::run(int pr)
{
    proc = pr;
    if (vNumber < proc) proc = vNumber;
    maxSizes = new int[proc]; //Максимальный размер независимого множества для каждого процессора
    setNumbers = new unsigned long long[proc]; //номер независимого множества
    fill_n(maxSizes, proc, 0);
    fill_n(setNumbers, proc, 0);

    //Все возможные подмножества вершин
    unsigned long long numberOfSets = pow(2, vNumber);

    t = GetTickCount();

    thread_group threadGroup;
    unsigned long long perProc = numberOfSets/proc;
    unsigned long long l = 0;
    unsigned long long r = perProc;
    //Распараллеливание задачи
    for (int i = 0; i < proc; ++i)
    {
        bool* vector = new bool[vNumber];
        thread* th = new thread(&GraphMatrix::findMaxIndependentSet, this, vector, l, r, &setNumbers[i], &maxSizes[i]);
        threadGroup.add_thread(th);
        l = r + 1;
        r = r + perProc;
    }
    threadGroup.join_all();
}

void GraphMatrix::findMaxIndependentSet(bool* vector, unsigned long long l, unsigned long long r, unsigned long long* setNumber, int* maxSize)
{
    //Рассматриваем все возможные подмножества (2^n)
    for (unsigned long long i = l; i < r; ++i)
    {
        //Получаем вектор вершин для текущего подмножества
        unsigned long long value = i;
        for (int j = 0; j < vNumber; ++j) {
            vector[j] = (value >> j) & 1;
        }

        bool isInSet = true;
        int currSize = 0;
        //Попарно проверяем вершины из вектора
        for(int k = 0; k < vNumber; ++k)
        {
            //Если вершина есть в векторе
            if (vector[k] == true)
            {
                for(int l = k+1; l < vNumber; ++l)
                {
                    if (vector[l] == true)
                    {
                        //Если ребро присутствует в матрице
                        if (graph[k][l] == true)
                        {
                            //Множество не является независимым
                            isInSet = false;
                            currSize = 0;
                            break;
                        }
                    }
                }
                if (isInSet) ++currSize;
                else break;
            }
        }
        //Сравниваем размер текущего множества с максимальными
        if (currSize > *maxSize)
        {
            *maxSize = currSize;
            *setNumber = i;
        }
    }
}

void GraphMatrix::printResult()
{
    int maxSize = 0;
    long long setNumber = 0;
    //Поиск максимального
    for (int i = 0; i < proc; ++i) //N
    {
        if (maxSizes[i] > maxSize)
        {
            maxSize = maxSizes[i];
            setNumber = setNumbers[i];
        }
    }

    t = GetTickCount() - t;

    //Вывод результата
    cout << "Brute force algorithm" << endl;
    cout << "Time: " << t << " ms" << endl;
    cout << "Set number: " << setNumber << endl;
    cout << "Size: " << maxSize << " vertices" << endl;

    bool* vector = new bool[vNumber];
    for (int j = 0; j < vNumber; ++j) {
        vector[j] = (setNumber >> j) & 1;
    }

    //Вывод максимального независимого подмножества
    cout << "Result: ( ";
    for(int k = 0; k < vNumber; ++k)
    {
        if (vector[k] == true)
            cout << k << " ";
    }
    cout << ")" << endl << endl;
}

GraphMatrix::GraphMatrix(GraphBoost &gr): vNumber(gr.vNumber), t(0), proc(0)
{
    graph = new bool*[vNumber];
    for(int i = 0; i < vNumber; ++i)
    {
        graph[i] = new bool[vNumber];
        fill_n(graph[i], vNumber, false);
    }

    graph_traits<Graph>::vertex_descriptor u, v;
    graph_traits<Graph>::edge_iterator start, end;
    boost::tie(start, end) = edges(gr.graph);
    for (; start != end; ++ start)
    {
        u = source(*start, gr.graph);
        v = target(*start, gr.graph);
        graph[u][v] = true;
        graph[v][u] = true;
    }
}
