# Saints-Row-The-Third-Remastered-Internal-Cheat
An internal cheat for the game Saints Row The Third Remastered.

I reversed everything from scratch by myself, since there’s no public information available (at least that I could find) about the engine this game uses. It took me around a week and a half to get everything done. I made this project mainly to practice, learn, and improve my reverse engineering skills. I coded everything myself (except for the DirectX11 kiero hook and MinHook). The cheat doesn’t include a pattern scanner, so if the game gets updated it’ll probably stop working, but just updating the offsets should be enough.
The most complicated part was finding the world and the object array, the WorldToScreen function, and the function to get the head position. I spent over 15 hours looking for the NPCs’ bones, but the only thing I found was a function that returns the head position. It’s very likely that there isn’t an array of bones with their positions stored as a Vec3, and instead they’re either stored as a matrix or calculated through transform and matrix functions.

CREDITS:
Kiero hook: https://github.com/rdbo/ImGui-DirectX-11-Kiero-Hook
MinHook: https://github.com/TsudaKageyu/minhook

HOW TO USE:
Just inject the Debug or Release dll into the game with any injector, i use process hacker. The game must be windowed or borderless.

FEATURES:

-ESP
  -Box
  -Cornered Box
  -Snaplines
  -Objects
  -HealthBar
  -Show Civilians
  -Show Teammates
  -WORLD ESP:
    -Players
    -NPCs
    -Vehicles
    -Weapons
    -Items
-Aimbot
  -Aim at Civilians
  -FOV
  -Smooth
-Exploits
  -Infinite ammo
  -No Spread
  -No Recoil
  -Godmode
  -Infinite Money
  -Superweapon
  -Inmune to fire
  -Inmune to explosions
  -No ragdoll
  -Perfect aim
  -No flinch
  -No bump
-Misc
  -Show FPS
  -Show crosshair
  -Show mouse
