# SPEC-0007 Plan

## Spec summary
Build a standalone Problem 0 executable that trains over [0,1], stops on epoch limit or residual threshold, and writes residual-per-epoch and solution CSVs to `results/`.

## Acceptance Criteria checklist
- [ ] AC1: Given a fixed seed and configured stop criteria, running the app produces a residual CSV with one entry per epoch.
- [ ] AC2: Given a configured set of sample points in [0,1], running the app produces a solution CSV with one entry per sample point.
- [ ] AC3: Results are written to `results/` with filenames that match the documented naming scheme.

## Plan steps (<= 6)
1) Add a new standalone executable under `app/` and register it in CMake.
2) Implement a simple training loop over 10 collocation points using `Problem0Trainer` and Adam, stopping on epoch limit or residual threshold.
3) Define residual aggregation across collocation points (mean absolute residual) and log one value per epoch.
4) Write `results/problem0_residuals.csv` with header and one row per epoch.
5) Evaluate y(x) over 101 evenly spaced points and write `results/problem0_solution.csv` with header and one row per point.
6) Add a short run note if needed and verify output files exist after a run.

## File-level changes
Add:
- `app/problem0_main.cpp`

Edit:
- `app/CMakeLists.txt` (or root `CMakeLists.txt` to add executable)
- `CMakeLists.txt` if required to wire the new app

## Testing plan
- Unit: None.
- Integration: None.
- Smoke/manual: build the new executable and run it once to verify output files and row counts.

## Risks / assumptions
- Assumptions:
  - Residual aggregation uses mean absolute residual over collocation points.
  - Output CSVs include header row.
- Risks + mitigations:
  - Risk: inconsistent residual definition.
    - Mitigation: document aggregation in the app.
  - Risk: results directory missing.
    - Mitigation: create `results/` if needed.
  - Risk: training instability.
    - Mitigation: fixed seed, small learning rate, stop threshold.
