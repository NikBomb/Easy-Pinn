# SPEC-0007 Architecture Notes

## Summary
Move all Problem 0 logic into `app/problem0/`, keep it out of the public library headers, and update the app to write outputs under `results/problem0/`. Add an app-specific test target that includes the app-local headers.

## Proposed structure (modules/classes)
- app/problem0/problem0_main.cpp (standalone executable entry)
  - Responsibility: configure training constants, run training loop, write CSV outputs.
- app/problem0/problem0.h / problem0_trainer.h (app-local)
  - Responsibility: trial, residual, loss, and trainer logic for Problem 0.
- app/problem0/problem0_tests.cpp (app-specific tests)
  - Responsibility: validate Problem 0 trial/residual/loss and trainer/optimizer behavior.

## Key interactions / data flow
- App constructs `PINN` with fixed seed and `AdamOptimizer`.
- App defines 10 collocation points in [0,1].
- Training loop iterates epochs, computes residual metric across collocation points (mean absolute residual) and stops when threshold met or epoch limit reached.
- App writes residuals-per-epoch CSV and final solution samples (101 points) CSV under `results/problem0/`.

## Interfaces (sketch)
- main()
  - configure: epochs=1000, residual_threshold=1e-3, collocation_count=10, sample_count=101, seed
  - train: for epoch in [0..max): update parameters per collocation point
  - output: write `results/problem0/problem0_residuals.csv` and `results/problem0/problem0_solution.csv`

## Risks + mitigations
- Risk: breaking public API by removing headers from `lib/include`.
  - Mitigation: ensure only app code and app-specific tests include Problem 0 headers.
- Risk: CMake wiring for app-specific tests.
  - Mitigation: add a dedicated test target with include path `app/problem0`.
- Risk: results directory missing.
  - Mitigation: ensure `results/problem0/` is created before writing.

## Notes for Planner
- Keep Problem 0 headers app-local and out of library install rules.
- Update includes in app and tests to point at `app/problem0/`.
- Keep tests in a separate target under `app/` or `test/` with explicit include dirs.
