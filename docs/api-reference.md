# API reference

This is a practical index of the public engine API, not a promise of long-term
binary compatibility. It documents the source currently shipped with this
repository. See [getting started](getting-started.md) for a complete example
and [the user guide](user-guide.md) for behavior and recipes.

Include `<DQEngine/DQEngine.h>` for the main gameplay API. All names below are
inside `dqengine`. Specific headers are relative to `DQEngine/`; the SDK property
sheet supplies both include roots used by the engine headers.

## Ownership — `Core/Memory.h`

- `Unique<T>(args...)`: forwards arguments to `std::make_unique<T>`.
- `Shared<T>(args...)`: forwards arguments to `std::make_shared<T>`.
- `UniquePtr<T, Deleter>`: `std::unique_ptr`, with the standard deleter by default.
- `SharedPtr<T>` / `WeakPtr<T>`: standard shared/weak pointer aliases.

Prefer `auto p = Unique<T>()` for locals and explicit pointer types for fields
and return types. `Unique<T[]>(count)` also supports owned arrays.

## Scene tree — `Components/Component.h`

- `Add(UniquePtr<T>) -> T*`: transfer child ownership; return a borrowed pointer.
- `QueueAdd(UniquePtr<T>) -> T*`: transfer ownership, defer attachment.
- `Add(Component*)` / `QueueAdd(Component*)`: legacy owning overloads; never pass
  stack objects or pointers owned elsewhere.
- `GetComponent<T>() -> T*`: exact-type, direct-child lookup, or `nullptr`.
- `GetParent() -> Component*`: borrowed parent, or `nullptr`.
- `GetChildren()`: const reference to the vector of unique child owners.
- `RemoveFromParent()` / `IsRemovalPending()`: request/inspect deferred removal.
- `ClearChildren()`: immediate recursive cleanup; use outside traversal.
- `Load()`, `Update(dt)`, `Draw()`: tree traversal entry points.
- `OnAttach`, `OnLoad`, `OnUpdate`, `OnDraw`, `OnDetach`: virtual extension hooks.

Children cannot have multiple owning parents. Destroying a tree invalidates all
borrowed references into it. Do not create a second owner from `.get()`.

## Application — `Components/GameApp.h`

- `GameApp(int width, int height, const char* title)`: creates the rendering context.
- `Run()`: loads the tree and runs until the SDL quit event.
- `GetBoardphase() -> Boardphase*`: access shared collision storage.

The public name is `Boardphase` despite the header spelling `Core/Broadphase.h`.
Use one application at a time. Scene children are cleared before renderer shutdown.

## Transform and visuals

`Components/PositionComponent.h`:

- `position`, `size`, `anchor`, `angle`, `flip`, `layer`, `m_Visible`: visual fields.
- `SetPosition(x,y)`, `SetAnchor(x,y)`, `HorizontalFlip()`, `VerticalFlip()`.
- `GetWorldPosition()`: combines local position with a positional parent's drawing origin.

`Components/SpriteComponent.h`:

- `SpriteComponent(path)`: load a cached image.
- `SpriteComponent()`: use a white texture; set a nonzero display size.
- `SetColor(Color)`: set RGBA tint.

`Components/CameraComponent.h`:

- `CameraComponent(width,height)`: logical viewport dimensions.
- `Follow(PositionComponent*, smooth = 5.f)`, `SetPosition(Vector)`.
- `SetOrigin(Vector)`, `SetZoom(float)`, `SetBounds(Rect)`.
- `Shake(strength,duration)`, `GetTarget()`, `GetViewBounds()`, `CanSee(...)`.
- `GetViewProjection()` / `GetBackdropMatrix()`: rendering matrices.

`Renderer/Renderer2D.h`:

- `Submit(RenderCommand)` / `SubmitRect(Rect, Color, RenderLayer)`: enqueue drawing.
- `GetCamera()` / `SetCamera(CameraComponent*)`: borrowed active camera.
- `GetViewportSize()` / `GetWindowSize()`: logical view and window dimensions.
- `RenderLayer::Background`, `World`, `UI`: frame rendering order.

The application manages renderer initialization, batching, presentation, and
shutdown. Advanced direct rendering calls require an active context and correct
batch ordering; prefer submission from components.

## Animation

`Components/Animation.h`:

- `Animation(path, count, secondsPerFrame = 0.08f, loop = true)`.
- `Animation(path, count, row, frameSize, secondsPerFrame = 0.08f, loop = true)`.
- `Animation(vector<string> paths, secondsPerFrame = 0.08f, loop = true)`.
- `CurrentFrame`, `FrameCount`, `FrameWidth`, `FrameHeight`, `FrameSpeed`, `row`.
- `GetCurrentTexture() -> Texture*`: borrowed texture for the current frame.

`Components/Animation2DComponent.h`:

- `AddAnimation(int id, const Animation&)`, `Play(int id)`, `IsPlaying(int id)`.
- `SetSize(w,h)`, `SetColor(Color)`, `GetSrcRect()`.
- `animationClip`: current clip, exposing `IsDone()` and `ResetFrame()`.
- `Stop()`: moves to the last frame; see playback limitations in the user guide.

Supply valid frame counts, non-empty frame lists, and registered animation IDs.
Call `Play` before updates; the default animation is not ready for playback.

## Collision

`Components/ShapeComponent.h` / `Components/RectangleComponent.h`:

- `RectangleComponent(position,size)`: explicit local rectangle.
- `RectangleComponent()`: takes its positional parent's size on attachment.
- `GetWorldBounds()` / `GetBounds()`: world/local rectangles.
- `layer`, `mask`, `active`, `bodyType`: collision participation settings.
- `Layer`: `None`, `Player`, `Enemy`, `Ground`, `Attack`, `Sensor`, `Item`.
- `ToMask(Layer)` / `HasLayer(mask,Layer)`: mask helpers.
- `BodyType`: `Static`, `Dynamic`, `Kinematic`, `NoneType`.

Masks are bilateral. `ShapeType` includes other names, but the provided narrow
phase/controller implementation works with rectangles. Rotation is visual;
colliders remain axis-aligned. Do not assume full rigid-body behavior from the
body-type enum or from linked Box2D dependencies.

`Core/CollisionCallbacks.h`:

- `OnCollisionStart(self, otherShape, otherParent)`.
- `OnCollision(self, otherShape, otherParent)`.
- `OnCollisionEnd(self, otherShape, otherParent)`.

Implement these on a shape's immediate parent. Arguments are borrowed pointers.

`Components/CharacterController.h`:

- `hitbox`: borrowed rectangle shape used for movement queries.
- `velocity`: pixels/second, applied during update.
- `gravity`, `jumpForce`: values for your gameplay logic; not automatic integration.
- `Move(Vector)`, `MoveX(dx)`, `MoveY(dy)`: immediate displacement operations.

Use a positional parent and rectangle shapes. The exposed static `StepSize`
currently does not control the subdivision used in the implementation; do not
rely on it for tuning. Sensors should use `NoneType` for controller filtering.

`Core/Broadphase.h`, `Core/Board.h`, `Core/SpatialGrid.h`:

- `Boardphase::SetBoard(UniquePtr<Board>)`: replace the owned collision board.
- `GetColliders()`: borrowed collider-list view; do not retain removed objects.
- `GetBoard() -> Board*`: borrowed active board.
- `SpatialGrid::Init(cellSize)`: configure a positive cell size.
- `Board::Query(shape, outputVector)`: refill reusable neighbor storage.
- `Board::Query(shape)`: return an independent neighbor vector.

Queries return candidates, not guaranteed overlaps. The default `BaseBoard`
returns its collider list; the spatial grid filters by occupied cells.

## States, time, and input

`Core/IState.h` / `Components/StateMachineComponent.h`:

- Override `IState::Enter()`, `Update(float)`, `Exit()`.
- `CreateState<T>(args...) -> T*`: create an owned state, return an observer.
- `ChangeState(IState*)`: borrow a state and perform Exit/Enter transitions.
- `Core/StateMachine.h` provides the underlying non-owning machine.

`Core/Input.h`:

- `Key[SDL_SCANCODE_...]`: current held state.
- `PreKey[SDL_SCANCODE_...]`: previous held state.
- `Input.Quit()`: window-close status; `GameApp` polls input automatically.

`Core/Time.h`:

- `Time::timeScale`: update-time multiplier.
- `Time::Freeze(seconds)`: temporarily set scale to zero, then restore one.

## Tile maps, textures, and utilities

`Components/TiltedMapComponent.h`:

- `TiltedMapComponent(path)`: load a TMX and build its render tiles.
- `GetWidth()` / `GetHeight()`: map dimensions in world units.
- `GetObjectGroup(name) -> vector<Rect>`: retrieve object rectangles.

`Core/TextureManager.h` (include explicitly when used directly):

- `Load(path = "default") -> SharedPtr<Texture>`: cache by path string.
- `Clear()`: release cache ownership; consumers may keep textures alive.

`Renderer/Texture.h`: constructors for a file, a white texture, or pixel data;
`GetID`, `GetWidth`, `GetHeight`, `Bind`, and `Unbind`. Texture objects cannot be
copied. Their lifetime must remain within the lifetime of the OpenGL context.

`Renderer/Shader.h`: low-level shader object with uniform setters. The current
constructor embeds the renderer's shader source rather than loading its path
arguments; editing external shader files alone will not change the shader.

`Core/Vector.h`, `Core/Rect.h`, `Core/Color.h`, `Core/Flip.h`, `Core/Utils.h`:

- `Vector(x,y)`, `Vector::Zero()`, `Vector::All(value)`: 2D positions and sizes.
- `Rect(x,y,w,h)` or `Rect(position,size)`: rectangles and intersection helpers.
- `Color(r,g,b,a = 1.f)`: floating-point RGBA tint.
- `Flip`: `None`, `Horizontal`, `Vertical`, `Diagonal`.
- `RandomFloat(min,max)`: random float helper.

## Current scope

The gameplay API provides a component tree, sprite rendering, keyboard input,
animation, cameras, rectangle collision/controller logic, and basic TMX drawing.
Dependencies such as SDL_mixer and Box2D do not imply a complete engine audio API
or general physics system. The SDK currently exposes no scene manager, entity
editor, text/font component, or packaged CMake config. Build such game-specific
systems above the existing components, or extend the engine explicitly.
