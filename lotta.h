#include <vector>
#include <math.h>
#include "NeuralNetwork.h"

const int w = 1200, h = 800;

int qdistance(int x1, int y1, int x2, int y2);
int getModulo(int x, int y);
void setModulo(int &x, int &y, int nouvoModulo);

class pallina;

typedef std::vector<pallina> listaOggetti;

class pallina{
    public:
        pallina();
        pallina(int x, int y, int tipo);
        pallina(int x, int y, int dx, int dy, int tipo);

        bool RunOneFrame();
        void attacca(listaOggetti &li); 
        void collisione(listaOggetti &li);
        void ai(listaOggetti &li);
        void drow();
        void setXY(int x, int y);
        void setV(int dx, int dy);

    private:
        int x, y, dx, dy, vita, time, tipo, raggio;
        nn::NeuralNetwork cervello;
        static int indexC;
        int index;
};