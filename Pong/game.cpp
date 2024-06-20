//button status macros
#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

//position variables
const float player_position = 45;
//size variables
const float arena_half_w = 50, arena_half_h = 48, char_block_half_w = 0.28125, char_block_half_h = 0.5;
const float player_half_w = 1, player_half_h = 8, max_half_h = 14, ball_half_w = 0.5625, ball_half_h = 1;
//movement variables
const float p_friction = 10, start_ball_a_x = 100, start_ball_a_y = 0.25, max_ball_v_x = 100;
//factor variables
const float angle_multiplier = 10, ball_v_y_reset = 0.1;
//gameplay variables
const int max_score = 10;
float p_acceleration = 1250, c_acceleration = 1.25, c_detection = 0;
//highscore variables
std::string highscore_name = "COMPUTER";
int highscore_value = 0;

//main class definition
class Object {
    public:
        //object attributes
        u32 color;
        float x, y, half_w, half_h;
        float velocity_x, velocity_y, acceleration_x, acceleration_y;

        //draw object to frame
        void draw() {
            draw_rect(x, y, half_w, half_h, color);
        }
        
        //erase object frame
        void erase() {
            draw_rect(x, y, half_w, half_h, background_color);
        }

        //update object movement
        void update(float dt) {
            //update x attributes
            x += velocity_x * dt + 0.5 * acceleration_x * dt * dt;
            velocity_x += acceleration_x * dt;

            //update y attributes
            y += velocity_y * dt + 0.5 * acceleration_y * dt * dt;
            velocity_y += acceleration_y * dt;

            //collision detection
            collide_wall(dt);

            //draw object
            draw();
        }

        //bounce object off wall
        bool collide_wall(float dt) {
            if (y + half_h > arena_half_h || y - half_h < -arena_half_h) {
                y = (y > 0) ? arena_half_h - half_h : half_h - arena_half_h;
                velocity_y *= -1;
                return true;
            }
            return false;
        }
};

//player class definition
class Player : public Object {
    public:
        //additional player attributes
        int score;
        float power_timer;

        //player constructor
        Player(float a, u32 b) {
            x = a;
            y = 0;
            half_w = player_half_w;
            half_h = player_half_h;
            color = b;
            velocity_x = 0;
            velocity_y = 0;
            acceleration_x = 0;
            acceleration_y = 0;
            score = 0;
            power_timer = -1;
        }

        //reset player
        void reset(u32 a) {
            y = 0;
            half_h = player_half_h;
            color = a;
            velocity_x = 0;
            velocity_y = 0;
            acceleration_x = 0;
            acceleration_y = 0;
            score = 0;
            power_timer = -1;
        }

        //apply friction to player movement
        void decelerate() {
            acceleration_x = velocity_x * -p_friction;
            acceleration_y = velocity_y * -p_friction;
        }
};

//player objects initialization
Player Player1(-player_position, background_contrast);
Player Player2(player_position, background_contrast);

//ball class definition
class Ball : public Object {
    public:
        //additional ball attributes
        bool hit;

        //ball constructor
        Ball(u32 a) {
            hit = false;
            x = 0;
            y = 0;
            half_w = ball_half_w;
            half_h = ball_half_h;
            color = a;
            velocity_x = -1;
            velocity_y = 0;
            acceleration_x = 0;
            acceleration_y = 0;
        }

        //reset ball
        void reset(float a) {
            hit = false;
            x = 0;
            y = 0;
            velocity_x = -1 * a;
            velocity_y = 0;
        }

        //reset hit indicator and acceleration
        void decelerate() {
            hit = false;
            if (abs(velocity_x) < max_ball_v_x) {
                acceleration_x = (velocity_x > 0) ? start_ball_a_x : -start_ball_a_x;
                acceleration_y = (velocity_y != 0) ? ((velocity_y > 0) ? start_ball_a_y : -start_ball_a_y) : 0;
            }
            else {
                acceleration_x = 0;
                acceleration_y = 0;
            }
        }

        //player collision and scoring system
        bool collide_player(Player* player, float dt) {
            //bounce ball off player
            if (x + half_w > player->x - player->half_w &&
                x - half_w < player->x + player->half_w &&
                y + half_h > player->y - player->half_h &&
                y - half_h < player->y + player->half_h) {
                x -= velocity_x * dt;
                velocity_x *= -1;
                velocity_y = (y - player->y) * angle_multiplier + player->velocity_y;
                hit = true;
                return true;
            }
            //player score and round reset
            else if ((player->x > 0) ? (x - half_w < -arena_half_w) : (x + half_w > arena_half_w)) {
                x = 0;
                y = player->y;
                velocity_x = (player->x > 0 ? 1 : -1) * (survival ? max_ball_v_x / 2 : 1);
                velocity_y = (velocity_y > 0) ? 1 : -1;
                player->score++;
            }
            return false;
        }
};

//ball objects initialization
Ball Ball1(background_contrast);
Ball Ghost(background_contrast);

//game starter function
internal void build_game() {
    Player1.reset(background_contrast);
    Player2.reset(background_contrast);
    Ball1.reset(survival ? max_ball_v_x / 2 : 1);
    Ghost.reset(survival ? max_ball_v_x / 2 : 1);
}

//ghost variables
const float ghost_v_x_ratio = 2.00, ghost_v_y_ratio = 0.33, ghost_trigger_speed = 100, ghost_delay = 10, power_duration = 10;
bool ghost_exist = false;
int ghost_timer = -1;

//power timer countdown and reset macro
inline void check_power_timer(Player* player, float dt) {\
    if (player->power_timer >= 0) {\
        if (player->power_timer < power_duration) {\
            player->power_timer += dt;\
        }\
        else {\
            player->power_timer = -1;\
            player->half_h = player_half_h;\
            player->color = background_contrast;\
        }\
    }\
}

//ghost ball function
internal void simulate_ghost_ball(float dt) {
    //delay ghost ball creation
    if (Ball1.hit) ghost_timer = 0;
    else if (ghost_timer >= 0) ghost_timer += 1;

    //limit power-up duration
    check_power_timer(&Player1, dt);
    check_power_timer(&Player2, dt);

    //create ghost ball
    if ((Ball1.velocity_y > ghost_trigger_speed || Ball1.velocity_y < -ghost_trigger_speed) && ghost_timer > ghost_delay) {
        Ghost.x = Ball1.x;
        Ghost.y = Ball1.y;
        Ghost.velocity_x = Ball1.velocity_x * ghost_v_x_ratio;
        Ghost.velocity_y = Ball1.velocity_y * ghost_v_y_ratio;
        ghost_exist = true;
        ghost_timer = -1;
    }

    //update ghost ball movement
    if (ghost_exist) {
        //update ghost ball position and color
        Ghost.x += Ghost.velocity_x * dt;
        Ghost.y += Ghost.velocity_y * dt;
        Ghost.color = Rainbow[(int)Ghost.x % 14];

        //collision detection
        Ghost.collide_wall(dt);
        
        //ghost ball out of bounds
        if ((Ghost.x - Ghost.half_w < -arena_half_w) || (Ghost.x + Ghost.half_w > arena_half_w)) {
            ghost_exist = false;
        }
        //power-up player upon catch
        else if (Ghost.collide_player(&Player1, dt)) {
            ghost_exist = false;
            Player1.power_timer = 0;
            Player1.half_h = clamp(player_half_h, Player1.half_h + 2, max_half_h);
            Player1.color = Blue[(int)(Player1.half_h - player_half_h) / 2 - 1];
        }
        else if (Ghost.collide_player(&Player2, dt)) {
            ghost_exist = false;
            Player2.power_timer = 0;
            Player2.half_h = clamp(player_half_h, Player2.half_h + 2, max_half_h);
            Player2.color = Pink[(int)(Player2.half_h - player_half_h) / 2 - 1];
        }
        
        //draw ghost ball
        Ghost.draw();
    }
}

//main game control and display function
internal void simulate_game(Input* input, float dt) {
    //stop game upon reaching max score
    if (survival) {
        if (Player2.score >= 1) {
            playing = false;
            win = false;
        }
    }
    else {
        if (Player1.score >= max_score || Player2.score >= max_score) {
            playing = false;
            win = (Player1.score > Player2.score);
        }
    }

    //display playing field and player scores
    render_background(background_color);
    render_arena(50 - arena_half_h, background_contrast);
    draw_text(&(std::to_string(Player1.score))[0], -10, 41, char_block_half_w, char_block_half_h, background_contrast);
    draw_text("-", 0, 41, char_block_half_w, char_block_half_h, background_contrast);
    draw_text(&(std::to_string(Player2.score))[0], 10, 41, char_block_half_w, char_block_half_h, background_contrast);

    //reset object accelerations
    Player1.decelerate();
    Player2.decelerate();
    Ball1.decelerate();
    
    //player 1 controls
    if (is_down(BUTTON_W)) Player1.acceleration_y += p_acceleration;
    if (is_down(BUTTON_S)) Player1.acceleration_y -= p_acceleration;

    if (pvp) {
        //player 2 controls
        if (is_down(BUTTON_UP)) Player2.acceleration_y += p_acceleration;
        if (is_down(BUTTON_DOWN)) Player2.acceleration_y -= p_acceleration;
    }
    else {
        //player 1 alternative controls
        if (is_down(BUTTON_UP)) Player1.acceleration_y += p_acceleration;
        if (is_down(BUTTON_DOWN)) Player1.acceleration_y -= p_acceleration;

        //computer controls
        if (Ball1.y + c_detection > Player2.y + Player2.half_h) Player2.acceleration_y += p_acceleration * (survival ? 0.9 + Player2.score * 0.01 : c_acceleration);
        if (Ball1.y - c_detection < Player2.y - Player2.half_h) Player2.acceleration_y -= p_acceleration * (survival ? 0.9 + Player2.score * 0.01 : c_acceleration);
    }
    
    //update object frames
    Player1.update(dt);
    Player2.update(dt);
    Ball1.update(dt);
    Ball1.collide_player(&Player1, dt);
    Ball1.collide_player(&Player2, dt);

    //simulate ghost ball
    simulate_ghost_ball(dt);
}