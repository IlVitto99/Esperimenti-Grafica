#include <iostream>
#include "NeuralNetwork.h"
#include <stdlib.h>
#include <time.h>

using namespace nn;
using namespace std;

int main(int argc, char const *argv[])
{
    NeuralNetwork faffa = NeuralNetwork({2, 4, 1});

    for(int i = 0;  i < 100000; i++){
        int a = rand() % 2; 
        int b = rand() % 2; 
        int c = (a and b); // insegnare operazioni a vittorio (promemoria)
        cout << "iterazione: " << i << endl;
        cout << "input: " << a << " " << b << endl;
        faffa.FeedForword({double(a), double(b)});
        faffa.BackProp({double(c)});
        nn::data ris = faffa.getRisultato();
        cout << "output: " << ris[0] << endl;
        cout << "target: " << c << endl;
        cout << "precisione: "<< faffa.getPrecisione() * 100 << endl << endl;
    }

    return 0;
}

