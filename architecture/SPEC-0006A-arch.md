# SPEC-0006A Architecture Notes

## Summary
Introduce a lightweight Adam optimizer and a minimal training loop class to update PINN parameters for Problem 0, verifying residual reduction at x=0.5.

## Proposed structure (modules/classes)
- AdamOptimizer
  - Responsibility: maintain per-parameter moments (m, v), timestep t, and apply bias-corrected updates.
- Problem0Trainer
  - Responsibility: run N steps of training on a fixed collocation point using Problem 0 trial/residual/loss.

## Key interactions / data flow
- PINN provides N(x), dN/dx, and parameter gradients for output and first derivative.
- Problem0Trainer computes trial/residual/loss and backpropagates to parameter gradients using analytic formulas + Adam.
- AdamOptimizer updates PINN parameters in-place via get/set.

## Interfaces (sketch)
- AdamOptimizer
  - AdamOptimizer(lr, beta1, beta2, eps)
  - step(parameters, gradients)
- Problem0Trainer
  - Problem0Trainer(pinn, optimizer)
  - train(x, steps): returns initial_residual, final_residual

## Risks + mitigations
- Risk: gradient wiring mismatch across trial/residual/loss and parameter gradients.
  - Mitigation: add an integration test that checks residual decreases at x=0.5.
- Risk: parameter array size mismatches.
  - Mitigation: template on PINN type and use pinn.parameterCount().
- Risk: non-determinism from random init.
  - Mitigation: use fixed seed in tests.

## Notes for Planner
- Keep optimizer generic over parameter arrays but minimal (no scheduler).
- Trainer should be narrowly scoped to Problem 0 and single point.
