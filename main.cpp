#include <iostream>
#include "NeuralNetwork.h"
#define MNIST_DOUBLE
#define USE_MNIST_LOADER
#include "mnist.h"

using namespace nn;
using namespace std;

int main(int argc, char const *argv[])
{
    mnist_data *data;
    unsigned int cnt;
    int ret;

    NeuralNetwork faffa = NeuralNetwork({28*28, 10, 7,  10});
    nn::data lb;
    nn::data tmp;
    


    if (ret = mnist_load("img", "lab", &data, &cnt)) {
            printf("An error occured: %d\n", ret);
    } 
    else {
            printf("image count: %d\n", cnt);
            for(int n = 0; n < cnt; n ++) {
                for(int i = 0; i < 28 * 28; i ++) tmp.push_back(data[n].data[i]);
                for(int i = 0;i < 10; i++) lb.push_back(0);
                lb[data[n].label] = 1;

                faffa.FeedForword(tmp);
                faffa.BackProp(lb);

                if( n % 100 == 0)
                    cout << n  << " ac: " << faffa.getPrecisione() * 100 <<"%\n";

                tmp.clear();
                lb.clear();
            }
            salvaW("pesi.txt", faffa);
            free(data);
    }

        return 0;
}

