# Mini-ECS
An implementation of ECS for ITMO test task.

## Build
- `make` or `make run` to build & run.
- `make out` to build.

## File structure
- `src/common.h` --- definitions that are common to all parts of the project.
- `src/main.cpp` --- a `main` function with an example of ECS usage.
- `src/math.h` --- 2D and 3D vectors and operations with them.
- `src/pool.h` --- A collection with fast add/remove/get/contains queries.
- `src/system.h` --- An interface for Systems.
- `src/type_id.h` --- ~Static type indexing.
- `src/world.h` --- A `World` class that runs it all & a `View` class to query Entities with a required set of Components.
  Those two classes are practically impossible to separate so they are in a single file.
- `src/components/` --- Examples of Components --- those are just common `struct`s.
- `src/systems/` --- Examples of Systems --- subclasses of `System`.

## Design
All the code except `src/main.cpp` is in header files.
It is usually a good practice to put the implementation into `.cpp` files but
in this project almost all the code is templated, and C++ needs templates implementation in
headers to correctly compile it.

- Entities are represented with `EntityID` type, which is an alias to `uint32_t`, for there is no need to distinguish between `EntityID` and `uint32_t`.
- Components are literally any types. A value of any type can be attached to an Entity and then retrieved. The user is responsible for not attaching methods to those types and only storing data in them.
- All Systems inherit the `System` class and override `update` function with `World& world` and `float dt` as parameters.
  In this function, the user is free to query Entities with any set of Components, query 
  It is user's responsibility not to call `World::update` from within a System's `update` function.
