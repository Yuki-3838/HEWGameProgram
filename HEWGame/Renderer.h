#pragma once
#include <d3d11.h>  // DirectX11を使うためのヘッダーファイル
#include<DirectXMath.h>//DirextXの数学関連のヘッダーファイル
class Renderer
{
private:
	// デバイス＝DirectXの各種機能を作る ※ID3D11で始まるポインタ型の変数は、解放する必要がある
	ID3D11Device* g_pDevice;
	// コンテキスト＝描画関連を司る機能
	ID3D11DeviceContext* g_pDeviceContext;
	//カメラ行列用定数バッファ
	ID3D11Buffer* g_pConstantBuffer;
	//スワップチェーン(画面表示用)
	IDXGISwapChain* g_pSwapChain;
	//描画先バッファ
	ID3D11RenderTargetView* m_pRenderTargetView;
	//表示領域の設定データ
	D3D11_VIEWPORT m_viewPort;

	ID3D11BlendState* m_pBlendState = nullptr;
public:
	//DirectXの初期化
	HRESULT Init(HWND hWnd, int width, int height);
	//DirectX終了
	void Uninit();
	//画面クリア、ターゲットの設定
	void StartFrame(const float clearColor[4]);
	//画面表示
	void EndFrame();
	//デバイスの取得
	ID3D11Device* GetDevice();
	//コンテキストの取得
	ID3D11DeviceContext* GetContext();

};