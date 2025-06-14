#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include "easypinn/public/PINN.h"

TEST_CASE("PINN compute close to analytical derivative") {
    PINN pinn{};
    double x = 0.5;
    double num_der = pinn_fd::derivativeWrtInput(pinn, x, 2);
    double der_lag = pinn.derivativeWrtInput(x, 2);
    
    //Log the results for debugging
    std::cout << "Numerical derivative wrt input: " << num_der << "\n";
    std::cout << "Analytical derivative wrt input: " << der_lag << "\n";
    std::cout << "-----------------------------------------" << "\n";
    
    CHECK(num_der == doctest::Approx(der_lag).epsilon(0.01));
}
