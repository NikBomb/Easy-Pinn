# Skill: ImplementFeature

## Intent
Implement the planned feature to satisfy the spec’s acceptance criteria, with minimal scope creep.

## Inputs
- Spec file: `specs/<SPEC-ID>.md` 
- Plan file: `plans/<SPEC-ID>-plan.md`
- Any relevant architecture notes: `architecture/<SPEC-ID>-arch.md`

## Flow
1) Confirm scope:
   - list which ACs are being implemented now
2) Implement incrementally:
   - prefer small, focused edits
   - avoid unrelated refactors
   - avoid `#pragma once`; use include guards
3) Keep boundaries consistent with architecture notes.
4) Add/update tests aligned with ACs.
5) Update docs or comments if they clarify usage.
6) Record deviations:
   - if something must diverge from spec, note it clearly and propose a spec update

## Output
- What changed (bullets)
- Where to look (files touched)
- Which ACs are now satisfied / partial
