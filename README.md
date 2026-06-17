# Component Based Game Engine

Ce README a ete genere par Codex a partir des demandes suivantes:

```text
je veux faire un gros readme qui detaille l'entiereté du fonctionnement du moteur avec des exemples et tout.

Fais un plan un peu détaillé de quoi parler je veux qu'a partir de cette doc on puisse creer quelque chose en se basant sur le moteur

on généra le readme apres validation du plan
```

```text
ok tout les points sont bien , enleve le 21 sur les tests, le 24 et 25 aussi.

le readme est destiné au moteur, AUCUNE DOC POUR LE RESTE NE FOIT Y FIGURER. tu peux néanmoins t'appuyer sur du code existant dans le reste pour illustrer des fonctionnements.

dans le point 20 sur le build, pense bien a parler des dependances et comment les mettre dans le CMakeList, exemple de comment organiser les CmakeList (comme c'est actuellement en fait)

génère le ReadMe dans engine directement

met un sommaire au debut
```

Ce dossier contient un moteur 2D oriente composants, pense pour construire un jeu a partir de scenes XML, de prefabs, de composants C++ auto-enregistres, d'un systeme d'input, de rendu, d'animation, de collisions et d'UI.

La documentation ci-dessous decrit le fonctionnement du moteur. Les exemples utilisent parfois des noms de composants ou de fichiers proches d'un projet existant, mais ils servent uniquement a illustrer l'utilisation du moteur.

## Sommaire

1. [Vue d'ensemble](#vue-densemble)
2. [Organisation recommandee d'un projet](#organisation-recommandee-dun-projet)
3. [Cycle de vie global](#cycle-de-vie-global)
4. [GameObject, Transform et Component](#gameobject-transform-et-component)
5. [Scenes XML](#scenes-xml)
6. [Prefabs](#prefabs)
7. [ComponentFactory et auto-enregistrement](#componentfactory-et-auto-enregistrement)
8. [Creer un nouveau composant](#creer-un-nouveau-composant)
9. [Composants moteur disponibles](#composants-moteur-disponibles)
10. [Systeme de rendu](#systeme-de-rendu)
11. [Systeme d'animation](#systeme-danimation)
12. [Systeme de collisions](#systeme-de-collisions)
13. [Systeme d'input](#systeme-dinput)
14. [Systeme d'UI](#systeme-dui)
15. [SceneManager et navigation entre scenes](#scenemanager-et-navigation-entre-scenes)
16. [Scripts de gameplay cote projet](#scripts-de-gameplay-cote-projet)
17. [Ajouter une nouvelle entite](#ajouter-une-nouvelle-entite)
18. [Ajouter une nouvelle map](#ajouter-une-nouvelle-map)
19. [Ajouter une nouvelle UI](#ajouter-une-nouvelle-ui)
20. [Build, dependances et CMake](#build-dependances-et-cmake)
21. [Tutoriel complet: creer une mini-scene](#tutoriel-complet-creer-une-mini-scene)
22. [Conventions et bonnes pratiques](#conventions-et-bonnes-pratiques)

## Vue d'ensemble

Le moteur suit une architecture proche de Unity:

- un `GameObject` represente un objet dans la scene;
- un `Transform` porte sa position, sa rotation, son echelle et sa relation parent/enfant;
- un `Component` ajoute un comportement a un `GameObject`;
- une `Scene` contient des objets, charges depuis XML;
- un `Prefab` est un `GameObject` reutilisable, lui aussi decrit en XML;
- une `ComponentFactory` construit les composants a partir de leur nom dans le XML;
- des managers statiques gerent le rendu, les collisions, les inputs et les changements de scene.

Le point important: le moteur ne connait pas les composants propres a un jeu. Le projet utilisateur fournit ses scripts dans un dossier separe, les enregistre dans la factory, puis les utilise dans ses scenes XML.

## Organisation recommandee d'un projet

Une integration typique ressemble a ceci:

```text
src/
├── engine/
│   ├── Components/
│   ├── Managers/
│   ├── Game.h
│   ├── GameObject.h
│   ├── Transform.h
│   └── Engine.h
├── assets/
│   ├── Assets.h
│   ├── scripts/
│   └── resources/
│       ├── scenes/
│       ├── prefabs/
│       ├── textures/
│       ├── animations/
│       ├── fonts/
│       └── config/
└── main/
    └── mainLauncher.cpp
```

Role des dossiers:

- `engine/`: code reutilisable du moteur.
- `assets/scripts/`: composants et managers specifiques au projet utilisateur.
- `assets/resources/`: fichiers charges a l'execution: scenes, prefabs, textures, animations, polices, configuration d'input.
- `assets/Assets.h`: header d'activation qui inclut les composants auto-enregistres du projet.
- `main/`: executable final et point d'entree.

## Cycle de vie global

Le lancement standard ressemble a ceci:

```cpp
#include "Engine.h"
#include "Assets.h"

int main()
{
    Game game("resources/scenes/start.xml");
    game.run();
    return 0;
}
```

Le cycle complet:

1. L'executable demarre.
2. Les variables statiques des composants inclus par `Engine.h` et `Assets.h` s'initialisent.
3. Chaque composant auto-enregistre appelle `ComponentFactory::Register(...)`.
4. `Game` initialise les managers principaux.
5. `Game` cree une `Scene`.
6. `SceneManager::requestLoading(...)` demande le chargement de la scene initiale.
7. La boucle de jeu traite les events SFML, met a jour la scene, puis rend l'image.
8. Les `GameObject` appellent `Start()` une fois, puis `Update()` a chaque frame logique.

La boucle de jeu utilise un pas fixe:

```cpp
const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);
```

Le rendu se fait apres les updates. Le moteur utilise SFML pour la fenetre et TGUI pour l'interface.

## GameObject, Transform et Component

### GameObject

Un `GameObject` est le conteneur de base du moteur. Il possede:

- un label;
- un `Transform`;
- une liste de composants;
- une liste d'enfants;
- un etat actif/inactif;
- un flag de destruction differee.

Creation typique par XML:

```xml
<GameObject label="Player" x="0" y="0" angle="0" sx="1" sy="1" active="true">
    <Components>
        <Component name="Renderer" src="resources/textures/player.png" sprite_w="32" sprite_h="32"/>
    </Components>
</GameObject>
```

Recuperer un composant depuis un script:

```cpp
auto renderer = gameObject->getComponent<Renderer>();
```

`getComponent<T>()` utilise un `dynamic_cast`, donc `T` doit heriter de `Component`.

### Transform

`Transform` gere:

- position locale;
- rotation locale;
- echelle locale;
- parent;
- enfants;
- position/rotation/echelle monde calculees.

Exemples:

```cpp
gameObject->transform.setLocalPosition({100.f, 50.f});
gameObject->transform.move({10.f, 0.f});
gameObject->transform.rotate(sf::degrees(90.f));
gameObject->transform.rescale({2.f, 2.f});
```

Un enfant herite du transform monde de son parent. Cela permet de construire des objets composes: personnage + camera enfant, personnage + hitbox enfant, map + colliders enfants, etc.

### Component

Un composant represente un comportement:

```cpp
class MyComponent : public Component {
public:
    void Start() override;
    void Update(const sf::Time& elapsedTime) override;
};
```

Le moteur appelle:

- `Start()` une fois, apres construction de l'objet et de ses composants;
- `Update(elapsedTime)` a chaque frame logique si l'objet est actif.

Le composant possede un pointeur vers son `GameObject` parent, defini par `GameObject::addComponent`.

Bonne pratique: faire les recuperations de composants dans `Start()`, pas dans le constructeur.

```cpp
void MyController::Start()
{
    renderer = gameObject->getComponent<Renderer>();
    collider = gameObject->getComponent<Collider>();
}
```

## Scenes XML

Une scene est un fichier XML dont le noeud racine est `Scene`.

```xml
<?xml version="1.0" encoding="UTF-8" ?>
<Scene>
    <GameObject label="CameraRoot" x="0" y="0">
        <Components>
            <Component name="Camera"/>
        </Components>
    </GameObject>

    <Prefab src="resources/prefabs/player.xml" x="100" y="50"/>
</Scene>
```

Enfants autorises dans `Scene`:

- `GameObject`;
- `Prefab`.

Enfants autorises dans `GameObject`:

- `Components`;
- `GameObject`;
- `Prefab`.

La balise `Components` doit contenir uniquement des balises `Component`.

```xml
<Components>
    <Component name="Renderer" src="resources/textures/object.png" sprite_w="32" sprite_h="32"/>
    <Component name="Collider" x="0" y="0" width="32" height="32"/>
</Components>
```

Attributs courants de `GameObject`:

- `label`: nom lisible de l'objet;
- `x`, `y`: position locale;
- `angle`: rotation en degres;
- `sx`, `sy`: echelle locale;
- `active`: activation initiale.

Valeurs par defaut:

- `sx = 1`;
- `sy = 1`;
- `active = true`.

Si une scene contient un composant inconnu, le moteur leve une `IllegalOperationException`:

```text
Class MyComponent does not exist.
```

Cela veut dire que le composant n'a pas ete enregistre dans la `ComponentFactory`, ou que son header n'a pas ete inclus par le projet final.

## Prefabs

Un prefab est un fichier XML reutilisable dont la racine est un `GameObject`.

```xml
<?xml version="1.0" encoding="UTF-8" ?>
<GameObject label="Player" x="0" y="0" angle="0" sx="2" sy="2">
    <Components>
        <Component name="Collider" x="0" y="0" width="8" height="8"/>
        <Component name="Renderer" src="resources/textures/player.png" sprite_w="32" sprite_h="32"/>
    </Components>

    <GameObject label="AttackTrigger" active="false">
        <Components>
            <Component name="Collider" x="0" y="0" width="8" height="8" trigger="true"/>
        </Components>
    </GameObject>
</GameObject>
```

Utilisation depuis une scene:

```xml
<Prefab src="resources/prefabs/player.xml" x="100" y="50"/>
```

Un prefab peut etre surcharge au point d'instanciation:

```xml
<Prefab
    src="resources/prefabs/enemy.xml"
    label="Enemy_01"
    x="300"
    y="120"
    angle="0"
    sx="1.5"
    sy="1.5"
    active="true"/>
```

Overrides pris en charge:

- `label`;
- `x`, `y`;
- `angle`;
- `sx`, `sy`;
- `active`.

Les prefabs sont utiles pour:

- creer des entites reutilisables;
- alleger les scenes;
- partager une structure hierarchique complexe;
- instancier dynamiquement un objet via `SceneManager::instantiate`.

## ComponentFactory et auto-enregistrement

Le moteur cree les composants depuis le XML avec:

```xml
<Component name="Renderer"/>
```

Le nom `Renderer` est resolu par `ComponentFactory`.

La factory stocke une map:

```cpp
std::unordered_map<std::string, Creator>
```

ou `Creator` est une fonction:

```cpp
std::function<std::unique_ptr<Component>(const pugi::xml_node& node)>
```

Chaque composant s'enregistre lui-meme dans la factory avec une variable statique:

```cpp
class MyComponent : public Component {
private:
    static inline bool s_registered =
        ComponentFactory::Register("MyComponent",
            [](const pugi::xml_node& node) -> std::unique_ptr<Component>
            {
                return std::make_unique<MyComponent>();
            });
};
```

Le registre de la factory est une variable statique locale:

```cpp
static std::unordered_map<std::string, Creator> registry;
```

Cela evite les problemes d'ordre d'initialisation statique entre fichiers.

Point crucial: pour que l'auto-enregistrement se produise, le header du composant doit etre inclus dans une unite de compilation liee a l'executable.

Le moteur inclut ses composants internes via `Engine.h`. Le projet utilisateur doit inclure ses composants dans un header d'activation, par exemple:

```cpp
// Assets.h
#ifndef PROJECT_ASSETS_H
#define PROJECT_ASSETS_H

#include "scripts/PlayerController.h"
#include "scripts/EnemyController.h"
#include "scripts/UI/MainMenu.h"

#endif
```

Puis le point d'entree inclut ce header:

```cpp
#include "Engine.h"
#include "Assets.h"
```

Sans cet include, le composant peut compiler dans une librairie statique mais ne jamais etre initialise avant `main()`.

## Creer un nouveau composant

### 1. Creer le header

```cpp
#ifndef PROJECT_DAMAGE_ZONE_H
#define PROJECT_DAMAGE_ZONE_H

#include "Engine.h"

class DamageZone : public Component {
    int damage;
    Collider* collider{nullptr};

public:
    explicit DamageZone(int damage);

    void Start() override;
    void Update(const sf::Time& elapsedTime) override;

private:
    static inline bool s_registered =
        ComponentFactory::Register("DamageZone",
            [](const pugi::xml_node& node) -> std::unique_ptr<Component>
            {
                return std::make_unique<DamageZone>(
                    node.attribute("damage").as_int()
                );
            });
};

#endif
```

### 2. Creer le fichier source

```cpp
#include "DamageZone.h"

DamageZone::DamageZone(int damage)
    : damage(damage)
{}

void DamageZone::Start()
{
    collider = gameObject->getComponent<Collider>();
}

void DamageZone::Update(const sf::Time& elapsedTime)
{
    // Logique frame-by-frame.
}
```

### 3. Inclure le composant dans le header d'activation du projet

```cpp
#include "scripts/DamageZone.h"
```

### 4. Ajouter les fichiers au CMake du module utilisateur

```cmake
add_library(lib_assets
    Assets.h
    scripts/DamageZone.cpp
    scripts/DamageZone.h
)
```

### 5. Utiliser le composant dans XML

```xml
<Component name="DamageZone" damage="10"/>
```

Regle pratique: le nom donne a `ComponentFactory::Register("DamageZone", ...)` doit etre exactement le meme que l'attribut XML `name="DamageZone"`.

## Composants moteur disponibles

### Renderer

Affiche une texture sur un rectangle SFML.

Exemple XML:

```xml
<Component
    name="Renderer"
    src="resources/textures/character.png"
    anchorX="0"
    anchorY="0"
    sprite_w="32"
    sprite_h="32"/>
```

Attributs:

- `src`: chemin de texture;
- `anchorX`, `anchorY`: decalage d'origine;
- `sprite_w`, `sprite_h`: taille du sprite dans la texture.

Le `Renderer` s'enregistre dans `RenderManager` lors de sa creation.

### Animator

Anime un `Renderer` avec un graphe d'animation XML.

```xml
<Component name="Animator" src="resources/animations/CharacterAnimationTree.xml"/>
```

Il attend generalement qu'un `Renderer` soit present sur le meme `GameObject`.

### Collider

Ajoute une hitbox.

```xml
<Component name="Collider" x="0" y="0" width="32" height="32"/>
```

Collider trigger:

```xml
<Component name="Collider" x="0" y="0" width="32" height="32" trigger="true"/>
```

Attributs:

- `x`, `y`: position locale de la hitbox;
- `width`, `height`: dimensions;
- `trigger`: si `true`, le collider ne sert pas aux collisions solides mais peut declencher un callback.

### Camera

Applique une vue SFML au rendu.

```xml
<Component name="Camera"/>
```

Une camera est souvent placee comme enfant d'un objet a suivre.

### CharacterController

Composant de deplacement simple base sur un `Collider`.

```xml
<Component name="CharacterController"/>
```

Il fournit notamment une methode `move(...)` qui deplace le `GameObject` en respectant les collisions gerees par le moteur.

### UIDocument

Base pour creer une interface TGUI.

```xml
<Component name="UIDocument"/>
```

En pratique, on herite de `UIDocument` pour creer une UI concrete: menu, barre de vie, panneau de dialogue, etc.

## Systeme de rendu

Le rendu est centralise dans `RenderManager`.

Le flux standard:

1. Les composants `Renderer` s'enregistrent dans `RenderManager`.
2. A chaque frame, `Game::render()` appelle `RenderManager::renderAll(...)`.
3. Chaque `Renderer` synchronise sa forme avec le `Transform` monde de son `GameObject`.
4. La fenetre SFML affiche le resultat.

Dans un `Renderer::Update`:

```cpp
mShape.setPosition(gameObject->transform.getWorldPosition());
mShape.setRotation(gameObject->transform.getWorldRotation());
mShape.setScale(gameObject->transform.getWorldScale());
```

Le resize de fenetre est transmis au manager:

```cpp
RenderManager::handleResize(resized->size);
```

Conseils:

- utilisez un `Renderer` par sprite visible;
- gardez les chemins de textures relatifs au dossier de ressources copie pres du binaire;
- configurez correctement `sprite_w` et `sprite_h` pour que l'animation puisse decouper la texture.

Note :
- par defaut, les rendus sont faits dans l'ordre de (position y - hauteur de la texture) pour avoir un effet de perspective cohérents (les objets de plus grand y doivent etre rendus par dessus ceux de plus petit y) 

## Systeme d'animation

Le composant `Animator` charge un graphe d'animation depuis XML.

Exemple:

```xml
<AnimationTree label="CharacterAnimator">
    <Parameters>
        <Parameter label="moving" default="false" type="Bool"/>
        <Parameter label="speed" default="0" type="Float"/>
        <Parameter label="attack" default="false" type="Trigger"/>
    </Parameters>

    <EntryState label="Idle"/>

    <Tree>
        <Animation label="Idle" startx="0" starty="0" length="2" frameDuration="300" loop="true" breakable="true">
            <Transition to="Move">
                <Condition parameter="moving" value="true"/>
            </Transition>
        </Animation>

        <Animation label="Move" startx="2" starty="0" length="4" frameDuration="100" loop="true" breakable="true">
            <Transition to="Idle">
                <Condition parameter="moving" value="false"/>
            </Transition>
        </Animation>
    </Tree>
</AnimationTree>
```

Concepts:

- `Parameter`: valeur pilotee par le code (`Bool`, `Float`, `Trigger`);
- `EntryState`: etat initial;
- `Animation`: sequence de frames;
- `Transition`: changement d'etat possible;
- `Condition`: condition sur un parametre;
- `BlendTree`: groupe d'animations charge depuis un fichier XML separe.

Modifier un parametre depuis un script:

```cpp
animator->setParam("moving", true);
animator->setParam("speed", 1.f);
```

Enregistrer un evenement d'animation:

```cpp
animator->registerAnimationEvent("Attack", 2, [this]() {
    // Declencher une hitbox ou un effet.
});
```

Les evenements sont utiles pour synchroniser logique de gameplay et animation.

## Systeme de collisions

Les collisions reposent sur:

- `Collider`;
- `CollisionsManager`.

Un collider solide sert aux deplacements bloques. Un trigger sert a detecter une entree dans une zone sans bloquer le mouvement.

Exemple de trigger:

```cpp
void LoadingZoneLikeComponent::Start()
{
    auto collider = gameObject->getComponent<Collider>();
    collider->setTriggerCallback(
        [this](const std::vector<Collider*>& hits, Collider* self) {
            for (auto hit : hits) {
                // Reagir a l'objet touche.
            }
        }
    );
}
```

Pattern courant:

- collider principal sur l'entite;
- enfant inactif avec un collider trigger pour une attaque;
- zone trigger pour changer de scene;
- colliders fixes sur une map.

Le debug draw des collisions est appele dans `Game::render()`:

```cpp
CollisionsManager::debugDraw(mWindow);
```

## Systeme d'input

Les inputs sont decrits dans:

```text
resources/config/input-config.xml
```

Format:

```xml
<InputActions>
    <Action label="Move" type="Vector2D">
        <binding>
            <up bind="Z"/>
            <down bind="S"/>
            <left bind="Q"/>
            <right bind="D"/>
        </binding>
    </Action>

    <Action label="Attack" type="Button">
        <binding bind="P"/>
    </Action>
</InputActions>
```

Types pris en charge:

- `Vector2D`: action directionnelle;
- `Button`: bouton simple.

Recuperer une action:

```cpp
InputAction* moveAction = InputManager::findAction("Move");
InputAction* attackAction = InputManager::findAction("Attack");
```

Lire une direction:

```cpp
sf::Vector2f direction = moveAction->ReadValue<sf::Vector2f>();
```

Detecter une pression sur la frame:

```cpp
if (attackAction->wasPerformedThisFrame()) {
    // Attaque, interaction, menu...
}
```

Le moteur appelle automatiquement:

- `InputManager::beginFrame()`;
- `InputManager::processEvents(event)`;
- `InputAction::processEvent(...)`.

## Systeme d'UI

L'UI est basee sur TGUI.

Le composant moteur `UIDocument` fournit:

- acces au `tgui::Gui`;
- stockage d'une liste de widgets;
- methode `addElement(...)`;
- cycle `Start()` / `Update()`.

Exemple de composant UI:

```cpp
class MainMenuLikeUI : public UIDocument {
public:
    void Start() override;
};

void MainMenuLikeUI::Start()
{
    UIDocument::Start();

    auto button = tgui::Button::create();
    button->setText("Play");
    button->setPosition("50% - width*0.5", "50%");
    button->onPress([]() {
        SceneManager::requestLoading("resources/scenes/level.xml");
    });

    addElement(button, "PlayButton");
}
```

Une UI concrete doit etre enregistree dans `ComponentFactory` comme n'importe quel composant.

```xml
<GameObject label="UI">
    <Components>
        <Component name="MainMenuLikeUI"/>
    </Components>
</GameObject>
```

Conseils:

- creez les widgets dans `Start()`;
- stockez les widgets importants en attributs;
- mettez a jour les valeurs dynamiques dans `Update()`;
- appelez `UIDocument::Start()` et `UIDocument::Update(...)` dans les classes derivees.

## SceneManager et navigation entre scenes

`SceneManager` est l'interface principale pour manipuler la scene courante.

Initialisation faite par `Game`:

```cpp
scene = std::make_unique<Scene>(mWindow);
SceneManager::init(scene.get());
SceneManager::requestLoading(scenePath);
```

Changer de scene:

```cpp
SceneManager::requestLoading("resources/scenes/level_02.xml");
```

Le chargement est differe: la demande est appliquee a un moment controle de l'update pour eviter de detruire la scene pendant qu'elle est parcourue.

Instancier un prefab:

```cpp
GameObject* spawned = SceneManager::instantiate("resources/prefabs/projectile.xml");
spawned->transform.setLocalPosition({100.f, 200.f});
```

Geler / degeler la scene:

```cpp
SceneManager::freeze();
SceneManager::unfreeze();
```

Recuperer le GUI:

```cpp
tgui::Gui* gui = SceneManager::getGui();
```

## Scripts de gameplay cote projet

Le moteur ne fournit pas de gameplay final. Il fournit les briques pour que le projet utilisateur ecrive ses scripts.

Un script de gameplay est simplement un composant:

```cpp
class PlayerLikeController : public Component {
    InputAction* moveAction{nullptr};
    Collider* collider{nullptr};

public:
    void Start() override
    {
        moveAction = InputManager::findAction("Move");
        collider = gameObject->getComponent<Collider>();
    }

    void Update(const sf::Time& elapsedTime) override
    {
        sf::Vector2f direction = moveAction->ReadValue<sf::Vector2f>();
        gameObject->transform.move(direction * elapsedTime.asSeconds() * 100.f);
    }
};
```

Les scripts projet peuvent utiliser:

- `GameObject::getComponent<T>()`;
- `SceneManager::requestLoading(...)`;
- `SceneManager::instantiate(...)`;
- `InputManager::findAction(...)`;
- `Collider::setTriggerCallback(...)`;
- `Animator::setParam(...)`;
- `Animator::registerAnimationEvent(...)`.

Pour des besoins globaux, le projet peut creer ses propres managers dans `assets/scripts`.

## Ajouter une nouvelle entite

Une entite est generalement un prefab compose de plusieurs composants.

Exemple:

```xml
<GameObject label="Enemy" x="0" y="0" sx="2" sy="2">
    <Components>
        <Component name="EnemyLikeController" speed="80"/>
        <Component name="Collider" x="0" y="0" width="16" height="16"/>
        <Component name="Renderer" src="resources/textures/enemy.png" sprite_w="32" sprite_h="32"/>
        <Component name="Animator" src="resources/animations/enemy.xml"/>
    </Components>
</GameObject>
```

Etapes:

1. Creer le composant C++ de controle.
2. L'enregistrer dans `ComponentFactory`.
3. L'ajouter au header d'activation du projet.
4. Ajouter ses fichiers au CMake du projet.
5. Creer un prefab XML.
6. Instancier le prefab dans une scene.

Scene:

```xml
<Prefab src="resources/prefabs/enemy.xml" x="300" y="100"/>
```

## Ajouter une nouvelle map

Une map peut etre representee par un prefab contenant:

- un `Renderer` pour l'image de fond;
- des `Collider` fixes;
- des zones trigger;
- des enfants pour organiser les elements.

Exemple simplifie:

```xml
<GameObject label="Map" x="0" y="0">
    <Components>
        <Component name="Renderer" src="resources/textures/maps/level.png" sprite_w="640" sprite_h="640"/>
    </Components>

    <GameObject label="WallTop" x="0" y="-320">
        <Components>
            <Component name="Collider" x="0" y="0" width="640" height="32"/>
        </Components>
    </GameObject>

    <GameObject label="ExitZone" x="200" y="0">
        <Components>
            <Component name="Collider" x="0" y="0" width="32" height="64" trigger="true"/>
            <Component name="ScenePortal" sceneToLoad="resources/scenes/next.xml"/>
        </Components>
    </GameObject>
</GameObject>
```

La scene peut ensuite charger cette map avec:

```xml
<Prefab src="resources/prefabs/maps/level.xml"/>
```

## Ajouter une nouvelle UI

Etapes:

1. Creer une classe qui herite de `UIDocument`.
2. Creer les widgets dans `Start()`.
3. Mettre a jour les valeurs dynamiques dans `Update()`.
4. Enregistrer le composant dans `ComponentFactory`.
5. Ajouter le header dans `Assets.h`.
6. Ajouter les fichiers au CMake.
7. Ajouter le composant a une scene ou a un prefab.

Exemple:

```cpp
class DialogueBox : public UIDocument {
    tgui::Label::Ptr text;

public:
    void Start() override
    {
        UIDocument::Start();
        text = tgui::Label::create();
        text->setText("Hello");
        addElement(text, "DialogueText");
    }

private:
    static inline bool s_registered =
        ComponentFactory::Register("DialogueBox",
            [](const pugi::xml_node& node) -> std::unique_ptr<Component>
            {
                return std::make_unique<DialogueBox>();
            });
};
```

XML:

```xml
<GameObject label="DialogueUI">
    <Components>
        <Component name="DialogueBox"/>
    </Components>
</GameObject>
```

## Build, dependances et CMake

Le moteur depend principalement de:

- SFML;
- TGUI;
- pugixml;
- CMake.

L'organisation recommandee est de construire trois cibles:

- `lib_engine`: bibliotheque du moteur;
- `lib_assets`: bibliotheque du projet utilisateur;
- `mainLauncher`: executable final.

### CMake racine

Exemple de `CMakeLists.txt` racine:

```cmake
cmake_minimum_required(VERSION 3.26)

include(FetchContent)
project(component_based_arch VERSION 1.0.0 LANGUAGES CXX)

set(BUILD_SHARED_LIBS FALSE)
set(CMAKE_CXX_STANDARD 20)

FetchContent_Declare(
    sfml
    URL https://github.com/SFML/SFML/archive/refs/tags/3.0.1.tar.gz
)
option(SFML_BUILD_AUDIO "Build audio" OFF)
option(SFML_BUILD_NETWORK "Build network" OFF)
FetchContent_MakeAvailable(sfml)

FetchContent_Declare(
    pugixml
    URL https://github.com/zeux/pugixml/releases/download/v1.15/pugixml-1.15.tar.gz
)
FetchContent_MakeAvailable(pugixml)

FetchContent_Declare(
    TGUI
    URL https://github.com/texus/TGUI/archive/refs/tags/v1.12.1.tar.gz
)
set(TGUI_BACKEND "SFML_GRAPHICS" CACHE STRING "TGUI backend" FORCE)
FetchContent_MakeAvailable(TGUI)

add_subdirectory(src/engine)
add_subdirectory(src/assets)
add_subdirectory(src/main)
```

Les dependances sont telechargees par CMake via `FetchContent`. Le projet utilisateur n'a donc pas besoin de les ajouter manuellement au depot.

### CMake du moteur

Exemple de structure pour `src/engine/CMakeLists.txt`:

```cmake
add_library(lib_engine
    GameObject.cpp
    GameObject.h
    Transform.cpp
    Transform.h
    Game.cpp
    Game.h
    Engine.h

    Components/Component.cpp
    Components/Component.h
    Components/Renderer.cpp
    Components/Renderer.h
    Components/Animator.cpp
    Components/Animator.h
    Components/Collider.cpp
    Components/Collider.h
    Components/CharacterController.cpp
    Components/CharacterController.h
    Components/Camera.cpp
    Components/Camera.h
    Components/UIDocument.cpp
    Components/UIDocument.h

    Managers/Scene/Scene.cpp
    Managers/Scene/Scene.h
    Managers/Scene/SceneManager.cpp
    Managers/Scene/SceneManager.h
    Managers/Scene/ComponentFactory.cpp
    Managers/Scene/ComponentFactory.h

    Managers/Input/InputManager.cpp
    Managers/Input/InputManager.h
    Managers/Input/InputAction.cpp
    Managers/Input/InputAction.h

    Managers/Render/RenderManager.cpp
    Managers/Render/RenderManager.h
    Managers/Collisions/CollisionsManager.cpp
    Managers/Collisions/CollisionsManager.h
    Managers/Animation/AnimatorGraph.cpp
    Managers/Animation/AnimatorGraph.h
    Managers/Animation/AnimatorTreeLoader.cpp
    Managers/Animation/AnimatorTreeLoader.h
)

target_include_directories(lib_engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(lib_engine PUBLIC tgui pugixml sfml-graphics)
```

Le `PUBLIC` sur `target_include_directories` permet aux autres cibles d'inclure directement:

```cpp
#include "Engine.h"
#include "Components/Renderer.h"
```

### CMake du projet utilisateur

Exemple de `src/assets/CMakeLists.txt`:

```cmake
add_library(lib_assets
    Assets.h
    scripts/PlayerController.cpp
    scripts/PlayerController.h
    scripts/EnemyController.cpp
    scripts/EnemyController.h
    scripts/UI/MainMenu.cpp
    scripts/UI/MainMenu.h
)

target_include_directories(lib_assets PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(lib_assets PUBLIC lib_engine pugixml sfml-graphics tgui)
```

Important:

- ajoutez les `.cpp` des composants, sinon leur implementation ne sera pas compilee;
- ajoutez les `.h` pour que l'IDE les voie clairement;
- incluez les composants auto-enregistres dans `Assets.h`;
- liez `lib_assets` a `lib_engine`.

### CMake de l'executable

Exemple de `src/main/CMakeLists.txt`:

```cmake
add_executable(mainLauncher
    mainLauncher.cpp
)

target_link_libraries(mainLauncher PUBLIC lib_engine lib_assets pugixml sfml-graphics)

add_custom_target(copy-resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/src/assets/resources
            ${CMAKE_CURRENT_BINARY_DIR}/resources
    COMMENT "Copying project resources...")

add_dependencies(mainLauncher copy-resources)
```

Le target `copy-resources` copie `src/assets/resources` a cote du binaire. C'est important car le moteur charge les ressources avec des chemins relatifs comme:

```text
resources/scenes/start.xml
resources/textures/player.png
resources/config/input-config.xml
```

### Commandes de build

Depuis la racine du projet:

```bash
cmake -S . -B cmake-build-debug
cmake --build cmake-build-debug
```

L'executable se trouve ensuite dans le dossier de build, selon le generateur CMake utilise.

## Tutoriel complet: creer une mini-scene

Objectif: creer une scene minimale avec un objet visible, une collision et un script custom.

### 1. Creer un composant custom

`src/assets/scripts/Spinner.h`:

```cpp
#ifndef PROJECT_SPINNER_H
#define PROJECT_SPINNER_H

#include "Engine.h"

class Spinner : public Component {
    float speed;

public:
    explicit Spinner(float speed) : speed(speed) {}

    void Update(const sf::Time& elapsedTime) override
    {
        gameObject->transform.rotate(sf::degrees(speed * elapsedTime.asSeconds()));
    }

private:
    static inline bool s_registered =
        ComponentFactory::Register("Spinner",
            [](const pugi::xml_node& node) -> std::unique_ptr<Component>
            {
                return std::make_unique<Spinner>(
                    node.attribute("speed").as_float()
                );
            });
};

#endif
```

Si l'implementation grossit, deplacez les methodes dans `Spinner.cpp`.

### 2. Ajouter le composant au header d'activation

```cpp
// Assets.h
#include "scripts/Spinner.h"
```

### 3. Ajouter le composant au CMake utilisateur

```cmake
add_library(lib_assets
    Assets.h
    scripts/Spinner.h
)
```

Si vous creez `Spinner.cpp`, ajoutez-le aussi.

### 4. Creer un prefab

`resources/prefabs/spinning_object.xml`:

```xml
<GameObject label="SpinningObject" x="0" y="0" sx="2" sy="2">
    <Components>
        <Component name="Spinner" speed="90"/>
        <Component name="Renderer" src="resources/textures/object.png" sprite_w="32" sprite_h="32"/>
        <Component name="Collider" x="0" y="0" width="32" height="32"/>
    </Components>
</GameObject>
```

### 5. Creer une scene

`resources/scenes/demo.xml`:

```xml
<Scene>
    <GameObject label="Camera" x="0" y="0">
        <Components>
            <Component name="Camera"/>
        </Components>
    </GameObject>

    <Prefab src="resources/prefabs/spinning_object.xml" x="100" y="100"/>
</Scene>
```

### 6. Lancer cette scene

```cpp
#include "Engine.h"
#include "Assets.h"

int main()
{
    Game game("resources/scenes/demo.xml");
    game.run();
    return 0;
}
```

Au lancement, le composant `Spinner` est enregistre avant `main()`, la scene XML est chargee, le prefab est construit, puis l'objet tourne pendant `Update()`.

## Conventions et bonnes pratiques

- Gardez `engine` independant du projet utilisateur.
- Placez les composants propres au projet dans `assets/scripts`.
- Placez les ressources chargees a l'execution dans `assets/resources`.
- Ajoutez chaque nouveau composant auto-enregistre au header d'activation du projet.
- Ajoutez chaque `.cpp` au CMake correspondant.
- Utilisez les prefabs pour les objets reutilisables.
- Gardez les scenes XML lisibles: scene pour l'organisation, prefab pour le detail.
- Ne faites pas de recherche de composants dans les constructeurs; utilisez `Start()`.
- Utilisez `Update()` pour la logique frame-by-frame.
- Gardez les chemins de ressources relatifs a `resources/`.
- Verifiez que le nom XML du composant correspond exactement au nom enregistre dans `ComponentFactory`.
- Preferez des composants petits et specialises plutot qu'un seul gros script.
- Pour une UI concrete, heritez de `UIDocument`.
- Pour un comportement visible, combinez souvent `Renderer`, `Animator`, `Collider` et un composant custom.
