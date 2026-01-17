# AGENTS.md


## Project context

High-level project goals, scope, and mental model are defined in `README.md`.

Agents must:
- Read `README.md` before planning or implementing features.
- Ensure proposed changes align with the project’s stated goals and non-goals.

## Purpose
This repository uses a single-agent workflow with a deterministic pipeline for feature work.
This file defines **how we work** (process + rules). Feature requirements live in `specs/`.

---

## Repository layout (folder conventions)

### Specs (source of truth for WHAT)
- `specs/` contains feature specs.
- File naming:
  - Parent: `SPEC-0001-short-name.md`
  - Optional sub-spec: `SPEC-0001A-short-slice.md`
- Each spec has:
  - `Status: Draft | In progress | Complete`
  - Requirements (R*)
  - Acceptance Criteria (AC*) that are testable

### Skills (HOW to execute work)
- `skills/` contains workflow playbooks, e.g.:
  - `run_feature_workflow.md`
  - `architect_review.md`
  - `plan_feature.md`
  - `implement_feature.md`
  - `test_feature.md`
  - `review_feature.md`
  - `explainer.md`

### Architecture notes / ADRs (design decisions)
- `architecture/` stores:
  - `ADR-0001-...md` (architecture decision records)
  - `<SPEC-ID>-arch.md` (per-spec architecture notes)

### Plans (implementation plan artifacts)
- `plans/` stores:
  - `<SPEC-ID>-plan.md` (step-by-step implementation plan)

### Results (if you run experiments)
- `results/` stores generated outputs.
- Do not commit large artifacts unless explicitly intended.

---

## Global rules (guardrails)

- Specs are the source of truth for scope. No scope creep without updating the spec (or explicitly noting a deviation).
- Prefer small, reversible changes.
- Do not add heavy dependencies without asking.
- Do not delete files unless explicitly asked.
- Never commit secrets (API keys, tokens).
- Cloud/API usage must be explicit and intentional. Default is local-first.

Definition of done:
- Acceptance criteria satisfied (or clearly marked partial with next steps)
- Tests/smoke checks run
- Short summary + how to run

---

## Modes

### Lightweight mode (for small changes)
Use lightweight mode when ALL are true:
- Change is small (roughly ≤ 50 lines OR one file OR pure refactor)
- No new modules/public APIs
- No architectural decisions required
- Spec impact is minimal

Lightweight pipeline:
1) PlanLite
2) ImplementFeature
3) TestLite
4) ReviewLite

Rules:
- Keep plan to 3 bullets max.
- Prefer minimal diffs.
- Run the smallest relevant check (unit test/smoke).

### Full pipeline mode (default for features)
Use full pipeline when ANY are true:
- New feature, new module, or public API
- Changes across multiple files/modules
- Risk of messy architecture / interactions
- Spec has multiple ACs or spans weekends

Full pipeline stages:
0) SelectFeature
1) ArchitectReview
2) PlanFeature
3) ImplementFeature
4) TestFeature
5) ReviewFeature
Loop:
- If TestFeature or ReviewFeature fails, return to ImplementFeature with a fix list and repeat.

---

## Stage selection logic (how the single agent chooses what to work on)

### A) If the user specifies a spec
- Work on that spec ID (and only that spec’s scope).

### B) If the user does not specify a spec
Pick work in this order:
1. Lowest-numbered spec with `Status: In progress` AND (if present) sub-specs:
   - Prefer sub-specs (e.g., `SPEC-0007A`) that are `In progress`
2. If none are `In progress`, pick the lowest-numbered `Draft` sub-spec whose parent is `In progress`
3. If no sub-specs match, pick the lowest-numbered `Draft` spec
4. If there are no specs, propose creating one using the spec template.

### C) If a selected spec is too large
- Propose splitting into sub-specs (vertical slices) and proceed with the smallest viable slice first.

---

## Pipeline stage rules (what each stage must produce)

### 0) SelectFeature
Output:
- Selected spec ID
- Whether to use Lightweight or Full pipeline (and why)

### 1) ArchitectReview (Full pipeline only)
Goal:
- Clean boundaries, SOLID-ish structure, testability, minimal future pain

Output artifact:
- Write `architecture/<SPEC-ID>-arch.md` containing:
  - Responsibilities/boundaries (modules/classes)
  - Key interfaces (high-level signatures ok)
  - Data flow / interactions
  - Risks + mitigations
  - Notes for Planner

Gate:
- Do not implement until ArchitectReview is complete.

### 2) PlanFeature
Output artifact:
- Write `plans/<SPEC-ID>-plan.md` containing:
  - Spec summary (1–2 lines)
  - AC checklist
  - Plan steps (≤6)
  - File-level changes (add/edit)
  - Testing plan
  - Risks/assumptions

Gate:
- Do not implement until plan is written.

### 2b) PlanLite (Lightweight mode)
Output (in chat is fine; optional file):
- 3 bullet plan max + which tests to run

### 3) ImplementFeature
Rules:
- Implement only what’s in scope for the spec/plan.
- If you discover missing requirements, update the spec or record an explicit deviation.
- Prefer incremental commits/patches.

Output:
- Brief summary of changes made.

### 4) TestFeature
Rules:
- Run the repo’s standard checks if available.
- If tests are not set up, run a minimal smoke check.

Output:
- Commands run
- Results summary
- If failures: top causes + fix list

### 4b) TestLite (Lightweight mode)
Rules:
- Run the smallest relevant test/smoke check.

Output:
- What you ran + pass/fail

### 5) ReviewFeature
Checklist:
- ACs satisfied or explicitly marked partial
- Code quality: naming, small functions, typing/docs where appropriate
- No scope creep
- No secrets
- No unnecessary deps

Output:
- Pass/Fail
- Findings
- Required fixes (if any)

Loop rule:
- If ReviewFeature fails, return to ImplementFeature with required fixes.

---

## Optional: Explainer stage (recommended on request or before merging)
Use skill `Explainer` to map in-progress code to spec:
- Technologies/patterns used
- Spec → code mapping
- Gaps/risks
- Next steps

---

## Spec template (quick)
Each spec in `specs/` should include:

- Status: Draft | In progress | Complete
- Summary
- Goals / Non-goals
- Requirements (R1..)
- Acceptance Criteria (AC1.., testable)
- Testing notes

