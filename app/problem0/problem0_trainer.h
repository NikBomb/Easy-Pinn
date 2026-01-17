#ifndef EASYPINN_APP_PROBLEM0_PROBLEM0_TRAINER_H
#define EASYPINN_APP_PROBLEM0_PROBLEM0_TRAINER_H

#include "problem0.h"
#include "easypinn/public/adam.h"
#include "easypinn/public/PINN.h"
#include <array>
#include <cstddef>
#include <tuple>

namespace easyPinn::problem0 {

template<typename Pinn>
class Problem0Trainer {
public:
    using ParameterArray = typename Pinn::ParameterArray;
    static constexpr size_t kParameterCount = std::tuple_size<ParameterArray>::value;
    using Optimizer = easyPinn::AdamOptimizer<kParameterCount>;

    Problem0Trainer(Pinn& pinn, Optimizer& optimizer)
        : m_pinn(pinn), m_optimizer(optimizer) {}

    struct Metrics {
        double residual{};
        double loss{};
    };

    struct TrainMetrics {
        Metrics initial{};
        Metrics final{};
    };

    double residualAt(double x) const {
        return computeResidual(x).residual;
    }

    Metrics metricsAt(double x) const {
        const double residual = computeResidual(x).residual;
        ResidualMSELoss loss_fn;
        const std::array<double, 1> residuals{residual};
        return {residual, loss_fn.forward(residuals)};
    }

    void train(double x, size_t steps) {
        for (size_t i = 0; i < steps; ++i) {
            step(x);
        }
    }

    TrainMetrics trainWithMetrics(double x, size_t steps) {
        TrainMetrics metrics;
        metrics.initial = metricsAt(x);
        train(x, steps);
        metrics.final = metricsAt(x);
        return metrics;
    }

private:
    struct ResidualState {
        double trial{};
        double trial_derivative{};
        double residual{};
    };

    ResidualState computeResidual(double x) const {
        const double n = m_pinn.evaluate(x);
        const double dn_dx = m_pinn.derivativeWrtInput(x, 1);

        TrialFunctionProblem0 trial_fn;
        const auto trial_out = trial_fn.forward(x, n, dn_dx);

        ResidualProblem0 residual_fn;
        const double residual = residual_fn.forward(trial_out.value, trial_out.derivative);

        return {trial_out.value, trial_out.derivative, residual};
    }

    void step(double x) {
        static_assert(std::tuple_size<typename Pinn::InputArray>::value == 1,
                      "Problem0Trainer requires NI == 1.");
        static_assert(std::tuple_size<typename Pinn::OutputArray>::value == 1,
                      "Problem0Trainer requires NO == 1.");

        ResidualState state = computeResidual(x);

        ResidualMSELoss loss_fn;
        const std::array<double, 1> residuals{state.residual};
        auto grad_residuals = loss_fn.backward(residuals);

        ResidualProblem0 residual_fn;
        const auto residual_grads = residual_fn.backward(grad_residuals[0]);

        TrialFunctionProblem0 trial_fn;
        const auto trial_grads =
            trial_fn.backward(x, residual_grads.grad_trial, residual_grads.grad_trial_derivative);

        typename Pinn::InputArray input{x};
        auto grad_n = m_pinn.parameterGradient(input);
        auto grad_dn = m_pinn.parameterGradientFirstDerivative(input);

        ParameterArray grads{};
        for (size_t i = 0; i < grads.size(); ++i) {
            grads[i] = trial_grads.grad_n * grad_n[0][i] +
                       trial_grads.grad_dn_dx * grad_dn[0][0][i];
        }

        ParameterArray params{};
        for (size_t i = 0; i < params.size(); ++i) {
            params[i] = m_pinn.getParameter(i);
        }

        m_optimizer.step(params, grads);

        for (size_t i = 0; i < params.size(); ++i) {
            m_pinn.setParameter(i, params[i]);
        }
    }

    Pinn& m_pinn;
    Optimizer& m_optimizer;
};

} // namespace easyPinn::problem0

#endif  // EASYPINN_APP_PROBLEM0_PROBLEM0_TRAINER_H
