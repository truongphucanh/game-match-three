#pragma once

// design scene size
#define DESIGN_WIDTH 640.0f
#define DESIGN_HEIGHT 960.0f
#define WIN_WIDTH 640.0f
#define WIN_HEIGHT 960.0f
#define LOGIC_FPS 30.0f
#define FRAME_ZOOM_FACTOR 0.7 // small laptop screen (14 inch)

// number of rows and COL of cookie matrix
#define NUM_ROWS 9
#define NUM_COLS 9

// number of cookie type
#define NUM_COOKIE_TYPES 7

// number of levels
#define NUM_LEVELS 5

// start-up level
#define START_LEVEL 0

// size of a tile in cookies layer
#define TILE_WIDTH 48
#define TILE_HEIGHT 54

// size of cookie layer
#define COOKIELAYER_WIDTH NUM_COLS*TILE_WIDTH
#define COOKIELAYER_HEIGHT NUM_ROWS*TILE_HEIGHT

// size of gameover panel and level complete panel
#define PANEL_WIDTH DESIGN_WIDTH
#define PANEL_HEIGHT DESIGN_HEIGHT*0.2f

// time for animations
#define DURATION_SWAP 0.2f
#define DURATION_FALLING 0.1f
#define DURATION_REMOVE 0.3f
#define DURATION_FADE_OUT 0.3f
#define DURATION_FADE_IN 0.05f
#define DURATION_FADE_IN_GAMEOVER 0.5f
#define DURATION_FADE_IN_LEVEL_COMPLETE 0.5f
#define FALLING_SPEED 50 // pixel per second
#define DELAY_TIME 0.1f
#define DURATION_MOVE_SCORE 0.5f
#define DELTA_MOVE_SCORE 10 // pixes
#define DURATION_GAME_OVER 1.5f
#define DURATION_FADE_IN_BEGIN 0.5f
#define MAX_DELAY_TIME_IN_BEGIN 0.75f
#define MIN_DELAY_TIME_IN_BEGIN 0.25

// labels
#define FONT_FILE "Consolas.ttf"
#define TEXT_SIZE 24
#define NUM_SIZE 30
#define FROM_TOP 50
#define FROM_BOT 50
#define FROM_LEFT 80
#define FROM_RIGHT 75

// score
#define SCORE_PER_COOKIE 20

#define MAKE_SURE_ANIMATION_FINISH 0.05f

#define IS_WIN32 (CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)
