# Skill: RunFeatureWorkflow

## Intent
Execute the repository’s end-to-end feature workflow using the pipeline defined in `AGENTS.md`.

## Inputs
- Optional: explicit spec ID from user
- Otherwise: select spec using `AGENTS.md` stage selection logic

## Flow
0) SelectFeature
- Choose a spec deterministically (per `AGENTS.md`).
- Decide mode: Lightweight vs Full pipeline.
- State selection + mode + reason.

If Lightweight mode:
1) PlanLite
2) ImplementFeature
3) TestLite
4) ReviewLite
Loop: If TestLite/ReviewLite fails → ImplementFeature fixes → repeat.

If Full pipeline:
1) ArchitectReview (write `architecture/<SPEC-ID>-arch.md`)
2) PlanFeature (write `plans/<SPEC-ID>-plan.md`)
3) ImplementFeature
4) TestFeature
5) ReviewFeature
Loop: If TestFeature/ReviewFeature fails → ImplementFeature fixes → repeat.

## Output
- Final summary
- AC checklist (pass/partial/fail)
- Commands to run/tests
- Notes for next steps (if any)

