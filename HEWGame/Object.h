//#pragma once
//#include "Texture.h"
//#include "Renderer.h"
//class Object
//{
//private:
//	//頂点データを決める,
//	//{x,y,z,r,g,b,a}
//	Vertex m_vertexList[4] =
//	{
//		{  -0.5f, +0.5f, 0.5f, 1.0f, 1.0f,  1.0f, 1.0f, 0.0f, 0.0f }, //左上 -x +y
//		{  +0.5f, +0.5f, 0.5f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f, 0.0f }, //右上 +x +y
//		{  -0.5f, -0.8f, 0.5f, 1.0f, 1.0f,  1.0f, 1.0f, 0.0f, 1.0f }, //左下 -x -y
//		{  +0.5f, -0.8f, 0.5f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f, 1.0f }, //右下 +x -y
//	};
//
//	//座標
//	DirectX::XMFLOAT3 m_pos = { 0.0f,0.0f,0.0f};
//	//大きさ
//	DirectX::XMFLOAT3 m_size = { 100.0f,100.0f,0.0f };
//	//角度
//	float angle = 0.0f;
//	//色
//	DirectX::XMFLOAT4 m_color = {1.0f,1.0f,1.0f,1.0f};
//	//頂点バッファ
//	ID3D11Buffer* m_pVertexBuffer;
//	//テクスチャ用変数
//	ID3D11ShaderResourceView* m_pTextureView;
//
//	//テクスチャが縦横に何分割されているか
//	int splitX = 1;
//	int splitY = 1;
//
//
//protected:
//	DirectX::XMFLOAT3 pos;
//	DirectX::XMFLOAT3 size;
//public:
//	//左上から何段目を切り抜いて表示するか
//	float numU = 0;
//	float numV = 0;
//	HRESULT Init(const char* imgname,int sx = 1, int sy = 1);//初期化
//	void Draw();
//	void Uninit();
//	void SetPos(const DirectX::XMFLOAT3& newPos);//座標をセット
//	void SetSize(const DirectX::XMFLOAT3& newSize);//大きさをセット
//	void SetAngle(float a);                 //角度をセット
//	void SetColor(const DirectX::XMFLOAT4& newCol);//色をセット
//	DirectX::XMFLOAT3 GetPos() const;
//	DirectX::XMFLOAT3 GetSize() const;
//	float GetAngle();
//	DirectX::XMFLOAT4 GetColor();
//};