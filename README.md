# ComponentBasedGame
Attemps to create a simple game engine with an Object/Component architecture 

## Next things to implement
* Need more on colliders, so far we can only detect collisions and prevent 2 solid colliders intersected one another
* Some refractoring in the project, maybe build the engine as a separate library than custom components and everything
* More testing

## Supported Features
* **GameObject** with transforms
* **Prefabs**
* **SceneManagement** : Scene can be created by writing xml files
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