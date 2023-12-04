#include <raylib.h>

#include "include/def.hpp"
#include "include/notes.hpp"

void KeySelLogic(data_t *d)
{
	int key = GetKeyPressed();

	switch (key) 
	{
		case KEY_F1:
			d->note = C;
			break;
		case KEY_F2:
			d->note = CS;
			break;
		case KEY_F3:
			d->note = D;
			break;
		case KEY_F4:
			d->note = DS;
			break;
		case KEY_F5:
			d->note = E;
			break;
		case KEY_F6:
			d->note = F;
			break;
		case KEY_F7:
			d->note = FS;
			break;
		case KEY_F8:
			d->note = G;
			break;
		case KEY_F9:
			d->note = GS;
			break;
		case KEY_F10:
			d->note = A;
			break;
		case KEY_F11:
			d->note = AS;
			break;
		case KEY_F12:
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
	DrawTextEx(d->font, "Note Selection:", Vector2 {1350, 950}, 50, 1, RAYWHITE);
	DrawTextEx(d->font, " F1: C    F2: C#   F3: D", Vector2 {1350, 1000}, 50, 1, RAYWHITE);
	DrawTextEx(d->font, " F4: D#   F5: E    F6: F", Vector2 {1350, 1050}, 50, 1, RAYWHITE);
	DrawTextEx(d->font, " F7: F#   F8: G    F9: G#", Vector2 {1350, 1100}, 50, 1, RAYWHITE);
	DrawTextEx(d->font, "F10: A   F11: A#  F12: B", Vector2 {1350, 1150}, 50, 1, RAYWHITE);

	if (d->start_program)
		DrawTextEx(d->font, "--Running--", Vector2 {60, 1180}, 50, 1, RAYWHITE); 
	else if (d->insert_mode) 
		DrawTextEx(d->font, "--Input Mode--", Vector2 {60, 1180}, 50, 1, RAYWHITE); 

	EndDrawing(); 
} 

data_t * Init() {

	InitWindow(1960, 1280, "Test");
	SetTargetFPS(60);
	InitAudioDevice();
	SetExitKey(0);

	data_t *d = new data_t;
	d->font = LoadFontEx("Assets/SpaceMono-Regular.ttf", 128, 0, 250);

	return d;
}

void Deinit(data_t *d)
{
	UnloadFont(d->font);

	CloseWindow();
	CloseAudioDevice();

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

