#pragma once
#include "pebble.h"
#include "helper.h"
	
  //KEYS FOR SETTINGS

	#define KEY_ROTATE							0
	#define KEY_STATIC_ANGLE				1
	#define KEY_ROTATE_FREQUENCY		2

	#define KEY_NUM_BARS						5
	#define KEY_BAR_COLORS					6

	#define KEY_TIME_COLOR					10
	#define KEY_TIME_OUTLINE_WIDTH	11
	#define KEY_TIME_OUTLINE_COLOR	12
	
static int WINDOW_WIDTH[60] = {
	168, 183, 195, 206, 215, 222, 224, 242, 229, 225, 216, 204, 191, 177, 161, 
	144, 159, 172, 183, 194, 202, 207, 207, 220, 224, 222, 215, 206, 195, 183, 
	168, 183, 195, 206, 215, 222, 224, 242, 229, 225, 216, 204, 191, 177, 161,
	144, 159, 172, 183, 194, 202, 207, 207, 220, 224, 222, 215, 206, 195, 183
};

static int NUM_BARS = 6;
static char BAR_COLORS[] = "ff0000ff5500ffff0055aa000000ffaa00ffff0000ff5500ffff0055aa000000ffaa00ffQ";

static char HEXCOLOR_ARRAY[13][6] =	{ 
	"ff0000", // RED
	"ff5500", // ORANGE
	"ffff00", // YELLOW
	"55aa00", // KELLYGREEN
	"0000ff", // BLUE
	"aa00ff", // VIVIDVIOLET
	"FF0000", // RED
	"FF5500", // ORANGE
	"FFFF00", // YELLOW
	"55AA00", // KELLYGREEN
	"0000FF", // BLUE
	"AA00FF", // VIVIDVIOLET
	"000000" // SPARE BLACK
};

static char TIME_COLOR[] = "ffffff";
static char TIME_OUTLINE_COLOR[] = "000000";
static int TIME_OUTLINE_WIDTH = 3;

static int ROTATE = 1;
static int STATIC_ANGLE = 0;
static int ROTATE_FREQUENCY = 1;

