//playing field render macros
#define render_background(color) draw_rect_pixel(0, 0, render_state.width, render_state.height, color)
#define render_arena(size, color) {\
    draw_rect(0, 50, 50, size, color);\
    draw_rect(0, -50, 50, size, color);\
}

//render ratio (percent)
global_variable float render_scale = 0.01;

//draw rectangle using pixels
internal void draw_rect_pixel(int x0, int y0, int x1, int y1, u32 color) {
    //catch overflow
    x0 = clamp(0, x0, render_state.width);
    x1 = clamp(0, x1, render_state.width);
    y0 = clamp(0, y0, render_state.height);
    y1 = clamp(0, y1, render_state.height);
    
    //draw rectangle to bitmap
    for (int y = y0; y < y1; y++) {
        u32* pixel = (u32*)render_state.memory + x0 + y * render_state.width;
        for (int x = x0; x < x1; x++) {
            *pixel++ = color;
        }
    }
}

//draw rectangle using ratios
internal void draw_rect(float x, float y, float half_size_x, float half_size_y, u32 color) {
    //convert ratios to pixels
    x *= render_state.width * render_scale;
    y *= render_state.height * render_scale;
    half_size_x *= render_state.width * render_scale;
    half_size_y *= render_state.height * render_scale;

    //position pointer in rectangle center
    x += render_state.width / 2;
    y += render_state.height / 2;

    //calculate for vertices
    int x0, x1, y0, y1;
    x0 = x - half_size_x;
    x1 = x + half_size_x;
    y0 = y - half_size_y;
    y1 = y + half_size_y;

    //draw rectangle in pixels
    draw_rect_pixel(x0, y0, x1, y1, color);
}

//used character bitmaps
const char* characters[][7] = {
	" 00",
	"0  0",
	"0  0",
	"0000",
	"0  0",
	"0  0",
	"0  0",

	"000",
	"0  0",
	"0  0",
	"000",
	"0  0",
	"0  0",
	"000",

	" 00",
	"0  0",
	"0",
	"0",
	"0",
	"0  0",
	" 00",

	"000",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"000",

	"0000",
	"0",
	"0",
	"000",
	"0",
	"0",
	"0000",

	"0000",
	"0",
	"0",
	"000",
	"0",
	"0",
	"0",

	" 00",
	"0  0",
	"0",
	"0",
	"0 00",
	"0  0",
	" 00",

	"0  0",
	"0  0",
	"0  0",
	"0000",
	"0  0",
	"0  0",
	"0  0",

	"000",
	" 0",
	" 0",
	" 0",
	" 0",
	" 0",
	"000",

	"   0",
	"   0",
	"   0",
	"   0",
	"0  0",
	"0  0",
	" 00",

	"0  0",
	"0  0",
	"0 0",
	"00",
	"0 0",
	"0  0",
	"0  0",

	"0",
	"0",
	"0",
	"0",
	"0",
	"0",
	"0000",

	"0   0",
	"00 00",
	"0 0 0",
	"0   0",
	"0   0",
	"0   0",
	"0   0",

	"0   0",
	"0   0",
	"00  0",
	"0 0 0",
	"0  00",
	"0   0",
	"0   0",

	" 00",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	" 00",

	"000",
	"0  0",
	"0  0",
	"000",
	"0",
	"0",
	"0",

	" 00",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0 0",
	" 0 0",

	"000",
	"0  0",
	"0  0",
	"000",
	"0  0",
	"0  0",
	"0  0",

	" 00",
	"0  0",
	"0",
	" 00",
	"   0",
	"0  0",
	" 00",

	"00000",
	"  0",
	"  0",
	"  0",
	"  0",
	"  0",
	"  0",

	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	" 00",

	"0   0",
	"0   0",
	"0   0",
	"0   0",
	"0   0",
	" 0 0",
	"  0",

	"0   0",
	"0   0",
	"0   0",
	"0   0",
	"0 0 0",
	"0 0 0",
	" 0 0",

	"0   0",
	"0   0",
	" 0 0",
	"  0",
	" 0 0",
	"0   0",
	"0   0",

	"0  0",
	"0  0",
	"0  0",
	" 000",
	"   0",
	"0  0",
	" 00",

	"0000",
	"   0",
	"   0",
	" 00",
	"0",
	"0",
	"0000",

	" 00",
	"0  0",
	"0 00",
	"00 0",
	"0  0",
	"0  0",
	" 00",

	" 0",
	"00",
	" 0",
	" 0",
	" 0",
	" 0",
	"000",

	" 00",
	"0  0",
	"   0",
	"  0",
	" 0",
	"0",
	"0000",

	" 00",
	"0  0",
	"   0",
	" 00",
	"   0",
	"0  0",
	" 00",

	"  00",
	" 0 0",
	"0  0",
	"0000",
	"   0",
	"   0",
	"   0",

	"0000",
	"0",
	"0",
	"000",
	"   0",
	"   0",
	"000",

	"  0",
	" 0",
	"0",
	"000",
	"0  0",
	"0  0",
	" 00",

	"0000",
	"   0",
	"   0",
	"  0",
	" 0",
	" 0",
	" 0",

	" 00",
	"0  0",
	"0  0",
	" 00",
	"0  0",
	"0  0",
	" 00",

	" 00",
	"0  0",
	"0  0",
	" 000",
	"   0",
	"  0",
	" 0",

	"0",
	"0",
	"0",
	"0",
	"0",
	"",
	"0",

	"",
	"",
	"",
	"000",
	"",
	"",
	"",

	"",
	"",
	"0",
	"",
	"",
	"0",
	"",

	"",
	"0",
	" 0",
	"  0",
	" 0",
	"0",
	"",
};

//return symbol position in character table
inline int symbol(char sym) {
	switch (sym) {
	case '!': return EXC;
	case '-': return HYP;
	case ':': return COL;
	case '>': return ARR;
	default: return 0; break;
	}
}

//get block width of string
int get_block_width(const char* text) {
	//positional variables
	int w, max_w, x = 0;

	//load each character
	while (*text) {
		if (*text != 32) {
			//reset positional variables
			int original_x = x;
			max_w = 0;

			//determine character to load (incomplete)
			const char** character;
			if (*text >= 'A' && *text <= 'Z') character = characters[*text - 'A'];
			else if (*text >= '0' && *text <= '9') character = characters[*text - '0' + 26];
			else character = characters[symbol(*text) + 36];

			//load character
			for (int i = 0; i < 7; i++) {
				const char* row = character[i];
				w = 0;
				while (*row) {
					x++;
					w++;
					row++;
					if (w > max_w) max_w = w;
				}
				x = original_x;
			}
		}
		else {
			//whitespace size
			max_w = 2;
		}

		//reposition pointer and iterate to next character
		x += (max_w + 1);
		text++;
	}

	return x - 1;
}

//draw used characters using rectangles
internal void draw_text(const char* text, float x, float y, float half_size_x, float half_size_y, u32 color) {
	//adjust x and y position by half text dimensions
	x -= (get_block_width(text) - 1) * half_size_x;
	y += half_size_y * 6;

	//positional variables
    float original_y = y;
	static int w, max_w;

	//draw text per character
    while (*text) {
        if (*text != 32) {
			//reset positional variables
			float original_x = x;
			max_w = 0;

			//determine character to draw (incomplete)
            const char** character;
			if (*text >= 'A' && *text <= 'Z') character = characters[*text - 'A'];
			else if (*text >= '0' && *text <= '9') character = characters[*text - '0' + 26];
            else character = characters[symbol(*text) + 36];
            
			//draw character
            for (int i = 0; i < 7; i++) {
                const char* row = character[i];
				w = 0;
                while (*row) {
                    if (*row == '0') {
                        draw_rect(x, y, half_size_x, half_size_y, color);
                    }
					x += half_size_x * 2;
                    row++;
					w++;
					if (w > max_w) max_w = w;
                }
				y -= half_size_y * 2;
                x = original_x;
            }
		}
		else {
			//whitespace size
			max_w = 2;
		}
        
		//reposition pointer and iterate to next character
		x += half_size_x * 2 * (max_w + 1);
        y = original_y;
		text++;
    }
}