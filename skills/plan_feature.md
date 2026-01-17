# Skill: PlanFeature

## Intent
Convert a selected spec into a concrete, scoped implementation plan that an implementer can follow without inventing requirements.

## Inputs
- Selected spec file (e.g., `specs/SPEC-0007-...md`)
- Optional: architecture notes for this spec (e.g., `architecture/<SPEC-ID>-arch.md`)
- Repo conventions from `AGENTS.md` (commands, guardrails)

## Preconditions
- A single spec is selected.
- The spec has testable Acceptance Criteria (AC*).
- If the spec is too large/unclear, stop and recommend:
  - running SpecWriter again to split/refine, or
  - editing the spec to add missing ACs.

---

## Flow

1) **Extract spec checklist**
- List all Requirements (R*) and Acceptance Criteria (AC*) from the spec.
- Identify dependencies or prerequisites noted in the spec.

2) **Clarify only if blocked**
- If any AC cannot be made testable from the spec as written, list the exact missing info.
- If not blocked, proceed with explicit assumptions (keep them short).

3) **Design the plan (≤ 6 steps)**
- Produce a step-by-step plan that maps directly to ACs.
- Each step should be concrete and verifiable (avoid vague steps like “refactor things”).

4) **File-level impact**
- List files to:
  - Add
  - Edit
  - (Avoid renames/deletes unless required)

5) **Testing plan**
- Specify:
  - Unit tests to add/modify
  - Integration or smoke command(s) to run
- Each AC should be verifiable by a test or a specific run command.

6) **Risks & mitigations**
- Identify top 3 risks:
  - unclear scope
  - coupling points
  - performance/stability concerns
- For each, propose a minimal mitigation.

---

## Output artifact
Write: `plans/<SPEC-ID>-plan.md`

## Output format (in the plan file)

# <SPEC-ID> Plan

## Spec summary
(1–2 lines)

## Acceptance Criteria checklist
- [ ] AC1: ...
- [ ] AC2: ...
...

## Plan steps (≤ 6)
1. ...
2. ...
...

## File-level changes
Add:
- ...

Edit:
- ...

## Testing plan
- Unit:
- Integration:
- Smoke/manual:

## Risks / assumptions
- Assumptions:
  - ...
- Risks + mitigations:
  - ...
