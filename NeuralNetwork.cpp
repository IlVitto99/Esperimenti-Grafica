#include"NeuralNetwork.h"

namespace nn
{
    double alfa = 0.5;
    double eta = 0.2;

    template<typename T>
    std::vector<T> getData(std::string id, std::fstream& file)
    {
        std::vector<T> ris;

        std::string linea;
        std::string _id_;
        T x;

        if(!file.is_open()) 
            mes("file non aperto", 2);

        std::getline(file, linea);
        std::stringstream ss(linea);
        
        ss >> _id_;

        if(_id_ != id) 
            mes("id non combacianti", 1);

        do
        {
            if(!(ss >> x))
                mes("tipo non numerco", 1);

            ris.push_back(x);

        }while(!ss.eof());

        return ris;
    }

    void caricaW(std::string filep, NeuralNetwork& nn)
    {
        std::fstream file(filep, std::ios::in);
        nn.open(getData<unsigned>("#str", file));
            for(unsigned i = 0; i < nn.Net.size() - 1; i++)
                for(unsigned k = 0; k < nn.Net[i].size(); k++)
                    nn.Net[i][k]._weight_ = getData<double>("#nnw", file);
        file.close();
    }

    void salvaW(std::string filep, NeuralNetwork& nn)
    {
        std::fstream file(filep, std::ios::out | std::ios::trunc);

        file << "#str ";
        for(unsigned i = 0; i < nn.Net.size(); i++)
            file << nn.Net[i].size() - 1 << " ";
        file << "\n";

        for(unsigned i = 0; i < nn.Net.size() - 1; i++)
            for(unsigned k = 0; k < nn.Net[i].size(); k++)
            {
                file << "#nnw ";
                for(unsigned x = 0; x < nn.Net[i][k]._weight_.size(); x++)
                    file << nn.Net[i][k]._weight_[x] << " ";
                file << "\n";
            }
        file.close();
    }

    void mes(std::string tipo, int gravita)
    {
        std::string faffa = "ERRORE: ";
        faffa += tipo;
        switch(gravita)
        {
            case 0:
                std::cout<<faffa<<std::endl;
                break;
            case 1:
                std::cout<<"\033[33m"<<faffa<<"\033[0m\n";
                break;
            case 2:
                std::cout<<"\033[31m"<<faffa<<"\033[0m\n";
                break;
        }
    }

    double fun(double x) 
    {
        return tanh(x);
    }

    double dFun(double x) 
    {
        return 1.0f - (tanh(x)*tanh(x));
    }

    NeuralNetwork::NeuralNetwork()
    {
        Precisione = 0;
    }

    NeuralNetwork::NeuralNetwork(struttura str)
    {
        this->open(str);
    }

    void NeuralNetwork::FeedForword(data input)
    {
        if(input.size() != Net[0].size() - 1) 
            mes("Quantita di dati in imput non coicidenti numero di neuroni imput", 2);
        else{
        for(unsigned i = 0; i < Net[0].size() - 1; i++)
            Net[0][i].setValore(input[i]);
        
        for(unsigned i = 1; i < Net.size(); i++)
            for(unsigned k = 0; k < Net[i].size() - 1; k++)
                Net[i][k].calcValore(Net[i - 1]);
        }
    }//no comment

    void NeuralNetwork::BackProp(data target, bool calcW)
    {
        if(Net.back().size() - 1 != target.size()) 
            mes("Quantita di dati in imput non coicidenti numero di neuroni risultato", 2);
        else
        {
            double errore = 0.0f;

            for(unsigned i = 0; i < Net.back().size() - 1; i++) 
                errore += pow(target[i] - Net.back()[i].getValore(), 2);
            
            errore /= Net.back().size() - 1;
            Precisione = (Precisione*pnc + (1 - sqrt(errore))) / (pnc + 1);

            if(calcW)
            {
                for(unsigned i = 0; i < Net.back().size() - 1; i++)
                    Net.back()[i].calcGradiante(target[i]);

                for(unsigned i = Net.size() - 2; i > 0; i--)
                    for(unsigned k = 0; k < Net[i].size(); k++)
                        Net[i][k].calcGradianteHiden(Net[i + 1]);

                for(unsigned i = Net.size() - 1; i > 0; i--)
                    for(unsigned k = 0; k < Net[i].size() - 1; k++)
                        Net[i][k].calcWeight(Net[i - 1]);
            }
        }
    }

    data NeuralNetwork::getRisultato()
    {
        data ris;
        for(unsigned i = 0; i < Net.back().size() - 1; i++)
            ris.push_back(Net.back()[i].getValore());
        return ris;
    }

    double NeuralNetwork::getPrecisione()
    {
        return Precisione;
    }

    void NeuralNetwork::close()
    {       
        for(unsigned i = 0; i < Net.size(); i++)
            Net[i].clear();
        Net.clear();
    }
    void NeuralNetwork::open(struttura str)
    {
        this->close();

        for(unsigned i = 0; i < str.size(); i++)
        {
            Net.push_back(layer());
            for(unsigned k = 0; k <= str[i]; k++)
            {
                Net.back().push_back(Neurone(((str.size() - 1) == i ? 0 : str[i + 1]), k));
            }

            Net.back().back().setValore(1.0f);
        }

        Precisione = 0;
    }

    Neurone::Neurone(unsigned numWeight, unsigned index)
    {
        _index_ = index;

        for(unsigned i = 0; i < numWeight; i++)
        {
            _weight_.push_back(rand() / double(RAND_MAX));
            _deltaWeight_.push_back(0);
        }
    }

    Neurone::Neurone(data weight, unsigned index)
    {
        _weight_ = weight;
        _index_ = index;
        for(unsigned i = 0; i < _weight_.size(); i++)
            _deltaWeight_.push_back(0);
    }

    double Neurone::getWeight(unsigned index)
    {
        return _weight_[index];
    }

    double Neurone::getValore()
    {
        return _valore_;
    }

    void Neurone::setValore(double valore)
    {
        _valore_ = valore;
    }

    void Neurone::calcValore(layer& pre)
    {
        _valore_ = 0;
        for(unsigned i = 0; i < pre.size(); i++)
            _valore_ += pre[i].getValore() * pre[i].getWeight(_index_);
        _valore_ = fun(_valore_);
    }

    double Neurone::getGradiante()
    {
        return _Gradiante_;
    }

    void Neurone::calcGradiante(double target)
    {
        _Gradiante_ = (target - _valore_) * dFun(_valore_);  
    }

    void Neurone::calcGradianteHiden(layer& target)
    {
        double somma = 0.0f;

        for(unsigned i; i < target.size() - 1; i++)
            somma += _weight_[i] * target[i].getGradiante();

        _Gradiante_ = somma * dFun(_valore_);
    }

    void Neurone::calcWeight(layer& pre)
    {
        for (unsigned i = 0; i < pre.size(); ++i)
        {
            pre[i]._deltaWeight_[_index_] = 
                pre[i].getValore() * eta * _Gradiante_ + 
                pre[i]._deltaWeight_[_index_] * alfa;

            pre[i]._weight_[_index_] += pre[i]._deltaWeight_[_index_];
        }
        
    }
}