#include "ResourceManager.h"
#include "stb_image.h"

ResourceManager::ResourceManager()
{
    m_pDevice = nullptr;
}

ID3D11ShaderResourceView* ResourceManager::LoadTexture(const std::string& fileName, ID3D11Device* pDevice)
{
    m_pDevice = pDevice;
    // 1. すでに読み込み済みかチェック
    if (m_Resources.count(fileName) > 0)
    {
        return m_Resources[fileName];
    }

    // 2. stbiを使って画像をメモリ上に読み込む
    int width, height, channels;
    // 最後の引数を「4」にすることで、強制的にRGBA形式で読み込む
    unsigned char* pixels = stbi_load(fileName.c_str(), &width, &height, &channels, 4);

    if (!pixels)
    {
        // 読み込み失敗
        return nullptr;
    }

    // 3. DirectX 11 テクスチャの作成
    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.Width = width;
    texDesc.Height = height;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // RGBA 8bit
    texDesc.SampleDesc.Count = 1;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE; // シェーダーで使う設定

    // 最初のデータ（画素データ）を指定
    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = pixels;              // 画素データの先頭ポインタ
    initData.SysMemPitch = width * 4;       // 横1行のバイト数

    ID3D11Texture2D* pTexture = nullptr;
    HRESULT hr = pDevice->CreateTexture2D(&texDesc, &initData, &pTexture);

    // 読み込んだ画素データはテクスチャ作成が終われば不要なので即解放
    stbi_image_free(pixels);

    if (FAILED(hr)) return nullptr;

    // 4. シェーダーリソースビュー(SRV)の作成
    ID3D11ShaderResourceView* pSRV = nullptr;
    hr = pDevice->CreateShaderResourceView(pTexture, nullptr, &pSRV);

    // テクスチャ本体のポインタはSRVが内部で管理するためReleaseしてOK
    pTexture->Release();

    if (FAILED(hr)) return nullptr;

    // 5. 保管庫に登録して返す
    m_Resources[fileName] = pSRV;
    return pSRV;
}


void ResourceManager::UninitAll() 
{
    // マップ内のすべてのSRVを解放
    for (auto& pair : m_Resources)
    {
        if (pair.second) 
        {
            pair.second->Release();
        }
    }
    m_Resources.clear();
}
