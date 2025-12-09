#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <d3d11.h>  // DirectX11を使うためのヘッダーファイル
#include<DirectXMath.h>//DirextXの数学関連のヘッダーファイル

#define SCREEN_WIDTH (640)	// ウインドウの幅
#define SCREEN_HEIGHT (480)	// ウインドウの高さ

// 関数のプロトタイプ宣言
HRESULT RendererInit(HWND hwnd);
void    RendererDrawStart();
void    RendererDrawEnd();
void    RendererUninit();
HRESULT ResizeWindow(int width, int height);

// 構造体の定義
// 頂点データを表す構造体
struct Vertex
{
	// 頂点の位置座標
	float x, y, z;
	//頂点の色
	float r, g, b, a;

	//テクスチャ座標(UV座標)
	float u, v;
};
//定数バッファ用構造体
struct ConstBuffer
{
	//頂点カラー
	DirectX::XMFLOAT4 color;
	//UV座標行列
	DirectX::XMMATRIX matrixTex;
	//プロジェクション変換行列
	DirectX::XMMATRIX matrixProj;
	//ワールド変換行列
	DirectX::XMMATRIX matrixWorld;
};
// デバイス＝DirectXの各種機能を作る ※ID3D11で始まるポインタ型の変数は、解放する必要がある
extern ID3D11Device* g_pDevice;
// コンテキスト＝描画関連を司る機能
extern ID3D11DeviceContext* g_pDeviceContext;
extern ID3D11Buffer* g_pConstantBuffer;
extern IDXGISwapChain* g_pSwapChain;

// Direct3D解放の簡略化マクロ
#define SAFE_RELEASE(p) { if( NULL != p ) { p->Release(); p = NULL; } }