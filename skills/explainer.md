# Skill: Explainer

## Intent
Explain the current implementation relative to an in-progress spec:
- what technologies/patterns are used
- how changes map to requirements/ACs
- what’s missing or risky

## Inputs
- Spec file
- Current code state (prefer: diff + list of touched files)

## Flow
1) Extract: Goals, R*, AC*.
2) Inspect implementation:
   - start from entry points / changed files
   - identify key classes/modules and interactions
3) Identify technologies/patterns:
   - frameworks/libs, architectural patterns, conventions
   - why they are used here
4) Map spec → code:
   - for each R*/AC*: file/function/class + status
5) Flag gaps/mismatches and risks.
6) Recommend next steps:
   - smallest actions to reach next milestone/AC

## Output format
### Summary
(1–2 short paragraphs)

### Technologies and patterns used
- <tech/pattern>: where used; why it matters

### Spec → Code mapping
- R1 / AC1: Implemented | Partial | Not started — location — notes

### Gaps / risks
- ...

### Next steps
1. ...
2. ...

