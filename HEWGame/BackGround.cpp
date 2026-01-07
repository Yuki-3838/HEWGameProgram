#include "BackGround.h"

void BackGround::Init(ID3D11ShaderResourceView* pTexture)
{
    m_pTexture = pTexture;
    m_Position = { 0.0f, 0.0f,0.0f };    // 左上から
    m_Size = { 1280.0f, 720.0f,0.0f };   // 画面全体サイズ
}

void BackGround::Update()
{

}
