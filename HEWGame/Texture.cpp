#include "Texture.h"
#include "stb_image.h"

//テクスチャをロード
HRESULT LoadTexture(ID3D11Device* device, const char* filename, ID3D11ShaderResourceView** srv)
{
    bool sts = true;
    unsigned char* pixels;

    int width;//幅
    int height;//高さ
    int bpp;//BPP

    //画像読み込み
    pixels = stbi_load(filename, &width, &height, &bpp, 4);
    if (pixels == nullptr)
    {
        MessageBoxA(NULL, filename, "load error", MB_OK);
        return S_FALSE;
    }

    //テクスチャ2Dリソース生成
    ID3D11Texture2D* pTexture;

    D3D11_TEXTURE2D_DESC desc{};
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // RGBA
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA subResource{};
    subResource.pSysMem = pixels;
    subResource.SysMemPitch = desc.Width * 4; // RGBA = 4 bytes per pixel
    subResource.SysMemSlicePitch = 0;

    HRESULT hr = device->CreateTexture2D(&desc, &subResource, &pTexture);
    if (FAILED(hr))
    {
        stbi_image_free(pixels);
        return hr;
    }

    //SRV(シェーダーリソースビュー)の生成
    hr = device->CreateShaderResourceView(pTexture, nullptr, srv);
    if (FAILED(hr))
    {
        stbi_image_free(pixels);
        pTexture->Release();
        return hr;
    }

    //ID3D11Texture2Dを解放
    pTexture->Release();

    //ピクセルイメージ解放
    stbi_image_free(pixels);

    return S_OK;
}
