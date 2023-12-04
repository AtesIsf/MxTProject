#ifndef NOTES_HPP
#define NOTES_HPP

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

int * ScaleFormula(bool is_maj);

float * GetScale(float start, bool is_maj);

#endif
