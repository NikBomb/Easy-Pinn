# SPEC-0007 Plan

## Spec summary
Build a standalone Problem 0 executable with app-local headers and app-specific tests, and write residual-per-epoch and solution CSVs to `results/problem0/`.

## Acceptance Criteria checklist
- [ ] AC1: Given a fixed seed and configured stop criteria, running the app produces a residual CSV with one entry per epoch.
- [ ] AC2: Given a configured set of sample points in [0,1], running the app produces a solution CSV with one entry per sample point.
- [ ] AC3: Results are written to `results/problem0/` with filenames that match the documented naming scheme.
- [ ] AC4: Problem 0 headers are located under `app/problem0/` and no Problem 0 headers remain under `lib/include/easypinn/public/`.
- [ ] AC5: Problem 0 tests build as an app-specific test target using `app/problem0/` headers.

## Plan steps (<= 6)
1) Move Problem 0 headers (`problem0.h`, `problem0_trainer.h`) into `app/problem0/` and update includes.
2) Update `problem0_app` to include app-local headers and write outputs under `results/problem0/`.
3) Adjust CSV output headers to match spec and keep residual aggregation as mean absolute residual.
4) Add an app-specific test target that includes `app/problem0/` headers and move Problem 0 tests to it.
5) Remove Problem 0 headers from `lib/include/easypinn/public/` and ensure library install rules do not include them.
6) Build and run the app to confirm output files and row counts.

## File-level changes
Add:
- `app/problem0/` (Problem 0 headers and executable source)
- `app/problem0/problem0_tests.cpp`

Edit:
- `app/CMakeLists.txt` (add app-specific test target)
- `app/problem0_main.cpp` (move/rename)
- `test/CMakeLists.txt` (remove Problem 0 tests from library test target)
- `lib/include/easypinn/public/` (remove Problem 0 headers)

## Testing plan
- Unit: app-specific test target for Problem 0 headers.
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
    - Mitigation: create `results/problem0/` if needed.
  - Risk: training instability.
    - Mitigation: fixed seed, small learning rate, stop threshold.
