#include "windows.h"
#include "math.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE L"IMBOX"
#define WINDOW_CLASSNAME L"IMBOX_ONE"




HWND ghWnd = NULL;
HDC ghDc = NULL;


#define MAX 10000

#define AXISNUM 10

#define SBXL 0.1
#define SBXR 0.1
#define SBYT 0.1
#define SBYB 0.2

typedef struct tagIMPOINT
{
	double x;
	double y;
}IMPOINT;

void imbox_paint2D(double x[], double y[]);


void imboxmain()
{
	double x[MAX], y[MAX];
	for (int i = 0; i < MAX; i++)
	{
		x[i] = i * 0.01;
		y[i] = sin(x[i]);
	}


	imbox_paint2D(x, y);

}

void imbox_paint2D(double x[], double y[])
{
	double xmax = x[0], xmin = x[0];
	double ymax = y[0], ymin = y[0];
	double xaxislen = 0, yaxislen = 0;
	double xaxis[AXISNUM+1], yaxis[AXISNUM+1];
	for (int i = 0; i < MAX; i++)
	{
		if (x[i] > xmax) {
			xmax = x[i];
		}
		if (x[i] < xmin) {
			xmin = x[i];
		}
		if (y[i] > ymax) {
			ymax = y[i];
		}
		if (y[i] < ymin) {
			ymin = y[i];
		}
	}
	xaxislen = (xmax - xmin);
	yaxislen = (ymax - ymin);

	for (int i = 0; i < AXISNUM+1; i++)
	{
		xaxis[AXISNUM] = xmin + xaxislen / AXISNUM*i;
		yaxis[AXISNUM] = ymin + yaxislen / AXISNUM*i;
	}


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
		axisp[i].x = x[i];
		axisp[i].y = y[i];
	}

	double everypx = WINDOW_WIDTH * (1 - SBXL - SBXR) / xaxislen;
	double everypy = WINDOW_HEIGHT * (1 - SBYT - SBYB) / yaxislen;


	IMPOINT opa = { 0,0 }, xpa = { xaxislen,0 }, ypa = { 0,yaxislen };
	POINT osa = { (long)(WINDOW_WIDTH * SBXL + everypx * opa.x), (long)(WINDOW_HEIGHT*(1 - SBYB) - everypy * opa.y) },
		xsa = { (long)(WINDOW_WIDTH * SBXL + everypx * xpa.x), (long)(WINDOW_HEIGHT*(1 - SBYB) - everypy * xpa.y) },
		osy = { (long)(WINDOW_WIDTH * SBXL + everypx * ypa.x), (long)(WINDOW_HEIGHT*(1 - SBYB) - everypy * ypa.y) };
	

	for (int i = 0; i < MAX; i++)
	{
		screenp[i].x = (long)(WINDOW_WIDTH * SBXL + everypx * (axisp[i].x-xmin));
		screenp[i].y = (long)(WINDOW_HEIGHT*(1 - SBYB) - everypy * (axisp[i].y-ymin));
		
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

