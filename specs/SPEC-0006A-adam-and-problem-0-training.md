# SPEC-0006A: Adam optimizer and Problem 0 training check

## Status
Complete

## Summary
Implement an Adam optimizer for the PINN parameters and demonstrate residual reduction on Problem 0.

## Goals
- G1: Provide an Adam optimizer implementation usable by the PINN training loop.
- G2: Show that residual decreases on Problem 0 after a small number of steps.

## Non-goals
- NG1: Hyperparameter tuning or advanced learning-rate schedules.
- NG2: Benchmarking or performance optimization.

## Requirements
- R1: Adam implements bias-corrected first/second moment updates.
- R2: Configurable lr, betas, epsilon; deterministic behavior with fixed seed and fixed data.
- R3: Training loop uses the Problem 0 loss to update parameters.
- R4: Training loop is a dedicated class.

## Acceptance Criteria
- AC1: Given a fixed seed and fixed collocation point x=0.5, after N Adam steps the residual at that point is lower than the initial residual.
- AC2: Adam updates maintain per-parameter state (m, v, t) and apply bias correction.

## Design notes (optional)
- Constraints: no external optimizer dependencies; integrate with existing parameter storage.

## Testing
- Unit: Adam update on a scalar parameter matches expected value for one step.
- Integration: Problem 0 residual decreases after a small training run.
- Manual / smoke: run a short training loop and log residual.
