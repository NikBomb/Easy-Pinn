# Skill: ArchitectReview

## Intent
Review the spec and propose a maintainable design (SOLID-ish boundaries, clean interactions, testability), without over-engineering.

## Inputs
- Spec file in `specs/`
- Current repo structure

## Flow
1) Extract from spec:
   - Goals
   - Requirements (R*)
   - Acceptance Criteria (AC*)
2) Identify impacted areas:
   - modules, classes, packages, responsibilities
3) Propose boundaries:
   - what each module/class owns
   - what is explicitly out-of-scope
4) Define interfaces:
   - key functions/classes and their responsibility
   - data passed between layers
   - avoid premature abstraction
5) Testability check:
   - how will ACs be tested?
   - seams for mocking / deterministic behavior
6) Risks and mitigations:
   - coupling hot spots
   - likely future changes
   - performance/complexity traps
7) Produce architecture notes artifact.

## Output artifact
Write: `architecture/<SPEC-ID>-arch.md`

## Output format (in the file)
- Summary
- Proposed structure (modules/classes)
- Key interactions / data flow
- Interfaces (sketch)
- Risks + mitigations
- Notes for Planner

