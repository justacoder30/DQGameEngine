# DQGameEngine user guide

Start with [getting started](getting-started.md) for installation and a runnable
game. The snippets here are recipes to place in your own component classes;
they are not separate complete programs. Engine names are qualified unless
a snippet explicitly uses `using namespace dqengine`.

## Components and the frame loop

Your game derives from `dqengine::GameApp`, which itself is a `Component`.
Components own children, forming a tree. A player can be an
`Animation2DComponent` with rectangle colliders, a `CharacterController`,
and a `StateMachineComponent` as its children.

Override lifecycle hooks to implement behavior:

- `OnAttach()`: called when added to a parent. Engine shapes register with the
  collision system here; cameras become active here.
- `OnLoad()`: one-time initialization when the component is loaded.
- `OnUpdate(float dt)`: per-frame behavior. `dt` is scaled time in seconds.
- `OnDraw()`: submits rendering work.
- `OnDetach()`: cleanup before an attached child is removed.

The `Load`, `Update`, and `Draw` methods manage traversal; normally override
the `On...` hooks. Parent hooks run before children, and children are visited in
insertion order. When deriving from an engine component, call its hook when
needed: `SpriteComponent::OnUpdate(dt)` refreshes drawing bounds,
`Animation2DComponent::OnUpdate(dt)` also advances animation, and shape attach/
detach hooks register/unregister collision references.

Each `GameApp` frame polls input, updates time, performs collision detection,
updates the component tree, and draws. Collision callbacks therefore happen
before that frame's component movement. The renderer draws Background, World,
then UI. Use one active `GameApp`; renderer, input, time, and collision storage
include global/static state.

The loop clamps elapsed time to at most `1/60` second before scaling it. It is
not a fixed-step accumulator and does not catch up on long frames. Rendering
continues when time is frozen.

## Ownership, spawning, and removal

```cpp
auto child = dqengine::Unique<dqengine::Component>();
auto* observer = Add(std::move(child));
// child is now empty; this component owns the new child.
```

Use `Unique<T>(args...)` to create a uniquely owned object, and `UniquePtr<T>`
as a member/parameter/return type. `Shared<T>(args...)` creates shared ownership;
`SharedPtr<T>` stores it and `WeakPtr<T>` observes it without extending lifetime.
These wrap the corresponding standard C++ smart pointers and factories.

`Add` and `QueueAdd` take ownership and return borrowed pointers. Do not delete
those pointers, wrap them in another owning pointer, or retain them after the
object is destroyed. Parent pointers, component lookup results, camera targets,
and collider lists are also borrowed. Use shared ownership only where several
owners need the same object, such as cached textures.

For a projectile from a player component, after defining your `Projectile` class:

```cpp
if (auto* scene = GetParent()) {
    scene->QueueAdd(dqengine::Unique<Projectile>());
}
```

`QueueAdd` takes ownership immediately. It attaches/loads queued children after
the parent's load/update traversal; a child spawned during an update starts
updating on the next frame. `Add` automatically queues if the parent is currently
traversing children. Adding to an idle parent is immediate. Additions during
drawing stay queued until the next load/update flush. A queued child can be
drawn after attachment before receiving its first update; initialize any required
draw bounds in `OnAttach` for projectiles that must appear immediately.

From inside a child, call `RemoveFromParent()` to mark it for removal. The
parent deletes it during update traversal, so the pointer is not a permanent
handle. `IsRemovalPending()` lets you inspect that flag. `ClearChildren()`
immediately destroys attached and queued children; call it outside traversal,
not from a child callback that would invalidate the parent's iteration.

`GetComponent<T>()` finds an exact concrete type among direct, attached children.
It is not recursive and does not search base classes. If several children have
that type, it selects the most recently indexed one. For multiple enemies,
iterate `GetChildren()` and use `child.get()` / `dynamic_cast` as appropriate.
Queued children are not yet included in this lookup.

## Sprites, coordinates, and layers

Create sprites after the application's OpenGL context exists, typically in
`OnLoad`. Texture paths are relative to the working directory.

```cpp
auto* hero = Add(dqengine::Unique<dqengine::SpriteComponent>("resource/hero.png"));
hero->position = dqengine::Vector(200.f, 120.f);
hero->size = dqengine::Vector(64.f, 64.f);
hero->anchor = dqengine::Vector(0.5f, 0.5f);
hero->SetColor(dqengine::Color(1.f, 1.f, 1.f, 0.8f));
```

X increases rightward and Y downward. `size` controls display dimensions;
set it explicitly because a sprite constructor currently defaults to one third
of the loaded image dimensions. Anchor `(0,0)` places the top-left at `position`;
`(0.5,0.5)` centers it there. `m_Visible = false` hides rendering, not updating.

For a world child of a `PositionComponent`, position is relative to the parent's
computed top-left drawing position. This is not a general matrix transform
hierarchy: do not assume inherited rotation or scale. UI drawing uses the
component's own position directly.

Use `HorizontalFlip()` / `VerticalFlip()` to toggle orientation. These helpers
also adjust the corresponding anchor. To assign a fixed orientation, set `flip`
to `dqengine::Flip::None`, `Horizontal`, `Vertical`, or `Diagonal`.

`PositionComponent::layer` selects `RenderLayer::Background`, `World`, or `UI`.
World sprites use the active camera and are culled against its bounds. UI and
Background use the backdrop projection. Later submissions within a layer draw
over earlier ones; there is no automatic Y/depth sorting.

For custom flat graphics in an `OnDraw` hook:

```cpp
dqengine::Renderer2D::SubmitRect(
    dqengine::Rect(10.f, 10.f, 100.f, 12.f),
    dqengine::Color(1.f, 0.f, 0.f), dqengine::RenderLayer::UI);
```

Prefer queued submission for normal component drawing; the application owns
batch start/end and frame presentation.

## Input and time

The application updates `Key` and `PreKey` before calling your update hook.
Use SDL scancodes:

```cpp
const bool held = dqengine::Key[SDL_SCANCODE_SPACE];
const bool pressed = held && !dqengine::PreKey[SDL_SCANCODE_SPACE];
const bool released = !held && dqengine::PreKey[SDL_SCANCODE_SPACE];
```

Multiply a speed in pixels/second by `dt` to obtain displacement. Do not call
`Input.Update()` again in a normal component: that would change the previous
keyboard state used for edge detection. Closing the window ends `Run()`.

Set `Time::timeScale` for slow motion, or call `Time::Freeze(0.05f)` for hit pause.
Freeze currently restores `timeScale` to `1` when it expires, even if a different
scale was set previously. Avoid dividing by `dt`; it can be zero while frozen.

## Camera

Inside `OnLoad`, after creating a player:

```cpp
auto* camera = Add(dqengine::Unique<dqengine::CameraComponent>(800.f, 450.f));
camera->SetOrigin(dqengine::Vector(0.5f, 0.5f));
camera->Follow(hero, 5.f);
camera->SetBounds(dqengine::Rect(0.f, 0.f, 2400.f, 900.f));
camera->SetZoom(1.25f);
```

The most recently attached camera becomes active. Its width/height are logical
view dimensions; the renderer has a default camera if you do not attach one.
Add a following camera after its target so its update sees the target's movement.
`Follow` borrows the target. Use `Follow(nullptr)` to stop following, and
`Shake(strength, duration)` for camera shake. Removing the active camera restores
the default camera.

## Animation

Use `Animation2DComponent` for an animated object. Inside its constructor, after
the game exists:

```cpp
AddAnimation(0, dqengine::Animation("resource/idle.png", 4, 0.15f, true));
AddAnimation(1, dqengine::Animation("resource/attack.png", 6, 0.08f, false));
SetSize(96.f, 96.f);
Play(0);
```

The first overload divides the full image width into `frame_count` horizontal
frames. Frame speed means **seconds per frame**. For a particular sheet row,
use `Animation(path, count, row, Vector(frameWidth, frameHeight), seconds, loop)`;
rows start at zero. Another overload accepts a non-empty
`std::vector<std::string>` of individual frame paths.

Add valid animations and call `Play` before the component updates. `Play(id)`
switches animations, but does not restart the same current ID. To restart the
current clip explicitly, use `animationClip.ResetFrame()`. In a derived update
hook, call `Animation2DComponent::OnUpdate(dt)` to advance animation and bounds.
Use `animationClip.IsDone()` to detect completion of a non-looping animation.

Current limitations: playback advances at most one frame per update and discards
leftover frame time. `Stop()` moves to the last frame; it is not a reliable pause
API for looping clips. Use state changes and non-looping animations for actions.

## Collision and character movement

The implemented collision path uses axis-aligned `RectangleComponent` shapes.
Attach shapes to the object that should receive callbacks. That immediate
parent must inherit `CollisionCallbacks` and override its methods.

```cpp
class Actor : public dqengine::SpriteComponent, public dqengine::CollisionCallbacks {
public:
    Actor() {
        size = dqengine::Vector(32.f, 48.f);
        auto* box = Add(dqengine::Unique<dqengine::RectangleComponent>(
            dqengine::Vector(0.f, 0.f), size));
        box->layer = dqengine::Layer::Player;
        box->mask = dqengine::ToMask(dqengine::Layer::Ground);
        box->bodyType = dqengine::BodyType::Dynamic;
        controller = Add(dqengine::Unique<dqengine::CharacterController>());
        controller->hitbox = box;
        controller->gravity = 900.f;
    }

    void OnCollisionStart(dqengine::ShapeComponent* self,
                          dqengine::ShapeComponent* otherShape,
                          dqengine::Component* other) override {
        // self is our shape; other is the other shape's immediate parent.
    }

protected:
    void OnUpdate(float dt) override {
        controller->velocity.y += controller->gravity * dt;
        dqengine::SpriteComponent::OnUpdate(dt);
    }

private:
    dqengine::CharacterController* controller = nullptr; // Borrowed child.
};
```

A floor's shape needs `layer = Layer::Ground`,
`mask = ToMask(Layer::Player)`, and `bodyType = BodyType::Static`.
Both masks must allow the other shape's layer. The default mask is zero,
which allows no collision pairs. Combine masks using bitwise OR.

Collision `Layer` and visual `RenderLayer` are separate concepts. `active = false`
disables a shape's collision participation. `OnCollisionStart` runs on the first
overlap, `OnCollision` on continuing overlaps, and `OnCollisionEnd` when a
previous pair no longer overlaps. Removing a collider purges its tracked pairs;
do not depend on receiving an end callback for a destroyed object.

The controller must be a child of a `PositionComponent`, and `hitbox` must refer
to its live rectangle collider. Set `velocity` in pixels/second; the controller
applies movement in its update. Gravity and jumping are game logic: assigning
`gravity` alone does not integrate it. Apply `velocity.y += gravity * dt` and
set a negative Y velocity for a jump, using your own grounded check.

`BodyType::NoneType` keeps attack/sensor shapes out of controller obstacle
resolution. The controller does not currently use `isTrigger`/`hasPhysics`
as a complete physics policy; setting `isTrigger` alone is insufficient.
Collision detection itself does not automatically resolve movement.

For larger scenes, replace the default broad phase once during scene setup:

```cpp
auto grid = dqengine::Unique<dqengine::SpatialGrid>();
grid->Init(128.f); // Positive cell size in world units; tune for your scene.
dqengine::GameApp::GetBoardphase()->SetBoard(std::move(grid));
```

The next collision pass rebuilds the board from registered shapes. Avoid changing
the board or clearing the scene from inside a collision callback. Use deferred
spawns/removal where possible. The grid is rebuilt once per collision pass;
it is not continuously updated after every controller movement.

## State machines

Derive from `dqengine::IState` and override `Enter`, `Update(float dt)`, and `Exit`.
Create states through the owning `StateMachineComponent`:

```cpp
class IdleState : public dqengine::IState {
public:
    void Enter() override { /* Select idle animation. */ }
    void Update(float dt) override { /* Decide when to transition. */ }
};

// Inside the actor's setup:
auto* machine = Add(dqengine::Unique<dqengine::StateMachineComponent>());
auto* idle = machine->CreateState<IdleState>();
machine->ChangeState(idle);
```

The machine owns states made with `CreateState`; returned pointers are borrowed.
Constructor arguments can pass an actor pointer to a state, as in
`CreateState<MyState>(this)`. `ChangeState` calls the old state's `Exit`, then the
new state's `Enter`. Passing `nullptr` stops state updates. Changing to the same
state still calls Exit/Enter; guard transitions if you do not want a restart.
The lower-level `StateMachine` only borrows its current state.

## Tile maps and resources

The class name is currently `TiltedMapComponent` (including that spelling).
Construct it with a Tiled `.tmx` file. Use an orthogonal map with square tiles
and tileset images for the current rendering implementation. It builds render
tiles from tile layers; collision objects are not created automatically.

```cpp
auto* map = Add(dqengine::Unique<dqengine::TiltedMapComponent>("resource/map.tmx"));
for (const auto& rect : map->GetObjectGroup("Collision")) {
    auto* ground = map->Add(dqengine::Unique<dqengine::RectangleComponent>(
        dqengine::Vector(rect.x, rect.y), dqengine::Vector(rect.w, rect.h)));
    ground->layer = dqengine::Layer::Ground;
    ground->mask = dqengine::ToMask(dqengine::Layer::Player);
    ground->bodyType = dqengine::BodyType::Static;
}
```

Use rectangle object groups for collision and spawn regions. Keep the map at
the world origin: tile drawing currently uses the map's prebuilt world
coordinates and does not apply the component's position as a general transform.
Distribute referenced tileset images and any external tileset files with the TMX.

Sprites and animations load through `TextureManager`, sharing cached textures
by path string. `TextureManager::Clear()` drops cache ownership; live consumers
keep their textures. Include `<DQEngine/Core/TextureManager.h>` when using the
manager directly. Release your own texture owners before the application and
its OpenGL context are destroyed. Avoid static shared texture owners.

## Learn from Sandbox

In a source checkout, these files show larger examples:

- `Sandbox/GameTest.cpp`: scene construction, camera, map, and spatial grid.
- `Sandbox/Player.cpp` and `PlayerState/`: character animation and state transitions.
- `Sandbox/Map.h`: rectangle colliders from a TMX object layer.
- `Sandbox/Healthbar.cpp`: UI rendering.
- `Sandbox/Head.cpp`: a projectile component with its own movement, hit logic,
  lifetime, and weak reference to the boss's volley.
- `Sandbox/DarkMage.cpp` and `DarkMageState/`: boss behavior, spawning, and healing.

Sandbox gameplay classes are examples, not part of the engine SDK API.
