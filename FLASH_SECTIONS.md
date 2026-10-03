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

## Link fixture

Run `pio run -d qualification/flash-sections` from this repository. The fixture
pins the driver change at `95be8e9487442230d9bcb45d23ffba72f6070c85` and
PlatformIO Teensy 5.2.0. It calls overlays and touch calibration explicitly.
This is a link-only image with placeholder pins; it must not be uploaded.
Inspect its ELF with `arm-none-eabi-nm -C`: `overlayText`, `overlayFPS`,
`calibrateTouch` and OpenSans font data must remain present. In a consumer
that does not call these features, the unused font data should be absent.

On 2026-10-03, this fixture built successfully with the stock Teensy linker
script. Core at `ca67f7bfacae6a1bc582bf7fb6a85eb3271818e3` also built with
the pinned driver override in both release and dev profiles. Release Flash code
was 1209820 B (upstream: 1221892 B); the total Flash reduction was 43008 B.
The release code advisory remained exceeded by 1500 B. Free RAM1/RAM2/PSRAM
remained 157024/336192/7165600 B. Hardware qualification is still pending.
