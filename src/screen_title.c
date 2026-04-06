#include "tafa.h"

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;

//----------------------------------------------------------------------------------
// Module Variables Definition
//----------------------------------------------------------------------------------
const char *menuItems[] = {"Hilalao", "Fikirakirana", "Hiala"};

//----------------------------------------------------------------------------------
// Title Screen Functions Definition
//----------------------------------------------------------------------------------

// Title Screen Initialization logic
void InitTitleScreen(t_title *title)
{
	framesCounter = 0;
	finishScreen = 0;

	title->menu_font_size = font.baseSize * 2.5f;
	const float menuSpacing = title->menu_font_size * 1.8f;
	float menuStartY = screen_height / 2 - menuSpacing;

	for (int i = 0; i < 3; i++)
	{
		title->text_size[i] = MeasureTextEx(font, menuItems[i], title->menu_font_size, 2);
		title->text_pos[i] = (Vector2){(screen_width - title->text_size[i].x) / 2, menuStartY + i * menuSpacing};
		title->highlight_rec[i] = (Rectangle){title->text_pos[i].x - 20, title->text_pos[i].y - 5, title->text_size[i].x + 40, title->text_size[i].y + 10};
	}

	title->hover_state = 0;
}

// Title Screen Update logic
void UpdateTitleScreen(t_title *title)
{
	if (IsKeyPressed(KEY_UP))
	{
		if (title->hover_state == 0)
			title->hover_state = 2;
		else
			title->hover_state--;
	}
	if (IsKeyPressed(KEY_DOWN))
	{
		if (title->hover_state == 2)
			title->hover_state = 0;
		else
			title->hover_state++;
	}

	for (int i = 0; i < 3; i++)
	{
		bool hovered = CheckCollisionPointRec((Vector2){mouseX, mouseY}, title->highlight_rec[i]);
		if (hovered)
			title->hover_state = i;
	}

	// Press enter or click to change to GAMEPLAY screen
	if (IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		if (title->hover_state == 0)
			finishScreen = 2; // GAMEPLAY
		if (title->hover_state == 1)
			finishScreen = 1; // OPTIONS
		if (title->hover_state == 2)
			gameShouldClose = true; // QUIT - close game
		// Play sound only if audio device is ready
		if (IsAudioDeviceReady() && fxCoinLoaded)
			PlaySound(fxCoin);
	}
}

// Title Screen Draw logic
void DrawTitleScreen(t_title title)
{
	ClearBackground(WHITE); // Use a clean white background

	const float titleFontSize = font.baseSize * 6.0f;
	Vector2 titleTextSize = MeasureTextEx(font, "TAFA", titleFontSize, 2);
	Vector2 titlePos = {(screen_width - titleTextSize.x) / 2, 60};
	DrawTextEx(font, "TAFA", titlePos, titleFontSize, 2, BLACK);

	for (int i = 0; i < 3; i++)
	{
		Color color = (title.hover_state == i) ? BLACK : GRAY;
		if (title.hover_state == i)
		{
			DrawRectangleRec(title.highlight_rec[i], BLACK);
			DrawTextEx(font, menuItems[i], title.text_pos[i], title.menu_font_size, 2, WHITE);
		}
		else
		{
			DrawTextEx(font, menuItems[i], title.text_pos[i], title.menu_font_size, 2, color);
		}
	}
}

// Title Screen Unload logic
void UnloadTitleScreen(void)
{
	// TODO: Unload TITLE screen variables here!
}

// Title Screen should finish?
int FinishTitleScreen(void)
{
	return finishScreen;
}
