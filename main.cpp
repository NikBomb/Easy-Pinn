#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <functional>

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

    double DerivativeWrtInput(double x)
    {
        double res = 0.0;
        for (size_t i = 0; i < m_hweights.size(); ++i){
            double z_i = m_hweights[i] * x + m_hbiases[i];
            res += m_weights[i] * m_hweights[i] * n_deriv_tanh(1, z_i);
        } 
        return res;
    }

    private:
    

    double n_deriv_tanh(size_t order, double x){
        return finite_diff_recursive([](double x){return std::tanh(x);}, x, order);
    }

    double finite_diff_recursive(std::function<double(double)> fn, double x, int n) {
        if (n == 0) {
            return fn(x);
        }
        return (finite_diff_recursive(fn, x + EPS , n - 1) - finite_diff_recursive(fn, x , n - 1)) / (EPS);
    }
};

namespace finite_difference {

    std::vector<double> n_derivative_num (std::function<std::vector<double>(std::vector<double>)> fn, size_t n) {
        
    }

    double firstDerivativeWrtInput(const PINN& pinn, double x){
        double out = pinn.evaluate(x);
        double out1 = pinn.evaluate(x + EPS);
        return (out1 - out) / EPS;
    }

    std::vector<double> firstDerivativeWrtWeights(const PINN& pinn, double x){
        std::vector<double> out(pinn.getWeightSize(), 0.0);

        double eval = pinn.evaluate(x);

        for (int i =0; i < pinn.getWeightSize(); ++i){
            PINN p = pinn;
            p.setWeight(i, p.getWeight(i) + EPS);
            double new_eval = p.evaluate(x);
            out[i] = (new_eval - eval)/EPS;
        }
        return out;
    }
}

int main(){
    PINN pinn{};
    double x = 0.5;
    double num_der = finite_difference::firstDerivativeWrtInput(pinn, x);
    double der_lag = pinn.DerivativeWrtInput(x);
    
    std::cout << "Numerical first derivative wrt input: " << num_der << "\n";
    std::cout << "Analytical first derivative wrt input: " << der_lag << "\n";
    std::cout << "-----------------------------------------" << "\n";


    
}