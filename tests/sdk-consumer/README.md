# Native Visual Studio SDK check

Build and install Release and Debug with `DQGAMEENGINE_INSTALL_SDK=ON` first.
Copy the installed SDK to another location (preferably with spaces in its path)
to exercise relocation. From a Visual Studio developer command prompt, run:

```bat
msbuild tests\sdk-consumer\SDKConsumer.vcxproj /p:Configuration=Release /p:Platform=x64 /p:PlatformToolset=v145 /p:SDKRoot="C:\SDK test\DQGameEngineSDK"
msbuild tests\sdk-consumer\SDKConsumer.vcxproj /p:Configuration=Debug /p:Platform=x64 /p:PlatformToolset=v145 /p:SDKRoot="C:\SDK test\DQGameEngineSDK"
```

Use the toolset recorded in the SDK README in place of `v145` if different.
Run each `build/sdk-native/<Configuration>/SDKConsumer.exe` from its own folder.
It opens and closes a small OpenGL window, initializes Box2D and SDL_mixer,
and should exit with code 0. All engine includes, libraries, and SDL3.dll copying
come from the installed property sheet. No engine source targets are linked.
