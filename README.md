# Real-Time Video Effects

A real-time webcam video processing application built with **C++** and **OpenCV**. Apply a variety of image filters and effects to your live camera feed using simple keyboard shortcuts — including face detection, edge detection, artistic filters, and more.

---

## Features

| Key | Effect |
|-----|--------|
| `g` | Grayscale |
| `p` | Sepia tone |
| `b` | 5×5 Gaussian blur |
| `x` | Sobel X (horizontal edge detection) |
| `y` | Sobel Y (vertical edge detection) |
| `m` | Gradient magnitude |
| `l` | Blur & quantize |
| `f` | Face detection (Haar cascade) |
| `n` | Negative / color inversion |
| `e` | Emboss |
| `r` | Blur background, keep face sharp |
| `c` | Colorize detected face region |
| `w` | Halo & sparkles on face |
| `s` | Save current frame to file |
| `q` | Quit |

---

## Prerequisites

### C++ Compiler
A C++17-compatible compiler such as `g++`:
```bash
g++ --version
```

### CMake (3.10+)
```bash
cmake --version
```

### OpenCV (development version)
On Ubuntu/Debian:
```bash
sudo apt update
sudo apt install libopencv-dev
```

Verify:
```bash
pkg-config --modversion opencv4
```

---

## Build

From the project root directory:

```bash
mkdir build
cd build
cmake ..
make
```

This produces the executable at:
```
build/realtime_effects
```

---

## Run

From the `build/` directory:

```bash
./realtime_effects
```

The application opens your default webcam (device `0`) and displays a live preview window. Use the keyboard shortcuts listed above to toggle effects on and off in real time.

---

## Project Structure

```
real_effects/
├── CMakeLists.txt          # Build configuration
├── vidDisplay.cpp          # Main loop: webcam capture & keyboard input
├── filter.cpp              # Image filter implementations
├── filter.h                # Filter function declarations
└── faceDetect/
    ├── faceDetect.cpp      # Face detection logic
    ├── faceDetect.h
    ├── showFaces.cpp
    └── haarcascade_frontalface_alt2.xml   # Haar cascade model
```

---

## Notes

- Multiple effects can be toggled on simultaneously and are applied in sequence.
- Face-aware effects (`r`, `c`, `w`) run face detection internally each frame.
- Press `s` to save a snapshot; the filename reflects the currently active filter.

---

