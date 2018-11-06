#include <iostream>
#include "graphboost.h"
#include "graphmatrix.h"

int main()
{
    cout << "Enter number of vertices: ";
    int N;
    cin >> N;

    //Количество процессоров
    int processorCount = 8;

    //Полный перебор
    GraphMatrix bruteForce(N);
    bruteForce.printGraph();
    bruteForce.run(processorCount);
    bruteForce.printResult();

    //Жадный алгоритм на том же графе
    GraphBoost greedy(bruteForce);
    //greedy.printGraph();
    greedy.run(processorCount);
    greedy.printResult();

    return 0;
}
