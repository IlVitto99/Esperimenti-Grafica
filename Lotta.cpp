#include <vector>
#include <math.h>
#include <stdlib.h>

typedef float neurone;
typedef std::vector<neurone> data;
typedef std::vector<int> struttura;
typedef std::vector<std::vector<float>> peso;

float RandomNumber(float Min, float Max)
{
    return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}

class neuralnetwork{
    public:
        neuralnetwork();
        neuralnetwork(struttura ss);
        data feedforward(data input);              
    protected:
        std::vector<data> rete; 
        std::vector<peso> pesi;
        std::vector<data> bies;        
};

data neuralnetwork::feedforward(data input){
    if(input.size() == rete[0].size()){
        rete[0] = input; // probabilmente da riscrivere utilizzando un ciclo for
        for(int i = 1; i < rete.size(); i++){
    
            for(int k = 0; k < rete[i].size(); k++){
                float somma = 0;

                for(int j = 0; j < pesi[i-1][k].size(); j++) somma += pesi[i-1][k][j] * rete[i-1][j];                    
                
                rete[i][k] = tanh(somma + bies[i][k]) ;               
            }            
        }   
    }
    return rete.back();
}


neuralnetwork::neuralnetwork(struttura ss){
    for(int i = 0; i < ss.size(); i++){
        data temp, temp2;
        for(int k = 0; k < ss[i]; k++ ) {
            temp.push_back(0);
            temp2.push_back(RandomNumber(-10, 10));
        }
        rete.push_back(temp);
        bies.push_back(temp2);
        temp.clear();
        temp2.clear();
    }
    for(int i = 1; i < ss.size(); i++){
        peso temp2;
        for(int k = 0; k < ss[i]; k++ ){ 
            data temp;
            for(int j = 0; j < ss[i - 1]; j++) temp.push_back(RandomNumber(-10,10));
            temp2.push_back(temp);
            temp.clear();
        }
        pesi.push_back(temp2);
    }
}

struct ai{
    ai(struttura ss){
        nn = neuralnetwork(ss);
    }

    void calcolapunteggio(){
        data out;
        punteggio = 0;
        out = nn.feedforward({1,1});
        errore = 1 - out[0];
        punteggio += 1 - errore;
    }

    neuralnetwork nn;
    float punteggio;
    float errore;
}

// 1 1 = 1 
// 0 1 = 0
// 1 0 = 0
// 0 0 = 0
// 0.5

int main(void){
    struttura s = {2, 4, 1};
    
    neuralnetwork nn(s);
    nn.feedforward(in);
}