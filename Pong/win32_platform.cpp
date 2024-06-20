//library inclusions
#include <windows.h>
#include <fstream>
#include <string>
#include "manual_resource.h"
#include "utilities.cpp"
#include "platform_common.cpp"

//window attributes struct
struct Render_State {
    int height, width;
    void* memory;
    BITMAPINFO bitmap_info;
};

//window and game variables
global_variable Render_State render_state;
global_variable bool running, playing, main_menu, difficulty_menu, speed_menu, win, pvp, survival;
global_variable int choice = 0;

//window actions library
#include "renderer.cpp"
#include "game.cpp"

//process window actions
LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;
    switch (uMsg) {
        //handle window close
        case WM_CLOSE: case WM_DESTROY: {
            running = false;
        }   break;
        //handle window resize
        case WM_SIZE: {
            RECT rect;
            GetClientRect(hwnd, &rect);
            render_state.width = rect.right - rect.left;
            render_state.height = rect.bottom - rect.top;
            int size = render_state.width * render_state.height * sizeof(u32);
            if (render_state.memory) VirtualFree(render_state.memory, 0, MEM_RELEASE);
            render_state.memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
            render_state.bitmap_info.bmiHeader.biSize = sizeof(render_state.bitmap_info.bmiHeader);
            render_state.bitmap_info.bmiHeader.biWidth = render_state.width;
            render_state.bitmap_info.bmiHeader.biHeight = render_state.height;
            render_state.bitmap_info.bmiHeader.biPlanes = 1;
            render_state.bitmap_info.bmiHeader.biBitCount = 32;
            render_state.bitmap_info.bmiHeader.biCompression = BI_RGB;
        }   break;
        //handle other window actions
        default: {
            result = DefWindowProc(hwnd, uMsg, wParam, lParam);
        }   break;
    }
    return result;
}

//main function
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCMD) {
    //window setup
    WNDCLASS window_class = {};
    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpszClassName = L"Game Window Class";
    window_class.lpfnWndProc = window_callback;
    window_class.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
    RegisterClass(&window_class);
<<<<<<< HEAD
    HWND window = CreateWindow(window_class.lpszClassName, L"Pong", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 960, 0, 0, hInstance, 0);
=======
    HWND window = CreateWindow(window_class.lpszClassName, L"Pong", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
>>>>>>> 3d354787a3ffaed10664a286702c048d3f1c2576
    HDC hdc = GetDC(window);
    ShowCursor(false);

    //fps tracker
    float delta_time = 0.01666666f;
    LARGE_INTEGER frame_begin_time;
    QueryPerformanceCounter(&frame_begin_time);
    float performance_frequency; {
        LARGE_INTEGER perf;
        QueryPerformanceFrequency(&perf);
        performance_frequency = (float)perf.QuadPart;
    }

    //button attributes and loop variables
    Input input = {};
    running = true, main_menu = true;
    load_highscore();

    //program loop
    while (running) {
        //reset buttons status
        for (int i = 0; i < BUTTON_COUNT; i++) {
            input.buttons[i].changed = false;
        }

        //catch message
        MSG message;
        while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
            switch (message.message) {
                //process button press
                case WM_KEYUP: case WM_KEYDOWN: {
                    u32 vk_code = (u32)message.wParam;
                    bool is_down = ((message.lParam & (1 << 31)) == 0);
                    switch (vk_code) {
                        process_button(BUTTON_W, 'W');
                        process_button(BUTTON_S, 'S');
                        process_button(BUTTON_UP, VK_UP);
                        process_button(BUTTON_DOWN, VK_DOWN);
                        process_button(BUTTON_SPACE, VK_SPACE);
                        process_button(BUTTON_ENTER, VK_RETURN);
                        process_button(BUTTON_ESCAPE, VK_ESCAPE);
                    }
                }   break;
                //process window actions
                default: {
                    TranslateMessage(&message);
                    DispatchMessage(&message);
                }   break;
            }
        }

        //display main menu
        if (main_menu) {
            //reset game variables
            difficulty_menu = false, speed_menu = false, win = false, pvp = false, survival = false;
            ghost_exist = false, p_acceleration = 1250, c_acceleration = 1.25, c_detection = 0;

            //display main menu texts
            render_background(background_color);
            draw_text("PONG: EVOLVED", 0, 25, char_block_half_w * 2, char_block_half_h * 2, background_contrast);
            draw_text("SINGLE PLAYER", 0, 0, char_block_half_w, char_block_half_h, background_contrast);
            draw_text("MULTIPLAYER", 0, -12, char_block_half_w, char_block_half_h, background_contrast);
            draw_text("SURVIVAL MODE", 0, -24, char_block_half_w, char_block_half_h, background_contrast);
            
            //control arrow position
            arrow_3_tiered(-20, 0, -12);

            //build game mode
            if (w_pressed(BUTTON_ENTER)) {
                switch (choice % 3) {
                    case 0: difficulty_menu = true; break;
                    case 1: speed_menu = true, pvp = true; break;
                    case 2: survival = true, p_acceleration = 1250; break;
                }
                main_menu = false, playing = true, choice = 0;
                build_game();
            }
        }

        else if (difficulty_menu) {
            //display difficulty menu texts
            render_background(background_color);
            draw_text("PONG: EVOLVED", 0, 25, char_block_half_w * 2, char_block_half_h * 2, background_contrast);
            draw_text("NORMAL", 0, 0, char_block_half_w, char_block_half_h, background_contrast);
            draw_text("DIFFICULT", 0, -12, char_block_half_w, char_block_half_h, background_contrast);
            draw_text("IMPOSSIBLE", 0, -24, char_block_half_w, char_block_half_h, background_contrast);

            //control arrow position
            arrow_3_tiered(-15.5, 0, -12);

            //choose difficulty
            if (w_pressed(BUTTON_ENTER)) {
                switch (choice % 3) {
                case 0: p_acceleration = 1000, c_acceleration = 1.25, c_detection = 0; break;
                case 1: p_acceleration = 1250, c_acceleration = 1.5, c_detection = 1; break;
                case 2: p_acceleration = 1500, c_acceleration = 1.75, c_detection = 2; break;
                }
                difficulty_menu = false, choice = 0;
            }
        }

        else if (speed_menu) {
            //display difficulty menu texts
            render_background(background_color);
            draw_text("PONG: EVOLVED", 0, 25, char_block_half_w * 2, char_block_half_h * 2, background_contrast);
            draw_text("SLOW", 0, 0, char_block_half_w, char_block_half_h, background_contrast);
            draw_text("MEDIUM", 0, -12, char_block_half_w, char_block_half_h, background_contrast);
            draw_text("FAST", 0, -24, char_block_half_w, char_block_half_h, background_contrast);

            //control arrow position
            arrow_3_tiered(-15.5, 0, -12);

            //choose difficulty
            if (w_pressed(BUTTON_ENTER)) {
                switch (choice % 3) {
                case 0: p_acceleration = 1000; break;
                case 1: p_acceleration = 1250; break;
                case 2: p_acceleration = 1500; break;
                }
                speed_menu = false, choice = 0;
            }
        }

        //build game frame
        else if (playing) simulate_game(&input, delta_time);
        
        else {
            //display game end screen
            Ball1.erase();
            Ghost.erase();
            draw_text("PRESS SPACE TO PLAY AGAIN", 0, -15, char_block_half_w / 2, char_block_half_h / 2, background_contrast);
            draw_text("PRESS ENTER TO RETURN TO MENU", 0, -20, char_block_half_w / 2, char_block_half_h / 2, background_contrast);
            draw_text("PRESS ESCAPE TO QUIT", 0, -25, char_block_half_w / 2, char_block_half_h / 2, background_contrast);

            //survival highscore display
            if (survival) {
                if (Player1.score > highscore_value) {
                    //highscore_name = "NEYRO";
                    highscore_value = Player1.score;
                }
                draw_text(&("YOUR SCORE: " + std::to_string(Player1.score))[0], 0, 10, char_block_half_w, char_block_half_h, background_contrast);
                draw_text(&("HIGHSCORE: " + std::to_string(highscore_value))[0], 0, 0, char_block_half_w, char_block_half_h, background_contrast);
            }
            //round winner display
            else {
                draw_text((win ? "PLAYER 1 WINS!" : (pvp ? "PLAYER 2 WINS!" : "COMPUTER WINS!")), 0, 5, char_block_half_w, char_block_half_h, background_contrast);
            }
            
            //exit options
            if (w_pressed(BUTTON_SPACE)) {
                build_game();
                playing = true;
            }
            if (w_pressed(BUTTON_ENTER)) main_menu = true;
        }

        //render game frame
        StretchDIBits(hdc, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height, render_state.memory, &render_state.bitmap_info, DIB_RGB_COLORS, SRCCOPY);

        //compute fps
        LARGE_INTEGER frame_end_time;
        QueryPerformanceCounter(&frame_end_time);
        delta_time = (float)(frame_end_time.QuadPart - frame_begin_time.QuadPart) / performance_frequency;
        frame_begin_time = frame_end_time;

        //quit options
        if (w_pressed(BUTTON_ESCAPE)) playing ? playing = false, main_menu = true : running = false;
    }

    save_highscore();
    return 0;
}