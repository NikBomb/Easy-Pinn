# SPEC-0006A Plan

## Spec summary
Add an Adam optimizer and a dedicated Problem 0 training class that reduces residual at x=0.5 using analytic gradients.

## Acceptance Criteria checklist
- [ ] AC1: Given a fixed seed and fixed collocation point x=0.5, after N Adam steps the residual at that point is lower than the initial residual.
- [ ] AC2: Adam updates maintain per-parameter state (m, v, t) and apply bias correction.

## Plan steps (<= 6)
1) Identify parameter storage and gradient shapes in `PINN` to align optimizer inputs.
2) Implement `AdamOptimizer` with per-parameter m/v/t state and bias correction.
3) Implement `Problem0Trainer` that computes loss gradients and updates parameters with Adam.
4) Add a unit test for single-step Adam update on a scalar parameter.
5) Add an integration test showing residual decreases at x=0.5 after N steps.
6) Document any assumptions about gradient computation or parameter ordering.

## File-level changes
Add:
- `lib/include/easypinn/public/adam.h`
- `lib/include/easypinn/public/problem0_trainer.h`

Edit:
- `test/test_easypinn.cpp`

## Testing plan
- Unit: Adam single-step update test.
- Integration: Problem 0 residual reduction after small training run.
- Smoke/manual: `ctest --test-dir build`.

## Risks / assumptions
- Assumptions:
  - Use default Adam hyperparameters (lr=1e-3, betas=0.9/0.999, eps=1e-8).
  - Single collocation point at x=0.5 for training test.
- Risks + mitigations:
  - Risk: incorrect gradient flow from loss to parameters.
    - Mitigation: cross-check residual decreases and keep formula explicit.
  - Risk: mismatch in parameter array ordering.
    - Mitigation: rely on PINN parameterCount and get/set accessors only.
  - Risk: deterministic tests flake due to numerical issues.
    - Mitigation: use fixed seed, small step count, and relaxed epsilon.
