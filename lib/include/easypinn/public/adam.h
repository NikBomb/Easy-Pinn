#pragma once

#include "../api.h"
#include <array>
#include <cmath>
#include <cstddef>

namespace easyPinn {

template<size_t N>
class EASYPINN_API AdamOptimizer {
public:
    using ParameterArray = std::array<double, N>;

    AdamOptimizer(double lr = 1e-3,
                  double beta1 = 0.9,
                  double beta2 = 0.999,
                  double eps = 1e-8)
        : m_lr(lr), m_beta1(beta1), m_beta2(beta2), m_eps(eps) {}

    void step(ParameterArray& parameters, const ParameterArray& gradients) {
        ++m_t;
        const double beta1_pow = std::pow(m_beta1, static_cast<double>(m_t));
        const double beta2_pow = std::pow(m_beta2, static_cast<double>(m_t));

        for (size_t i = 0; i < N; ++i) {
            m_m[i] = m_beta1 * m_m[i] + (1.0 - m_beta1) * gradients[i];
            m_v[i] = m_beta2 * m_v[i] + (1.0 - m_beta2) * gradients[i] * gradients[i];

            const double m_hat = m_m[i] / (1.0 - beta1_pow);
            const double v_hat = m_v[i] / (1.0 - beta2_pow);
            parameters[i] -= m_lr * m_hat / (std::sqrt(v_hat) + m_eps);
        }
    }

private:
    ParameterArray m_m{};
    ParameterArray m_v{};
    size_t m_t = 0;
    double m_lr;
    double m_beta1;
    double m_beta2;
    double m_eps;
};

} // namespace easyPinn
