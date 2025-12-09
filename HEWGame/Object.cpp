#include "Object.h"

HRESULT Object::Init(const char* imgname, int sx, int sy)
{
	//UV座標の設定
	splitX = sx;
	splitY = sy;
	m_vertexList[1].u = 1.0f / splitX;
	m_vertexList[2].v = 1.0f / splitY;
	m_vertexList[3].u = 1.0f / splitX;
	m_vertexList[3].v = 1.0f / splitY;

	//頂点バッファを作成する
	//※頂点バッファ->VRAMに頂点データを置くための機能
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = sizeof(m_vertexList);//確保するバッファサイズを指定
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;//頂点バッファ作成を指定
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA subResourceData;
	subResourceData.pSysMem = m_vertexList;//
	subResourceData.SysMemPitch = 0;
	subResourceData.SysMemSlicePitch = 0;

	HRESULT hr = g_pDevice->CreateBuffer(&bufferDesc, &subResourceData, &m_pVertexBuffer);
	if (FAILED(hr)) return hr;

	//テクスチャ読み込み
	hr = LoadTexture(g_pDevice, imgname, &m_pTextureView);
	if (FAILED(hr))
	{
		MessageBoxA(NULL, "テクスチャ読み込み失敗", "エラー", MB_ICONERROR | MB_OK);
		return hr;
	}
	return S_OK;
}

void Object::Draw()
{
	//頂点バッファを設定
	UINT strides = sizeof(Vertex);
	UINT offsets = 0;
	g_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &strides, &offsets);

	//テクスチャをピクセルシェーダーに渡す
	g_pDeviceContext->PSSetShaderResources(0, 1, &m_pTextureView);

	//プロジェクション変換行列を作成
	DirectX::XMMATRIX matrixProj = DirectX::XMMatrixOrthographicLH(
		SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 3.0f);

	//ワールド変換行列を作成
	DirectX::XMMATRIX matrixScale =
		DirectX::XMMatrixScaling(m_size.x, m_size.y, m_size.z);//大きさ
	DirectX::XMMATRIX matrixAngle =
		DirectX::XMMatrixRotationZ(angle * 3.14f / 180);//向き
	DirectX::XMMATRIX matrixPosition =
		DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);//位置
	DirectX::XMMATRIX matrixWorld =
		matrixScale * matrixAngle * matrixPosition;
	//ワールド変換行列を作成

//UVアニメーションの行列を作成
	float u = (float)numU / splitX;
	float v = (float)numV / splitY;
	DirectX::XMMATRIX matrixTex =
		DirectX::XMMatrixTranslation(u, v, 0.0f);

	//定数バッファを更新
	ConstBuffer cb;
	cb.matrixProj = DirectX::XMMatrixTranspose(matrixProj);
	//プロジェクション変換行列
	cb.matrixWorld = DirectX::XMMatrixTranspose(matrixWorld);
	//ワールド変換行列
	cb.matrixTex = DirectX::XMMatrixTranspose(matrixTex);
	//UVアニメーション行列
	cb.color = m_color;//頂点カラーのデータを作成
	//行列をシェーダーに渡す
	g_pDeviceContext->UpdateSubresource(
		g_pConstantBuffer, 0, NULL, &cb, 0, 0);

	g_pDeviceContext->Draw(4, 0); // 描画命令

}

void Object::Uninit()
{
	//開放処理
	SAFE_RELEASE(m_pTextureView);
	SAFE_RELEASE(m_pVertexBuffer);
}
void Object::SetPos(float x, float y, float z)
{
	//座標をセットする
	m_pos.x = x;
	m_pos.y = y;
	m_pos.z = z;
}

void Object::SetSize(float x, float y, float z)
{
	//大きさをセットする
	m_size.x = x;
	m_size.y = y;
	m_size.z = z;
}

void Object::SetAngle(float a)
{
	//角度をセットする
	angle = a;
}
void Object::SetColor(float r, float g, float b, float a)
{
	//色をセットする
	m_color.x = r;
	m_color.y = g;
	m_color.z = b;
	m_color.w = a;
}
void Object::SetPosition(const DirectX::XMFLOAT3& newPos)
{

}
DirectX::XMFLOAT3 Object::GetPos() const
{
	return m_pos;      //座標をゲット
}

DirectX::XMFLOAT3 Object::GetSize() const
{
	return m_size;     //大きさをゲット
}

float Object::GetAngle()
{
	return angle;    //角度をゲット
}

DirectX::XMFLOAT4 Object::GetColor()
{
	return m_color;    //色をゲット	
}