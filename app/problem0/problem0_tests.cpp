#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <array>
#include <cmath>
#include <tuple>

#include "problem0.h"
#include "problem0_trainer.h"
#include "easypinn/public/adam.h"
#include "easypinn/public/PINN.h"

TEST_CASE("Problem 0 trial function forward/backward") {
    easyPinn::problem0::TrialFunctionProblem0 trial;

    const double x = 0.5;
    const double n = 2.0;
    const double dn_dx = 0.3;

    auto out = trial.forward(x, n, dn_dx);
    CHECK(out.value == doctest::Approx(1.0 + x * n));
    CHECK(out.derivative == doctest::Approx(n + x * dn_dx));

    const double grad_trial = 1.1;
    const double grad_trial_derivative = -0.4;
    auto grads = trial.backward(x, grad_trial, grad_trial_derivative);
    CHECK(grads.grad_n == doctest::Approx(grad_trial * x + grad_trial_derivative));
    CHECK(grads.grad_dn_dx == doctest::Approx(grad_trial_derivative * x));
}

TEST_CASE("Problem 0 residual forward/backward") {
    easyPinn::problem0::ResidualProblem0 residual;

    const double trial = 1.2;
    const double trial_derivative = 0.7;
    const double r = residual.forward(trial, trial_derivative);
    CHECK(r == doctest::Approx(trial_derivative - trial));

    const double grad_r = 2.0;
    auto grads = residual.backward(grad_r);
    CHECK(grads.grad_trial == doctest::Approx(-grad_r));
    CHECK(grads.grad_trial_derivative == doctest::Approx(grad_r));
}

TEST_CASE("Problem 0 residual MSE loss") {
    easyPinn::problem0::ResidualMSELoss loss;

    const std::array<double, 2> residuals{1.0, -2.0};
    const double value = loss.forward(residuals);
    CHECK(value == doctest::Approx(2.5));

    auto grads = loss.backward(residuals);
    CHECK(grads[0] == doctest::Approx(1.0));
    CHECK(grads[1] == doctest::Approx(-2.0));
}

TEST_CASE("Problem 0 training reduces residual at x=0.5") {
    PINN<1, 6, 1> pinn{4242};
    constexpr size_t kParamCount = std::tuple_size<PINN<1, 6, 1>::ParameterArray>::value;
    easyPinn::AdamOptimizer<kParamCount> optimizer(1e-2);
    easyPinn::problem0::Problem0Trainer trainer(pinn, optimizer);

    const double x = 0.5;
    const double initial_residual = trainer.residualAt(x);
    trainer.train(x, 100);
    const double final_residual = trainer.residualAt(x);

    CHECK(std::abs(final_residual) < std::abs(initial_residual));
}
