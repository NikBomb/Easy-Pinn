# SPEC-0006: Problem 0 PINN baseline (y' = y, y(0) = 1)

## Status
Draft

## Summary
Add the Problem 0 trial function, residual, and loss objects (with forward/backward) so the PINN can represent the ODE y' = y with the boundary condition y(0) = 1.

## Goals
- G1: Provide a trial function that exactly satisfies y(0) = 1.
- G2: Define residual and loss for the ODE y' = y on x in [0, 1].
- G3: Add basic tests for trial function, residual, and loss.

## Non-goals
- NG1: Implement a full training/optimizer pipeline.
- NG2: Generalize to PDEs or multi-dimensional domains.

## Requirements
- R1: Trial function must enforce y(0) = 1 for any network parameters.
- R2: Trial function for Problem 0 is y_trial(x) = 1 + x * N(x), where N(x) is the network output.
- R3: Trial, residual, and loss are implemented as objects with forward and backward passes.
- R4: Residual for Problem 0 is r(x) = d/dx y_trial(x) - y_trial(x).
- R5: Loss is the mean of squared residuals over provided collocation points.
- R6: Tests cover trial function boundary, residual formula, and loss aggregation.

## Acceptance Criteria
- AC1: Given any network parameters and x=0, the trial function returns y_trial(0)=1 exactly.
- AC2: Given N(x), the trial function forward pass returns y_trial(x) = 1 + x * N(x).
- AC3: Given a trial function value and derivative at x, residual equals d/dx y_trial(x) - y_trial(x).
- AC4: Given residuals at collocation points, loss equals the mean of squared residuals.

## Design notes (optional)
- Constraints: use analytic derivatives already implemented; no new heavy dependencies.

## Testing
- Unit: trial function boundary; residual formula; loss mean-of-squares.
- Integration: none required.
- Manual / smoke: optional evaluation at a single point.
