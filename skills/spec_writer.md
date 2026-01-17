# Skill: SpecWriter

## Intent
Convert a user problem statement into one or more **clear, scoped, testable specs**
so downstream skills do not invent requirements.

## When to use
- The user provides a problem statement and no suitable spec exists.

## When NOT to use
- A spec already exists and only needs small edits.
- Small changes (≤ ~50 LOC / single-file tweaks): use PlanLite.

---

## Responsibilities (authoritative)

1) **Decompose the request**
   - Decide whether it implies:
     A) one spec
     B) one parent spec + sub-specs (vertical slices)
     C) multiple independent specs
   - Prefer vertical slices that are runnable/testable.

2) **Identify blocking unknowns**
   - Ask questions ONLY if answers materially affect:
     - acceptance criteria
     - interfaces (CLI/API)
     - constraints (deps, runtime, environments)
     - priority/order
   - Ask at most 3–7 questions total.
   - If unanswered, proceed with explicit assumptions.

3) **Write specs**
   - Use the repo’s spec template.
   - Each spec MUST include:
     - Goals
     - Non-goals
     - Requirements (R*)
     - Acceptance Criteria (AC*) that are objectively testable
     - Constraints/assumptions
     - Testing notes
   - Assign IDs and filenames.
   - Mark `Status: Draft`.

4) **Order the work**
   - Declare dependencies and recommended execution order.

---

## Output

### A) Breakdown summary
- List of specs to be created (titles + IDs)
- Dependencies and recommended order

### B) Questions (if any)
- Numbered list of blocking questions
- If none, state assumptions explicitly

### C) Specs (ready to save)
- Full markdown for each spec

---

## Guardrails

- Do NOT include architecture or implementation steps.
- Do NOT plan files or code.
- If the request is too large, split; do not write a mega-spec.
