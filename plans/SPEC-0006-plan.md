# SPEC-0006 Plan

## Spec summary
Implement Problem 0 trial function, residual, and loss objects with forward/backward passes for y' = y, y(0)=1 using y_trial(x)=1 + x * N(x).

## Acceptance Criteria checklist
- [ ] AC1: Given any network parameters and x=0, the trial function returns y_trial(0)=1 exactly.
- [ ] AC2: Given N(x), the trial function forward pass returns y_trial(x) = 1 + x * N(x).
- [ ] AC3: Given a trial function value and derivative at x, residual equals d/dx y_trial(x) - y_trial(x).
- [ ] AC4: Given residuals at collocation points, loss equals the mean of squared residuals.

## Plan steps (<= 6)
1) Review existing PINN outputs/derivatives and decide header-only vs source-backed objects for trial/residual/loss in `lib/include/easypinn/public/`.
2) Implement `TrialFunctionProblem0` forward/backward for y_trial=1+x*N(x), returning gradient to N(x).
3) Implement `ResidualProblem0` forward/backward using r=dy_trial_dx - y_trial, returning gradients to y_trial and dy_trial_dx.
4) Implement `ResidualMSELoss` forward/backward computing mean of squared residuals and gradient to residuals.
5) Add unit tests for trial boundary/forward, residual formula at x=0.5, and loss mean-of-squares.
6) Add a light composition test using PINN outputs/derivatives to ensure wiring is consistent.

## File-level changes
Add:
- `lib/include/easypinn/public/problem0.h` (trial/residual/loss objects)

Edit:
- `test/test_easypinn.cpp` (new unit tests)
- `lib/CMakeLists.txt` or `lib/src` only if a non-header implementation is needed

## Testing plan
- Unit: doctest cases for trial, residual, loss, composition.
- Integration: none.
- Smoke/manual: `ctest --test-dir build`.

## Risks / assumptions
- Assumptions:
  - Use mean as sum/N in loss.
  - Use one collocation point at x=0.5 for tests.
- Risks + mitigations:
  - Risk: mismatch with existing derivative conventions.
    - Mitigation: keep interfaces explicit and test against hand-computed values.
  - Risk: ambiguous ownership of gradients.
    - Mitigation: define backward signatures clearly per object and document in header comments.
  - Risk: template/header bloat if too generic.
    - Mitigation: keep Problem 0 objects concrete and minimal.
