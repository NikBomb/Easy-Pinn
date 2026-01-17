# SPEC-0007 Architecture Notes

## Summary
Add a standalone Problem 0 executable that trains on 10 collocation points over [0,1], stops on epoch limit or residual threshold, and writes CSV outputs for residuals per epoch and solution samples over [0,1].

## Proposed structure (modules/classes)
- app/problem0_main.cpp (new executable entry)
  - Responsibility: configure training constants, run training loop, write CSV outputs.
- Problem0Trainer (existing)
  - Responsibility: per-step parameter updates and metrics at a point.
- CSV writer helpers (local to app file or small utility)
  - Responsibility: write two CSV files to `results/`.

## Key interactions / data flow
- App constructs `PINN` with fixed seed and `AdamOptimizer`.
- App defines 10 collocation points in [0,1].
- Training loop iterates epochs, computes residual metric across collocation points (mean absolute or mean squared residual) and stops when threshold met or epoch limit reached.
- App writes residuals-per-epoch CSV and final solution samples (101 points) CSV.

## Interfaces (sketch)
- main()
  - configure: epochs=1000, residual_threshold=1e-3, collocation_count=10, sample_count=101, seed
  - train: for epoch in [0..max): update parameters (e.g., per collocation point)
  - output: write `results/problem0_residuals.csv` and `results/problem0_solution.csv`

## Risks + mitigations
- Risk: ambiguous definition of "residual" across multiple collocation points.
  - Mitigation: define explicit aggregation (e.g., mean absolute residual) in app and document.
- Risk: results directory missing.
  - Mitigation: ensure `results/` is created before writing.
- Risk: training loop performance or instability.
  - Mitigation: keep simple and deterministic; fixed seed.

## Notes for Planner
- Keep all configuration constants near main for easy change.
- Prefer simple CSV with header row: epoch,residual and x,y.
- No tests required; add a short README note if needed.
