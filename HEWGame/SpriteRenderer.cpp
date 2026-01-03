#include "SpriteRenderer.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <d3d11.h>
#pragma comment(lib, "d3dcompiler.lib")


// 定数バッファのデータ構造
struct SpriteConstantBufferData 
{
    DirectX::XMMATRIX World;
    DirectX::XMMATRIX ViewProjection;
};

// 頂点データの構造
struct SpriteVertex 
{
    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT2 uv;
};

HRESULT SpriteRenderer::Init(ID3D11Device* pDevice) 
{
    HRESULT hr;

    // --- 1. 頂点バッファ作成 ---
    SpriteVertex vertices[] = {
        { DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f) }, // 左上
        { DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) }, // 右上
        { DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f) }, // 左下
        { DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) }, // 右下
    };

    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(vertices);
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    D3D11_SUBRESOURCE_DATA initData = { vertices };
    hr = pDevice->CreateBuffer(&bd, &initData, &m_pVertexBuffer);

    // --- 2. 頂点シェーダー読み込み ---
    ID3DBlob* pVSBlob = nullptr;
    hr = D3DCompileFromFile(L"VertexShader.hlsl", nullptr, nullptr, "main", "vs_4_0", 0, 0, &pVSBlob, nullptr);
    pDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &m_pVertexShader);

    // --- 3. 入力レイアウト作成 ---
    D3D11_INPUT_ELEMENT_DESC layout[] = 
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    pDevice->CreateInputLayout(layout, ARRAYSIZE(layout), pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_pInputLayout);
    pVSBlob->Release();

    // --- 4. ピクセルシェーダー読み込み ---
    ID3DBlob* pPSBlob = nullptr;
    hr = D3DCompileFromFile(L"PixelShader.hlsl", nullptr, nullptr, "main", "ps_4_0", 0, 0, &pPSBlob, nullptr);
    pDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &m_pPixelShader);
    pPSBlob->Release();

    // --- 5. サンプラーと定数バッファ ---
    D3D11_SAMPLER_DESC sampDesc = {};
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    pDevice->CreateSamplerState(&sampDesc, &m_pSamplerState);

    D3D11_BUFFER_DESC cbDesc = { sizeof(SpriteConstantBufferData), D3D11_USAGE_DEFAULT, D3D11_BIND_CONSTANT_BUFFER, 0, 0, 0 };
    pDevice->CreateBuffer(&cbDesc, nullptr, &m_pSpriteConstantBuffer);

    return S_OK;
}

void SpriteRenderer::Draw(ID3D11DeviceContext* pContext, ID3D11ShaderResourceView* pSRV, float x, float y, float w, float h, DirectX::XMMATRIX viewProj)
{

    // 行列計算（転置して送る）
    DirectX::XMMATRIX world = DirectX::XMMatrixScaling(w, h, 1.0f) * DirectX::XMMatrixTranslation(x, y, 0.0f);
    SpriteConstantBufferData cb = { DirectX::XMMatrixTranspose(world), DirectX::XMMatrixTranspose(viewProj) };
    pContext->UpdateSubresource(m_pSpriteConstantBuffer, 0, nullptr, &cb, 0, 0);

    // パイプライン設定
    pContext->VSSetShader(m_pVertexShader, nullptr, 0);
    pContext->VSSetConstantBuffers(0, 1, &m_pSpriteConstantBuffer);
    pContext->PSSetShader(m_pPixelShader, nullptr, 0);
    pContext->PSSetSamplers(0, 1, &m_pSamplerState);
    pContext->PSSetShaderResources(0, 1, &pSRV);
    pContext->IASetInputLayout(m_pInputLayout);

    UINT stride = sizeof(SpriteVertex), offset = 0;
    pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
    pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    pContext->Draw(4, 0);
}

void SpriteRenderer::Uninit() 
{
    // このようにクラス名を明示することで、m_pInputLayout などの
    // メンバ変数にアクセスできるようになります。
    if (m_pSpriteConstantBuffer) m_pSpriteConstantBuffer->Release();
    if (m_pSamplerState) m_pSamplerState->Release();
    if (m_pInputLayout) m_pInputLayout->Release();
    if (m_pPixelShader) m_pPixelShader->Release();
    if (m_pVertexShader) m_pVertexShader->Release();
    if (m_pVertexBuffer) m_pVertexBuffer->Release();
}