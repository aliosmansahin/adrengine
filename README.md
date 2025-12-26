# ADRENGINE
A game engine

## Features
* Asset Loading
  * Meshes
  * Textures
* Basic visual scripting system
* Interface with windows
* Scenes
* Entities
* Entity parameters
* Light system
* Physics system
* Project system

## Building With
* C++
* OpenGL
* GLM
* [GLFW][glfw-url]
* [GLAD][glad-url]
* [stb_image][stb-image-url]
* [ImGui][imgui-url]
* [ImGuiFileDialog][imgui-file-dialog-url]
* [ImNodes][imnodes-url]
* [nlohmann_json][nlohmann-json-url]
* [Bullet Physics][bullet-physics_url]
* [tinyobjloader][tiny-obj-loader-url]

## Roadmap
- [x] Physics System
- [x] Project System
- [x] Layout System
- [ ] Improving Light System
- [ ] Sound System
- [ ] Improving Asset System
- [ ] Improving Visual Scripting System
- [ ] Build System

## Installation
1- Clone this repo
```sh
git clone https://github.com/aliosmansahin/adrengine.git
```
2- Open the solution with VS 2022 <br><br>
3- Build and run the solution with "Local Windows Debugger" <br><br>

## How to Use
### Scenes
Scenes are like maps in ADRENGINE <br>
A scene stores all entities <br>

#### Adding a new scene
1- In "All Scenes" window: Click "Add" button <br>
2- Select the scene type <br>
3- Click "Add" button <br>

#### Change scenes
1- In "All Scenes" window: Double click one of all scenes to open it <br>

#### Delete scene
1- In "All Scenes" window: Right click one of all scenes <br>
2- Click "Delete" option <br>

### Entities
Entities are objects in ADRENGINE <br>
An entity belongs its scene <br>
There are some of types of entities which are inherit from entity object <br>
An entity can behave different depends on the type <br>
Stores a visual script <br>

#### Adding a new entity
1- In "Scene" window: Click "Add" button <br>
2- Search and select an entity type <br>

#### Opening entity properties
1- In "Scene" window: Click one of all entities <br>
2- Properties will be shown in "Entity Properties" window <br>

#### Deleting an entity
1- In "Scene" window: Right click one of all entities <br>
2- Select "Delete" option <br>

### Entity Hierarchy
You can update the hierarchy in "Scene" window <br>
An entity follows its parents transform parameter <br>

#### Moving into another entity
1- In "Scene" window: Press and hold an entity <br>
2- Move it into another entity <br>

#### Moving into root layer
1- In "Scene" window: Press and hold an entity <br>
2- Move it into "Root" <br>

### Visual Scripts
You can add node based visual script into entities <br>

#### Adding a new Visual Script
1- In "Entity Properties" window: Click "Add Script" button <br>

#### Editing a Visual Script
1- In "Entity Properties" window: Click "Edit Script" button <br>

#### Deleting a Visual Script
1- In "Entity Properties" window: Click "Delete Script" button <br>

#### Adding a New Node
1- Right click in script window <br>
2- Search and select a node <br>

#### Connecting nodes
1- Press and hold a property in a node <br>
2- Move it to another property of a node <br>

#### Deleting a node
1- Select nodes to delete <br>
2- Press "Del" button in your keyboard <br>

#### Deleting a connection
1- Move your mouse to a connection <br>
2- Press "Del" button in your keyboard <br>

### Assets
An asset stores some objects like textures, meshes etc... <br>
You can use them with entities <br>
#### Adding a new asset
1- In "Asset Explorer" window: Click "Create New One" button <br>
2- Select "Asset Type" <br>
3- Browse or write a path of your asset <br>
4- Give it a name <br>
5- Click "Add" button <br>

## PS
---This is an Alpha Version ---

This engine is already under development and it might have some bugs and issues.
The coder does not have any responsibilies about any issues. The person who uses this engine confirms that.




<!-- LINKS -->
[glfw-url]: https://github.com/glfw/glfw
[glad-url]: https://github.com/Dav1dde/glad
[stb-image-url]: https://github.com/nothings/stb
[imgui-url]: https://github.com/ocornut/imgui
[imgui-file-dialog-url]: https://github.com/aiekick/ImGuiFileDialog
[imnodes-url]: https://github.com/Nelarius/imnodes
[nlohmann-json-url]: https://github.com/nlohmann/json
[project-folder-line-url]: https://github.com/aliosmansahin/adrengine/blob/2cf4ffec57d05aec487006e40c4bdeb3d3ce659e/Engine/Engine.h#L53
[project-path-line-url]: https://github.com/aliosmansahin/adrengine/blob/2cf4ffec57d05aec487006e40c4bdeb3d3ce659e/Engine/Engine.h#L54
[bullet-physics_url]: https://github.com/bulletphysics/bullet3
[tiny-obj-loader-url]: https://github.com/tinyobjloader/tinyobjloader