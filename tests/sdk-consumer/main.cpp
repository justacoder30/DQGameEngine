#include <DQEngine/DQEngine.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <box2d/box2d.h>

int main()
{
    // Force references into the engine, renderer, map loader, Box2D and mixer
    // (including the mixer's static codecs), not just an empty header compile.
    GameApp game(320, 240, "SDK consumer test");
    TiltedMapComponent map;
    const auto worldDefinition = b2DefaultWorldDef();
    const auto world = b2CreateWorld(&worldDefinition);
    b2DestroyWorld(world);
    if (!MIX_Init())
        return 1;
    MIX_Quit();
    return 0;
}
