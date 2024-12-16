---
sidebar_position: 1
---

# Introduction

This is our guide on how to start contributing to R-Type.

## What's R-Type?
R-Type is a horizontally scrolling shooter arcade video game developed and released by Irem in 1987 and the first game in the R-Type series.
The player controls a star ship, the R-9 "Arrowhead", in its efforts to destroy the Bydo, a powerful alien race bent on wiping out all of mankind.

## Why did we create this game?
This is one of our 3 biggest projects in our third year at [Epitech](https://www.epitech.eu/), the goal is to create our own
game engine inspired on the R-Type's game in C++.

## ECS
ECS is the acronym for **Entity Component System**, it's an architectural design pattern, mostly used in video game development. This pattern follows the principle of composition over inheritance. Instead of defining an inheritance tree as usual in Object Ori- ented Programming, types are split into small yet highly reusable components.

A common composition pattern in OOP would be to put functions modifying these components together with the components, for example a physics components containing mass, velocity, acceleration and hit- box together with functions to move and collide entities that have this component.
In an ECS, we try to keep components as light as possible (more on that later). However, an issue arise if we try to put logic inside of components, as part of this logic could depends on multiple attributes compo- nents if they are too “small”. One way to prevent this issue while keeping components as small as possible is taking the logic (functions) outside of components. These standalone functions are then free to depend on an arbitrary number of components. We call these functions systems.
In our physics component example, it would be split into its part (position, velocity, mass, acceleration and hit-box), and an array of functions would interact with all entities that have a specific sub-set of compo- nents.

We would thus have the following systems:
- movement_system: update all entities positions and velocities if they have the following components: velocity, position and acceleration.
- gravity_system: update entities accelerations, by taking their mass into account. The following compo- nents must be present: acceleration and mass.
- collision_system: handle collisions between all entities that has a hit-box. The entities must be composed from the following components: position and hit-box.

This pattern allows for a more flexible architecture, with the ability to add/remove components on the fly. You could for example handle status effect by adding a component to an entity, and handling it in the proper system (i.e. adding a slow component to an entity, which would be taken into account by the movement_system , or a slow_system that would update the velocity before new positions are computed).

There are two other perks with this pattern:
- The way data is organised is close to the way it could be stored in a database (each component is a row in a table)
- The way data is organised plays well with the CPU cache (more on that in a bit).