# Benchmark of graphical librairies

Comparative study of the **SFML**, **Raylib**, **SDL** and **Vulkan** graphical libraries, for a complex project such as **R-Type**, to display different games running with the game engine.

## **Comparison Criteria**

The **libraries** are evaluated according to the following criteria:

- **Easy to use**: Simplicity of Setup and API Design.
- **Functionality**: Rendering Features, Cross-Platform Support.
- **Community and documentation**: Size of community and quality of available documentation.
- **Adaptability for complex projects**: Scalability and performance

## **Results**

| Criteria                       | SFML                                                                                          | Raylib                                                                                              | SDL                                                                                    | Vulkan                                                                                                                             |
|-------------------------------|-----------------------------------------------------------------------------------------------|-----------------------------------------------------------------------------------------------------|----------------------------------------------------------------------------------------|------------------------------------------------------------------------------------------------------------------------------------|
| **Easy to use**               | ⭐⭐⭐⭐: Intuitive and rich syntax. Simple for 2D graphics, moderate for 3D                      | ⭐⭐⭐⭐⭐: Simple, efficient, and very beginner-friendly API                                            | ⭐⭐:  Steep learning curve, requires external libraries for full functionality          | ⭐: Complex API, steep learning curve, low-level control for advanced users                                                         |
| **Features**                  | ⭐⭐⭐⭐⭐: Rich 2D/3D support, window management, and audio handling. Cross-platform              | ⭐⭐⭐⭐: Excellent 2D/3D rendering, integrated physics, and audio. Great for rapid prototyping         | ⭐⭐⭐⭐: Detailed rendering system, 2D/3D support, and audio. More flexible with OpenGL   | ⭐⭐⭐⭐⭐: High-performance graphics, shader support, multi-threading. Full GPU control                                                |
| **Community / Documentation** | ⭐⭐⭐⭐⭐: Comprehensive documentation and one of the largest C++ communities                     | ⭐⭐⭐⭐: Growing community, good documentation (simple and direct)                                     | ⭐⭐⭐: Large community, but documentation is inconsistent and lacking in depth           | ⭐⭐⭐⭐⭐: Large community, extensive documentation, but requires deeper understanding of graphics programming                         |
| **Adaptability**              | ⭐⭐⭐⭐⭐: Scales well to large projects, good performance on complex 2D/3D tasks                 | ⭐⭐⭐: Better suited for medium projects, limited scalability                                 | ⭐⭐⭐: Scales well for simpler projects, can be extended with external tools             | ⭐⭐⭐⭐⭐: Extremely scalable for large, complex projects; excellent for performance-heavy applications                                |
| **Overall**                   | ⭐⭐⭐⭐⭐: Widely used by both indie developers and large companies. Great for rapid development  | ⭐⭐⭐⭐: Simple, fast, ideal for small and mid-sized projects. Less suited for large-scale development | ⭐⭐⭐: Well-established and flexible, but not ideal for larger or more complex projects  | ⭐⭐⭐⭐⭐: Industry standard for high-performance applications. Requires significant setup but offers top-tier control and scalability |

## **Conclusion**

For a complex project like **R-Type**, **SFML** and **Raylib** are the best choices :

- Full of useful **features** with allowing cross-plateform.
- **SFML** has a very good **scalability** for large projects where **Raylib** has a bit less good **scalability**.
- Both has large and active community with a great **documentation**.
