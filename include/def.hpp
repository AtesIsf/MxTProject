#ifndef SOUND_DEF_HPP
#define SOUND_DEF_HPP 

#include <raylib.h>

#include "notes.hpp"

#define MAX_BUF_LEN 85

typedef struct Data
{
	unsigned short bpm = 60;
	// 1 -> int, 0 -> randim
	bool is_interval = false;
	// acts as a lock
	bool start_program = false;
	bool insert_mode = false;

	char inp_buf[MAX_BUF_LEN + 1] = {};
	// index for inp_buf
	unsigned int i = 0;
	// all letters + space & full stop
	float char_freq[28];
	
	float note = C;
	bool is_maj = 1;

	Font font;
} data_t;

#endif

