//button status changer macro
#define process_button(b, vk)\
case vk:{\
    input.buttons[b].changed = is_down != input.buttons[b].is_down;\
    input.buttons[b].is_down = is_down;\
}   break;

//button press inside window
#define w_pressed(b) (input.buttons[b].is_down && input.buttons[b].changed)

//arrow movement function
#define arrow_3_tiered(a, b, c) {\
	if (w_pressed(BUTTON_UP) && choice > 0) choice--;\
	if (w_pressed(BUTTON_DOWN) && choice < 2) choice++;\
	switch (choice % 3) {\
		case 0: draw_text(">", a, b, char_block_half_w, char_block_half_h, background_contrast); break;\
		case 1: draw_text(">", a, b + c, char_block_half_w, char_block_half_h, background_contrast); break;\
		case 2: draw_text(">", a, b + 2 * c, char_block_half_w, char_block_half_h, background_contrast); break;\
	}\
}

//load highscore from file
#define load_highscore() {\
	std::fstream FILE("highscores.txt", std::fstream::in);\
	FILE >> highscore_name >> highscore_value;\
	FILE.close();\
}

//save highscore to file
#define save_highscore() {\
	std::fstream FILE("highscores.txt", std::fstream::out); \
	FILE << highscore_name << "\t" << highscore_value; \
	FILE.close(); \
}

//used buttons
enum {
	BUTTON_W,
	BUTTON_S,
	BUTTON_UP,
	BUTTON_DOWN,
	//BUTTON_RIGHT,
	//BUTTON_LEFT,
	BUTTON_SPACE,
	BUTTON_ENTER,
	BUTTON_ESCAPE,

	BUTTON_COUNT,
};

//used symbols
enum {
	EXC,
	HYP,
	COL,
	ARR,

	SYM_COUNT,
};

//button attributes struct
struct Button_State {
	bool is_down;
	bool changed;
};

//button attributes array
struct Input {
	Button_State buttons[BUTTON_COUNT];
};

//used colors
u32 background_color = 0x000000, background_contrast = 0xffffff;
u32 Blue[] = { 0x9bedff, 0x50e0ff, 0x00d2ff };
u32 Pink[] = { 0xfdd2e8, 0xffb7dd, 0xfd9ed0 };
u32 Rainbow[] = { 0xFFADAD, 0xFFC2A9, 0xFFD6A5, 0xFEEBAE, 0xFDFFB6, 0xE4FFBB, 0xCAFFBF, 0xB3FBDF, 0x9BF6FF, 0x9EDDFF, 0xA0C4FF, 0xAFBBFF, 0xBDB2FF, 0xDEB0D6 };
