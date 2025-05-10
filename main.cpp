#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include <iostream>

class PINN{
    std::vector<double> m_hweights; 
    std::vector<double> m_hbiases;
    double m_biases;
    
    public:
    PINN(): m_hweights(10), m_hbiases(10) {
        std::random_device rd; 
        std::mt19937 gen(rd());

        std::uniform_real_distribution dis(0.0, 1.0);
        std::generate(m_hweights.begin(), m_hweights.end(), [&](){ return dis(gen); });
        std::generate(m_hbiases.begin(), m_hbiases.end(), [&](){ return dis(gen); });
        m_biases = dis(gen); 
    }
    
    double evaluate(double x){
        double acc;
        for (int i=0; i < m_hweights.size(); ++i) {
            acc = m_hweights[i] * x;
        }

        double res = std::tanh(acc) * m_biases;
        return res;
    }
};

int main(){
    PINN pinn{};
    double res = pinn.evaluate(3);
    std::cout << "Evaluates as: " << res << "\n"; 
}