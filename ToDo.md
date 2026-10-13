# Nav2 Capability Dependency Plan

## Goal

Make Nav2 capability dependencies backend-authoritative while still surfacing them to planning and retrieval as metadata.

## Phase 1: Fix Capability Declarations

- [x] Add real `depends_on` blocks to provider specs that require Nav2 to be running.
- [x] Update at least these providers:
	- `WaypointRunner.yaml`
	- `RobotPoseRunner.yaml`
- Defer generic capabilities2-server relation persistence work until later.
- Keep Experience DB/CoreGraph relation handling in scope.

## Phase 2: Fix Capabilities2 Server Semantics

- [x] Keep `provider.depends_on` as the authoritative execution dependency source.
- [x] Update runnable-spec generation so dependency metadata is surfaced to clients.
- [x] Fix the current mismatch where runnable specs copy interface relations but not provider dependency data.
- [x] Prefer synthesizing dependency relations from `provider.depends_on` when building runnable specs.

Notes:
- Runnable specs now synthesize `depends_on` relations from provider dependency metadata.
- This currently solves the hard-dependency bridge to Experience.
- Generic capabilities2-server `relations` persistence is out of scope for now and can be added later.
- Experience-side relation storage and retrieval in CoreGraph remains in scope.

## Phase 3: Expose Dependency Metadata to Planning

- [x] Ensure `/capabilities/get_runnable_specs` returns dependency information in a form Experience can consume.
- [x] Update Experience runnable-spec parsing if schema changes are introduced.
- [x] Make planner prompts describe hard dependencies as implicit prerequisites.
- [x] Add prompt guidance that agents should not emit dependency runners as explicit plan nodes unless the task is explicitly about lifecycle management.

Notes:
- Experience planner prompt now renders `depends_on` edges as `implicit_prerequisites`.
- Semantic validator prompt now uses the same backend-managed dependency wording so planning and validation stay aligned.
- No runnable-spec schema expansion was needed because synthesized dependency relations were sufficient for the current Experience parser.

## Phase 4: Align CoreGraph and Retrieval

- [x] Keep dependency edges visible to CoreGraph for retrieval and explanation.
- [x] Distinguish dependency edges from general semantic relations where possible.
- [x] Prevent retrieval context from implying that every dependency edge should become a plan node.
- [x] Verify that dependency edges improve ranking and critique without causing over-planning.

Notes:
- CoreGraph now tags declared `depends_on` edges as `declared_dependency`.
- Retrieval now routes declared dependency edges into `semantic_context.dependencies`.
- Experience DB/CoreGraph relation handling is in scope when it improves retrieval or explanation.
- What stays deferred is generic capabilities2-server relation persistence, not Experience-side graph relations.

## Phase 5: Validate Plan Execution Semantics

- [x] Do not use `parallel_any` as the default mechanism for launch prerequisites.
- [x] Treat `Nav2LaunchRunner` as infrastructure unless a task explicitly asks to manage Nav2 lifecycle.
- [x] Verify that requesting `WaypointRunner` or `RobotPoseRunner` auto-starts `Nav2LaunchRunner` through backend dependency handling.
- [x] Verify shutdown behavior when dependent capabilities are freed.

Notes:
- Added an in-process capabilities2 server test using `DummyRunner` to verify dependency auto-start and teardown behavior.
- This validates the backend dependency mechanism without requiring a live Nav2 stack.

## Phase 6: Tests and Documentation

- [x] Add or update tests for backend dependency startup and shutdown behavior.
- [x] Add or update tests for runnable-spec dependency metadata.
- [x] Add or update tests for Experience prompt generation when dependencies are present.
- [x] Document the contract clearly:
	- `depends_on` = executable dependency
	- `relations` = semantic context
	- plan XML = task intent, not infrastructure duplication

Notes:
- Decision: `parallel_any` is not the default mechanism for launch prerequisites.
- Decision: `Nav2LaunchRunner` is infrastructure unless a task explicitly asks to manage Nav2 lifecycle.
- Decision: keep runnable specs exposing dependency metadata through synthesized dependency relations for now.
- The dependency contract is now settled as:
	- `depends_on` = executable dependency
	- plan XML = task intent, not infrastructure duplication
- Tests now cover runnable-spec dependency synthesis, backend dependency auto-start/teardown, planner prompt wording, and semantic validator prompt wording.

## Recommended Order

1. Document the dependency contract and current limitations.
2. Revisit generic capabilities2-server relation persistence later if it becomes needed.

## Open Decisions

- None for the current `depends_on` dependency path.

Updates:
- Decision: add a dedicated `DependencyValidation` plugin that warns when a plan explicitly includes a runner that is already an implicit backend dependency of another step.
