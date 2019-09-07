#pragma once

#include "resource.h"

#define ABOUT_AUTHOR_TEXT L"Krestinin Vladislav, 751005\r\nhttps://github.com/IHavePortableBrain/win-sprite"
#define ABOUT_AUTHOR_TITLE L"Author"
#define SPRITE_PATH L"sprite.bmp"

#define E_IMAGE_NOT_LOADED L"Can't load image"
#define ERROR_CAPTION L"Error"
#define MASK_TRANSPARENT RGB(255, 0, 255)

#define SPRITE_MOVEMENT_STEP 5
#define MOVE_UP 0x01
#define MOVE_DOWN 0x02
#define MOVE_LEFT 0x03
#define MOVE_RIGHT 0x04
#define IDT_SPRITE_TIMER 0xFF
#define SPRITE_TIMER_INTERVAL 50


// Отправить объявления функций, включенных в этот модуль кода:
ATOM                RegisterWindowClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
VOID AddMenus(HWND);
BOOL LoadSprite();
VOID DrawSprite(PAINTSTRUCT ps);

VOID ProtectBorders(INT, INT, INT, INT, INT, INT);
VOID MoveSpriteMW(WPARAM);
VOID MoveSpriteOnMouseWheel(WPARAM);
VOID CALLBACK MoveSpriteOnTimer(HWND, UINT, UINT_PTR, DWORD);
