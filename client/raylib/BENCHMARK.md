# Raylib vs SFML: Why We Chose Raylib for our R-Type project ?

This benchmark compares **Raylib** and **SFML** for developing our R-Type in C++ and explains why we selected Raylib as the better choice for our project.

## 1. **3D Graphics support**

Raylib, by default, natively supports 3D rendering that will be useful in enhancing the graphical fidelity of our game with 3D elements on a traditionally 2D environment. SFML, however, is mostly about 2D graphics, and for 3D, it needs to use third-party libraries such as OpenGL.

### Example: 3D rendering with Raylib
```cpp
#include "raylib.h"

int main() {
    InitWindow(800, 600, "R-Type: 3D Example");

    Camera3D camera = { 0 };
    camera.position = (Vector3){ 4.0f, 2.0f, 4.0f };
    camera.target = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.type = CAMERA_PERSPECTIVE;

    SetCameraMode(camera, CAMERA_FREE);

    while (!WindowShouldClose()) {
        UpdateCamera(&camera);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
        DrawCube((Vector3){0, 1, 0}, 1.0f, 1.0f, 1.0f, BLUE);
        DrawGrid(10, 1.0f);
        EndMode3D();

        DrawText("Move the camera with WASD", 10, 10, 20, DARKGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
```

### SFML Limitation
SFML does not natively support 3D. To achieve similar functionality, you need to integrate OpenGL, which increases complexity.

```cpp
// SFML Example would require OpenGL integration, making it verbose and less intuitive
```

## 2. **Easy to use**

Raylib was designed to be easy to use and allow for speed development, especially for beginners and small projects. Its API is minimalistic and intuitive. SFML is quite user-friendly, but it needs more time to learn to master similar functionality.

### Example: Window Creation
#### Raylib
```cpp
#include "raylib.h"

int main() {
    InitWindow(800, 600, "R-Type");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Welcome to R-Type!", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
```

#### SFML
```cpp
#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "R-Type");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);

        sf::Font font;
        if (!font.loadFromFile("arial.ttf")) return -1;
        sf::Text text("Welcome to R-Type!", font, 20);
        text.setFillColor(sf::Color::Black);
        window.draw(text);

        window.display();
    }

    return 0;
}
```

The Raylib example is significantly shorter and easier to read.

## 3. **Performance**

Raylib is lightweight and optimized for real-time applications, making it ideal for fast-paced games like R-Type. SFML, while efficient, can be heavier due to its object-oriented design and additional abstractions.

### Benchmark Example
Rendering 100,000 objects:
- **Raylib:** Achieved smoother FPS with less CPU overhead.
- **SFML:** Struggled at higher object counts without further optimizations.

## 4. **Built-in Tools**

Raylib includes utilities for managing assets like textures, sounds, and shaders. It also has debugging tools for quick testing. SFML requires more setup and often external libraries for similar functionality.

### Example: Audio with Raylib
```cpp
#include "raylib.h"

int main() {
    InitAudioDevice();
    Sound fx = LoadSound("resources/laser.wav");

    PlaySound(fx);
    CloseAudioDevice();
    return 0;
}
```

### SFML Audio Example
```cpp
#include <SFML/Audio.hpp>

int main() {
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("resources/laser.wav")) return -1;

    sf::Sound sound;
    sound.setBuffer(buffer);
    sound.play();

    return 0;
}
```

While both libraries provide audio playback, Raylib's simpler API shines in rapid prototyping.

## 5. **Cross-Platform Support**

Both libraries support Windows, Linux, and macOS, but Raylib also simplifies deployment on consoles like Raspberry Pi and supports web deployment via WebAssembly.

## 6. **Community and Documentation**

Raylib's documentation is clear and beginner-friendly, making it easy to get started. The community around Raylib is growing, with resources tailored to game development. SFML, while mature, can feel less tailored to game-specific projects.

## Conclusion
While SFML is a robust library, Raylib's support for 3D, ease of use, performance, and built-in tools make it the better choice for our R-Type remake in C++. It allows us to push graphical boundaries while keeping development fast and enjoyable.

---

### References
- [Raylib Official Documentation](https://www.raylib.com/)
- [SFML Official Documentation](https://www.sfml-dev.org/)
- [Raylib vs. SFML Discussion on StackOverflow](https://stackoverflow.com/)

