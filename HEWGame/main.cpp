#undef UNICODE  // Unicodeではなく、マルチバイト文字を使う
#define _CRT_SECURE_NO_WARNINGS //警告を出さなくさせる
#include "input.h"
#include <iostream>
#include<Windows.h>
#include "Game.h"

// マクロ定義
#define CLASS_NAME   "DX21Hew"// ウインドウクラスの名前
#define WINDOW_NAME  "DirectX初期化"// ウィンドウの名前

#define SCREEN_WIDTH (1920)		// ウインドウの幅
#define SCREEN_HEIGHT (1080)		// ウインドウの高さ


// 関数のプロトタイプ宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

Input input;
bool g_showA = false;   // Aを表示するかどうか


//--------------------------------------------------------------------------------------
// エントリポイント＝一番最初に実行される関数
//--------------------------------------------------------------------------------------
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// ウィンドウクラス情報をまとめる
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX); //この構造体のサイズ
	wc.style = CS_CLASSDC; //ウィンドウクラスの動作スタイル
	wc.lpfnWndProc = WndProc; //ウィンドウプロシージャ（メッセージ処理関数）への関数ポインタ
	wc.cbClsExtra = 0; //クラスに付随する拡張メモリ
	wc.cbWndExtra = 0; //ウィンドウインスタンスに付随する拡張メモリ
	wc.hInstance = hInstance; //このクラスを登録するモジュールのインスタンスハンドル
	wc.hIcon = NULL; //クラスのデフォルトのアイコン
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); //クラスのデフォルトのアイコン
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); //背景塗りつぶし用のブラシ
	wc.lpszMenuName = NULL; //クラスに関連付けるメニューリソース名
	wc.lpszClassName = CLASS_NAME; //このウィンドウクラスの識別名（文字列）
	//タスクバーや Alt + Tab などで使われる小さいアイコン。
	// ここでは NULL（設定なし）。必要なら LoadIcon(NULL, IDI_APPLICATION) 等を設定
	wc.hIconSm = NULL; 

	RegisterClassEx(&wc);

	// ウィンドウの情報をまとめる
	HWND hWnd;
	hWnd = CreateWindowEx(0,	// 拡張ウィンドウスタイル
		CLASS_NAME,				// ウィンドウクラスの名前
		WINDOW_NAME,			// ウィンドウの名前
		WS_OVERLAPPEDWINDOW,	// ウィンドウスタイル
		CW_USEDEFAULT,			// ウィンドウの左上Ｘ座標
		CW_USEDEFAULT,			// ウィンドウの左上Ｙ座標 
		SCREEN_WIDTH,			// ウィンドウの幅
		SCREEN_HEIGHT,			// ウィンドウの高さ
		NULL,					// 親ウィンドウのハンドル
		NULL,					// メニューハンドルまたは子ウィンドウID
		hInstance,				// インスタンスハンドル
		NULL);					// ウィンドウ作成データ

	//ウィンドウのサイズを調整
	RECT rc1, rc2;
	GetWindowRect(hWnd, &rc1);
	GetClientRect(hWnd, &rc2);
	int sx = SCREEN_WIDTH;
	int sy = SCREEN_HEIGHT;
	sx += ((rc1.right - rc1.left)) - ((rc2.right - rc2.left));
	sy += ((rc1.bottom - rc1.top)) - ((rc2.bottom - rc2.top));
	SetWindowPos(hWnd, NULL, 0, 0, sx, sy, (SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE));

	// 指定されたウィンドウの表示状態を設定(ウィンドウを表示)
	ShowWindow(hWnd, nCmdShow);
	// ウィンドウの状態を直ちに反映(ウィンドウのクライアント領域を更新)
	UpdateWindow(hWnd);

	//ゲームの初期化
	Game game;
	game.Init(hWnd, SCREEN_WIDTH, SCREEN_HEIGHT);
	

	MSG msg;

	//FPS計測用変数
	int fpsCounter = 0;
	long long oldTick = GetTickCount64(); //前回計測時の時間
	long long nowTick = oldTick; //今回計測時の時間 

	//fps固定用
	LARGE_INTEGER liWork;
	long long frequency;
	QueryPerformanceFrequency(&liWork);
	frequency = liWork.QuadPart;
	//時間（単位：アカウント）
	QueryPerformanceCounter(&liWork);
	long long oldCount = liWork.QuadPart;
	long long nowCount = oldCount;



	// ゲームループ
	while (1)
	{
		// 新たにメッセージがあれば
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// ウィンドウプロシージャにメッセージを送る
			DispatchMessage(&msg);

			// 「WM_QUIT」メッセージを受け取ったらループを抜ける
			if (msg.message == WM_QUIT) {
				break;
			}
		}
		else
		{
			QueryPerformanceCounter(&liWork);
			nowCount = liWork.QuadPart;
			//1/60秒が経過したか？
			if (nowCount >= oldCount + frequency / 60) //ここにゲーム関数の呼び出し
			{
				//ゲーム処理実行
				game.Update();
				game.Draw();

				fpsCounter++; //ゲーム処理を実行したら＋１をする
				oldCount = nowCount;
			}

			nowTick = GetTickCount64(); //現在時刻を取得
			//前回計測時から1000ミリ秒が経過したか？
			if (nowTick >= oldTick + 1000)
			{
				//fps表示
				char str[32];
				wsprintfA(str, "FPS =%d", fpsCounter);
				SetWindowTextA(hWnd, str);
				//カウンターリセット
				fpsCounter = 0;
				oldTick = nowTick;
			}
		}
	}
	//ゲーム終了
	game.Uninit();
	

	UnregisterClass(CLASS_NAME, hInstance);

	return (int)msg.wParam;
}

//--------------------------------------------------------------------------------------
//ウィンドウプロシージャ
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:// ウィンドウ破棄のメッセージ
		PostQuitMessage(0);// 「WM_QUIT」メッセージを送る　→　アプリ終了
		break;

	case WM_CLOSE:  // 「x」ボタンが押されたら
	{
		int res = MessageBoxA(NULL, "終了しますか？", "確認", MB_OKCANCEL);
		if (res == IDOK) {
			DestroyWindow(hWnd);  // 「WM_DESTROY」メッセージを送る
		}
	}
	break;



	case WM_KEYDOWN: //キー入力があったメッセージ
		if (LOWORD(wParam) == VK_ESCAPE)
		{ //入力されたキーがESCAPEなら
			PostMessage(hWnd, WM_CLOSE, wParam, lParam);//「WM_CLOSE」を送る
		}
		if (LOWORD(wParam) == VK_C) //お試し
		{
			int res = MessageBoxA(hWnd, "あ", "キー入力", MB_OK);
		}
		break;


	default:
		// 受け取ったメッセージに対してデフォルトの処理を実行
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}

	return 0;
}

