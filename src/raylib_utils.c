#include "raylib_utils.h"

Texture2D	LoadTextureFromFile(const char *path)
{
	Image		img = LoadImage(path);
	Texture2D	tex = LoadTextureFromImage(img);
	UnloadImage(img);
	return (tex);
}

