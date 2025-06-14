#ifndef FINITEDIFF_HPP
#define FINITEDIFF_HPP

#include <functional>

namespace easyPinn::finiteDifferences
{
    constexpr double EPS();
    
    double finite_diff_recursive(std::function<double(double)> fn, double x, int n);
    
    std::vector<int> getBinomialCoefficients(int n);
    
    double finite_diff(std::function<double(double)> f, double x, int n);

}
#endif