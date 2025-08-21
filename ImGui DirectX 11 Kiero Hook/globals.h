#pragma once
#include "includes.h"

class ctg_object;
class Player;
class Camera;

inline bool showmenu = true;
inline bool alive = true;
inline ImDrawList* draw_list{};
inline std::vector<ctg_object*> PlayerList{};
inline std::vector<ctg_object*> VehicleList{};
inline int* GameResX{};
inline int* GameResY{};
inline Player* localplayer = nullptr;
inline ImColor NpcColor;
inline Camera* LocalCamera = nullptr;


using WorldToScreen_t = bool(__fastcall*)(__m128* out, __m128* worldLocation);
using GetHeadPos_t = __m128* (__fastcall*)(int64_t Player, __m128 * HeadPosOut, __m128 * a3);

inline WorldToScreen_t WorldToScreen = nullptr;
inline GetHeadPos_t GetHeadPos = nullptr;

enum BoxTypes
{
	Nornmal = 0,
	Cornered = 1,
	Box_3D = 2
};

namespace mem
{
	inline uintptr_t GameBase{};
}

namespace gl
{
	namespace ESP
	{
		inline bool ESP = true;
		inline bool ShowCivilians = true;
		inline bool ShowTeammates = true;
		inline bool ESP_Box = false;
		inline bool ESP_CorneredBox = true;
		inline bool ESP_Snaplines = false;
		inline bool ESP_Objects = false;
		inline bool ESP_health = true;

		inline float RenderDistance = 2000.f;
	}

	namespace Aimbot
	{
		inline bool Aimbot = true;
		inline bool AttackCivilians = false;
		inline bool ShowFov = false;
		inline float Fov = 500.f;
		inline float Smooth = 0.f;
	}

	namespace Exploits
	{
		inline bool InfiniteAmmo = true;
		inline bool NoSpread = true;
		inline bool NoRecoil = true;
		inline bool GodMode = true;
		inline bool SuperWeapon = false;
		inline bool InfiniteMoney = true;

		inline bool NoRagdoll = true;
		inline bool NoFire = true;
		inline bool NoFlinch = true;
		inline bool NoBump = true;
		inline bool NoExplosions = true;
		inline bool PerfectAim = true;
	}
	namespace World
	{
		inline bool Weapons = true;
		inline bool Vehicles = true;
		inline bool Items = true;
		inline bool Npcs = true;
		inline bool Humans = true;
	}
	namespace Misc
	{
		inline bool ShowMouse = true;
		inline bool ShowFPS = true;
		inline bool DrawCrosshair = true;
		inline float CrosshairSize = 5.f;
		inline float CrosshairThickness = 1.f;
	}

	namespace esp_Colors
	{
		inline ImColor BoxColor{ 255.f, 0.f , 255.f };
		inline ImColor Box3DColor{ 255.f, 255.f , 0.f };
		inline ImColor SnaplineColor{ 255.f, 255.f , 255.f };
		inline ImColor SkeletonColor{ 255.f, 255.f, 255.f };
		inline ImColor FovColor{ 255.f, 255.f, 255.f };
		inline ImColor CrosshairColor{ 255.f, 0.f, 0.f };
		inline ImColor Visible{ 0.f, 0.f, 255.f };
		inline ImColor NotVisible{ 255.f, 0.f, 0.f };
	}
}

namespace Colors
{
	inline ImColor White(255.f, 255.f, 255.f);
	inline ImColor Black(0.f, 0.f, 0.f);
	inline ImColor Red(255.f, 0.f, 0.f);
	inline ImColor DarkRed(0.700f, 0.f, 0.f);
	inline ImColor Green(0.f, 255.f, 0.f);
	inline ImColor DarkGreen(0.f, 0.600f, 0.f);
	inline ImColor Blue(0.f, 0.f, 255.f);
	inline ImColor DarkBlue(0.f, 0.f, 0.700f);
	inline ImColor Pink(255.f, 0.f, 255.f);
	inline ImColor Cian(0.f, 255.f, 255.f);
	inline ImColor Yellow(255.f, 255.f, 0.f);
	inline ImColor Grey(0.600f, 0.600f, 0.600f);
	inline ImColor Purple(0.200f, 0.f, 0.600f);
}

namespace of
{
	inline uint64_t InfiniteAmmo = 0x564B94;
	inline uint64_t NoSpread = 0x54E10D;
	inline uint64_t NoRecoil = 0xED871; //5
	inline uint64_t LocalPlayer = 0x018E3B10;
	inline uint64_t WorldToScreen = 0x174C90;
	inline uint64_t WayPoint = 0x1592198; // + 0x0
	inline uint64_t ScreenRes = 0x11B9FF0;
	inline uint64_t EntityList = 0x52BABC;
	inline uint64_t World = 0x294C9B8;
	inline uint64_t GetHeadPos = 0x4CF600;
	inline uint64_t Camera = 0x1405B20;
	inline uint64_t ViewMatrix = 0x159D2C0;
}


