# Skill: ReviewFeature

## Intent
Quality gate: verify spec compliance + maintainability + repo conventions.

## Inputs
- Spec
- Implementation changes (prefer diff / list of changed files)
- Test results summary

## Checklist
Spec & scope:
- Each AC is satisfied, or clearly marked partial with next steps
- No scope creep beyond spec

Code quality:
- Clear naming, small functions
- Reasonable structure, low coupling
- Types/docs where appropriate
- No dead code / debug prints

Safety & hygiene:
- No secrets in repo
- No destructive/unrequested deletes
- Dependencies justified (no heavy deps without approval)

Testing:
- Tests added/updated appropriately
- Smoke check documented

## Output
- Pass/Fail
- Findings (bullets)
- Required fixes (if any)
- Suggested nice-to-haves (optional)

