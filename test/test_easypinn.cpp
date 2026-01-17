#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include "easypinn/public/adam.h"
#include "easypinn/public/PINN.h"

TEST_CASE("PINN compute close to analytical derivative") {
    PINN<1, 10, 1> pinn{123456};
    double x = 0.5;
    double num_der = pinn_fd::derivativeWrtInput(pinn, x, 2);
    double der_lag = pinn.derivativeWrtInput(x, 2);
    
    //Log the results for debugging
    std::cout << "Numerical derivative wrt input: " << num_der << "\n";
    std::cout << "Analytical derivative wrt input: " << der_lag << "\n";
    std::cout << "-----------------------------------------" << "\n";
    
    CHECK(num_der == doctest::Approx(der_lag).epsilon(0.01));
}

TEST_CASE("PINN parameter gradient matches finite differences") {
    PINN<1, 6, 1> pinn{9876};
    double x = 0.25;

    auto grad = pinn.parameterGradient(PINN<1, 6, 1>::InputArray{x});
    auto fd = pinn_fd::firstDerivativeWrtWeights(pinn, x);

    for (size_t i = 0; i < fd.size(); ++i){
        CHECK(grad[0][i] == doctest::Approx(fd[i]).epsilon(1e-4));
    }
}

TEST_CASE("PINN parameter gradient of first derivative matches finite differences") {
    PINN<1, 5, 1> pinn{2468};
    PINN<1, 5, 1>::InputArray x{0.35};

    auto grad = pinn.parameterGradientFirstDerivative(x);

    const double eps = easyPinn::finiteDifferences::EPS;
    for (size_t i = 0; i < pinn.parameterCount(); ++i){
        PINN<1, 5, 1> p_plus = pinn;
        PINN<1, 5, 1> p_minus = pinn;
        p_plus.setParameter(i, p_plus.getParameter(i) + eps);
        p_minus.setParameter(i, p_minus.getParameter(i) - eps);

        double fd = (p_plus.firstDerivativeWrtInput(x)[0][0] -
                     p_minus.firstDerivativeWrtInput(x)[0][0]) / (2.0 * eps);
        CHECK(grad[0][0][i] == doctest::Approx(fd).epsilon(1e-4));
    }
}

TEST_CASE("PINN parameter gradient of second derivative matches finite differences") {
    PINN<1, 5, 1> pinn{13579};
    PINN<1, 5, 1>::InputArray x{0.42};

    auto grad = pinn.parameterGradientSecondDerivative(x);

    const double eps = easyPinn::finiteDifferences::EPS;
    for (size_t i = 0; i < pinn.parameterCount(); ++i){
        PINN<1, 5, 1> p_plus = pinn;
        PINN<1, 5, 1> p_minus = pinn;
        p_plus.setParameter(i, p_plus.getParameter(i) + eps);
        p_minus.setParameter(i, p_minus.getParameter(i) - eps);

        double fd = (p_plus.secondDerivativeWrtInput(x)[0][0] -
                     p_minus.secondDerivativeWrtInput(x)[0][0]) / (2.0 * eps);
        CHECK(grad[0][0][i] == doctest::Approx(fd).epsilon(1e-4));
    }
}

TEST_CASE("Adam single-step update matches expected") {
    easyPinn::AdamOptimizer<1> optimizer(0.1, 0.9, 0.999, 1e-8);
    std::array<double, 1> params{1.0};
    std::array<double, 1> grads{0.5};

    optimizer.step(params, grads);

    CHECK(params[0] == doctest::Approx(0.9));
}
