//===============================================================================
//
//  3Dゲーム制作(main.cpp)
//								制作：元地弘汰
// 
//===============================================================================
#include "main.h"
#include "manager.h"

//マクロ定義
#define CLASS_NAME	"WindowClass" //ウィンドウクラスの名前
#define WINDOW_NAME	"Sky Unit"
#define ID_BUTTON_RESET	(101)

//プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void ToggleFullscreen(HWND hWnd);// ウィンドウをフルスクリーンにする方法

//グローバル変数
bool g_bExit = false;
bool g_isFullscreen = false;// ウィンドウを切り替えるためのフラグ
RECT g_windowRect;// ウィンドウを切り替えるための変数
int g_nCountFPS;
//============================================
//メイン関数
//============================================
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hInstancePrev, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WindowProc,
		0,
		0,
		hInstance,
		LoadIcon(NULL,IDI_APPLICATION),
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		LoadIcon(NULL,IDI_APPLICATION)
	};
	
	HWND hWnd;
	MSG msg;
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	//ウィンドウクラスの登録
	RegisterClassEx(&wcex);
	//クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	//ウィンドウを生成
	hWnd = CreateWindowEx(0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(rect.right - rect.left),
		(rect.bottom - rect.top),
		NULL,
		NULL,
		hInstance,
		NULL);

	// 現在のウィンドウスタイルを取得
	DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);

#ifndef _DEBUG

	g_isFullscreen = true;
	// フルスクリーンモードに切り替え
	GetWindowRect(hWnd, &g_windowRect);
	SetWindowLong(hWnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
	SetWindowPos(hWnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
		SWP_FRAMECHANGED | SWP_NOACTIVATE);
	ShowWindow(hWnd, SW_MAXIMIZE);

#endif // _RELEASE

	DWORD dwCurrentTime;		//現在時刻
	DWORD dwExecLastTime;		//現在時刻を取得
	DWORD dwFrameCount;			//フレームカウント
	DWORD dwFPSLastTime;		//最後にFPSを計測した時刻
	CManager* pManager = CManager::GetInstance();
	pManager->Init(hInstance, hWnd, TRUE);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//分解能を設定
	timeBeginPeriod(1);
	dwCurrentTime = 0;				//初期化する
	dwExecLastTime = timeGetTime();	//現在時刻取得
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//windowsの設定
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				//メッセージの設定
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{//DirectXの処理
			dwCurrentTime = timeGetTime();	//現在時刻を取得
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5秒経過
				//FPSを計測
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}
			if((dwCurrentTime - dwExecLastTime) >= (1000/60))
			{
				dwExecLastTime = dwCurrentTime;	//処理開始の時刻を保存
				//更新処理
				pManager->Update();
				//描画処理
				pManager->Draw();
				dwFrameCount++;
				if (g_bExit == true)
				{
					// ウィンドウを破棄する
					DestroyWindow(hWnd);
				}
			}
		}
	}
	//終了処理
	if (pManager != nullptr)
	{
		pManager->Uninit();
		delete pManager;
		pManager = nullptr;
	}

	//分解能を戻す
	timeEndPeriod(1);
	g_nCountFPS = 0;
	//ウィンドウクラスの登録解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return(int)msg.wParam;
}

//==========================================================================================
//ウィンドウプロシージャ
//==========================================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	int nID;
	//HWND hWndButtonReset;
	switch (uMsg)
	{
	case WM_CLOSE:
		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO | MB_ICONEXCLAMATION);
		if (nID == IDYES)
		{
			//ウィンドウを破棄する
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;
		}
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO | MB_ICONEXCLAMATION);
			if (nID == IDYES)
			{
				//ウィンドウを破棄する
				DestroyWindow(hWnd);
			}
			break;
		case VK_F11:
			ToggleFullscreen(hWnd);
			break;
		}
		break;

	case WM_LBUTTONDOWN:
		//ウィンドウにフォーカスを合わせる
		SetFocus(hWnd);
		break;
	case WM_DESTROY:
		//WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void SetExitGame(void)
{
	g_bExit = true;
}

//************************************************
// ウィンドウをフルスクリーンに変える処理
//************************************************
void ToggleFullscreen(HWND hWnd)
{
	// 現在のウィンドウスタイルを取得
	DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);

	if (g_isFullscreen)
	{
		// ウィンドウモードに切り替え
		SetWindowLong(hWnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPos(hWnd, HWND_TOP, g_windowRect.left, g_windowRect.top,
			g_windowRect.right - g_windowRect.left, g_windowRect.bottom - g_windowRect.top,
			SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(hWnd, SW_NORMAL);
	}
	else
	{
		// フルスクリーンモードに切り替え
		GetWindowRect(hWnd, &g_windowRect);
		SetWindowLong(hWnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
		SetWindowPos(hWnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
			SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(hWnd, SW_MAXIMIZE);
	}

	g_isFullscreen = !g_isFullscreen;
}
