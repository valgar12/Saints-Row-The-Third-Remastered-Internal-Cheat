#include "includes.h"
#include "kiero/minhook/include/MinHook.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

bool FirstInject = true;
bool debug = true;
FILE* Dummy{};

void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

	SetColorsFlags();
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Tahoma.ttf", 18.0f);

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);

	draw_list = ImGui::GetBackgroundDrawList();
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

	switch (uMsg) {
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_INSERT:
			showmenu = !showmenu;
			break;
		}
	}

	if (showmenu && !FirstInject) return true;
	if (!showmenu && FirstInject) FirstInject = false;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

bool init = false;
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)& pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			init = true;
		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (showmenu)
		DrawMenu();

	GameLoop();

	if (gl::Misc::ShowFPS)
	{
		std::string fps = std::to_string(static_cast<int>(ImGui::GetIO().Framerate));
		ImGui::GetBackgroundDrawList()->AddText(ImVec2(*GameResX - ImGui::CalcTextSize(fps.c_str()).x, 0), ImColor(0.f, 255.f, 0.f), fps.c_str());
		auto gameinstance = (GameInstance*)(mem::GameBase + of::World);

		if (gameinstance && gameinstance->World && gameinstance->World->ArrayNum)
		{
			std::string CurrentObjects = "CurrentObjects: " + std::to_string(gameinstance->World->ArrayNum);
			std::string CurrentSize = "CurrentArraySize: " + std::to_string(gameinstance->World->ArryCapacity);

			ImGui::GetBackgroundDrawList()->AddText(ImVec2(*GameResX - ImGui::CalcTextSize(CurrentObjects.c_str()).x, 20), ImColor(255.f, 255.f, 255.f), CurrentObjects.c_str());
			ImGui::GetBackgroundDrawList()->AddText(ImVec2(*GameResX - ImGui::CalcTextSize(CurrentSize.c_str()).x, 40), ImColor(255.f, 255.f, 255.f), CurrentSize.c_str());
		}

		
	}
	if (gl::Misc::ShowMouse && showmenu)
	{
		auto MousePos = ImGui::GetMousePos();
		ImGui::GetForegroundDrawList()->AddCircleFilled(MousePos, 4.f, ImColor{ 1.f, 1.f, 1.f });
	}
	if (gl::Misc::DrawCrosshair)
		ESP::DrawCrosshair(gl::Misc::CrosshairSize, gl::Misc::CrosshairThickness, gl::esp_Colors::CrosshairColor);

	if (!alive)
	{
		kiero::shutdown();
		pDevice->Release();
		pContext->Release();
		pSwapChain->Release();
		oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)(oWndProc));
	}

	ImGui::Render();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oPresent(pSwapChain, SyncInterval, Flags);
}

DWORD MainThread(HMODULE hModule)
{
	if (debug)
	{
		AllocConsole();
		freopen_s(&Dummy, "CONOUT$", "w", stdout);
	}

	mem::GameBase = (uintptr_t)GetModuleHandleA(NULL);
	printf("ImageBase: 0x%p\n", mem::GameBase);

	printf("ViewMatrix: %p\n", (void*)(mem::GameBase + of::ViewMatrix));

	WorldToScreen = (WorldToScreen_t)(mem::GameBase + of::WorldToScreen);
	GetHeadPos = (GetHeadPos_t)(mem::GameBase + of::GetHeadPos);

	printf("World To Screen located at 0x%p\n", WorldToScreen);
	printf("GetHeadPos located at 0x%p\n", GetHeadPos);

	GameResX = (int*)(mem::GameBase + of::ScreenRes);
	LocalCamera = (Camera*)(mem::GameBase + of::Camera);
	printf("LocalCamera located at 0x%p\n", LocalCamera);

	if (GameResX != nullptr)
	{
		GameResY = (int*)(mem::GameBase + of::ScreenRes + 0x4);
		printf("Game resolution found: X: %d - Y: %d at 0x%p\n", *GameResX, *GameResY, GameResX);
		ScreenCenter.x = (float)*GameResX;
		ScreenCenter.y = (float)*GameResY;
	}

	std::thread(PlayerCache::PlayerCache).detach();

	bool init_hook = false;
	if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
	{
		kiero::bind(8, (void**)&oPresent, hkPresent);
		init_hook = true;
	}

	while (alive)
	{
		if (GetAsyncKeyState(VK_END))
		{
			alive = false;
			Sleep(100);
			if (debug)
			{
				FreeConsole();
				if (Dummy)
					fclose(Dummy);
			}
			FreeLibraryAndExitThread(hModule, 0);
		}
	}

	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, 0);
		break;
	case DLL_PROCESS_DETACH:
		
		break;
	}
	return TRUE;
}