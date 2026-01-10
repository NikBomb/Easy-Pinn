#include "easypinn/public/PINN.h"
#include <iostream>

int main(){
    PINN<1, 10, 1> pinn{};
    double x = 0.5;
    double num_der = pinn_fd::derivativeWrtInput(pinn, x, 2);
    double der_lag = pinn.derivativeWrtInput(x, 2);
    
    std::cout << "Numerical  derivative wrt input: " << num_der << "\n";
    std::cout << "Analytical derivative wrt input: " << der_lag << "\n";
    std::cout << "-----------------------------------------" << "\n";
}
