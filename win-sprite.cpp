// win-sprite.cpp : Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "win-sprite.h"

#define MAX_LOADSTRING 100
#define IDM_AUTHOR 0x01

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

HMENU hMenu;
HBITMAP hBitmap;

INT xSpriteOffset = 0;
INT ySpriteOffset = 0;

SHORT nSpriteDirection = MOVE_RIGHT;


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINSPRITE, szWindowClass, MAX_LOADSTRING);
    RegisterWindowClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINSPRITE));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM RegisterWindowClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINSPRITE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINSPRITE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
    {
    case WM_COMMAND:
        {
        int wmId = LOWORD(wParam);
        // Разобрать выбор в меню:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
		case IDM_AUTHOR:
			MessageBox(NULL, ABOUT_AUTHOR_TEXT, ABOUT_AUTHOR_TITLE, NULL);
			break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        }
        break;
    case WM_PAINT: 
        {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
		DrawSprite(ps);
        EndPaint(hWnd, &ps);
        }
        break;
	case WM_MOUSEWHEEL:
		{
		MoveSpriteOnMouseWheel(wParam);
		RECT wndRect;
		GetClientRect(hWnd, &wndRect);
		InvalidateRect(hWnd, &wndRect, TRUE);
		break;
		}
	case WM_CREATE:
		if (!LoadSprite()) {
			MessageBox(NULL, E_IMAGE_NOT_LOADED, ERROR_CAPTION, MB_OK);
			PostQuitMessage(0);
		}
		SetTimer(hWnd, IDT_SPRITE_TIMER, SPRITE_TIMER_INTERVAL, MoveSpriteOnTimer);
		AddMenus(hWnd);
		break;
    case WM_DESTROY:
		KillTimer(hWnd, IDT_SPRITE_TIMER);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
	
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

VOID AddMenus(HWND hWnd)
{
	hMenu = GetMenu(hWnd);
	AppendMenu(hMenu, MF_STRING, IDM_AUTHOR, ABOUT_AUTHOR_TITLE);
	//SetMenu(hWnd, g_hMenu);
}

BOOL LoadSprite()
{
	return (BOOL)(hBitmap = (HBITMAP)LoadImage(NULL, SPRITE_PATH, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
}

VOID DrawSprite(PAINTSTRUCT ps)
{
	HDC hMemDc;
	BITMAP bmp;
	//HBITMAP hOldBmp;
	RECT wndRect = ps.rcPaint;
	INT wndWidth = wndRect.right - wndRect.left;
	INT wndHeight = wndRect.bottom - wndRect.top;

	hMemDc = CreateCompatibleDC(ps.hdc);
	GetObject(hBitmap, sizeof(BITMAP), &bmp);
	//hOldBmp = (HBITMAP)SelectObject(hMemDc, hBitmap);
	(HBITMAP)SelectObject(hMemDc, hBitmap);
	
	GdiTransparentBlt(
		ps.hdc,
		xSpriteOffset,
		ySpriteOffset,
		bmp.bmWidth,
		bmp.bmHeight,
		hMemDc,
		0,
		0,
		bmp.bmWidth,
		bmp.bmHeight,
		MASK_TRANSPARENT);
	//SelectObject(hMemDc, hOldBmp);
	DeleteDC(hMemDc);
	//DeleteObject(hOldBmp);
}

VOID MoveSpriteOnMouseWheel(WPARAM wParam)
{
	if (GET_KEYSTATE_WPARAM(wParam) == MK_SHIFT) {
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0) {
			nSpriteDirection = MOVE_LEFT;
			//xSpriteOffset += SPRITE_MOVEMENT_STEP;
		}
		else {
			nSpriteDirection = MOVE_RIGHT;
			//xSpriteOffset -= SPRITE_MOVEMENT_STEP;
		}
	}
	else {
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0) {
			nSpriteDirection = MOVE_UP;
			//ySpriteOffset += SPRITE_MOVEMENT_STEP;
		}
		else {
			nSpriteDirection = MOVE_DOWN;
			//ySpriteOffset -= SPRITE_MOVEMENT_STEP;
		}
	}
}

VOID ProtectBorders(INT xDest, INT yDest, INT widthDest, INT heightDest, INT wndWidth, INT wndHeight)
{
	if (xDest <= 0) {
		nSpriteDirection = MOVE_RIGHT;
		//xSpriteOffset = -(widthWnd - widthDest) / 2;
	}
	else if (xDest + widthDest >= wndWidth) {
		nSpriteDirection = MOVE_LEFT;
		//xSpriteOffset = (widthWnd - widthDest) / 2;
	}
	else if (yDest <= 0) {
		nSpriteDirection = MOVE_DOWN;
		//ySpriteOffset = -(heightWnd - heightDest) / 2;
	}
	else if (yDest + heightDest >= wndHeight) {
		nSpriteDirection = MOVE_UP;
		//ySpriteOffset = (heightWnd - heightDest) / 2;
	}
}

VOID CALLBACK MoveSpriteOnTimer(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
	BITMAP bmp;
	GetObject(hBitmap, sizeof(BITMAP), &bmp);
	RECT wndRect;
	GetClientRect(hWnd, &wndRect);
	INT wndWidth = wndRect.right - wndRect.left;
	INT wndHeight = wndRect.bottom - wndRect.top;
	ProtectBorders(xSpriteOffset, ySpriteOffset, bmp.bmWidth, bmp.bmHeight, wndWidth, wndHeight);
	switch (nSpriteDirection) {
	case MOVE_UP:
		ySpriteOffset -= SPRITE_MOVEMENT_STEP;
		break;
	case MOVE_DOWN:
		ySpriteOffset += SPRITE_MOVEMENT_STEP;
		break;
	case MOVE_LEFT:
		xSpriteOffset -= SPRITE_MOVEMENT_STEP;
		break;
	case MOVE_RIGHT:
		xSpriteOffset += SPRITE_MOVEMENT_STEP;
		break;
	}
	
	InvalidateRect(hWnd, &wndRect, TRUE);
}