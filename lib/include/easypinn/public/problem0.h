#pragma once

#include "../api.h"
#include <array>
#include <cstddef>

namespace easyPinn::problem0 {

struct TrialOutput {
    double value{};
    double derivative{};
};

struct TrialBackward {
    double grad_n{};
    double grad_dn_dx{};
};

struct ResidualBackward {
    double grad_trial{};
    double grad_trial_derivative{};
};

struct EASYPINN_API TrialFunctionProblem0 {
    TrialOutput forward(double x, double n, double dn_dx) const {
        return {1.0 + x * n, n + x * dn_dx};
    }

    TrialBackward backward(double x, double grad_trial, double grad_trial_derivative) const {
        return {grad_trial * x + grad_trial_derivative, grad_trial_derivative * x};
    }
};

struct EASYPINN_API ResidualProblem0 {
    double forward(double trial, double trial_derivative) const {
        return trial_derivative - trial;
    }

    ResidualBackward backward(double grad_residual) const {
        return {-grad_residual, grad_residual};
    }
};

struct EASYPINN_API ResidualMSELoss {
    template<size_t N>
    double forward(const std::array<double, N>& residuals) const {
        double sum = 0.0;
        for (double r : residuals) {
            sum += r * r;
        }
        return sum / static_cast<double>(N);
    }

    template<size_t N>
    std::array<double, N> backward(const std::array<double, N>& residuals,
                                   double grad_loss = 1.0) const {
        std::array<double, N> grads{};
        const double scale = (2.0 * grad_loss) / static_cast<double>(N);
        for (size_t i = 0; i < N; ++i) {
            grads[i] = scale * residuals[i];
        }
        return grads;
    }
};

} // namespace easyPinn::problem0
