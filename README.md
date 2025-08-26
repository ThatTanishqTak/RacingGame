# RacingGame 🏎️

A management + race simulation project written in **C++17**, using a custom **Engine** (OpenGL + ImGui) and a **Game** layer for management, races, and UI.

This README documents the current codebase, how to build and run it, and how to extend common systems (tracks, palettes, races).

---

## Table of Contents
- [Features](#features)
- [Repository Structure](#repository-structure)
- [Build \& Run](#build--run)
- [Gameplay & Systems](#gameplay--systems)
  - [Races & Season](#races--season)
  - [Finance & Development](#finance--development)
  - [Teams, Drivers, Cars, Contracts](#teams-drivers-cars-contracts)
  - [EventBus \& Race Events](#eventbus--race-events)
  - [Palettes (Team Colours)](#palettes-team-colours)
  - [Tracks (ASCII Layouts)](#tracks-ascii-layouts)
- [Engine Overview](#engine-overview)
  - [Rendering](#rendering)
  - [Camera \& Controller](#camera--controller)
  - [UI (Dear ImGui)](#ui-dear-imgui)
  - [State Streaming](#state-streaming)
- [Management Mode](#management-mode)
- [Extending the Project](#extending-the-project)
- [Known Issues \& TODO](#known-issues--todo)
- [License](#license)
- [Credits](#credits)

---

## Features

### Engine
- **Windowing** with **GLFW**; OpenGL 4.3 core via **GLAD**.
- **Renderer** with Mesh/Shader abstraction and depth/cull states.
- **State interpolation** (`StateBuffer`) for smooth car movement.
- **Camera** class + **CameraController** (Fit All / Follow Selected / Free).
- **Dear ImGui** (docking enabled) for HUD and tools.
- Minimal **track rendering** (3D quad) and **car meshes** (cube) in non-management builds.

### Game
- **Races**: Practice, Qualifying, Race – generates results and *publishes events* (pit-in/out, DNFs).
- **Season**: Aggregates results, points table (25–18–15…1) and leaderboard.
- **Finance**: Total budget, per-department allocation/expenses, random events.
- **Development**: Upgrade queue (Chassis/Aero/Power Unit) with random issues.
- **Teams / Drivers / Cars / Contracts**: simple data models, sponsorship hooks.
- **UI Panels**: Race Dashboard, Timing Tower, Flag Panel, ASCII Track View with zoom.
- **Palette Manager**: Team colours with colour-blind alternative palette.

---

## Repository Structure

```
RacingGame/
├─ CMakeLists.txt                # Root; builds Engine (static) and Game (exe)
├─ CMakePresets.json             # Optional: use CMake presets if desired
├─ Engine/
│  ├─ CMakeLists.txt
│  ├─ src/
│  │  ├─ Core/
│  │  │  ├─ Application.{h,cpp}
│  │  │  ├─ EntryPoint.h
│  │  │  └─ PaletteManager.{h,cpp}
│  │  ├─ Renderer/
│  │  │  ├─ Camera.{h,cpp}
│  │  │  ├─ CameraController.{h,cpp}
│  │  │  ├─ Mesh.{h,cpp}
│  │  │  ├─ Renderer.{h,cpp}
│  │  │  ├─ Shader.{h,cpp}
│  │  │  ├─ StateStream.{h,cpp}
│  │  │  ├─ IndexBuffer.{h,cpp}
│  │  │  ├─ VertexArray.{h,cpp}
│  │  │  └─ VertexBuffer.h
│  │  ├─ UI/
│  │  │  └─ ImGuiLayer.{h,cpp}
│  │  └─ Window/
│  │     └─ Window.{h,cpp}
│  └─ vendor/
│     ├─ GLFW/                   # prebuilt glfw3.lib (Windows)
│     ├─ GLAD/
│     ├─ glm/
│     └─ imgui/
├─ Game/
│  ├─ CMakeLists.txt
│  ├─ Assets/
│  │  ├─ Palettes/               # primary.json, colourblind.json
│  │  ├─ Shaders/                # Shader.vert, Shader.frag
│  │  └─ Tracks/                 # ASCII track files (e.g., SampleTrack.txt)
│  └─ src/
│     ├─ Core/
│     │  ├─ Car.{h,cpp}
│     │  ├─ Circuit.{h,cpp}
│     │  ├─ Contract.{h,cpp}
│     │  ├─ DevelopmentManager.{h,cpp}
│     │  ├─ Driver.{h,cpp}
│     │  ├─ EventBus.{h,cpp}
│     │  ├─ FinanceManager.{h,cpp}
│     │  ├─ Race.{h,cpp}
│     │  ├─ RaceState.h
│     │  ├─ Season.{h,cpp}
│     │  └─ Team.{h,cpp}
│     ├─ Panels/
│     │  ├─ FlagPanel.{h,cpp}
│     │  ├─ RaceDashboard.{h,cpp}
│     │  └─ TimingTowerPanel.{h,cpp}
│     ├─ GameLayer.{h,cpp}
│     └─ Game.cpp
└─ LICENSE (placeholder)
```

> **Output:** Binaries are written to `bin/{OS}-{Build}-{Arch}/RacingGame/` (e.g., `bin/Windows-Debug-x64/RacingGame/`). Game assets are copied to the same runtime folder post-build.

---

## Build & Run

### Prerequisites
- **CMake 3.28+**
- **C++17 compiler**
- **Windows + Visual Studio 2022** (current vendor setup uses a prebuilt `glfw3.lib` for MSVC).

> Other platforms can be supported by swapping the GLFW import in `Engine/CMakeLists.txt` to a source build and linking the appropriate OpenGL libs.

### Configure & Build (generic)
```bash
# From the repository root:
cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug
cmake --build build --target Game --config Debug
```

### Configure & Build (MSVC + Ninja example)
```bash
cmake -B build -S . -G "Ninja" -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

### Run
```text
bin/Windows-Debug-x64/RacingGame/Game.exe
```
Assets are copied automatically via a post-build step in `Game/CMakeLists.txt`.

---

## Gameplay & Systems

### Races & Season
- `Race` simulates **Practice**, **Qualifying**, and **Race** sessions with random lap times and emits random events:
  - `PitIn`, `PitOut`, `DNF` (10–15% chance per driver for event generation in race).
- `Season` aggregates races and computes the leaderboard using a fixed points table `(25, 18, 15, 12, 10, 8, 6, 4, 2, 1)`.

### Finance & Development
- `FinanceManager` manages the total budget, per-department allocations, and expenses. It can apply a random unexpected cost.
- `DevelopmentManager` enqueues upgrade tasks for **Chassis**, **Aerodynamics**, **Power Unit**, processes them, and triggers random events (e.g., part failure, wind-tunnel budget cut).

### Teams, Drivers, Cars, Contracts
- `Team` holds drivers and optional primary sponsor.
- `Driver` has name, age, optional personal sponsor, and roles.
- `Car` stores model, optional sponsor, and a driver assignment.
- `Contract` ties a driver to a team and models payment as either a single value or a pair `(fixed, bonus)`.

### EventBus & Race Events
- `EventBus` is a header-only (plus a global instance) pub/sub system:
  - **Subscribe**: `g_EventBus.Subscribe<Event>(handler);`
  - **Publish**:   `g_EventBus.Publish(Event{...});`
- Current events: `PitIn`, `PitOut`, `DNF`, `Overtake` (defined; `Overtake` not yet published).
- `RaceDashboard` subscribes and shows **HUD toasts** when events occur.

### Palettes (Team Colours)
- `PaletteManager` loads two palettes:
  - **Primary**: `Assets/Palettes/primary.json`
  - **Colour-blind**: `Assets/Palettes/colourblind.json`
- Expected JSON content: a list of RGB triplets `[r, g, b]` (0–1 float values). Example:
  ```json
  {
    "teams": [[0.86,0.08,0.24], [0.12,0.56,1.00], [0.98,0.82,0.12]]
  }
  ```
  > The loader uses a regex and will extract `[r,g,b]` triplets anywhere in the file; strict keys are not required.

### Tracks (ASCII Layouts)
- Tracks are `txt` files (e.g. `Assets/Tracks/SampleTrack.txt`) composed of these characters:
  - Road: `- | \ /`
  - Start/finish: `*` (white)
  - Pitlane: `&` (blue)
  - Curbs/Markers: `[` and `]` (green)
  - Spaces are allowed (treated as empty)
- Validation happens in `Circuit::LoadLayout`. The **Track View** panel in `RaceDashboard` renders the ASCII grid into an ImGui canvas and supports **mouse wheel zoom**.

---

## Engine Overview

### Rendering
- OpenGL states: **depth test** on, **back-face culling** (GL_BACK).
- Shaders are loaded from `Assets/Shaders/Shader.vert` and `Shader.frag` via `ShaderLibrary`.
- `Renderer` draws:
  - A simple flat **track quad** (disabled in Management Mode).
  - A **cube mesh** for each car at the interpolated position from `StateBuffer`.

### Camera & Controller
- `Camera` (perspective) defaults to position `(0,0,3)`, yaw `-90°`, pitch `0°`.
- `CameraController` supports:
  - **FitAll**: frame the whole track bounds.
  - **FollowSelected**: smoothly follow a selected car.
  - **Free**: smooth pan/zoom to explicit targets.
- Track clamping is supported to keep the camera over the circuit.

> Note: The controller exists but is not yet wired into the main loop; hook it up in `Application::Run()`/`Game::Run()` to animate camera behavior.

### UI (Dear ImGui)
- ImGui initialized in `ImGuiLayer` with docking enabled.
- Panels:
  - **Race Info** (date/time/weather)
  - **Track View** (ASCII renderer + zoom)
  - **Drivers** (team-coloured names)
  - **Positions** (scoreboard)
  - **Timing Tower**
  - **Flags**
  - **HUD Toasts**
  - **Settings** (toggle colour-blind palette)

### State Streaming
- `StateBuffer` collects `Snapshot{ Time, std::vector<CarState{ID, Position} > }` and interpolates car positions at render time.
- Example seed (see `Game::Run()`):
  ```cpp
  double t0 = glfwGetTime();
  g_StateBuffer.SubmitSnapshot({ t0,     { {0,{0,0,0}}, {1,{2,0,0}} } });
  g_StateBuffer.SubmitSnapshot({ t0+5.0, { {0,{0,0,-10}}, {1,{2,0,-10}} } });
  ```
  The renderer queries `g_StateBuffer.Interpolate(glfwGetTime())` each frame.

---

## Management Mode

There are two places where `MANAGEMENT_MODE` can be defined:

- **Root `CMakeLists.txt`** exposes an option:
  ```cmake
  option(ENABLE_MANAGEMENT_MODE "Disable 3D track rendering" OFF)
  if(ENABLE_MANAGEMENT_MODE)
    add_compile_definitions(MANAGEMENT_MODE)
  endif()
  ```
- **Game/CMakeLists.txt** currently adds `add_compile_definitions(MANAGEMENT_MODE)` **unconditionally**.

> **If you want to toggle Management Mode via the root option**, remove or guard the unconditional line in `Game/CMakeLists.txt`. When `MANAGEMENT_MODE` is defined, the 3D track draw call is skipped in `Renderer`.

---

## Extending the Project

- **Top-Down Live Viewer**: draw cars as team-coloured circles on the ImGui Track View; sample positions come from `StateBuffer`. Consider mapping world-to-grid coordinates or parsing a higher-fidelity layout.
- **Hook CameraController**: update camera based on selected mode and track bounds.
- **Mesh::LoadFromFile**: implement simple OBJ loader if you want 3D cars.
- **Event Coverage**: publish `Overtake` events from race logic; add UI hooks.
- **Persistence**: save/load seasons, finances, and team rosters (YAML/JSON).
- **Cross-Platform Build**: swap prebuilt GLFW lib for source build; add Linux/macOS profiles.

---

## Known Issues & TODO
- `Game/CMakeLists.txt` forces `MANAGEMENT_MODE` on; prefer the root toggle.
- `TimingTowerPanel` uses a malformed format string (`"%it_Driver %s ..."`) – replace with something like `ImGui::Text("%d %s %.3fs", pos, code.c_str(), interval);`.
- `Mesh::LoadFromFile` is a stub.
- `CameraController` is present but not integrated into the main loop.
- Track rendering (3D) is a flat quad placeholder.
- Input system currently defers to ImGui capture flags; no dedicated bindings yet.

---

## License
Placeholder. Choose a license (e.g., MIT) before open-sourcing.

---

## Credits
- Inspired by F1 management titles and **Football Manager**’s match viewer.
- Architecture style influenced by **TheCherno/Hazel**.
- Libraries: **GLFW**, **GLAD**, **glm**, **Dear ImGui**.
