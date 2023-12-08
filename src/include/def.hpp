#ifndef SOUND_DEF_HPP
#define SOUND_DEF_HPP 

#include "rl/raylib.h" 

#define MAX_BUF_LEN 85

#define C 261.64f
#define CS 277.18f
#define D 293.66f
#define DS 311.13f
#define E 329.64f
#define F 349.23f
#define FS 369.99f
#define G 392.0f
#define GS 415.30f
#define A 440.0f
#define AS 466.16f
#define B 493.88f

typedef struct Data
{
	// 1 -> int, 0 -> randim
	bool is_interval = false;
	// acts as a lock
	bool start_program = false;
	bool insert_mode = false;

	char inp_buf[MAX_BUF_LEN + 1] = {};
	// index for inp_buf
	unsigned int i = 0;
	// all letters + space & full stop
	// Indexes -> a to z, last 2 are space and full stop
	float *char_freq;
	
	float note = C;
	bool is_maj = 1;

	char key_str[4] = {};

	Font font;
} data_t;

int * ScaleFormula(bool is_maj);

float * GetScale(float start, bool is_maj);

void CurrKeyStr(data_t *d);

void NoteCharMatchup(data_t *d);

int CharIndex(data_t *d, char c);

#endif

