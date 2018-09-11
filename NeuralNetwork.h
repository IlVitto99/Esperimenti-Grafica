#include<vector>
#include<string>
#include<math.h>
#include<fstream>
#include<sstream>
#include<cstdlib>
#include<iostream>

#ifndef __NeuralNetwork__
#define __NeuralNetwork__

namespace nn
{
    class NeuralNetwork;
    class Neurone;

    typedef std::vector<Neurone> layer;
    typedef std::vector<double> data;
    typedef std::vector<unsigned int> struttura;

    const int pnc = 1000;
    extern double alfa;
    extern double eta;

    template<typename T>
    std::vector<T> getData(std::string id, std::fstream& file);

    void caricaW(std::string filep, NeuralNetwork& nn);
    void salvaW(std::string filep, NeuralNetwork& nn);
    void mes(std::string tipo, int gravita);
    double fun(double x);
    double dFun(double x);

    class NeuralNetwork
    {
    public:
        friend void caricaW(std::string filep, NeuralNetwork& nn);
        friend void salvaW(std::string filep, NeuralNetwork& nn);

        NeuralNetwork();
        NeuralNetwork(struttura str);

        void FeedForword(data input);
        void BackProp(data target, bool calcW = true);
        data getRisultato();
        double getPrecisione();
        void close();
        void open(struttura str);

    private:
        std::vector<layer> Net;
        double Precisione;
    };


    class Neurone
    {
    public:
        friend void caricaW(std::string filep, NeuralNetwork& nn);
        friend void salvaW(std::string filep, NeuralNetwork& nn);

        Neurone();
        Neurone(unsigned numWeight, unsigned index);
        Neurone(data weight, unsigned index);

        double getValore();
        void setValore(double valore);
        double getWeight(unsigned index);
        void calcValore(layer& pre);
        double getGradiante();
        void calcGradiante(double target);
        void calcGradianteHiden( layer& target);
        void calcWeight(layer& pre);

    private:
        double _valore_;
        double _Gradiante_;
        unsigned _index_;
        data _weight_;
        data _deltaWeight_;
    };
}
#endif