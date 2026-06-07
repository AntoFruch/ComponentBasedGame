# ComponentBasedGame
Attemps to create a game engine with an Object/Component architecture. 

## Integration in a Project :
I propose that architecture :

```
├── src/
│   ├── assets/-> your custom game component / resources and all that makes your game your game
│   │   └── resources/
│   ├── engine/   -> submodule the repo here
│   └── main/     -> with mainLauncher.cpp in there which is the entrypoint
└── CMakeLists.txt
```


## Next things to do
* Testing

## Supported Features

### GameObjects
See class [GameObject.h](GameObject.h)

### SceneManagement

* **GameObject** with transforms
* **Prefabs**
* **SceneManagement** : Scene can be created by writing xml files, loaded handled by SceneManager static methods
* **Custom Components**
  * So far you need to create a class that inherits <code>Component</code> and then add the parser support in <code>SceneManager</code>
  * I'd like to make component creation easier by not making the user wirte code in <code>SceneManager</code>
* **InputSystem** : similar to Unity's one 
  * Just add your actions with different bindings in input-config.xml
* **Renderer** component : for displaying objects
* **Animation** support : sprite animation only
  * need an Animator component and an animatorTree xml file for transitions and stuff
  * (it is like the animator graph in unity but in xml)
* **Collision** support : 
  * Collider Component, Collisions Manager