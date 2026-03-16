# Pong: Evolved

A feature-extended Pong game for Windows, written in C++ using the Win32 API with a custom software renderer. No external libraries or game engines — all rendering, input handling, and game logic are implemented from scratch.

---

## Game Modes

### Single Player
Face off against a computer-controlled opponent. Choose from three difficulty levels before the match begins.

| Difficulty | Effect |
|------------|--------|
| Normal | Slower paddle, lower AI accuracy |
| Difficult | Faster paddle, higher AI accuracy, slight ball-tracking offset |
| Impossible | Maximum paddle speed and AI reaction with tightest ball tracking |

### Multiplayer
Two-player local PvP. Choose a paddle speed (Slow / Medium / Fast) before the match begins.

### Survival Mode
Player 1 rallies against an AI opponent that gets progressively faster with each point scored. The game ends the moment the AI scores once. Your rally count is tracked as your score and saved to a highscore file.

---

## Controls

| Key | Action |
|-----|--------|
| `W` / `S` | Move Player 1 paddle up / down |
| `↑` / `↓` | Move Player 2 paddle up / down (Multiplayer), or alternate controls for Player 1 (Single Player) |
| `↑` / `↓` | Navigate menus |
| `Enter` | Confirm menu selection |
| `Space` | Play again (end screen) |
| `Escape` | Return to menu (in-game) / Quit (menus & end screen) |

---

## Features

- **Ghost ball** — when the ball's vertical speed exceeds a threshold, a ghost ball spawns and trails ahead of it. Hitting the ghost ball with your paddle triggers a power-up.
- **Power-ups** — catching the ghost ball temporarily grows your paddle and tints it blue (Player 1) or pink (Player 2). Up to three stacking size upgrades are available, each shown in a progressively deeper colour. The power-up expires after a set duration.
- **Rainbow ghost ball** — the ghost ball cycles through a 14-colour rainbow palette as it moves.
- **Angle control** — the ball's outgoing vertical velocity is influenced by where on the paddle it hits and how fast the paddle is moving at the moment of contact.
- **Accelerating ball** — the ball continuously accelerates in both axes up to a maximum horizontal speed, after which acceleration cuts off.
- **Score display** — live score shown at the top of the arena throughout the match. First to 10 points wins (standard modes).
- **Survival highscore** — the highest survival score is saved to `highscores.txt` and displayed on the end screen alongside your current score.
- **Custom software renderer** — all graphics are drawn by writing 32-bit colour values directly into a bitmap, then blitted to the window via `StretchDIBits`.
- **Custom bitmap font** — uppercase letters A–Z, digits 0–9, and select symbols (`!`, `-`, `:`, `>`) are rendered from hand-crafted 7-row character bitmaps.
- **Delta-time physics** — all movement uses `QueryPerformanceCounter`-based delta time for frame-rate-independent simulation.
- **Resizable window** — the renderer scales to the current window size at all times.

---

## Building

### Requirements

- Windows
- A C++ compiler targeting Win32 (e.g. MSVC via Visual Studio, or MinGW)

### Files

| File | Description |
|------|-------------|
| `win32_platform.cpp` | Entry point, Win32 window/message loop, rendering pipeline |
| `game.cpp` | Game logic, physics, ball and player simulation |
| `renderer.cpp` | Software renderer — rectangle drawing and bitmap font |
| `platform_common.cpp` | Input structs, button enums, colour palettes, menu macros |
| `utilities.cpp` | Typedefs, common macros, `clamp` helper |
| `manual_resource.h` | Defines `IDI_ICON` for the window icon resource |

### Compile (MSVC example)

```bat
cl win32_platform.cpp /Fe:pong.exe user32.lib gdi32.lib
```

### Compile (MinGW example)

```bash
g++ win32_platform.cpp -o pong.exe -lgdi32 -luser32 -mwindows
```

The default window size is **1280×720**.

---

## Highscores

Survival mode scores are persisted in a plain-text file `highscores.txt` in the same directory as the executable. The file is created automatically on first run and updated whenever a new highscore is set.

---

## Project Structure

```
.
├── win32_platform.cpp   # Entry point and Win32 platform layer
├── game.cpp             # Core game simulation
├── renderer.cpp         # Software renderer and font engine
├── platform_common.cpp  # Input, colours, shared macros
├── utilities.cpp        # Base typedefs and utilities
└── manual_resource.h    # Icon resource ID definition
```
