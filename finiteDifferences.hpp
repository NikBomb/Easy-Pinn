#ifndef FINITEDIFF_HPP
#define FINITEDIFF_HPP

#include <functional>
#include <vector>
#include <cmath>

namespace easyPinn::finiteDifferences
{
    constexpr double EPS = 0.000001;

    double finite_diff_recursive(std::function<double(double)> fn, double x, int n)
    {
        if (n == 0)
        {
            return fn(x);
        }
        return (finite_diff_recursive(fn, x + EPS, n - 1) - finite_diff_recursive(fn, x, n - 1)) / (EPS);
    }

    std::vector<int> getBinomialCoefficients(int n)
    {
        std::vector<int> coeffs(n + 1, 1);
        for (int k = 1; k < n; ++k)
        {
            coeffs[k] = coeffs[k - 1] * (n - k + 1) / k;
        }
        return coeffs;
    }

    double finite_diff(std::function<double(double)> f, double x, int n)
    {
        auto binom = getBinomialCoefficients(n);
        double result = 0.0;

        for (int k = 0; k <= n; ++k)
        {
            double term = binom[k] * f(x + (n - k) * EPS);
            if (k % 2 != 0)
                term *= -1; // Apply (-1)^k
            result += term;
        }

        result /= std::pow(EPS, n);
        return result;
    }

}
#endif