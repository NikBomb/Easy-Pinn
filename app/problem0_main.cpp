#include "easypinn/public/PINN.h"
#include "easypinn/public/adam.h"
#include "easypinn/public/problem0_trainer.h"

#include <array>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

namespace {
constexpr size_t kEpochLimit = 1000;
constexpr double kResidualThreshold = 1e-3;
constexpr size_t kCollocationCount = 10;
constexpr size_t kSampleCount = 101;
constexpr uint32_t kSeed = 2025;
constexpr double kLearningRate = 1e-2;

std::array<double, kCollocationCount> makeCollocationPoints() {
    std::array<double, kCollocationCount> points{};
    const double step = 1.0 / static_cast<double>(kCollocationCount - 1);
    for (size_t i = 0; i < kCollocationCount; ++i) {
        points[i] = step * static_cast<double>(i);
    }
    return points;
}

std::array<double, kSampleCount> makeSamplePoints() {
    std::array<double, kSampleCount> points{};
    const double step = 1.0 / static_cast<double>(kSampleCount - 1);
    for (size_t i = 0; i < kSampleCount; ++i) {
        points[i] = step * static_cast<double>(i);
    }
    return points;
}

double meanAbsoluteResidual(const std::array<double, kCollocationCount>& residuals) {
    double sum = 0.0;
    for (double r : residuals) {
        sum += std::abs(r);
    }
    return sum / static_cast<double>(kCollocationCount);
}
} // namespace

int main() {
    using Pinn = PINN<1, 6, 1>;
    Pinn pinn{kSeed};
    constexpr size_t kParamCount = std::tuple_size<Pinn::ParameterArray>::value;
    easyPinn::AdamOptimizer<kParamCount> optimizer(kLearningRate);
    easyPinn::problem0::Problem0Trainer trainer(pinn, optimizer);

    const auto collocation = makeCollocationPoints();
    std::vector<double> residual_history;
    residual_history.reserve(kEpochLimit);

    for (size_t epoch = 0; epoch < kEpochLimit; ++epoch) {
        std::array<double, kCollocationCount> residuals{};
        for (size_t i = 0; i < kCollocationCount; ++i) {
            trainer.train(collocation[i], 1);
            residuals[i] = trainer.residualAt(collocation[i]);
        }
        const double mean_residual = meanAbsoluteResidual(residuals);
        residual_history.push_back(mean_residual);
        if (mean_residual <= kResidualThreshold) {
            break;
        }
    }

    std::filesystem::create_directories("results");
    {
        std::ofstream out("results/problem0_residuals.csv");
        out << "epoch,residual\n";
        for (size_t i = 0; i < residual_history.size(); ++i) {
            out << i << "," << residual_history[i] << "\n";
        }
    }

    easyPinn::problem0::TrialFunctionProblem0 trial_fn;
    easyPinn::problem0::ResidualProblem0 residual_fn;
    const auto samples = makeSamplePoints();
    {
        std::ofstream out("results/problem0_solution.csv");
        out << "x,y_trial,exp_x,error\n";
        for (double x : samples) {
            const double n = pinn.evaluate(x);
            const double dn_dx = pinn.derivativeWrtInput(x, 1);
            const auto trial_out = trial_fn.forward(x, n, dn_dx);
            const double exact = std::exp(x);
            const double residual = residual_fn.forward(trial_out.value, trial_out.derivative);
            const double error = trial_out.value - exact;
            out << x << "," << trial_out.value << "," << exact << "," << error*error << "\n";
        }
    }

    std::cout << "Completed " << residual_history.size() << " epochs\n";
    return 0;
}
