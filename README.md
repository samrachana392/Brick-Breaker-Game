# 🧱 Brick Breaker — OpenGL / GLUT (C++)

A classic Brick Breaker arcade game built from scratch in **C++ with OpenGL and GLUT**, developed as a Computer Graphics course project. Features hand-implemented rasterization algorithms (DDA Line, Midpoint Circle, Scanline Fill), a toggleable 3D rendering mode with real-time lighting, and a complete game loop with multiple levels and difficulty settings.

---

## 📸 Screenshots

> `!![alt text](image.png)(screenshots/menu.png)`
> `!![alt text](image-1.png)(screenshots/gameplay_2d.png)`
> `!![alt text](image-2.png)(screenshots/gameplay_3d.png)`

---

## ✨ Features

- **Hand-implemented rasterization algorithms** — no built-in line or circle drawing used in 2D mode
  - DDA (Digital Differential Analyzer) Line Algorithm
  - Midpoint Circle Algorithm with 8-fold symmetry
  - Scanline Fill Algorithm for solid brick interiors
- **Toggleable 3D mode** — press `T` at any time to switch between flat 2D and perspective 3D rendering
- **3D mode features** — perspective projection, Gouraud shading, directional lighting, material properties (ambient, diffuse, specular, shininess), continuously rotating bricks
- **3 difficulty levels** — Easy, Medium, Hard (controls ball speed)
- **3 levels** — bricks reset and ball speeds up on each level progression
- **5 lives** with a 1-second LIFE LOST screen on each death
- **Countdown timer** (3-2-1) before gameplay begins
- **Complete game states** — Menu, Countdown, Playing, Paused, Life Lost, Game Over, Win Screen
- **HUD** — live score, lives, level, difficulty, 3D toggle status, and controls displayed at all times during play
- **Smooth paddle movement** via key-state polling (no jitter on held keys)
- **Double buffering** — flicker-free animation via `glutSwapBuffers()`

---

## 🗂️ Project Structure

```
brick-breaker/
│
├── main.cpp          # Entry point, glutMainLoop, game loop timer, collision detection
├── globals.h         # Shared constants, GameState enum, extern variable declarations
├── objects.cpp       # Paddle, Ball, BrickGrid — logic and drawing
├── objects.h         # Class declarations for all game objects
├── algorithms.cpp    # DDA line, Midpoint Circle, Scanline Fill implementations
├── algorithms.h      # Function declarations for rasterization algorithms
├── renderer.cpp      # display(), drawHUD(), lighting setup, projection, text rendering
├── renderer.h        # Renderer function declarations
├── input.cpp         # Keyboard and special key callbacks
└── input.h           # Input function declarations
```

---

## 🎮 Controls

| Key | Action |
|-----|--------|
| `S` | Start game / Play again (from Win screen) |
| `R` | Restart / Return to main menu |
| `P` | Pause / Resume |
| `T` | Toggle 3D mode (works at any time) |
| `1` | Set difficulty to Easy |
| `2` | Set difficulty to Medium |
| `3` | Set difficulty to Hard |
| `← →` Arrow keys | Move paddle left / right |
| `ESC` | Exit game |

---

## ⚙️ Requirements

| Dependency | Notes |
|---|---|
| C++ compiler | `g++` (MinGW on Windows, GCC on Linux) |
| OpenGL | `opengl32` — included with Windows / standard on Linux |
| GLU | `glu32` — OpenGL Utility Library |
| FreeGLUT | `freeglut` — window management and input |

### Installing FreeGLUT

**Windows (MinGW):**
Download FreeGLUT from [https://freeglut.sourceforge.net](https://freeglut.sourceforge.net) and place:
- `freeglut.dll` → next to your executable
- `freeglut.h` → in your MinGW `include/GL/` folder
- `libfreeglut.a` → in your MinGW `lib/` folder

**Linux (Ubuntu/Debian):**
```bash
sudo apt-get install freeglut3-dev
```

**Linux (Fedora/RHEL):**
```bash
sudo dnf install freeglut-devel
```

---

## 🔨 Build & Run

### Windows (MinGW / g++)

```bash
g++ main.cpp algorithms.cpp objects.cpp renderer.cpp input.cpp -o app -lfreeglut -lopengl32 -lglu32
./app.exe
```

### Linux

```bash
g++ main.cpp algorithms.cpp objects.cpp renderer.cpp input.cpp -o app -lGL -lGLU -lglut
./app
```

### macOS

```bash
g++ main.cpp algorithms.cpp objects.cpp renderer.cpp input.cpp -o app -framework OpenGL -framework GLUT
./app
```

> **Note:** On macOS you may need to add `-Wno-deprecated` as GLUT is deprecated in favour of Metal but still functional.

---

## 🧠 Algorithms Implemented

### DDA Line Algorithm (`algorithms.cpp`)
Draws a line between two points by computing incremental X and Y steps. The number of steps is `max(|dx|, |dy|) × 500` — the 500 factor scales from normalized [-1, 1] coordinate space to screen-pixel density. Used for all rectangle outlines and scanline fill.

### Midpoint Circle Algorithm (`algorithms.cpp`)
Draws a circle using integer arithmetic and a decision parameter `p = 1 - r`. Exploits 8-fold symmetry — computes one octant and reflects it into all 8 symmetric positions per step. Used for the ball in 2D mode.

### Scanline Fill (`algorithms.cpp`)
Fills a rectangle by drawing horizontal DDA lines from `y1` to `y2` in steps of `0.002` units (sub-pixel precision to avoid gaps). Used to fill brick interiors in 2D mode.

---

## 🎨 Rendering Architecture

### 2D Mode
- Orthographic projection via `gluOrtho2D(-1, 1, -1, 1)`
- All shapes drawn using the custom DDA and Midpoint Circle algorithms
- Lighting disabled — colors set directly via `glColor3f`

### 3D Mode
- Perspective projection via `gluPerspective(56.74°, aspect, 0.01, 100)`
- Camera positioned at `(0, 0, 1.85)` looking at origin via `gluLookAt`
- Directional light at `(0, 1, 1)` with ambient, diffuse, and specular components
- Bricks rendered as `glutSolidCube` with per-brick material properties and continuous Y-axis rotation
- Ball rendered as `glutSolidSphere`
- Paddle rendered as a scaled `glutSolidCube`
- Gouraud shading via `glShadeModel(GL_SMOOTH)`
- Depth testing enabled via `GL_DEPTH_TEST`

### HUD
Drawn in orthographic 2D regardless of 3D mode — achieved by pushing/popping the projection and modelview matrix stacks around the HUD draw calls, ensuring the score bar is never distorted by perspective.

---

## 🔄 Game State Machine

```
MENU ──(S)──► COUNTDOWN ──(3s)──► PLAYING
                                      │
                   ┌──────(P)─────────┤
                   ▼                  │
                PAUSED ──(P)──────────┘
                                      │
                              (ball falls)
                                      │
                              ┌───────▼───────┐
                        lives > 0         lives = 0
                              │               │
                         LIFE_LOST       GAME_OVER
                              │               │
                         (1s timer)        (R key)
                              │               │
                           PLAYING          MENU
                              │
                    (all bricks cleared)
                              │
                    level < 3         level = 3
                              │               │
                         COUNTDOWN       WIN_SCREEN
                                             │
                                      (S) ──► COUNTDOWN
                                      (R) ──► MENU
```

---

## 📐 Coordinate System

The entire game uses OpenGL Normalized Device Coordinates:
- X: `-1.0` (left edge) to `+1.0` (right edge)
- Y: `-1.0` (bottom edge) to `+1.0` (top edge)
- HUD occupies `Y = 0.78` to `Y = 1.0`
- Play field occupies `Y = -1.0` to `Y = 0.78`
- Bricks start at `Y = 0.72` and extend downward
- Paddle is fixed near `Y = -0.90`
- Ball falls out of bounds below `Y = -1.0`

---

## 🧪 Scoring

| Event | Points |
|---|---|
| Ball hits paddle | +5 |
| Ball destroys a brick | +10 |

---

## 👨‍💻 Authors

>Prayusha Pokhrel, Samrachana Sharma
IOE Pulchowk Campus

---

## 📄 License

This project was developed for academic purposes. Feel free to use it as a reference for learning OpenGL, GLUT, and computer graphics algorithms.