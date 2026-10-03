# Function-granular Teensy Flash sections

This branch is based on upstream tag `1.7.0`. It preserves the driver API and
the Flash/noinline annotations, while assigning each annotated definition in
`ILI9341Driver.cpp` its own `.flashmem.ili9341.*` input section. Public headers
and their inline definitions retain the upstream annotations. Interrupt handlers
without FLASHMEM retain their original placement.

Consumers must use a linker script collecting `.flashmem*`, and link with
`--gc-sections`. A monolithic `.flashmem` input section retains every annotated
method and all referenced fonts as soon as any one method is needed. Independent
sections allow unused text overlays and touch calibration to be removed, while
applications using those features still retain them through normal references.

## Qualification

For each consumer, compare the linked ELF against upstream 1.7.0 with identical
compiler, flags and application sources. Inspect section placement and sizes;
verify that used APIs are still linked, RAM/ITCM budgets pass, and unused font
data disappears only when no used feature references it. Build a second consumer
using `overlayText` and touch calibration to prove those APIs still link.

Physical qualification must cover display startup, framebuffer updates,
asynchronous DMA/VSync and the actually used diagnostic overlays. A successful
link and smaller ELF do not establish equivalent display behavior or faster
rendering. Pin the exact qualified commit in downstream dependency manifests.
