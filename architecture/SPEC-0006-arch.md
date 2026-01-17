# SPEC-0006 Architecture Notes

## Summary
Introduce dedicated objects for the Problem 0 trial function, residual, and loss. Each object supports forward and backward passes and composes cleanly with the existing Lagaris network derivatives.

## Proposed structure (modules/classes)
- TrialFunctionProblem0
  - Responsibility: compute y_trial(x) = 1 + x * N(x) and its backward pass to N(x).
- ResidualProblem0
  - Responsibility: compute r(x) = d/dx y_trial(x) - y_trial(x) and backward to y_trial and d/dx y_trial.
- ResidualMSELoss
  - Responsibility: compute mean squared residual over collocation points and backward to residuals.

## Key interactions / data flow
- Network provides N(x) and dN/dx (already implemented).
- TrialFunctionProblem0 forward takes x and N(x) and yields y_trial(x); backward yields dL/dN(x).
- ResidualProblem0 forward takes y_trial(x) and dy_trial/dx and yields residual r(x); backward yields gradients to y_trial and dy_trial/dx.
- ResidualMSELoss forward takes residuals and yields scalar loss; backward yields gradients to residuals.

## Interfaces (sketch)
- TrialFunctionProblem0
  - forward(x, n): y_trial
  - backward(x, grad_y_trial): grad_n
- ResidualProblem0
  - forward(y_trial, dy_trial_dx): residual
  - backward(grad_residual): (grad_y_trial, grad_dy_trial_dx)
- ResidualMSELoss
  - forward(residuals): loss
  - backward(residuals): grad_residuals

## Risks + mitigations
- Risk: derivative bookkeeping mismatch with existing Lagaris derivatives.
  - Mitigation: keep interfaces explicit and add unit tests for simple inputs.
- Risk: inconsistent scaling for mean squared loss.
  - Mitigation: define mean explicitly as sum / N in loss and test with small N.

## Notes for Planner
- Keep objects lightweight (no ownership of network parameters).
- Prefer value inputs/outputs to keep testing deterministic.
- Tests should include x=0 boundary for trial, residual formula at x=0.5, and mean-squared loss over 1–2 points.
