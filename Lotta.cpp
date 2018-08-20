#include <vector>
#include <math.h>
#include <stdlib.h>

typedef float neurone;
typedef std::vector<neurone> data;
typedef std::vector<int> struttura;
typedef std::vector<std::vector<float>> peso;

float qdistance(float x1, float y1, float x2, float y2){
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);                                     
}

float getModulo(float x, float y){
    return sqrt(x * x + y * y);
}

void setModulo(float &x, float &y, float nouvoModulo){
    float moduloAttuale = getModulo(x, y);
    x = x * (1 / moduloAttuale) * nouvoModulo
    y = y * (1 / moduloAttuale) * nouvoModulo
}

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
        rete[0] = input; 
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

const int w = 1200, h = 800;

class pallina;

typedef std::vector<pallina> listaOggetti

class pallina{
    public:
        pallina();
        pallina(int x, int y, int tipo);

        bool RunOneFrame();
        void attacca(listaOggetti &li); 
        void collisione(listaOggetti &li);
        void ai(listaOggetti &li);
        void drow();
        void setXY(int x, int y);
        void setV(int dx, int dy);

    private:
        int x, y, dx, dy, vita, time, tipo, raggio;
        neuralnetwork cervello;
        static int indexC;
        int index;
};

int pallina::indexC = 0;

void pallina::ai(listaOggetti &li){
    data ris;
    for (i = 0; i < li.size() i++){
            if (li[i].tipo == 0 && li[i].index != index){
               ris = cervello.feedforward({li[i].x,li[i].y,li[i].dx,li[i].dy, x, y, dx, dy, vita, time});
            }                                                                                    
    } 
    dx = ris[0];
    dy = ris[1];
    setModulo(dx, dy, 20);

    if(ris[2] > 0.4){
        attacca(li);
    } 
}

void pallina::setXY(int x, int y){
    this->x = x;
    this->y = y;  
}

void pallina::setV(int dx, int dy){
    this->dx = dx;
    this->dy = dy;
}

pallina::pallina(int x, int y, int tipo){
    this->x = x;
    this->y = y;
    this->tipo = tipo;
    cervello = neuralnetwork({});
    index = indexC++;
    if(tipo == 0) raggio = 20;
    else raggio = 5;
    vita = 0;
}

bool pallina::RunOneFrame(){
    if(time > 0)
        time--;
    if(tipo == 1  and time <= 0)
        return false;

    if(vita <= 0)
        return false;

    x += dx;
    y += dy;

    if (x >= w-raggio*2 or x <= 0){
        dx = -dx;
        x += dx;
    }
    if (y >= h-raggio*2 or y <= 0){
        dy = -dy;
        y += dy;
    }
    return true;
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