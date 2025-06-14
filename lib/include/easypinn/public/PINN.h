#pragma once
#include "../api.h"
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <functional>

#include "finiteDifferences.h"


class EASYPINN_API PINN{
    std::vector<double> m_hweights; 
    std::vector<double> m_hbiases;
    std::vector<double> m_weights;
    
    public:
    PINN();
    
    double evaluate(double x) const;

    void setHiddenWeigth(size_t idx, double value);

    void setHiddenBias(size_t idx, double value);

    void setWeight(size_t idx, double value);

    double getWeight(size_t idx);

    double getWeightSize() const; 

    double derivativeWrtInput(double x, int n);

    private:
    
    double n_deriv_tanh(size_t order, double x);
};

namespace pinn_fd {

    double derivativeWrtInput(const PINN& pinn, double x, int n);

    std::vector<double> firstDerivativeWrtWeights(const PINN& pinn, double x);
}