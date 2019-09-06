#pragma once

#include "resource.h"

#define ABOUT_AUTHOR_TEXT L"Krestinin Vladislav, 751005\r\nhttps://github.com/IHavePortableBrain/win-sprite"
#define ABOUT_AUTHOR_TITLE L"Author"
#define SPRITE_PATH L"sprite.bmp"

#define E_IMAGE_NOT_LOADED L"Can't load image"
#define ERROR_CAPTION L"Error"
#define MASK_TRANSPARENT RGB(255, 0, 255)

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                RegisterWindowClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
VOID AddMenus(HWND);
BOOL LoadSprite();
VOID DrawSprite(PAINTSTRUCT ps);
