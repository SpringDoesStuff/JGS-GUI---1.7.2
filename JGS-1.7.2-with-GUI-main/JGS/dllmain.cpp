#include <Windows.h>
#include <vector>

#include "minhook/MinHook.h"

#pragma comment(lib, "minhook/minhook.lib")

#pragma comment(lib, "ChilkatRelDll_x64.lib")

#include "SDK.hpp"

using namespace SDK;

#include "Globals.h"
#include "Util.h"
#include "Discord.h"

#include "Offsets.h"

#include "Containers.h"
#include "Classes.h"

#include "Replication.h"
#include "Inventory.h"
#include "Abilities.h"
#include "Functions.h"
#include "MiscHooks.h"
#include "NetHooks.h"

#include "Hooks.h"

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <tchar.h>
#include <thread>

#pragma comment(lib, "d3d9.lib")

// Data
static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int InitImGui()
{
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"1.7.2 Control Panel", NULL };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"1.7.2 Control Panel", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);

    // Initialize Direct3
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    
      void SetupImGuiStyle();
      {
          // Unreal style by dev0-1 from ImThemes
          ImGuiStyle& style = ImGui::GetStyle();
          ImGui::GetIO().Fonts->AddFontFromFileTTF("JGSResources/Resources/BurbankBigCondensed-Bold.otf", 17);
          ImGui::GetBackgroundDrawList();
          style.Alpha = 1.0f;
          style.DisabledAlpha = 0.6000000238418579f;
          style.WindowPadding = ImVec2(8.0f, 8.0f);
          style.WindowRounding = 0.5f;
          style.WindowBorderSize = 1.0f;
          style.WindowMinSize = ImVec2(32.0f, 32.0f);
          style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
          style.WindowMenuButtonPosition = ImGuiDir_Left;
          style.ChildRounding = 0.0f;
          style.ChildBorderSize = 1.0f;
          style.PopupRounding = 0.0f;
          style.PopupBorderSize = 1.0f;
          style.FramePadding = ImVec2(4.0f, 3.0f);
          style.FrameRounding = 0.0f;
          style.FrameBorderSize = 0.0f;
          style.ItemSpacing = ImVec2(8.0f, 4.0f);
          style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
          style.CellPadding = ImVec2(4.0f, 2.0f);
          style.IndentSpacing = 21.0f;
          style.ColumnsMinSpacing = 6.0f;
          style.ScrollbarSize = 14.0f;
          style.ScrollbarRounding = 9.0f;
          style.GrabMinSize = 10.0f;
          style.GrabRounding = 0.0f;
          style.TabRounding = 4.0f;
          style.TabBorderSize = 0.0f;
          style.TabMinWidthForCloseButton = 0.0f;
          style.ColorButtonPosition = ImGuiDir_Right;
          style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
          style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

          style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
          style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.4980392158031464f, 0.4980392158031464f, 0.4980392158031464f, 1.0f);
          style.Colors[ImGuiCol_WindowBg] = ImVec4(0.05882352963089943f, 0.05882352963089943f, 0.05882352963089943f, 0.9399999976158142f);
          style.Colors[ImGuiCol_ChildBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
          style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 0.9399999976158142f);
          style.Colors[ImGuiCol_Border] = ImVec4(0.4274509847164154f, 0.4274509847164154f, 0.4980392158031464f, 0.5f);
          style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
          style.Colors[ImGuiCol_FrameBg] = ImVec4(0.2000000029802322f, 0.2078431397676468f, 0.2196078449487686f, 0.5400000214576721f);
          style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.4000000059604645f, 0.4000000059604645f, 0.4000000059604645f, 0.4000000059604645f);
          style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.1764705926179886f, 0.1764705926179886f, 0.1764705926179886f, 0.6700000166893005f);
          style.Colors[ImGuiCol_TitleBg] = ImVec4(0.03921568766236305f, 0.03921568766236305f, 0.03921568766236305f, 1.0f);
          style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.2862745225429535f, 0.2862745225429535f, 0.2862745225429535f, 1.0f);
          style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0f, 0.0f, 0.0f, 0.5099999904632568f);
          style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.1372549086809158f, 0.1372549086809158f, 0.1372549086809158f, 1.0f);
          style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.01960784383118153f, 0.01960784383118153f, 0.01960784383118153f, 0.5299999713897705f);
          style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3098039329051971f, 1.0f);
          style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.407843142747879f, 0.407843142747879f, 0.407843142747879f, 1.0f);
          style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.5098039507865906f, 0.5098039507865906f, 0.5098039507865906f, 1.0f);
          style.Colors[ImGuiCol_CheckMark] = ImVec4(0.9372549057006836f, 0.9372549057006836f, 0.9372549057006836f, 1.0f);
          style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.5098039507865906f, 0.5098039507865906f, 0.5098039507865906f, 1.0f);
          style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.8588235378265381f, 0.8588235378265381f, 0.8588235378265381f, 1.0f);
          style.Colors[ImGuiCol_Button] = ImVec4(0.4392156898975372f, 0.4392156898975372f, 0.4392156898975372f, 0.4000000059604645f);
          style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.4588235318660736f, 0.4666666686534882f, 0.47843137383461f, 1.0f);
          style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.4196078479290009f, 0.4196078479290009f, 0.4196078479290009f, 1.0f);
          style.Colors[ImGuiCol_Header] = ImVec4(0.6980392336845398f, 0.6980392336845398f, 0.6980392336845398f, 0.3100000023841858f);
          style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.6980392336845398f, 0.6980392336845398f, 0.6980392336845398f, 0.800000011920929f);
          style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.47843137383461f, 0.4980392158031464f, 0.5176470875740051f, 1.0f);
          style.Colors[ImGuiCol_Separator] = ImVec4(0.4274509847164154f, 0.4274509847164154f, 0.4980392158031464f, 0.5f);
          style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.7176470756530762f, 0.7176470756530762f, 0.7176470756530762f, 0.7799999713897705f);
          style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.5098039507865906f, 0.5098039507865906f, 0.5098039507865906f, 1.0f);
          style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.9098039269447327f, 0.9098039269447327f, 0.9098039269447327f, 0.25f);
          style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.8078431487083435f, 0.8078431487083435f, 0.8078431487083435f, 0.6700000166893005f);
          style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.4588235318660736f, 0.4588235318660736f, 0.4588235318660736f, 0.949999988079071f);
          style.Colors[ImGuiCol_Tab] = ImVec4(0.1764705926179886f, 0.3490196168422699f, 0.5764706134796143f, 0.8619999885559082f);
          style.Colors[ImGuiCol_TabHovered] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.800000011920929f);
          style.Colors[ImGuiCol_TabActive] = ImVec4(0.196078434586525f, 0.407843142747879f, 0.6784313917160034f, 1.0f);
          style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.06666667014360428f, 0.1019607856869698f, 0.1450980454683304f, 0.9724000096321106f);
          style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1333333402872086f, 0.2588235437870026f, 0.4235294163227081f, 1.0f);
          style.Colors[ImGuiCol_PlotLines] = ImVec4(0.6078431606292725f, 0.6078431606292725f, 0.6078431606292725f, 1.0f);
          style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.4274509847164154f, 0.3490196168422699f, 1.0f);
          style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.729411780834198f, 0.6000000238418579f, 0.1490196138620377f, 1.0f);
          style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.6000000238418579f, 0.0f, 1.0f);
          style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
          style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f);
          style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 1.0f);
          style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
          style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
          style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.8666666746139526f, 0.8666666746139526f, 0.8666666746139526f, 0.3499999940395355f);
          style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.8999999761581421f);
          style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.6000000238418579f, 0.6000000238418579f, 0.6000000238418579f, 1.0f);
          style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
          style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
          style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f);
      }


    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);
   
    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
       
        ImGui::Begin("Game Functionality");
        
        ImGui::Text("WARNING: Press the buttons only when the map is fully loaded!");

        ImGui::Spacing();

        if (ImGui::Button("Start Bus"))
        {
            ((UKismetSystemLibrary*)UKismetSystemLibrary::StaticClass())->STATIC_ExecuteConsoleCommand(UObject::FindObject<UFortEngine>("FortEngine_")->GameViewport->World, L"startaircraft", nullptr);
        }

        if (ImGui::Button("Start Safezone"))
        {
            ((UKismetSystemLibrary*)UKismetSystemLibrary::StaticClass())->STATIC_ExecuteConsoleCommand(UObject::FindObject<UFortEngine>("FortEngine_")->GameViewport->World, L"startsafezone", nullptr);
        }

        if (ImGui::Button("Pause safezone"))
        {
            ((UKismetSystemLibrary*)UKismetSystemLibrary::StaticClass())->STATIC_ExecuteConsoleCommand(UObject::FindObject<UFortEngine>("FortEngine_")->GameViewport->World, L"pausesafezone", nullptr);
        }
         
        ImGui::Begin("TODM Bar");
        ImGui::Spacing();

        if (ImGui::Button("Destory TODM"))
        {
            ((UKismetSystemLibrary*)UKismetSystemLibrary::StaticClass())->STATIC_ExecuteConsoleCommand(UObject::FindObject<UFortEngine>("FortEngine_")->GameViewport->World, L"destroyall FortTimeOfDayManager", nullptr);
        }

        if (ImGui::Button("Set the time of day to Morning"))
        {
            ((UKismetSystemLibrary*)UKismetSystemLibrary::StaticClass())->STATIC_ExecuteConsoleCommand(UObject::FindObject<UFortEngine>("FortEngine_")->GameViewport->World, L"settimeofday 8:00", nullptr);
        }

        if (ImGui::Button("Set the time of day to Afternoon"))

        {
            ((UKismetSystemLibrary*)UKismetSystemLibrary::StaticClass())->STATIC_ExecuteConsoleCommand(UObject::FindObject<UFortEngine>("FortEngine_")->GameViewport->World, L"settimeofday 12:00", nullptr);
        }

        if (ImGui::Button("Set the time of day to Night"))
        {
            ((UKismetSystemLibrary*)UKismetSystemLibrary::StaticClass())->STATIC_ExecuteConsoleCommand(UObject::FindObject<UFortEngine>("FortEngine_")->GameViewport->World, L"settimeofday 19:00", nullptr);
        }

        ImGui::Begin("Speed Commands");
        ImGui::Spacing();

        if (ImGui::Button("Demospeed Server"))
        {
            ((UKismetSystemLibrary*)UKismetSystemLibrary::StaticClass())->STATIC_ExecuteConsoleCommand(UObject::FindObject<UFortEngine>("FortEngine_")->GameViewport->World, L"demospeed 20", nullptr);
        }


        if (ImGui::Button("Default Demospeed"))
        {
            ((UKismetSystemLibrary*)UKismetSystemLibrary::StaticClass())->STATIC_ExecuteConsoleCommand(UObject::FindObject<UFortEngine>("FortEngine_")->GameViewport->World, L"demospeed 1", nullptr);
        }

        ImGui::Begin("Fun Stuff");
        ImGui::Spacing();

        if (ImGui::Button("FOV 120"))
        {
            ((UKismetSystemLibrary*)UKismetSystemLibrary::StaticClass())->STATIC_ExecuteConsoleCommand(UObject::FindObject<UFortEngine>("FortEngine_")->GameViewport->World, L"fov 120", nullptr);
        }

        ImGui::Begin("Restarting Stuff");
        ImGui::Spacing();
       
        if (ImGui::Button("Restart"))
        {
            MH_DisableHook((LPVOID)(uintptr_t(GetModuleHandle(0)) + Offsets::SpawnPlayActor));
            ((UKismetSystemLibrary*)UKismetSystemLibrary::StaticClass())->STATIC_ExecuteConsoleCommand(UObject::FindObject<UFortEngine>("FortEngine_")->GameViewport->World, L"open Athena_Terrain", nullptr);
            auto nd = Beacons::GNetDriver;
            Beacons::GNetDriver = nullptr;
            reinterpret_cast<void(*)(UNetDriver*)>(Util::FindPattern("48 83 EC 68 48 89 5C 24 ? 48 89 7C 24"))(nd);
            Hooks::bHasInitedTheBeacon = false;
            
        }

        // Rendering
        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * clear_color.w * 255.0f), (int)(clear_color.y * clear_color.w * 255.0f), (int)(clear_color.z * clear_color.w * 255.0f), (int)(clear_color.w * 255.0f));
        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

        // Handle loss of D3D9 device
        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return false;

    // Create the D3DDevice
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}


DWORD WINAPI MainThread(LPVOID)
{
    Util::InitConsole();

    LOG("Setting Up!");

    srand(time(0));

    auto BaseAddr = Util::BaseAddress();
    auto GObjectsAddress = BaseAddr + 0x6661380;
    auto FNameToStringAddress = BaseAddr + 0x1302390;
    auto FreeMemoryAddress = BaseAddr + 0x1233210;
    auto ReallocAddress = BaseAddr + 0x123E4C0;
    auto MallocAddress = BaseAddr + 0x0;
    auto GNamesAddress = BaseAddr + 0x66587C8;

    FName::GNames = *reinterpret_cast<TNameEntryArray**>((uintptr_t**)GNamesAddress);
    UObject::GObjects = reinterpret_cast<FUObjectArray*>((uintptr_t*)GObjectsAddress);
    SDK::FNameToString = decltype(SDK::FNameToString)(FNameToStringAddress);
    SDK::FreeInternal = decltype(SDK::FreeInternal)(FreeMemoryAddress);

    Hooks::InternalTryActivateAbilityLong = decltype(Hooks::InternalTryActivateAbilityLong)(BaseAddr + Offsets::InternalTryActivateAbility);
    FMemory_Free = decltype(FMemory_Free)(FreeMemoryAddress);
    FMemory_Realloc = decltype(FMemory_Realloc)(ReallocAddress);
    FMemory_Malloc = decltype(FMemory_Malloc)(MallocAddress);
    Abilities::GiveAbility = decltype(Abilities::GiveAbility)(BaseAddr + Offsets::GiveAbility);

    auto FortEngine = SDK::UObject::FindObject<UFortEngine>("FortEngine_");
    Globals::FortEngine = FortEngine;
    Globals::World = FortEngine->GameViewport->World;
    Globals::GPS = reinterpret_cast<UGameplayStatics*>(UGameplayStatics::StaticClass());
    Globals::PC = reinterpret_cast<AFortPlayerController*>(FortEngine->GameInstance->LocalPlayers[0]->PlayerController);
    Globals::MathLib = reinterpret_cast<UKismetMathLibrary*>(UKismetMathLibrary::StaticClass());
    Globals::SystemLib = reinterpret_cast<UKismetSystemLibrary*>(UKismetSystemLibrary::StaticClass());

    Globals::HeadPart = FindObjectFast<UCustomCharacterPart>("/Game/Characters/CharacterParts/Female/Medium/Heads/F_Med_Head1.F_Med_Head1");
    Globals::BodyPart = FindObjectFast<UCustomCharacterPart>("/Game/Characters/CharacterParts/Female/Medium/Bodies/F_Med_Soldier_01.F_Med_Soldier_01");

    Globals::bSTWMode = false;

    auto NewConsole = Globals::GPS->STATIC_SpawnObject(UFortConsole::StaticClass(), FortEngine->GameViewport);
    FortEngine->GameViewport->ViewportConsole = (UFortConsole*)(NewConsole);

    auto NewCheatManager = Globals::GPS->STATIC_SpawnObject(UCheatManager::StaticClass(), FortEngine->GameInstance->LocalPlayers[0]->PlayerController);
    FortEngine->GameInstance->LocalPlayers[0]->PlayerController->CheatManager = (UCheatManager*)(NewCheatManager);

    MH_Initialize();

    LoadLootPools();

    Discord::UpdateStatus("Server is now loading map...");

    Globals::PC->SwitchLevel(TEXT("Athena_Terrain"));

    new std::thread(InitImGui);

    Replication::InitOffsets();

    Hooks::bIsReady = true;

    Hooks::Init();

    LOG("Setup!");

    return 0;
}

BOOL APIENTRY DllMain(HMODULE mod, DWORD reason, LPVOID res)
{
    if (reason == 1)
        CreateThread(0, 0, MainThread, mod, 0, 0);

    return TRUE;
}