# Package Manager Benchmark for CMake

Comparative study of the **Conan**, **Vcpkg**, and **CMake CPM** package managers, for a complex project such as **R-Type**, containing an ECS, network, graphics and a complete game.

## **Comparison Criteria**

**Package Managers** are evaluated according to the following criteria:

- **Easy to use**: Simple configuration and integration with CMake.
- **Features**: Management of dependencies, versions, cross-compilation, etc.
- **Community and documentation**: Size of community and quality of available documentation.
- **Suitability for complex projects**: Ability to manage a large number of dependencies in large projects.

## **Results**

| Criteria                       | Conan                                                                      | Vcpkg                                                          | CMake CPM                                                                |
|-------------------------------|----------------------------------------------------------------------------|----------------------------------------------------------------|--------------------------------------------------------------------------|
| **Easy to use**               | ⭐⭐⭐: Simple configuration, but requires a remote server for centralization | ⭐⭐⭐⭐: Easy to use and well integrated with CMake               | ⭐⭐⭐⭐⭐: Ultra simple, integrated directly into CMake                      |
| **Features**                  | ⭐⭐⭐⭐: Versioning, cross-compilation, fine-tuned dependency management      | ⭐⭐⭐⭐: Very comprehensive, multi-platform support               | ⭐⭐⭐⭐: Minimalist but covers all standard needs                           |
| **Community / Documentation** | ⭐⭐⭐⭐: Active community, well-documented                                    | ⭐⭐⭐⭐: Active community, Microsoft support, clear documentation | ⭐⭐⭐: Smaller community, correct but less detailed documentation          |
| **Adaptability**              | ⭐⭐⭐⭐: Ideal for projects requiring varied and complex dependencies         | ⭐⭐⭐: Less suitable for a very large number of dependencies     | ⭐⭐⭐⭐⭐: Perfect for modular projects requiring flexibility and simplicity |
| **Overall**                   | ⭐⭐⭐⭐: Popular and robust                                                   | ⭐⭐⭐⭐: Good choice for simplicity and the Microsoft ecosystem   | ⭐⭐⭐⭐⭐: Excellent for complex CMake projects                              |

## **Conclusion**

For a complex project like **R-Type**, **CMake CPM** is the best choice:

- **Smooth integration** with CMake, with no need to install a third-party tool.
- **Simple** dependency management, even for large projects.
- Ideal for a project requiring high **modularity**, such as an ECS and a complete game.
- **Quick to learn**, particularly useful in an agile development context.

In conclusion, **CMake CPM** is an elegant and efficient solution for the needs of a project such as **R-Type**.
