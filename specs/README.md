# Specs

This folder contains feature specifications. Specs are the source of truth for **what** we are building.

## Status values
Each spec must include:
- `Status: Draft | In progress | Complete`

## Naming
- Parent specs: `SPEC-0001-short-name.md`
- Optional sub-specs (vertical slices): `SPEC-0001A-short-slice.md`

## Guidelines
- Prefer **vertical slices** for sub-specs: something runnable/testable end-to-end.
- Each spec should have:
  - Goals / Non-goals
  - Requirements (R*)
  - Acceptance Criteria (AC*) that are testable
  - Testing notes

## Workflow (summary)
- Specs drive planning (`plans/`) and architecture notes (`architecture/`).
- Implementations must map back to ACs.

