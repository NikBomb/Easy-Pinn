#pragma once
#include "../api.h"
#include <array>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <random>

#include "finiteDifferences.h"

template<size_t NI, size_t NH, size_t NO>
class EASYPINN_API PINN{
    static constexpr size_t kParameterCount = NH * (NI + NO + 1);
    static constexpr size_t kInputHiddenCount = NI * NH;
    static constexpr size_t kHiddenBiasCount = NH;

    std::array<double, kParameterCount> m_parameters{};

    public:
    using InputArray = std::array<double, NI>;
    using OutputArray = std::array<double, NO>;
    using ParameterArray = std::array<double, kParameterCount>;
    using JacobianArray = std::array<std::array<double, NI>, NO>;
    using ParameterGradient = std::array<ParameterArray, NO>;
    using ParameterGradientByInput = std::array<std::array<ParameterArray, NI>, NO>;

    PINN(){
        std::random_device rd;
        std::mt19937 gen(rd());
        initialize(gen);
    }

    explicit PINN(uint32_t seed){
        std::mt19937 gen(seed);
        initialize(gen);
    }

    OutputArray evaluate(const InputArray& x) const{
        std::array<double, NH> hidden{};
        computeHidden(x, hidden, nullptr, nullptr, nullptr);

        OutputArray out{};
        for (size_t o = 0; o < NO; ++o){
            double sum = 0.0;
            for (size_t h = 0; h < NH; ++h){
                sum += outputWeight(h, o) * hidden[h];
            }
            out[o] = sum;
        }
        return out;
    }

    double evaluate(double x) const{
        static_assert(NI == 1 && NO == 1, "evaluate(double) requires NI==1 and NO==1.");
        return evaluate(InputArray{x})[0];
    }

    JacobianArray firstDerivativeWrtInput(const InputArray& x) const{
        std::array<double, NH> hidden{};
        std::array<double, NH> hidden_first{};
        // Lagaris-style PINN: dy/dx_i = sum_h v_ho * tanh'(z_h) * w_hi.
        computeHidden(x, hidden, &hidden_first, nullptr, nullptr);

        JacobianArray out{};
        for (size_t o = 0; o < NO; ++o){
            for (size_t i = 0; i < NI; ++i){
                double sum = 0.0;
                for (size_t h = 0; h < NH; ++h){
                    sum += outputWeight(h, o) * hidden_first[h] * inputWeight(h, i);
                }
                out[o][i] = sum;
            }
        }
        return out;
    }

    JacobianArray secondDerivativeWrtInput(const InputArray& x) const{
        std::array<double, NH> hidden{};
        std::array<double, NH> hidden_first{};
        std::array<double, NH> hidden_second{};
        // Lagaris-style PINN: d2y/dx_i^2 = sum_h v_ho * tanh''(z_h) * w_hi^2 (pure second derivatives only).
        computeHidden(x, hidden, &hidden_first, &hidden_second, nullptr);

        JacobianArray out{};
        for (size_t o = 0; o < NO; ++o){
            for (size_t i = 0; i < NI; ++i){
                double sum = 0.0;
                for (size_t h = 0; h < NH; ++h){
                    double w_in = inputWeight(h, i);
                    sum += outputWeight(h, o) * hidden_second[h] * w_in * w_in;
                }
                out[o][i] = sum;
            }
        }
        return out;
    }

    double derivativeWrtInput(double x, int n) const{
        static_assert(NI == 1 && NO == 1, "derivativeWrtInput(double, n) requires NI==1 and NO==1.");
        if (n == 1){
            return firstDerivativeWrtInput(InputArray{x})[0][0];
        }
        if (n == 2){
            return secondDerivativeWrtInput(InputArray{x})[0][0];
        }
        return easyPinn::finiteDifferences::finite_diff(
            [this](double v){ return evaluate(v); }, x, n);
    }

    ParameterGradient parameterGradient(const InputArray& x) const{
        std::array<double, NH> hidden{};
        std::array<double, NH> hidden_first{};
        computeHidden(x, hidden, &hidden_first, nullptr, nullptr);

        ParameterGradient out{};
        for (size_t o = 0; o < NO; ++o){
            for (size_t h = 0; h < NH; ++h){
                double w_out = outputWeight(h, o);
                // Lagaris-style: dy/dv_ho = tanh(z_h), dy/db_h = v_ho * tanh'(z_h), dy/dw_hi = v_ho * tanh'(z_h) * x_i.
                out[o][outputIndex(h, o)] = hidden[h];
                out[o][biasIndex(h)] = w_out * hidden_first[h];
                for (size_t i = 0; i < NI; ++i){
                    out[o][inputIndex(h, i)] = w_out * hidden_first[h] * x[i];
                }
            }
        }
        return out;
    }

    ParameterGradientByInput parameterGradientFirstDerivative(const InputArray& x) const{
        std::array<double, NH> hidden{};
        std::array<double, NH> hidden_first{};
        std::array<double, NH> hidden_second{};
        computeHidden(x, hidden, &hidden_first, &hidden_second, nullptr);

        ParameterGradientByInput out{};
        for (size_t o = 0; o < NO; ++o){
            for (size_t i = 0; i < NI; ++i){
                for (size_t h = 0; h < NH; ++h){
                    double w_out = outputWeight(h, o);
                    double w_in = inputWeight(h, i);
                    // Lagaris-style: d(dy/dx_i)/dv_ho = tanh'(z_h) * w_hi.
                    out[o][i][outputIndex(h, o)] = hidden_first[h] * w_in;
                    // Lagaris-style: d(dy/dx_i)/db_h = v_ho * tanh''(z_h) * w_hi.
                    out[o][i][biasIndex(h)] = w_out * w_in * hidden_second[h];
                    for (size_t k = 0; k < NI; ++k){
                        // Lagaris-style: d(dy/dx_i)/dw_hk includes chain term and direct term when k==i.
                        double term = w_out * w_in * hidden_second[h] * x[k];
                        if (k == i){
                            term += w_out * hidden_first[h];
                        }
                        out[o][i][inputIndex(h, k)] = term;
                    }
                }
            }
        }
        return out;
    }

    ParameterGradientByInput parameterGradientSecondDerivative(const InputArray& x) const{
        std::array<double, NH> hidden{};
        std::array<double, NH> hidden_first{};
        std::array<double, NH> hidden_second{};
        std::array<double, NH> hidden_third{};
        computeHidden(x, hidden, &hidden_first, &hidden_second, &hidden_third);

        ParameterGradientByInput out{};
        for (size_t o = 0; o < NO; ++o){
            for (size_t i = 0; i < NI; ++i){
                for (size_t h = 0; h < NH; ++h){
                    double w_out = outputWeight(h, o);
                    double w_in = inputWeight(h, i);
                    double w_in_sq = w_in * w_in;
                    // Lagaris-style: d(d2y/dx_i^2)/dv_ho = tanh''(z_h) * w_hi^2.
                    out[o][i][outputIndex(h, o)] = hidden_second[h] * w_in_sq;
                    // Lagaris-style: d(d2y/dx_i^2)/db_h = v_ho * tanh'''(z_h) * w_hi^2.
                    out[o][i][biasIndex(h)] = w_out * w_in_sq * hidden_third[h];
                    for (size_t k = 0; k < NI; ++k){
                        // Lagaris-style: d(d2y/dx_i^2)/dw_hk includes chain term and direct term when k==i.
                        double term = w_out * w_in_sq * hidden_third[h] * x[k];
                        if (k == i){
                            term += w_out * hidden_second[h] * 2.0 * w_in;
                        }
                        out[o][i][inputIndex(h, k)] = term;
                    }
                }
            }
        }
        return out;
    }

    constexpr size_t parameterCount() const{
        return kParameterCount;
    }

    double getParameter(size_t idx) const{
        return m_parameters[idx];
    }

    void setParameter(size_t idx, double value){
        m_parameters[idx] = value;
    }

    private:

    void initialize(std::mt19937& gen){
        std::uniform_real_distribution<double> dis(0.0, 1.0);
        std::generate(m_parameters.begin(), m_parameters.end(), [&](){ return dis(gen); });
    }

    size_t inputIndex(size_t h, size_t i) const{
        return h * NI + i;
    }

    size_t biasIndex(size_t h) const{
        return kInputHiddenCount + h;
    }

    size_t outputIndex(size_t h, size_t o) const{
        return kInputHiddenCount + kHiddenBiasCount + h * NO + o;
    }

    double inputWeight(size_t h, size_t i) const{
        return m_parameters[inputIndex(h, i)];
    }

    double outputWeight(size_t h, size_t o) const{
        return m_parameters[outputIndex(h, o)];
    }

    void computeHidden(const InputArray& x,
                       std::array<double, NH>& hidden,
                       std::array<double, NH>* hidden_first,
                       std::array<double, NH>* hidden_second,
                       std::array<double, NH>* hidden_third) const{
        for (size_t h = 0; h < NH; ++h){
            double z = m_parameters[biasIndex(h)];
            for (size_t i = 0; i < NI; ++i){
                z += inputWeight(h, i) * x[i];
            }
            double t = std::tanh(z);
            hidden[h] = t;

            if (hidden_first || hidden_second || hidden_third){
                // hidden_first stores tanh'(z_h), hidden_second stores tanh''(z_h).
                double t1 = 1.0 - t * t;
                if (hidden_first){
                    (*hidden_first)[h] = t1;
                }
                if (hidden_second || hidden_third){
                    double t2 = -2.0 * t * t1;
                    if (hidden_second){
                        (*hidden_second)[h] = t2;
                    }
                    if (hidden_third){
                        // tanh'''(z) = -2 * (tanh'(z)^2 + tanh(z) * tanh''(z)).
                        (*hidden_third)[h] = -2.0 * (t1 * t1 + t * t2);
                    }
                }
            }
        }
    }
};

namespace pinn_fd {

    template<size_t NI, size_t NH, size_t NO>
    double derivativeWrtInput(const PINN<NI, NH, NO>& pinn, double x, int n){
        static_assert(NI == 1 && NO == 1, "derivativeWrtInput(double, n) requires NI==1 and NO==1.");
        return easyPinn::finiteDifferences::finite_diff(
            [&pinn](double v){ return pinn.evaluate(v); }, x, n);
    }

    template<size_t NI, size_t NH, size_t NO>
    std::array<double, NH * (NI + NO + 1)> firstDerivativeWrtWeights(const PINN<NI, NH, NO>& pinn, double x){
        static_assert(NI == 1 && NO == 1, "firstDerivativeWrtWeights(double) requires NI==1 and NO==1.");
        std::array<double, NH * (NI + NO + 1)> out{};

        double eval = pinn.evaluate(x);
        for (size_t i = 0; i < out.size(); ++i){
            PINN<NI, NH, NO> p = pinn;
            p.setParameter(i, p.getParameter(i) + easyPinn::finiteDifferences::EPS);
            double new_eval = p.evaluate(x);
            out[i] = (new_eval - eval) / easyPinn::finiteDifferences::EPS;
        }
        return out;
    }
}
