#define _CRT_SECURE_NO_WARNINGS
#include <d3dcompiler.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#include <locale.h>
#include <atltypes.h> // CRectを使うためのヘッダーファイル
#include <io.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "Renderer.h"


//プロトタイプ宣言
// 頂点シェーダーオブジェクトを生成
HRESULT CreateVertexShader(ID3D11VertexShader** ppVertexShader, ID3D11InputLayout** ppVertexLayout, D3D11_INPUT_ELEMENT_DESC* pLayout, unsigned int numElements, const char* szFileName);
// ピクセルシェーダーオブジェクトを生成
HRESULT CreatePixelShader(ID3D11PixelShader** ppPixelShader, const char* szFileName);


//--------------------------------------------------------------------------------------
// グローバル変数
//--------------------------------------------------------------------------------------

//Direct3Dのバージョン
D3D_FEATURE_LEVEL m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;
// デバイス＝DirectXの各種機能を作る 
// ※ID3D11で始まるポインタ型の変数は、解放する必要がある
ID3D11Device* g_pDevice;
// コンテキスト＝描画関連を司る機能
ID3D11DeviceContext* g_pDeviceContext;
// スワップチェイン＝ダブルバッファ機能
IDXGISwapChain* g_pSwapChain;
// レンダーターゲット＝描画先を表す機能
ID3D11RenderTargetView* g_pRenderTargetView;
// デプスバッファ
ID3D11DepthStencilView* g_pDepthStencilView;
// インプットレイアウト
ID3D11InputLayout* g_pInputLayout;
// 頂点シェーダーオブジェクト
ID3D11VertexShader* g_pVertexShader;
// ピクセルシェーダーオブジェクト
ID3D11PixelShader* g_pPixelShader;
//サンプラー用変数
ID3D11SamplerState* g_pSampler;
//定数バッファ用変数
ID3D11Buffer* g_pConstantBuffer;
//ブレンドステート用変数(アルファブレンディング)
ID3D11BlendState* g_pBlendState;
//--------------------------------------------------------------------------------------
// DirectXの初期化を行う
//--------------------------------------------------------------------------------------
HRESULT RendererInit(HWND hwnd)
{
	HRESULT hr = S_OK; // HRESULT型→Windowsプログラムで関数実行の成功/失敗を受け取る

	// デバイス、スワップチェーン作成
	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	swapChainDesc.BufferCount = 1;                       // バックバッファの数（ダブルバッファ）
	swapChainDesc.BufferDesc.Width = SCREEN_WIDTH;       // バッファの幅をウィンドウサイズに合わせる
	swapChainDesc.BufferDesc.Height = SCREEN_HEIGHT;     // バッファの高さをウィンドウサイズに合わせる
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // バッファのピクセルフォーマット
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60; // リフレッシュレートを設定(Hz)
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // バッファの使用用途を設定
	swapChainDesc.OutputWindow = hwnd;          // スワップチェーンのターゲットウィンドウを設定
	swapChainDesc.SampleDesc.Count = 1;         // マルチサンプリングの設定（アンチエイリアスのサンプル数とクオリティ）
	swapChainDesc.SampleDesc.Quality = 0;       //同上
	swapChainDesc.Windowed = TRUE;              // ウィンドウモード（フルスクリーンではなく、ウィンドウモードで実行）

	// デバイスとスワップチェインを同時に作成する関数の呼び出し
	hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE, // ドライバータイプ(ハードウェアGPUを使用)
		NULL,               // ソフトウェアラスタライザを指定しないのでNULL
		0,                  // フラグ（D3D11_CREATE_DEVICE_DEBUGなど） 0は何も指定しない
		NULL,               // 機能レベルの配列。NULLならデフォルトの機能レベルセットが使われる
		0,                  // 機能レベルの配列の要素数(NULLなら0でOK)
		D3D11_SDK_VERSION,  // SDKのバージョン 常に「D3D11_SDK_VERSION」を指定
		&swapChainDesc,     // スワップチェーンの設定構造体へのポインタ
		&g_pSwapChain,      // 作成されたスワップチェーンを受け取るポインタ
		&g_pDevice,	        // 作成されたデバイスを受け取るポインタ
		&m_FeatureLevel,    // 作成されたデバイスの機能レベルを受け取る変数へのポインタ
		&g_pDeviceContext); // 作成されたデバイスコンテキストを受け取るポインタ
	if (FAILED(hr)) return hr;

	// レンダーターゲットビュー作成
	ID3D11Texture2D* renderTarget;
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&renderTarget);
	if (FAILED(hr)) return hr;
	hr = g_pDevice->CreateRenderTargetView(renderTarget, NULL, &g_pRenderTargetView);
	renderTarget->Release();
	if (FAILED(hr)) return hr;

	// デプスステンシルバッファ作成
	// ※（デプスバッファ = 深度バッファ = Zバッファ）→奥行を判定して前後関係を正しく描画できる
	ID3D11Texture2D* depthStencile{};
	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width = swapChainDesc.BufferDesc.Width;   // バッファの幅をスワップチェーンに合わせる
	textureDesc.Height = swapChainDesc.BufferDesc.Height; // バッファの高さをスワップチェーンに合わせる
	textureDesc.MipLevels = 1;                            // ミップレベルは1（ミップマップは使用しない）
	textureDesc.ArraySize = 1;                            // テクスチャの配列サイズ（通常1）
	textureDesc.Format = DXGI_FORMAT_D16_UNORM;           // フォーマットは16ビットの深度バッファを使用
	textureDesc.SampleDesc = swapChainDesc.SampleDesc;    // スワップチェーンと同じサンプル設定
	textureDesc.Usage = D3D11_USAGE_DEFAULT;              // 使用方法はデフォルト（GPUで使用）
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;     // 深度ステンシルバッファとして使用
	textureDesc.CPUAccessFlags = 0;                       // CPUからのアクセスは不要
	textureDesc.MiscFlags = 0;                            // その他のフラグは設定なし
	hr = g_pDevice->CreateTexture2D(&textureDesc, NULL, &depthStencile);
	if (FAILED(hr)) return hr;

	// デプスステンシルビュー作成
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
	depthStencilViewDesc.Format = textureDesc.Format; // デプスステンシルバッファのフォーマットを設定
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D; // ビューの次元を2Dテクスチャとして設定（2Dテクスチャ用のデプスステンシルビュー）
	depthStencilViewDesc.Flags = 0; // 特別なフラグは設定しない（デフォルトの動作）
	hr = g_pDevice->CreateDepthStencilView(depthStencile, &depthStencilViewDesc, &g_pDepthStencilView);
	if (FAILED(hr)) return hr;
	depthStencile->Release();

	// ビューポートを作成（→画面分割などに使う、描画領域の指定のこと）
	CRect rect;
	GetClientRect(hwnd, &rect);
	D3D11_VIEWPORT viewport;
	viewport.Width = (FLOAT)rect.Width();   // ビューポートの幅
	viewport.Height = (FLOAT)rect.Height(); // ビューポートの高さ
	viewport.MinDepth = 0.0f;               // 深度範囲の最小値
	viewport.MaxDepth = 1.0f;               // 深度範囲の最大値
	viewport.TopLeftX = 0;                  // ビューポートの左上隅のX座標
	viewport.TopLeftY = 0;                  // ビューポートの左上隅のY座標）
	g_pDeviceContext->RSSetViewports(1, &viewport);


	// インプットレイアウト作成
	D3D11_INPUT_ELEMENT_DESC layout[]
	{
		// 位置座標があるということを伝える
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		// 色情報があるということを伝える
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		//UV座標(uv)
		{
			"TEX",0,DXGI_FORMAT_R32G32_FLOAT,0,
					 D3D11_APPEND_ALIGNED_ELEMENT,
						  D3D11_INPUT_PER_VERTEX_DATA, 0
		}
	};
	unsigned int numElements = ARRAYSIZE(layout);

	// 頂点シェーダーオブジェクトを生成、同時に頂点レイアウトも生成
	hr = CreateVertexShader(&g_pVertexShader, &g_pInputLayout, layout, numElements, "VertexShader.hlsl");
	if (FAILED(hr)) 
	{
		MessageBoxA(NULL, "CreateVertexShader error", "error", MB_OK);
		return hr;
	}

	// ピクセルシェーダーオブジェクトを生成
	hr = CreatePixelShader(&g_pPixelShader, "PixelShader.hlsl");
	if (FAILED(hr))
	{
		MessageBoxA(NULL, "CreatePixelShader error", "error", MB_OK);
		return hr;
	}



	//サンプラー作成
	//→　テクスチャをポリゴンに貼るときに、拡大縮小される際のアルゴリズム
	D3D11_SAMPLER_DESC smpDesc{};
	smpDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	smpDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	smpDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	smpDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	hr = g_pDevice->CreateSamplerState(&smpDesc, &g_pSampler);
	if (FAILED(hr)) return hr;

	//定数バッファ作成
	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = sizeof(ConstBuffer);
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = 0;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;
	hr = g_pDevice->CreateBuffer(&cbDesc, NULL, &g_pConstantBuffer);
	if (FAILED(hr)) return hr;

	//ブレンドステートを作成→透過処理や加算合成を可能にする色の合成方法
	D3D11_BLEND_DESC BlendState{};
	BlendState.AlphaToCoverageEnable = FALSE;
	BlendState.IndependentBlendEnable = FALSE;
	BlendState.RenderTarget[0].BlendEnable = TRUE;
	BlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].RenderTargetWriteMask =
									 D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = g_pDevice->CreateBlendState(&BlendState, &g_pBlendState);
	if (FAILED(hr)) return hr;

	//深度テストを無効にする
	ID3D11DepthStencilState* pDSState;
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	dsDesc.DepthEnable = FALSE;//深度テストを無効にする
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	hr = g_pDevice->CreateDepthStencilState(&dsDesc, &pDSState);
	if (FAILED(hr)) return hr;
	g_pDeviceContext->OMSetDepthStencilState(pDSState,1);
	return S_OK;
}
//--------------------------------------------------------------------------------------
// DirectXの描画を行う
//--------------------------------------------------------------------------------------
void RendererDrawStart()
{
	// 画面塗りつぶし色
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; //red,green,blue,alpha

	// 描画先のキャンバスと使用する深度バッファを指定する
	g_pDeviceContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);
	// 描画先キャンバスを塗りつぶす
	g_pDeviceContext->ClearRenderTargetView(g_pRenderTargetView, clearColor);
	// 深度バッファをリセットする
	g_pDeviceContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	g_pDeviceContext->IASetInputLayout(g_pInputLayout);
	g_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	g_pDeviceContext->VSSetShader(g_pVertexShader, NULL, 0);
	g_pDeviceContext->PSSetShader(g_pPixelShader, NULL, 0);

	//サンプラーをピクセルシェーダーに渡す
	g_pDeviceContext->PSSetSamplers(0, 1, &g_pSampler);

	//定数バッファを頂点バッファにセットする
	g_pDeviceContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);

	//ブレンドステートをセットする
	g_pDeviceContext->OMSetBlendState(g_pBlendState, NULL, 0xffffffff);


}
void RendererDrawEnd()
{
	// ダブルバッファの切り替えを行い画面を更新する
	g_pSwapChain->Present(0, 0);
}

//--------------------------------------------------------------------------------------
// DirectXの解放（終了処理）を行う
//--------------------------------------------------------------------------------------
void RendererUninit()
{
	// ※DirectXの各機能は作成した後、アプリ終了時に必ず解放しなければならない
	if (g_pDeviceContext) g_pDeviceContext->ClearState();
	SAFE_RELEASE(g_pBlendState);
	SAFE_RELEASE(g_pConstantBuffer);
	SAFE_RELEASE(g_pSampler);
	SAFE_RELEASE(g_pPixelShader);
	SAFE_RELEASE(g_pVertexShader);
	SAFE_RELEASE(g_pInputLayout);
	SAFE_RELEASE(g_pDepthStencilView);
	SAFE_RELEASE(g_pRenderTargetView);
	SAFE_RELEASE(g_pSwapChain);
	SAFE_RELEASE(g_pDeviceContext);
	SAFE_RELEASE(g_pDevice);
}
//--------------------------------------------------------------------------------------
// ウィンドウをリサイズして画面の縦横比を維持する
//--------------------------------------------------------------------------------------
HRESULT ResizeWindow(int width, int height)
{
	//スワップチェインが存在にしない場合は処理しない
	if (!g_pSwapChain)return S_FALSE;

	//既存のレンダーターゲットビューを解放
	if (g_pRenderTargetView)
	{
		g_pRenderTargetView->Release();
		g_pRenderTargetView = nullptr;
	}

	//既存のデプスステンシルビューを解放
	if (g_pDepthStencilView)
	{
		g_pDepthStencilView->Release();
		g_pDepthStencilView = nullptr;
	}

	//スワップチェインのバッファサイズを新しいウィンドウサイズに合わせて変更
	g_pSwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
	// レンダーターゲットビュー作成
	ID3D11Texture2D* renderTarget;
	HRESULT hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&renderTarget);
	if (FAILED(hr)) return hr;
	hr = g_pDevice->CreateRenderTargetView(renderTarget, NULL, &g_pRenderTargetView);
	renderTarget->Release();
	if (FAILED(hr)) return hr;

	// デプスステンシルバッファ作成
	// ※（デプスバッファ = 深度バッファ = Zバッファ）→奥行を判定して前後関係を正しく描画できる
	ID3D11Texture2D* depthStencile{};
	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width = width;   // バッファの幅をスワップチェーンに合わせる
	textureDesc.Height = height; // バッファの高さをスワップチェーンに合わせる
	textureDesc.MipLevels = 1;                            // ミップレベルは1（ミップマップは使用しない）
	textureDesc.ArraySize = 1;                            // テクスチャの配列サイズ（通常1）
	textureDesc.Format = DXGI_FORMAT_D16_UNORM;           // フォーマットは16ビットの深度バッファを使用
	textureDesc.SampleDesc.Count = 1;    // スワップチェーンと同じサンプル設定
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;              // 使用方法はデフォルト（GPUで使用）
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;     // 深度ステンシルバッファとして使用
	textureDesc.CPUAccessFlags = 0;                       // CPUからのアクセスは不要
	textureDesc.MiscFlags = 0;                            // その他のフラグは設定なし
	hr = g_pDevice->CreateTexture2D(&textureDesc, NULL, &depthStencile);
	if (FAILED(hr)) return hr;

	// デプスステンシルビュー作成
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
	depthStencilViewDesc.Format = textureDesc.Format; // デプスステンシルバッファのフォーマットを設定
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D; // ビューの次元を2Dテクスチャとして設定（2Dテクスチャ用のデプスステンシルビュー）
	depthStencilViewDesc.Flags = 0; // 特別なフラグは設定しない（デフォルトの動作）
	hr = g_pDevice->CreateDepthStencilView(depthStencile, &depthStencilViewDesc, &g_pDepthStencilView);
	if (FAILED(hr)) return hr;
	depthStencile->Release();

	//ウィンドウとターゲットのアスペクト比を比較してビューポートを調節
	float windowAspect = (float)width / (float)height;
	float targetAspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;

	D3D11_VIEWPORT vi = {};
	if (windowAspect > targetAspect)
	{
		//ウィンドウが横長の場合は高さに合わせて幅を調節
		vi.Height = (float)width;
		vi.Height = height * targetAspect;
		vi.TopLeftX = width / targetAspect;
		vi.TopLeftY = (height - vi.Height) / 2.0f;
	}
	vi.MinDepth = 0.0f;
	vi.MaxDepth = 1.0f;

	//ビューポートを設定
	g_pDeviceContext->RSSetViewports(1, &vi);
	return S_OK;
}

//--------------------------------------------------------------------------------------
// シェーダーをファイル拡張子に合わせてコンパイル
//--------------------------------------------------------------------------------------
HRESULT CompileShader(const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, void** ppShaderObject, int* pShaderObjectSize)
{
	//拡張子csoのファイル名を作成
	char csoFileName[256];
	const char* dot = strrchr(szFileName, '.');  // 最後の '.' を探す
	if (dot) {
		int basenameLen = dot - szFileName;
		strncpy(csoFileName, szFileName, basenameLen); // 拡張子がある場合は拡張子を除いたファイル名をコピー
		csoFileName[basenameLen] = '\0';   // 終端文字を追加
	}
	else {
		strcpy(csoFileName, szFileName);   // 拡張子がない場合はそのままコピー
	}
	strcat(csoFileName, ".cso");// ".cso" 拡張子を付加

	//csoファイルがあれば開く
	FILE* fp;
	int ret = fopen_s(&fp, csoFileName, "rb");
	if (ret == 0)
	{
		// ファイルサイズを取得
		fseek(fp, 0, SEEK_END);
		int size = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		// バイナリデータを読み込み用にメモリ確保
		unsigned char* byteArray = new unsigned char[size];
		fread(byteArray, size, 1, fp);
		fclose(fp);

		// 呼び出し元にポインタとサイズを渡す
		*ppShaderObject = byteArray;
		*pShaderObjectSize = size;
	}
	//csoファイルがなければhlslファイルをコンパイルする
	else
	{
		HRESULT hr = S_OK;
		WCHAR	filename[512];
		size_t 	wLen = 0;
		int err = 0;

		// 文字コードを Shift-JIS → UTF-16 に変換
		setlocale(LC_ALL, "japanese");  // ロケールを設定（Windows特有）
		err = mbstowcs_s(&wLen, filename, 512, szFileName, _TRUNCATE);

		// シェーダーコンパイルオプションを設定
		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
		dwShaderFlags |= D3DCOMPILE_DEBUG; // デバッグビルドの場合はデバッグ情報も含める
#endif

		// コンパイル結果およびエラー情報格納用のBlob
		ID3DBlob* pErrorBlob = nullptr;
		ID3DBlob* pBlob = nullptr;

		// HLSLファイルをコンパイル
		hr = D3DCompileFromFile(
			filename,							// ファイル名
			nullptr,							// マクロ定義なし 
			D3D_COMPILE_STANDARD_FILE_INCLUDE,	// #include 対応 
			szEntryPoint,						// エントリーポイント名
			szShaderModel,						// シェーダーモデル
			dwShaderFlags,						// コンパイルフラグ
			0,									// エフェクトフラグ
			&pBlob,								// 成功時のコンパイル結果
			&pErrorBlob);						// コンパイルエラー出力

		// コンパイル失敗時のエラーメッセージを表示
		if (FAILED(hr))
		{
			if (pErrorBlob != nullptr) {
				MessageBoxA(NULL, (char*)pErrorBlob->GetBufferPointer(), "Error", MB_OK);
			}
			if (pErrorBlob) pErrorBlob->Release();
			if (pBlob)(pBlob)->Release();
			return E_FAIL;
		}

		// エラーブロブがあれば解放
		if (pErrorBlob) pErrorBlob->Release();

		// コンパイル成功時のバイナリデータを呼び出し元に渡す
		*ppShaderObject = (pBlob)->GetBufferPointer();
		*pShaderObjectSize = (pBlob)->GetBufferSize();
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
// 頂点シェーダーオブジェクトを生成する
//--------------------------------------------------------------------------------------
HRESULT CreateVertexShader(ID3D11VertexShader** ppVertexShader, ID3D11InputLayout** ppVertexLayout, D3D11_INPUT_ELEMENT_DESC* pLayout, unsigned int numElements, const char* szFileName)
{
	void* ShaderObject;
	int	ShaderObjectSize;

	// ファイルの拡張子に合わせてコンパイル
	HRESULT hr = CompileShader(szFileName, "main", "vs_5_0", &ShaderObject, &ShaderObjectSize);
	if (FAILED(hr)) return E_FAIL;

	// デバイスを使って頂点シェーダーを作成
	hr = g_pDevice->CreateVertexShader(ShaderObject, ShaderObjectSize, NULL, ppVertexShader);

	// デバイスを使って頂点レイアウトを作成
	g_pDevice->CreateInputLayout(pLayout, numElements, ShaderObject, ShaderObjectSize, ppVertexLayout);

	return S_OK;
}

//--------------------------------------------------------------------------------------
// ピクセルシェーダーオブジェクトを生成する
//--------------------------------------------------------------------------------------
HRESULT CreatePixelShader(ID3D11PixelShader** ppPixelShader, const char* szFileName)
{
	void* ShaderObject;
	int	ShaderObjectSize;

	// ファイルの拡張子に合わせてコンパイル
	HRESULT hr = CompileShader(szFileName, "main", "ps_5_0", &ShaderObject, &ShaderObjectSize);
	if (FAILED(hr)) return hr;

	// ピクセルシェーダーを生成
	hr = g_pDevice->CreatePixelShader(ShaderObject, ShaderObjectSize, nullptr, ppPixelShader);
	if (FAILED(hr)) return hr;

	return S_OK;
}