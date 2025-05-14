#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include <iostream>

constexpr double EPS = 0.0000001; 

class PINN{
    std::vector<double> m_hweights; 
    std::vector<double> m_hbiases;
    std::vector<double> m_weights;
    
    public:
    PINN(): m_hweights(10), m_hbiases(10), m_weights(10){
        std::random_device rd; 
        std::mt19937 gen(rd());

        std::uniform_real_distribution dis(0.0, 1.0);
        std::generate(m_hweights.begin(), m_hweights.end(), [&](){ return dis(gen); });
        std::generate(m_hbiases.begin(), m_hbiases.end(), [&](){ return dis(gen); });
        std::generate(m_weights.begin(), m_weights.end(), [&](){ return dis(gen); });; 
    }
    
    double evaluate(double x){
        std::vector<double> acc(10,0.0);
        for (int i=0; i < m_hweights.size(); ++i) {
            acc[i] = std::tanh(m_hweights[i] * x + m_hbiases[i]);
        }
        double res = 0.0;

        for (int i=0; i < m_hweights.size(); ++i) {
            res += m_weights[i] * acc[i];
        }
        return res;
    }

    void setHiddenWeigth(size_t idx, double value){
        m_hweights[idx] = value;
    }

    void setHiddenBias(size_t idx, double value){
        m_hbiases[idx] = value;
    }

    void setWeight(size_t idx, double value) {
        m_weights[idx] = value;
    }

    double firstDerivativeWrtInput_FD(double x)
    {
        double out1 = evaluate(x);
        double out2 = evaluate(x + EPS);
        return (out2 - out1)/ EPS; 
    }

    double DerivativeWrtInput(double x)
    {
        double res = 0.0;
        for (size_t i = 0; i < m_hweights.size(); ++i){
            double z_i = m_hweights[i] * x + m_hbiases[i];
            res += m_weights[i] * m_hweights[i] * first_deriv_tanh(z_i);
        } 
        return res;
    }

    private:
    double first_deriv_tanh(double x){
        return 1 - std::tanh(x) * std::tanh(x);
    } 
};

int main(){
    PINN pinn{};
    double res = pinn.evaluate(3);
    double num_der = pinn.firstDerivativeWrtInput_FD(3);
    double der_lag = pinn.DerivativeWrtInput(3);

    std::cout << "Evaluates as: " << res << "\n"; 
}