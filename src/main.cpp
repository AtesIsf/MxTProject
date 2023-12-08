#include <cstdlib>
#include <math.h>
#include "include/rl/raylib.h"
#include "include/def.hpp"

#define MAX_SAMPLES               512
#define MAX_SAMPLES_PER_UPDATE   4096

// Cycles per second (hz)
float frequency = 0.0f;

// Audio frequency, for smoothing
float audioFrequency = 440.0f;

// Previous value, used to test if sine needs to be rewritten, and to smoothly modulate frequency
float oldFrequency = 1.0f;

// Index for audio rendering
float sineIdx = 0.0f;

short *data;

int waveLength = 1;

void AudioInputCallback(void *buffer, unsigned int frames)
{
    audioFrequency = frequency + (audioFrequency - frequency)*0.95f;
    audioFrequency += 1.0f;
    audioFrequency -= 1.0f;
    float incr = audioFrequency/44100.0f;
    short *d = (short *) buffer;

    for (unsigned int i = 0; i < frames; i++)
    {
        d[i] = (short) (32000.0f * sinf(2 * PI * sineIdx));
        sineIdx += incr;
        if (sineIdx > 1.0f) sineIdx -= 1.0f;
    }
}

void KeySelLogic(data_t *d)
{
	int key = GetKeyPressed();

	if (d->insert_mode)
		return;

	switch (key) 
	{
		case KEY_ONE:
			d->note = C;
			break;
		case KEY_TWO:
			d->note = CS;
			break;
		case KEY_THREE:
			d->note = D;
			break;
		case KEY_Q:
			d->note = DS;
			break;
		case KEY_W:
			d->note = E;
			break;
		case KEY_E:
			d->note = F;
			break;
		case KEY_A:
			d->note = FS;
			break;
		case KEY_S:
			d->note = G;
			break;
		case KEY_D:
			d->note = GS;
			break;
		case KEY_Z:
			d->note = A;
			break;
		case KEY_X:
			d->note = AS;
			break;
		case KEY_C:
			d->note = B;
			break;
		default:
			break;
	}

	if (key == KEY_TAB)
		d->is_maj = !d->is_maj;

	return;
}

void InpUpdate(data_t *d)
{
	CurrKeyStr(d);

	KeySelLogic(d);

	if (IsKeyPressed(KEY_ESCAPE))
		d->insert_mode = !(d->insert_mode);

	if (!d->insert_mode)
		return;
	
	int key = GetCharPressed();

	if (key >= 32 && key <= 126 && d->i < MAX_BUF_LEN)
	{
		d->inp_buf[d->i] = (char) key;
		d->i++;
	}

	// backspace
	if ((IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE)) && d->i > 0)
	{
		d->i--;
		d->inp_buf[d->i] = '\0'; 
	}
}

void RunUpdate(data_t *d)
{
	static int frame_counter = 0;
	static int n_ticks = 0;
	
	frequency = d->char_freq[CharIndex(d, d->inp_buf[frame_counter%28])];
	if (frequency != oldFrequency)
	{
		// Compute wavelength. Limit size in both directions.
		//int oldWavelength = waveLength;
		waveLength = (int)(22050/frequency);
		if (waveLength > MAX_SAMPLES/2) waveLength = MAX_SAMPLES/2;
		if (waveLength < 1) waveLength = 1;

		// Write sine wave
		for (int i = 0; i < waveLength * 2; i++)
		{
			data[i] = (short) (sinf(((2 * PI * (float)i / waveLength))) * 32000);
		}
		// Make sure the rest of the line is flat
		for (int j = waveLength * 2; j < MAX_SAMPLES; j++)
		{
			data[j] = (short)0;
		}

		// Scale read cursor's position to minimize transition artifacts
		//readCursor = (int)(readCursor * ((float)waveLength / (float)oldWavelength));
		oldFrequency = frequency;
	}

	frame_counter++;
	if (frame_counter == 30)
	{
		n_ticks++;
		frame_counter = 0;
		if (d->char_freq[CharIndex(d, d->inp_buf[frame_counter%28])] == '\0')
		{
			n_ticks = 0;
			d->start_program = 0;
		}
	}
}

void Update(data_t *d)
{
	if (d->start_program)
		RunUpdate(d);
	else InpUpdate(d);

	if(IsKeyPressed(KEY_ENTER))
		d->start_program = true;
}

void Draw(data_t *d)
{
	BeginDrawing();
	ClearBackground(BLACK);

	// Border
	DrawRectangleLines(40, 40, 1880, 1200, RAYWHITE);	

	// Textbox
	DrawRectangleLines(60, 60, 1840, 100, RAYWHITE);
	DrawTextEx(d->font, d->inp_buf, Vector2 {80, 80}, 50, 1, RAYWHITE);

	// Controls
	DrawTextEx(d->font, "Tab: Maj/Min", Vector2 {1350, 900}, 50, 1, RAYWHITE);
	DrawTextEx(d->font, "Note Selection:", Vector2 {1350, 950}, 50, 1, RAYWHITE);
	DrawTextEx(d->font, "1: C    2: C#   3: D", Vector2 {1350, 1000}, 50, 1, RAYWHITE);
	DrawTextEx(d->font, "q: D#   w: E    e: F", Vector2 {1350, 1050}, 50, 1, RAYWHITE);
	DrawTextEx(d->font, "a: F#   s: G    d: G#", Vector2 {1350, 1100}, 50, 1, RAYWHITE);
	DrawTextEx(d->font, "z: A    x: A#   c: B", Vector2 {1350, 1150}, 50, 1, RAYWHITE);

	// Current Key
	DrawTextEx(d->font, "Current Key:", Vector2 {60, 180}, 50, 1, RAYWHITE);
	DrawTextEx(d->font, d->key_str, Vector2 {60, 220}, 70, 1, RAYWHITE);

	if (d->start_program)
		DrawTextEx(d->font, "--Running--", Vector2 {60, 1180}, 50, 1, RAYWHITE); 
	else if (d->insert_mode) 
		DrawTextEx(d->font, "--Input Mode--", Vector2 {60, 1180}, 50, 1, RAYWHITE); 

	EndDrawing(); 
} 

data_t * Init() 
{

	InitWindow(1960, 1280, "Test");
	SetTargetFPS(60);
	InitAudioDevice();
	SetExitKey(0);

	// Taken from raylib [audio] example - Raw audio streaming

    SetAudioStreamBufferSizeDefault(MAX_SAMPLES_PER_UPDATE);

    // Init raw audio stream (sample rate: 44100, sample size: 16bit-short, channels: 1-mono)
    AudioStream stream = LoadAudioStream(44100, 16, 1);

    SetAudioStreamCallback(stream, AudioInputCallback);

    // Buffer for the single cycle waveform we are synthesizing
    data = (short *)malloc(sizeof(short)*MAX_SAMPLES);

    // Frame buffer, describing the waveform when repeated over the course of a frame
    short *writeBuf = (short *)malloc(sizeof(short)*MAX_SAMPLES_PER_UPDATE);

    PlayAudioStream(stream);        // Start processing stream buffer (no data loaded currently)

	data_t *d = new data_t;
	d->font = LoadFontEx("Assets/SpaceMono-Regular.ttf", 128, 0, 250);
	d->char_freq = new float[28];

	return d;
}

void Deinit(data_t *d)
{
	UnloadFont(d->font);

	CloseWindow();
	CloseAudioDevice();

	delete d->char_freq;
	delete d;
}

int main()
{
	data_t *d = Init();

	while (!WindowShouldClose()) 
	{
		Update(d);
		Draw(d);
	}

	Deinit(d);
	return 0;
}

