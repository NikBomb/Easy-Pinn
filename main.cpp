#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <functional>



#include "finiteDifferences.hpp"


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
    
    double evaluate(double x) const{
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

    double getWeight(size_t idx) {
        return m_weights[idx];
    }

    double getWeightSize() const {
        return m_weights.size();
    }

    double derivativeWrtInput(double x, int n)
    {
        double res = 0.0;
        for (size_t i = 0; i < m_hweights.size(); ++i){
            double z_i = m_hweights[i] * x + m_hbiases[i];
            double der_tanh = n_deriv_tanh(n, z_i);
            res += m_weights[i] * pow(m_hweights[i], n)  * der_tanh;
        } 
        return res;
    }

    private:
    

    double n_deriv_tanh(size_t order, double x){
        return easyPinn::finiteDifferences::finite_diff_recursive([](double x){return std::tanh(x);}, x, order);
    }
};

namespace pinn_fd {

    double derivativeWrtInput(const PINN& pinn, double x, int n){
       
        return easyPinn::finiteDifferences::finite_diff([&pinn](const double x){return pinn.evaluate(x);}, x, n);
    }

    std::vector<double> firstDerivativeWrtWeights(const PINN& pinn, double x){
        std::vector<double> out(pinn.getWeightSize(), 0.0);

        double eval = pinn.evaluate(x);

        for (int i =0; i < pinn.getWeightSize(); ++i){
            PINN p = pinn;
            p.setWeight(i, p.getWeight(i) + easyPinn::finiteDifferences::EPS);
            double new_eval = p.evaluate(x);
            out[i] = (new_eval - eval)/easyPinn::finiteDifferences::EPS;
        }
        return out;
    }
}

int main(){
    PINN pinn{};
    double x = 0.5;
    double num_der = pinn_fd::derivativeWrtInput(pinn, x, 2);
    double der_lag = pinn.derivativeWrtInput(x, 2);
    
    std::cout << "Numerical  derivative wrt input: " << num_der << "\n";
    std::cout << "Analytical derivative wrt input: " << der_lag << "\n";
    std::cout << "-----------------------------------------" << "\n";


    
}