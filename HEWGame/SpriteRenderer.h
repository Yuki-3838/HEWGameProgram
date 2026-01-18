#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
//描画のたびに使いまわすクラス
class SpriteRenderer
{
private:
	//四角形（スプライト）の形を定義するデータの入れ物
	ID3D11Buffer* m_pVertexBuffer;
	//頂点の位置を計算する
	ID3D11VertexShader* m_pVertexShader;
	//画像の色を塗る
	ID3D11PixelShader* m_pPixelShader;
	//頂点データの構造をGPUに伝えるための定義
	ID3D11InputLayout* m_pInputLayout;
	//画像のドットをどう引き延ばすかの設定
	ID3D11SamplerState* m_pSamplerState;
	//スプライトごとの行列を送るための定数バッファ
	ID3D11Buffer* m_pSpriteConstantBuffer;

public:
	HRESULT Init(ID3D11Device* pDevice);
	void Draw(ID3D11DeviceContext* pContext,
		ID3D11ShaderResourceView* pSRV,
		float x, float y, float w, float h,
		DirectX::XMMATRIX viewProj,
		float uvX = 0.0f,float uvY = 0.0f,//切り抜く左上の座標
		float uvW = 0.0f,float uvH = 0.0f, //切り抜く幅・高さ
		float angle = 0.0f,//回転角度
		bool flipX = false//左右反転するかどうか
	);
	void Uninit();
};
