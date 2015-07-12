#pragma once
	
unsigned int HexStringToUInt(char const* hexstring);
char *itoa(int num);

// this substring didn't work = http://forums.getpebble.com/discussion/23945/solved-substring-like-function-in-c
// This Substring is adapted from https://github.com/Ludusamo/Pebble-Guide/blob/master/Pebble-Guide/src/Flashcards.h
static char *substring(char* array, int start, int end) {
  char *buffer = "      ";
  for(int i = 0; i < end; i++) {
		if ((start+i) < (int)(strlen(array))) buffer[i] = array[start+i];
  }
  APP_LOG(APP_LOG_LEVEL_DEBUG, "%s", buffer);
  return buffer;
}

