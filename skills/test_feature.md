# Skill: TestFeature

## Intent
Validate the implementation against acceptance criteria with the repository’s standard checks.

## Inputs
- Spec file
- Current working tree

## Flow
1) Identify relevant checks from `AGENTS.md` (or repo docs):
   - formatting/lint
   - unit tests
   - smoke run / example command
2) Run checks (or describe the exact commands if execution isn’t possible).
3) Map results to ACs:
   - which ACs are verified
4) If failures:
   - summarize error
   - top likely root cause(s)
   - minimal fix list (prioritized)

## Output
- Commands run (or to run)
- Results summary
- AC verification status
- Failures + fix list (if any)

