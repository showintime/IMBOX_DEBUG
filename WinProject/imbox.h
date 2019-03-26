#pragma once
#include "windows.h"
#include "math.h"
#include "stdio.h"












#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE L"IMBOX"
#define WINDOW_CLASSNAME L"IMBOX_ONE"

#define PI 3.1415926535


HWND ghWnd = NULL;
HDC ghDc = NULL;


#define MAX 10000

#define AXISNUM 5

#define SBXL 0.1
#define SBXR 0.1
#define SBYT 0.1
#define SBYB 0.2


typedef struct tagIMPOINT
{
	double x;
	double y;
}IMPOINT;

typedef struct tagNUME
{
	double ea;
	int en;
}NUME;



void imbox_paint2D(double IMx[], double IMy[]);
void imboxmain();
void imbox_init(HWND hWnd);
void imbox_clean();
void countfn(double x,NUME *nume);
LRESULT CALLBACK WndProc(
	HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam
);


void countfn(double x,NUME *nume)
{

	
	int flag = 1;
	if (x < 0) {
		flag = -1;
	}

	double fx = fabs(x);
	int b = 0;
	if ((fx < 1) &&(fx > 0)) {
		b = 0;
		while (fx < 1) {
			fx *= 10;
			b++;
		}
		nume->ea = flag*fx;
		nume->en = -b;
	}
	else if(fx>=10){
		b = 0;
		while (fx >= 10) {
			fx /= 10;
			b++;
		}
		nume->ea = flag * fx;
		nume->en = b;
	}
	else {
		nume->ea = fx;
		nume->en = 0;
	}




}



void imbox_paint2D(double IMx[], double IMy[])
{
	double xmax = IMx[0], xmin = IMx[0];
	double ymax = IMy[0], ymin = IMy[0];
	double xaxislen = 0, yaxislen = 0;
	double xaxis[AXISNUM + 1], yaxis[AXISNUM + 1];
	wchar_t xaxislabel[10],yaxislabel[10];
	for (int i = 0; i < MAX; i++)
	{
		if (IMx[i] > xmax) {
			xmax = IMx[i];
		}
		if (IMx[i] < xmin) {
			xmin = IMx[i];
		}
		if (IMy[i] > ymax) {
			ymax = IMy[i];
		}
		if (IMy[i] < ymin) {
			ymin = IMy[i];
		}
	}
	xaxislen = (xmax - xmin);
	yaxislen = (ymax - ymin);



	


	Rectangle(ghDc,
		SBXL*WINDOW_WIDTH,
		SBYT*WINDOW_HEIGHT,
		(1 - SBXL)*WINDOW_WIDTH,
		(1 - SBYB)*WINDOW_HEIGHT
	);


	IMPOINT axisp[MAX] = { 0 };
	POINT screenp[MAX] = { 0 };
	POINT clientp[MAX] = { 0 };
	for (int i = 0; i < MAX; i++)
	{
		axisp[i].x = IMx[i];
		axisp[i].y = IMy[i];
	}

	double everypx = WINDOW_WIDTH * (1 - SBXL - SBXR) / xaxislen;
	double everypy = WINDOW_HEIGHT * (1 - SBYT - SBYB) / yaxislen;


	IMPOINT opa = { 0,0 }, xpa = { xaxislen,0 }, ypa = { 0,yaxislen };
	POINT osa = { (long)(WINDOW_WIDTH * SBXL + everypx * opa.x), (long)(WINDOW_HEIGHT*(1 - SBYB) - everypy * opa.y) },
		xsa = { (long)(WINDOW_WIDTH * SBXL + everypx * xpa.x), (long)(WINDOW_HEIGHT*(1 - SBYB) - everypy * xpa.y) },
		osy = { (long)(WINDOW_WIDTH * SBXL + everypx * ypa.x), (long)(WINDOW_HEIGHT*(1 - SBYB) - everypy * ypa.y) };


	for (int i = 0; i < MAX; i++)
	{
		screenp[i].x = (long)(WINDOW_WIDTH * SBXL + everypx * (axisp[i].x - xmin));
		screenp[i].y = (long)(WINDOW_HEIGHT*SBYT + everypy * (ymax - axisp[i].y));

	}


	double temxaxis, temyaxis;
	NUME numex[AXISNUM+1] = { 0 }, numey[AXISNUM+1] = { 0 };

	SetBkMode(ghDc, TRANSPARENT);
	for (int i = 0; i < AXISNUM + 1; i++)
	{
		xaxis[i] = xmin + xaxislen / AXISNUM * i;
		yaxis[i] = ymin + yaxislen / AXISNUM * i;

		temxaxis = (WINDOW_WIDTH * SBXL + everypx * (xaxis[i] - xmin));
		temyaxis = (WINDOW_HEIGHT*SBYT + everypy * (ymax - yaxis[i]));

		countfn(xaxis[i], &numex[i]);
		countfn(yaxis[i], &numey[i]);

		swprintf(xaxislabel, 10, L"%5.3fE%3d", numex[i].ea, numex[i].en);
		swprintf(yaxislabel, 10, L"%5.3fE%3d", numey[i].ea, numey[i].en);


		TextOut(ghDc, temxaxis, (1 - SBYB)*WINDOW_HEIGHT, xaxislabel, sizeof(xaxislabel)/2);
		TextOut(ghDc, SBXL*WINDOW_WIDTH - 7 * (AXISNUM + 1), temyaxis, yaxislabel, sizeof(yaxislabel)/2);

	}



	
	



	for (int i = 0; i < MAX; i++)
	{

		SetPixel(ghDc, screenp[i].x, screenp[i].y, RGB(8, 255, 8));
	}


	/*MoveToEx(ghDc, screenp[0].x, screenp[0].y, NULL);
	for (int i = 0; i < MAX; i++)
	{
		LineTo(ghDc, screenp[i].x, screenp[i].y);
	}*/


}






void imbox_init(HWND hWnd)
{
	ghWnd = hWnd;
}





void imbox_clean()
{





}





LRESULT CALLBACK WndProc(
	HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam
)
{
	PAINTSTRUCT ps = { 0 };
	switch (msg)
	{
	case WM_PAINT:


		ghDc = BeginPaint(hWnd, &ps);

		imboxmain();

		EndPaint(hWnd, &ps);

		ValidateRect(hWnd, NULL);

		break;
	case WM_KEYDOWN:

		break;
	case WM_DESTROY:

		imbox_clean();
		PostQuitMessage(0);
		break;

	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}




int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPreHinstance,
	LPSTR lpCmdLine,
	int nCmdShow
)
{



	WNDCLASSEX wcx = { 0 };
	wcx.cbSize = sizeof(WNDCLASSEX);
	wcx.hInstance = hInstance;
	wcx.lpfnWndProc = WndProc;
	wcx.style = CS_VREDRAW | CS_HREDRAW;
	wcx.lpszClassName = WINDOW_CLASSNAME;


	RegisterClassEx(&wcx);


	HWND hWnd = CreateWindow(
		WINDOW_CLASSNAME,
		WINDOW_TITLE,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
	);




	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	imbox_init(hWnd);


	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		GetMessage(&msg, NULL, NULL, NULL);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}


	UnregisterClass(WINDOW_CLASSNAME, hInstance);

	return msg.wParam;

}

