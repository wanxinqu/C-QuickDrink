// firstpage.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "firstpage.h"
#include "MP3.h"
#include <math.h>
#include <Windows.h>
#include <ObjBase.h>
#include <math.h>
//注意：要想放音乐，要添加这两个头文件
#include <MMSystem.h>
#pragma comment(lib,"Winmm.lib")


#define MAX_LOADSTRING 100
#define R			   750
#define Pi             3.14
#define ID_TIMER1      1
#define ID_TIMER2      2
#define NUMBITMAP      7


// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
static int cxClient,cyClient;

static int cxSource[NUMBITMAP],cySource[NUMBITMAP];
static HBITMAP hBitmap[NUMBITMAP];
HDC hdcMem[NUMBITMAP];

//"display"和"exit"按钮
static int cxButton[4],cyButton[4];
static HBITMAP hButtonBitmap[4];
HDC hdcButtonMem[4];

//女孩嘴唇图片
static int cxGirlMouth[2],cyGirlMouth[2];
static HBITMAP hGirlMouthBitmap[2];
HDC hdcGirlMouthMem[2];

//睫毛图片
static int cxEyeLash[2],cyEyeLash[2];
static HBITMAP hEyeLashBitmap[2];
HDC hdcEyeMem[2];

//橘子图片
static int cxOrangeSource[6],cyOrangeSource[6];
static HBITMAP hOrangeBitmap[6];
HDC hdcOrangeMem[6];

static int CONTROL; 
//男孩嘴唇图片
static HBITMAP hMainBitmap[4];
BITMAP MainBitmap;
static int MainCxScource[4],MainCyScource[4];
HDC hdcMainMem[4];

static int x;
static int s;
static BOOL bOk1 = FALSE; //boy
static BOOL bOk2 = FALSE; //girl
static int d1;//boy
static int d2;//girl
static BOOL Go = TRUE;
static TCHAR szTitleName[] = TEXT("狂喝汽水比赛-wanxinqu@qq.com");

//定义mp3类对象
CMP3 g_theMP3Player;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
VOID CALLBACK TimerProc1(HWND,UINT,UINT,DWORD);
VOID CALLBACK TimerProc2(HWND,UINT,UINT,DWORD);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	CoInitialize(NULL);
	

	lstrcpy(szTitle,szTitleName);
	
//	LoadString(hInstance, IDS_APP_TITLE, szTitle
//		, MAX_LOADSTRING);
	LoadString(hInstance, IDC_FIRSTPAGE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FIRSTPAGE));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	CoUninitialize();
	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FIRSTPAGE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_FIRSTPAGE);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass,szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 800, 500, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	BITMAP bitmap;
	HINSTANCE hInstance;
	int i;
	static POINT pt;
	RECT rect;
	RECT rect1[4];
	static int drink;

	static TCHAR* szBackMusic = TEXT("backmusic.wav");
	
//	SetRect(&rect,140,157,358,800);
	
	SetRect(&rect,283,300,360,800);
	SetRect(&rect1[0],140,172,175,182);
	SetRect(&rect1[1],250,172,285,182);
	SetRect(&rect1[2],140,157,175,167);
	SetRect(&rect1[3],250,182,285,192);

//	static int cxClient,cyClient;
	
	switch (message)
	{
	case WM_SIZE:
		{
			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);
			return 0;
		}
	case WM_CREATE:
		{
			
			hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
			hdc = GetDC(hWnd);
						//szBackMusic
			//::PlaySound(_T("HelloWin.wav"),NULL,SND_FILENAME|SND_SYNC);
			g_theMP3Player.Load(L"backmusic.wav");
			g_theMP3Player.Play();

			hMainBitmap[0] = LoadBitmap(hInstance,TEXT("fifth"));
			hMainBitmap[1] = LoadBitmap(hInstance,TEXT("nineth"));
			hMainBitmap[2] = LoadBitmap(hInstance,TEXT("tenth"));
			hMainBitmap[3] = LoadBitmap(hInstance,TEXT("eleventh"));
			for (i=0;i<4;i++)
			{
				GetObject(hMainBitmap[i],sizeof(BITMAP),&MainBitmap);
				MainCxScource[i] = MainBitmap.bmWidth;
				MainCyScource[i] = MainBitmap.bmHeight;
				hdcMainMem[i] = CreateCompatibleDC(hdc);
				SelectObject(hdcMainMem[i],hMainBitmap[i]);
			}
			
			
			hBitmap[0] = LoadBitmap(hInstance,TEXT("first"));
			hBitmap[1] = LoadBitmap(hInstance,TEXT("second"));
			hBitmap[2] = LoadBitmap(hInstance,TEXT("third"));
			hBitmap[3] = LoadBitmap(hInstance,TEXT("forth"));
			hBitmap[4] = LoadBitmap(hInstance,TEXT("sixth"));
			hBitmap[5] = LoadBitmap(hInstance,TEXT("seventh"));
			hBitmap[6] = LoadBitmap(hInstance,TEXT("eighth"));
			for(i=0;i<7;i++)
			{
				GetObject(hBitmap[i],sizeof(BITMAP),&bitmap);
				cxSource[i] = bitmap.bmWidth;
				cySource[i] = bitmap.bmHeight;
				hdcMem[i] = CreateCompatibleDC(hdc);
				SelectObject(hdcMem[i],hBitmap[i]);
			}

			//按钮动画
			hButtonBitmap[0] = LoadBitmap(hInstance,TEXT("mybuttongirldisplay"));
			hButtonBitmap[1] = LoadBitmap(hInstance,TEXT("mybuttongirlexit"));
			hButtonBitmap[2] = LoadBitmap(hInstance,TEXT("mybuttonboydisplay"));
			hButtonBitmap[3] = LoadBitmap(hInstance,TEXT("mybuttonboyexit"));

			for(i=0;i<4;i++)
			{
				GetObject(hButtonBitmap[i],sizeof(BITMAP),&bitmap);
				cxButton[i] = bitmap.bmWidth;
				cyButton[i] = bitmap.bmHeight;
				hdcButtonMem[i] = CreateCompatibleDC(hdc);
				SelectObject(hdcButtonMem[i],hButtonBitmap[i]);
			}

			//嘴唇动画
			hGirlMouthBitmap[0] = LoadBitmap(hInstance,TEXT("bigmouth"));
			hGirlMouthBitmap[1] = LoadBitmap(hInstance,TEXT("smallmouth"));

			for (i=0;i<2;i++)
			{
				GetObject(hGirlMouthBitmap[i],sizeof(BITMAP),&bitmap);
				cxGirlMouth[i] = bitmap.bmWidth;
				cyGirlMouth[i] = bitmap.bmHeight;
				hdcGirlMouthMem[i] = CreateCompatibleDC(hdc);
				SelectObject(hdcGirlMouthMem[i],hGirlMouthBitmap[i]);
			}	
			
			//睫毛动画
			hEyeLashBitmap[0] = LoadBitmap(hInstance,TEXT("lefteyelash"));
			hEyeLashBitmap[1] = LoadBitmap(hInstance,TEXT("righteyelash"));

			for(i=0;i<2;i++)
			{
				GetObject(hEyeLashBitmap[i],sizeof(BITMAP),&bitmap);
				cxEyeLash[i] = bitmap.bmWidth;
				cyEyeLash[i] = bitmap.bmHeight;
				hdcEyeMem[i] = CreateCompatibleDC(hdc);
				SelectObject(hdcEyeMem[i],hEyeLashBitmap[i]);
			}

			//橘子动画
			hOrangeBitmap[0] = LoadBitmap(hInstance,TEXT("boy1"));
			hOrangeBitmap[1] = LoadBitmap(hInstance,TEXT("boy2"));
			hOrangeBitmap[2] = LoadBitmap(hInstance,TEXT("boy3"));
			hOrangeBitmap[3] = LoadBitmap(hInstance,TEXT("girl1"));
			hOrangeBitmap[4] = LoadBitmap(hInstance,TEXT("girl2"));
			hOrangeBitmap[5] = LoadBitmap(hInstance,TEXT("girl3"));

			for(i=0;i<6;i++)
			{
				GetObject(hOrangeBitmap[i],sizeof(BITMAP),&bitmap);
				cxOrangeSource[i] = bitmap.bmWidth;
				cyOrangeSource[i] = bitmap.bmHeight;
				hdcOrangeMem[i] = CreateCompatibleDC(hdc);
				SelectObject(hdcOrangeMem[i],hOrangeBitmap[i]);
			}

			

			SetTimer(hWnd,ID_TIMER1,150,TimerProc1);
		//	SetTimer(hWnd,ID_TIMER2,200,TimerProc2);
			
			return 0;
		}
	case WM_PAINT:
		{
			hdc = BeginPaint(hWnd,&ps);
		
			if(CONTROL==1)
			{
/*				SelectObject(hdc,GetStockObject(NULL_PEN));
				HBRUSH hBrush;
				hBrush = CreateSolidBrush(RGB(200,200,0));
				SelectObject(hdc,hBrush);
				Ellipse(hdc,250,250,550,400);*/
				HDC hdc1;
				hdc1 = GetDC(hWnd);
				BitBlt(hdc1,285,250,cxSource[3],cySource[3],hdcMem[3],0,0,SRCCOPY);
		
			}
			else if(CONTROL==2)
			{			
				//游戏的背景图片
				BitBlt(hdc,0,-35,MainCxScource[0],MainCyScource[0],hdcMainMem[0],0,0,SRCCOPY);		
			}
			else if(CONTROL==3)
			{
				//男孩胜利的图片
				BitBlt(hdc,-20,-30,MainCxScource[2],MainCyScource[2],hdcMainMem[2],0,0,SRCCOPY);
				SelectObject(hdc,CreatePen(PS_SOLID,3,RGB(255,255,0)));		
			}
			else if(CONTROL==4)
			{
				//女孩胜利的图片
				BitBlt(hdc,-20,-30,MainCxScource[1],MainCyScource[1],hdcMainMem[1],0,0,SRCCOPY);
				SelectObject(hdc,CreatePen(PS_SOLID,3,RGB(255,255,0)));		
			}
			
			
			EndPaint(hWnd,&ps);
			return 0;
		}
	case WM_CHAR:
		{
			switch(wParam)  //这是走CONTROL==2
			{
			case 115:
				{   
					//drink 是用来确保's'和'x'两个键都按才能喝饮料
					if (Go&&drink==1&&CONTROL!=1&&CONTROL!=0)
					{
						InvalidateRect(hWnd,&rect,TRUE);
						InvalidateRect(hWnd,&rect1[2],TRUE);
						InvalidateRect(hWnd,&rect1[3],TRUE);
						
						UpdateWindow(hWnd);

						d1++;
						HDC hdc2;
						hdc2 = GetDC(hWnd);
						SelectObject(hdc2,GetStockObject(BLACK_PEN));
						SelectObject(hdc2,GetStockObject(BLACK_BRUSH));


						Rectangle(hdc2,140,172,175,182);
						Rectangle(hdc2,250,172,285,182);
						DeleteObject(SelectObject(hdc2,GetStockObject(BLACK_PEN)));
						DeleteObject(SelectObject(hdc2,GetStockObject(BLACK_BRUSH)));

						HBRUSH hBrush;
						hBrush = CreateSolidBrush(RGB(176,194,192));
						SelectObject(hdc2,hBrush);
						HPEN hPen;
						hPen = CreatePen(PS_SOLID,2,RGB(176,194,192));
						SelectObject(hdc2,hPen);
						Rectangle(hdc2,284,345,358,345+2*d1);
						//	Rectangle(hdc2,435.5,341,509.5,341+2*d);

						BitBlt(hdc2,220,237,cxSource[5],cySource[5],hdcMem[5],0,0,SRCCOPY);

						x++;
						if(x==50&&(s<50||s==50))
						{
							//	MessageBox(hWnd,TEXT("Boy"),NULL,NULL);
							KillTimer(hWnd,ID_TIMER2);
							CONTROL = 3;
							x=0;
							s=0;
							d1=0;
							d2=0;
							Go = FALSE;
							InvalidateRect(hWnd,NULL,TRUE);
							UpdateWindow(hWnd);
						}

						drink = 0;
						DeleteObject(hBrush);
						DeleteObject(hPen);

						UpdateWindow(hWnd);
						break;
					}
					break;
			
				}
			case 120:
				{
					if (Go&&drink==0&&CONTROL!=1&&CONTROL!=0)  //添加CONTROL！=1为防止在程序运行前按's'和'x'键
					{
						InvalidateRect(hWnd,&rect,TRUE);
						InvalidateRect(hWnd,&rect1[0],TRUE);
						InvalidateRect(hWnd,&rect1[1],TRUE);
						UpdateWindow(hWnd);

						HDC hdc3;
						hdc3 = GetDC(hWnd);

						SelectObject(hdc3,GetStockObject(BLACK_PEN));
						SelectObject(hdc3,GetStockObject(BLACK_BRUSH));


						Rectangle(hdc3,140,157,175,167);
						Rectangle(hdc3,250,182,285,192);
						DeleteObject(SelectObject(hdc3,GetStockObject(BLACK_PEN)));
						DeleteObject(SelectObject(hdc3,GetStockObject(BLACK_BRUSH)));



						HBRUSH hBrush;
						hBrush = CreateSolidBrush(RGB(176,194,192));
						SelectObject(hdc3,hBrush);
						HPEN hPen;
						hPen = CreatePen(PS_SOLID,2,RGB(176,194,192));
						SelectObject(hdc3,hPen);
						Rectangle(hdc3,284,345,358,345+2*d1);
						//	Rectangle(hdc3,435.5,341,509.5,341+2*d);
						BitBlt(hdc3,220,240,cxSource[6],cySource[6],hdcMem[6],0,0,SRCCOPY);

						drink = 1;
						DeleteObject(hBrush);
						DeleteObject(hPen);

						UpdateWindow(hWnd);

						break;
				
					}
				}	
			}
		}
	case WM_LBUTTONDOWN:
		{
			if(pt.x>250&&pt.x<550&&pt.y>250&&pt.y<400&&CONTROL==1)
			{		
				CONTROL = 2;
				InvalidateRect(hWnd,NULL,TRUE);
				UpdateWindow(hWnd);
				Sleep(1000);

				SetTimer(hWnd,ID_TIMER2,300,TimerProc2);

			}
			else if(pt.x>560&&pt.x<800&&pt.y>270&&pt.y<340&&CONTROL==3)
			{
				CONTROL = 2;
				InvalidateRect(hWnd,NULL,TRUE);
				UpdateWindow(hWnd);
				Sleep(1000);
				Go = TRUE;
				SetTimer(hWnd,ID_TIMER2,300,TimerProc2);
			}
			else if (pt.x>560&&pt.x<800&&pt.y>370&&pt.y<500&&CONTROL==3)
			{
				HDC hdc4;
				hdc4 = GetDC(hWnd);
				KillTimer(hWnd,ID_TIMER1);
				KillTimer(hWnd,ID_TIMER2);
				//BitBlt(hdc4,0,-70,MainCxScource[3],MainCyScource[3],hdcMainMem[3],0,0,SRCCOPY);
				//Sleep(6000);
				PostQuitMessage(0);
				break;
			}
			else if(pt.x>600&&pt.x<800&&pt.y>300&&pt.y<370&&CONTROL==4)
			{
				CONTROL = 2;
				InvalidateRect(hWnd,NULL,TRUE);
				UpdateWindow(hWnd);
				Sleep(1000);
				Go = TRUE;
				SetTimer(hWnd,ID_TIMER2,300,TimerProc2);
			}
			else if (pt.x>600&&pt.x<800&&pt.y>400&&pt.y<500&&CONTROL==4)
			{
				HDC hdc4;
				hdc4 = GetDC(hWnd);
				KillTimer(hWnd,ID_TIMER1);
				KillTimer(hWnd,ID_TIMER2);
			//	BitBlt(hdc4,0,-70,MainCxScource[3],MainCyScource[3],hdcMainMem[3],0,0,SRCCOPY);
			//	Sleep(6000);
				PostQuitMessage(0);
				break;
			}
		}
	case WM_MOUSEMOVE:  //250,250,550,400
		{		
			HDC hdcmove = GetDC(hWnd);
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);

			switch(CONTROL)
			{
			case 1:  //注意：control==1时，是进入游戏的界面“GO”
				{				
					if(pt.x>300&&pt.x<550&&pt.y>265&&pt.y<400)
					{
						BitBlt(hdcmove,285,250,cxSource[4],cySource[4],hdcMem[4],0,0,SRCCOPY);
					}
					else
					{
						BitBlt(hdcmove,285,250,cxSource[3],cySource[3],hdcMem[3],0,0,SRCCOPY);
					}
					break;
				}
			case 3:  //男孩胜
				{

					if(pt.x>560&&pt.x<800&&pt.y>270&&pt.y<340) //点击display
					{
						BitBlt(hdcmove,-20,-30,cxButton[2],cyButton[2],hdcButtonMem[2],0,0,SRCCOPY);
					}
					else if(pt.x>560&&pt.x<800&&pt.y>370&&pt.y<500)
					{
						BitBlt(hdcmove,-20,-30,cxButton[3],cyButton[3],hdcButtonMem[3],0,0,SRCCOPY);
					}
					else
					{
						BitBlt(hdcmove,-20,-30,MainCxScource[2],MainCyScource[2],hdcMainMem[2],0,0,SRCCOPY);
					}
					break;
				}
			case 4:  //女孩胜
				{
					if(pt.x>600&&pt.x<800&&pt.y>300&&pt.y<370) //点击display
					{
						BitBlt(hdcmove,-20,-30,cxButton[0],cyButton[0],hdcButtonMem[0],0,0,SRCCOPY);
					}
					else if(pt.x>600&&pt.x<800&&pt.y>400&&pt.y<500)
					{
						BitBlt(hdcmove,-20,-30,cxButton[1],cyButton[1],hdcButtonMem[1],0,0,SRCCOPY);
					}
					else
					{
						BitBlt(hdcmove,-20,-30,MainCxScource[1],MainCyScource[1],hdcMainMem[1],0,0,SRCCOPY);
					}
					break;
				}
			}	
			
		}
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_DESTROY:
		{
			KillTimer(hWnd,ID_TIMER2);
			PostQuitMessage(0);
			break;
		}		
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

// Message handler for about box.
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


VOID CALLBACK TimerProc1(HWND hwnd,UINT message,UINT iTimerID,DWORD dwTime)
{
	HDC hdc;
	static BOOL bOk = FALSE;
	static int k;
	static int p;
	static int q;
	RECT rect;
	static int j ;
	static int g;
	static int length;

	GetWindowRect(hwnd,&rect);

	hdc = GetDC(hwnd);

	if(k<20)
	{
		if(bOk)
		{
			BitBlt(hdc,-10,-50,cxSource[0],cySource[0],hdcMem[0],0,0,SRCCOPY);
		}
		else
		{
			BitBlt(hdc,-10,-50,cxSource[1],cySource[1],hdcMem[1],0,0,SRCCOPY);

		}
	}
	else if(k<62&&k>19)
	{

		SelectObject(hdc,GetStockObject(NULL_PEN));
		HBRUSH hBrush;
		hBrush = CreateSolidBrush(RGB(244,37,96));
		SelectObject(hdc,hBrush);

		if(g%3==0)
		{
			g = 0;	
		}

		if(j%5!=0&&j%5!=4)
		{
			BitBlt(hdc,575-length,360,cxOrangeSource[g],cyOrangeSource[g],hdcOrangeMem[g],0,0,SRCCOPY);
			BitBlt(hdc,675-length,348,cxOrangeSource[g+3],cyOrangeSource[g+3],hdcOrangeMem[g+3],0,0,SRCCOPY);

			g++;
			j++;

		}
		else if(j%5==4)
		{		
			
			BitBlt(hdc,575-length,360,cxOrangeSource[1],cyOrangeSource[1],hdcOrangeMem[1],0,0,SRCCOPY);
			BitBlt(hdc,675-length,348,cxOrangeSource[4],cyOrangeSource[4],hdcOrangeMem[4],0,0,SRCCOPY);
			
			j++;
			length +=100;
		}
		else if(j%5==0)
		{
			Rectangle(hdc,675-length,348,775,800);
			j++;
		}
		
	}
	else if(k<96&&k>61)
	{

		SelectObject(hdc,GetStockObject(NULL_PEN));
		HBRUSH hBrush;
		hBrush = CreateSolidBrush(RGB(156,37,96));
		SelectObject(hdc,hBrush);
		Rectangle(hdc,30*p,0,31*(p+1),500);
		p++;
	}
	else if(k>95&&k<106)
	{
		BitBlt(hdc,-20,-60,cxSource[2],cySource[2],hdcMem[2],0,0,SRCCOPY);

	}
	else if(k<129&&k>105)
	{
		SelectObject(hdc,GetStockObject(NULL_PEN));
		HBRUSH hBrush;
		hBrush = CreateSolidBrush(RGB(255,255,0));
		SelectObject(hdc,hBrush);
		Rectangle(hdc,15+q*(25+15),350,15+q*(25+15)+30,400);

		q++;

	}
	else
	{
		SelectObject(hdc,GetStockObject(NULL_PEN));
		HBRUSH hBrush;
		hBrush = CreateSolidBrush(RGB(165,37,96));
		SelectObject(hdc,hBrush);
		Rectangle(hdc,0,350,800,400);
	}
	

	bOk = !bOk;
	k++;
	if(k==131)
	{
		KillTimer(hwnd,ID_TIMER1);

		CONTROL = 1;

		InvalidateRect(hwnd,&rect,FALSE);

		UpdateWindow(hwnd);
	}
	
	ReleaseDC(hwnd,hdc);

}


VOID CALLBACK TimerProc2(HWND hwnd,UINT message,UINT iTimerID,DWORD dwTime)
{
	HDC hdc;
	HBRUSH hBrush;
	HBRUSH hBrush2;
	HPEN hPen;
	HPEN hPen2;

	hdc = GetDC(hwnd);	

	//喝饮料
	hBrush = CreateSolidBrush(RGB(176,194,192));
	SelectObject(hdc,hBrush);
	hPen = CreatePen(PS_SOLID,2,RGB(176,194,192));
	SelectObject(hdc,hPen);
	Rectangle(hdc,437,341,509.5,341+2*d2);

	//动眉毛
	hBrush2 = CreateSolidBrush(RGB(136,59,3));
	hPen2 = CreatePen(PS_SOLID,1,RGB(136,59,3));
	SelectObject(hdc,hBrush2);
	SelectObject(hdc,hPen2);
	if(d2%2)  //48 23  46  21
	{
		
		Rectangle(hdc,505,165,553,188);
		Rectangle(hdc,620,170,666,189);
		BitBlt(hdc,505,175,cxEyeLash[0],cyEyeLash[0],hdcEyeMem[0],0,0,SRCCOPY);
		BitBlt(hdc,620,180,cxEyeLash[1],cyEyeLash[1],hdcEyeMem[1],0,0,SRCCOPY);
	}
	else
	{
		Rectangle(hdc,505,175,553,196);
		Rectangle(hdc,620,180,666,201);
		BitBlt(hdc,505,165,cxEyeLash[0],cyEyeLash[0],hdcEyeMem[0],0,0,SRCCOPY);
		BitBlt(hdc,620,170,cxEyeLash[1],cyEyeLash[1],hdcEyeMem[1],0,0,SRCCOPY);
	}

	hBrush2 = CreateSolidBrush(RGB(252,233,191));
	hPen2 = CreatePen(PS_SOLID,1,RGB(252,233,191));
	SelectObject(hdc,hBrush2);
	SelectObject(hdc,hPen2);
	//动嘴巴  64   28    36   28
	if(d2%2)
	{
		Rectangle(hdc,540,247,566,275);
		BitBlt(hdc,540,247,cxGirlMouth[0],cyGirlMouth[0],hdcGirlMouthMem[0],0,0,SRCCOPY);
	}
	else
	{
		Rectangle(hdc,540,247,604,275);
		BitBlt(hdc,532,247,cxGirlMouth[1],cyGirlMouth[1],hdcGirlMouthMem[1],0,0,SRCCOPY);
	}
	
	//鼻子动
	if (d2%2)
	{
		hBrush2 = CreateSolidBrush(RGB(254,56,196));
		hPen2 = CreatePen(PS_SOLID,2,RGB(254,56,196));
	}
	else
	{
		hBrush2 = CreateSolidBrush(RGB(254,230,195));
		hPen2 = CreatePen(PS_SOLID,2,RGB(254,230,195));
	}
	
	d2++;
	s++;
	
	if(s==50&&x<50)
	{
		KillTimer(hwnd,ID_TIMER2);
		CONTROL = 4;
		s=0;
		x=0;
		d1=0;
		d2=0;
		Go = FALSE;
		InvalidateRect(hwnd,NULL,TRUE);
		UpdateWindow(hwnd);
	}
	
}