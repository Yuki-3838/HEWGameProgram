#pragma once
#include <d3d11.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <wrl/client.h>
#pragma comment(lib, "mf.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfreadwrite.lib")
#pragma comment(lib, "mfuuid.lib")

class VideoPlayer
{
private:
	//スマートポインタ使用
	Microsoft::WRL::ComPtr<IMFSourceReader> m_pReader;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTextureSRV;

	UINT m_width;
	UINT m_height;
	UINT m_stride;//映像データ1行当たりのバイト数

	float m_timer;
	float m_frameDuration;//1フレームの時間(秒)

	bool m_isFinished;
	bool m_isLoop;

public:
    VideoPlayer();
    ~VideoPlayer();

    //初期化
    bool Init(const wchar_t* filename, ID3D11Device* device);

    //更新
    void Update(float deltaTime, ID3D11DeviceContext* context);

    //描画用にSRVを取得(SpriteRendererに渡す用)
    ID3D11ShaderResourceView* GetSRV() const { return m_pTextureSRV.Get(); }

    //動画のサイズ取得
    UINT GetWidth() const { return m_width; }
    UINT GetHeight() const { return m_height; }

    //再生終了したか
    bool IsFinished() const { return m_isFinished; }

    //ループ設定
    void SetLoop(bool loop) { m_isLoop = loop; }
};