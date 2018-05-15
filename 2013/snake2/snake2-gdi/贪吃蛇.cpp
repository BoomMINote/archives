//*********************̰����.cpp ******************************
//����Ĵ�������Ϣ�Ĵ���
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include "resource.h"
#include "manager.h"

//ȫ�ֱ���
HINSTANCE hInst;
Game* pGame;

//��������
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

//������
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;
	MyRegisterClass(hInstance);

	// �����ʼ��
	if (!InitInstance (hInstance, nCmdShow))return FALSE;
	//��Ϣת��
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

//ע�ᴰ����
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_MY);
	wcex.hCursor		= LoadCursor(hInstance, (LPCTSTR)IDC_CUR);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;(LPCSTR)IDC_MY;
	wcex.lpszClassName	= "SnakeWindow";
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//��ʼ������
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; 

   hWnd = CreateWindowEx((unsigned)NULL,"SnakeWindow", "Snake", WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX,
      CW_USEDEFAULT, 0, 800+GetSystemMetrics(SM_CXFIXEDFRAME)*2,
	  600+GetSystemMetrics(SM_CXFIXEDFRAME)*2+GetSystemMetrics(SM_CYCAPTION), NULL, NULL, hInstance, NULL);

   if (!hWnd)return FALSE;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   //������Ϸ
	pGame=new Game(hInst,hWnd,800,600);
	//���ö�ʱ��
	SetTimer(hWnd,0,1000/FPS,NULL);


   return TRUE;
}

//��Ϣ����
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message) 
	{
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;
		case WM_TIMER:
			pGame->Loop();
			break;
		case WM_DESTROY:
			delete pGame;
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}