// PathJumpPoint.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "PathJumpPoint.h"

#include "JumpPoint.h"
#include "BresenhamLine.h"

#define MAX_LOADSTRING 100


enum MODE
{
   START_END = 0,
   CREATE_BLOCK,
   INFO
};

MODE g_eMode;

JumpPoint g_CJump;
// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PATHJUMPPOINT));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"Graphic";
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    RegisterClassExW(&wcex);


    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    HWND hWnd = CreateWindowW(L"Graphic", L"Graphic", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

	MSG msg;

	// 기본 메시지 루프입니다:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
bool g_bIsDrag = false;
HDC	g_hMemDC;
HBITMAP	g_hMemBitmap;
HBITMAP g_hMemBitmapOld;
RECT Rect;
int moveX = -1;
int moveY = -1;
NODE* g_node;

bool isPlay = false;


//void DrawLine2(HDC hdc)
//{
//    std:list<stPOINT*>::iterator iter;
//    for (iter = testList.begin();;)
//    {
//        MoveToEx(hdc, ((*iter)->iX * TILE_SIZE + 7) + (TILE_SIZE / 2), ((*iter)->iY * TILE_SIZE) + (TILE_SIZE / 2), NULL);
//
//        ++iter;
//        if (iter == testList.end())
//            break;
//
//        LineTo(hdc, ((*iter)->iX * TILE_SIZE + 7) + (TILE_SIZE / 2), ((*iter)->iY * TILE_SIZE) + (TILE_SIZE / 2));
//    }
//
//}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {

    case WM_CREATE:
    {
        HDC hdc = GetDC(hWnd);
        GetClientRect(hWnd, &Rect);
        g_hMemDC = CreateCompatibleDC(hdc);
        g_hMemBitmap = CreateCompatibleBitmap(hdc, Rect.right, Rect.bottom);
        g_hMemBitmapOld = (HBITMAP)SelectObject(g_hMemDC, g_hMemBitmap);
        g_eMode = MODE::CREATE_BLOCK;

        ReleaseDC(hWnd, hdc);
    }
    break;
    case WM_TIMER:
        switch (wParam)
        {
        case 0:
            if (g_CJump.Search(hWnd) == true)
            {
                KillTimer(hWnd, 0);
                isPlay = false;
            }
            break;
        }
        break;

    case WM_CHAR:
        if (GetAsyncKeyState(VK_SPACE))
        {

            if (isPlay == false)
            {
                g_CJump.ClearMap();
                SetTimer(hWnd, 0, 1000, NULL);
                isPlay = true;
            }
        }

        if (GetAsyncKeyState(VK_TAB))
        {
            switch (g_eMode)
            {
            case START_END:
                g_eMode = MODE::INFO;
                break;
            case CREATE_BLOCK:
                g_eMode = MODE::START_END;
                break;
            case INFO:
                g_eMode = MODE::CREATE_BLOCK;
                break;
            }

            InvalidateRect(hWnd, NULL, FALSE);
        }

        break;
    case WM_LBUTTONDOWN:
        moveX = LOWORD(lParam) / TILE_SIZE;
		moveY = HIWORD(lParam) / TILE_SIZE;

		switch (g_eMode)
		{
		case START_END:
			g_CJump.CreateStart(moveX, moveY);
			break;
		case CREATE_BLOCK:
			g_bIsDrag = true;
			g_CJump.CreateBlock(moveX, moveY);
			break;
		case INFO:

			g_node = NULL;
			g_node = g_CJump.FindNode(moveX, moveY);
			break;
		}

        InvalidateRect(hWnd, NULL, FALSE);
        break;

    case WM_RBUTTONDOWN:
        if (g_eMode == MODE::START_END)
        {
            moveX = LOWORD(lParam) / TILE_SIZE;
            moveY = HIWORD(lParam) / TILE_SIZE;
            g_CJump.CreateEnd(moveX, moveY);
            InvalidateRect(hWnd, NULL, FALSE);
        }
        break;

    case WM_LBUTTONUP:
        g_bIsDrag = false;
        break;

    case WM_MOUSEMOVE:
        if (g_bIsDrag == true)
        {
            if (moveX != LOWORD(lParam) / TILE_SIZE || moveY != HIWORD(lParam) / TILE_SIZE)
            {
                moveX = LOWORD(lParam) / TILE_SIZE;
                moveY = HIWORD(lParam) / TILE_SIZE;

                g_CJump.CreateDrag(moveX, moveY);
                InvalidateRect(hWnd, NULL, FALSE);
            }
        }
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        PatBlt(g_hMemDC, 0, 0, Rect.right, Rect.bottom, WHITENESS);

        g_CJump.Draw(g_hMemDC);
        g_CJump.DrawLine(g_hMemDC);


        TextOut(g_hMemDC, 1025, 20, L"모드변환키 : Tab", 11);

        TextOut(g_hMemDC, 1025, 0, L"모드 : ", 5);

        TextOut(g_hMemDC, 1025, 40, L"탐색 : Space ", 11);

        if (g_eMode == MODE::CREATE_BLOCK)
        {
            TextOut(g_hMemDC, 1070, 0, L"블럭생성", 4);
            TextOut(g_hMemDC, 1025, 60, L"L 클릭 : 블럭생성", 11);
        }
        else if (g_eMode == MODE::START_END)
        {
            TextOut(g_hMemDC, 1070, 0, L"Start/End", 9);
            TextOut(g_hMemDC, 1025, 60, L"L 클릭 : 시작지점", 11);
            TextOut(g_hMemDC, 1025, 80, L"R 클릭 : 도착지점", 11);
        }
        else if(g_eMode == MODE::INFO)
        {
            TextOut(g_hMemDC, 1070, 0, L"Info", 4);
            TextOut(g_hMemDC, 1025, 60, L"L 클릭 : 블럭정보", 11);


            if (g_node != NULL)
            {
                WCHAR info[20];

                swprintf_s(info, L"X = %d", g_node->iX);
                TextOut(g_hMemDC, 1025, 80, info, lstrlenW(info));

                swprintf_s(info, L"Y = %d", g_node->iY);
                TextOut(g_hMemDC, 1025, 100, info, lstrlenW(info));


                swprintf_s(info, L"G = %f", g_node->fG);
                TextOut(g_hMemDC, 1025, 120, info, lstrlenW(info));

                swprintf_s(info, L"H = %f", g_node->fH);
                TextOut(g_hMemDC, 1025, 140, info, lstrlenW(info));

                swprintf_s(info, L"F = %f", g_node->fF);
                TextOut(g_hMemDC, 1025, 160, info, lstrlenW(info));
            }
        }

        BitBlt(hdc, 0, 0, Rect.right, Rect.bottom, g_hMemDC, 0, 0, SRCCOPY);

        EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY:
        SelectObject(g_hMemDC, g_hMemBitmapOld);
        DeleteObject(g_hMemBitmap);
        DeleteObject(g_hMemDC);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

