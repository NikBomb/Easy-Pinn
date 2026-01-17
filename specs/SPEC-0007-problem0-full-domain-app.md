# SPEC-0007: Problem 0 full-domain app with results export

## Status
Complete

## Summary
Create a standalone executable that trains Problem 0 over x in [0,1] using app-local Problem 0 code, and writes residual-per-epoch and solution values over [0,1] (including exact solution and error) to CSV files in `results/problem0/`.

## Goals
- G1: Provide an executable app that runs Problem 0 training over the full domain [0,1].
- G2: Export residual per epoch to a CSV file in `results/`.
- G3: Export the solution values y(x) over [0,1] with exact solution and error to a CSV file in `results/`.

## Non-goals
- NG1: Add new optimizer types or training algorithms beyond existing Adam + Problem 0 trainer.
- NG2: Build a general experiment runner or config system.

## Requirements
- R1: The app is a standalone executable dedicated to Problem 0 training.
- R2: The app trains Problem 0 over x in [0,1] using app-local trial/residual/loss and trainer code.
- R3: Use 10 collocation points in [0,1] for training.
- R4: Training runs until either a sensible epoch limit is reached or a residual threshold is met (whichever comes first).
- R5: The app writes residual values per epoch to a CSV file in `results/problem0/`.
- R6: The app writes solution values y(x), exact values exp(x), and errors over [0,1] to a CSV file in `results/problem0/`.
- R7: Results format and sampling density are deterministic for a fixed seed and fixed settings.
- R8: No automated tests are required for this spec.
 - R9: Default epoch limit is 1000 and residual threshold is 1e-3.
 - R10: Solution sampling uses 101 evenly spaced points in [0,1].
 - R11: Output filenames are `results/problem0/problem0_residuals.csv` and `results/problem0/problem0_solution.csv`.
 - R12: All Problem 0 headers live under `app/problem0/` and are not installed as part of the library.
 - R13: Problem 0 tests are built as an app-specific test target that includes `app/problem0/` headers.

## Acceptance Criteria
- AC1: Given a fixed seed and configured stop criteria, running the app produces a residual CSV with one entry per epoch.
- AC2: Given a configured set of sample points in [0,1], running the app produces a solution CSV with one entry per sample point and columns for x, y_trial(x), exp(x), and error.
- AC3: Results are written to `results/problem0/` with filenames that match the documented naming scheme.
- AC4: Problem 0 headers are located under `app/problem0/` and no Problem 0 headers remain under `lib/include/easypinn/public/`.
- AC5: Problem 0 tests build as an app-specific test target using `app/problem0/` headers.

## Design notes (optional)
- Constraints: use existing Problem 0 infrastructure; no new external dependencies; CSV output.
 - Assumptions: epoch limit 1000, residual threshold 1e-3, solution points 101, fixed filenames.

## Testing
- Unit: None.
- Integration: None.
- Manual / smoke: Run the app once and verify files exist and contain the expected number of rows.
