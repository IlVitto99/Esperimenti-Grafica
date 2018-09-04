#include "lotta.h"

int qdistance(int x1, int y1, int x2, int y2){
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);                                     
}

int getModulo(int x, int y){
    return sqrt(x * x + y * y);
}

void setModulo(int &x, int &y, int nouvoModulo){
    int moduloAttuale = getModulo(x, y);
    x = x * (1 / moduloAttuale) * nouvoModulo;
    y = y * (1 / moduloAttuale) * nouvoModulo;
}

int pallina::indexC = 0;

void pallina::ai(listaOggetti &li){
    nn::data ris;
    for (int i = 0; i < li.size(); i++){
            if (li[i].tipo == 0 && li[i].index != index){
                int xd = li[i].x - x;
                int yd = li[i].y - y;
                setModulo(x, y, 1);
                ris.push_back(double(xd));
                ris.push_back(double(yd));
                ris.push_back(double(vita));
                ris.push_back(double(time));
                cervello.FeedForword(ris);
                ris.clear();
                ris = cervello.getRisultato();
                dx = ris[0];
                dy = ris[1];
                setModulo(dx, dy, 20);

                if(ris[2] > 0.4){
                    attacca(li);
    } 
            }                                                                                    
    } // che la faffa sia con te  
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
    cervello = nn::NeuralNetwork({4,5,5,3});
    index = indexC++;
    if(tipo == 0) {
        raggio = 20;
        vita = 1;
        time = 0;
    }
    else {
        raggio = 5;
        vita = 1;
        time = 200;
    }
    
}

pallina::pallina(int x, int y, int dx, int dy, int tipo){
    this->x = x;
    this->y = y;
    this->tipo = tipo;
    this->dx = dx;
    this->dy = dy;
    cervello = nn::NeuralNetwork({4,5,5,3});
    index = indexC++;
    if(tipo == 0) raggio = 20;
    else raggio = 5;
    vita = 1;
}

bool pallina::RunOneFrame(){
    if(time > 0)
        time--;
    if(tipo == 1 and time <= 0)
        return index;

    if(vita <= 0)
        return index;

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
    return -1;
}

void pallina::attacca(listaOggetti &li){
    if (time <= 0){
        time = 200;
        int nx, ny, ndx, ndy;
        ndx = dx;
        ndy = dy;
        setModulo(ndx, ndy, 40);
        nx = x + ndx;
        ny = y + ndy;
        li.push_back(pallina(nx, ny, dx, dy, 1));
    }
}

void pallina::collisione(listaOggetti &li){
    for(int i = 0; i < li.size(); i++){
        if(li[i].index != index){
            int distanza = qdistance(x, y, li[i].x, li[i].y);
            if(distanza < (raggio * raggio + li[i].raggio * li[i].raggio)){
                vita--;
            }
        }
    }
}