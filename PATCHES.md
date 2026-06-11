# Android Portability Patches — OrcaSlicer `android/v2.3.1`

This branch = upstream tag `v2.3.1` + the patches below.

**Rule:** Patch only portability — never slicing behavior. Every behavioral change belongs upstream.
Upstream PRs are tracked in the last column; accepted PRs shrink this table permanently.

| # | Patch / file changed | What it does | Why upstream lacks it | Upstream PR |
|---|---|---|---|---|
| — | *(none yet — Phase 0 adds entries here)* | — | — | — |

## How to update to a new upstream tag

```bash
git fetch upstream --tags
git checkout -b android/vX.Y.Z vX.Y.Z
git cherry-pick <patch-commits>   # re-apply the series above onto the new tag
# Resolve conflicts, update this table, push
```

## References

- [PodSlicer app repo](https://github.com/alb-garj/podslicer) — consumes this branch as a submodule
- [OrcaSlicer upstream](https://github.com/OrcaSlicer/OrcaSlicer)
- [u1-slicer-for-android — NDK build recipes to crib from](https://github.com/taylormadearmy/u1-slicer-for-android)
