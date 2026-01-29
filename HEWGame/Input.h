#pragma once
#include <d3d11.h>  // DirectX11���g�����߂̃w�b�_�[�t�@�C��
#include <DirectXMath.h> // DirextX�̐��w�֘A�̃w�b�_�[�t�@�C��

#include <Xinput.h> //XInput(XBOX�K�i�̃R���g���[���[)���g�����߂̃w�b�_�[�t�@�C��
#pragma comment (lib, "xinput.lib") //XInput���g�����߂ɕK�v

#include<Dinput.h> //Dinput(PS�̃R���g���[����)���g�����߂̃w�b�_�[�t�@�C��
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib") //DInput���g�����߂ɕK�v

#define XINPUT_A              0x1000
#define XINPUT_B              0x2000
#define XINPUT_X              0x4000
#define XINPUT_Y              0x8000
#define XINPUT_UP             0x0001
#define XINPUT_DOWN           0x0002
#define XINPUT_LEFT           0x0004
#define XINPUT_RIGHT          0x0008
#define XINPUT_START          0x0010
#define XINPUT_BACK           0x0020
#define XINPUT_LEFT_THUMB     0x0040 //���X�e�B�b�N��������
#define XINPUT_RIGHT_THUMB    0x0080 //�E�X�e�B�b�N��������
#define XINPUT_LEFT_SHOULDER  0x0100 //L
#define XINPUT_RIGHT_SHOULDER 0x0200 //R

#define VK_A 0x41
#define VK_B 0x42
#define VK_C 0x43
#define VK_D 0x44
#define VK_E 0x45
#define VK_F 0x46
#define VK_G 0x47
#define VK_H 0x48
#define VK_I 0x49
#define VK_J 0x4A
#define VK_K 0x4B
#define VK_L 0x4C
#define VK_M 0x4D
#define VK_N 0x4E
#define VK_O 0x4F
#define VK_P 0x50
#define VK_Q 0x51
#define VK_R 0x52
#define VK_S 0x53
#define VK_T 0x54
#define VK_U 0x55
#define VK_V 0x56
#define VK_W 0x57
#define VK_X 0x58
#define VK_Y 0x59
#define VK_Z 0x5A

class Input{
private:

	//ウィンドウハンドル（マウス座標取得用）
	HWND m_hWnd;

	//ゲームの論理解像度（座標変換用）
	int m_gameWidth;
	int m_gameHeight;

	//マウス入力情報
	POINT m_mousePos;
	bool m_mouseLeftState;
	bool m_mouseLeftState_old;

	//�L�[���͏���ۑ�����ϐ�
	BYTE keyState[256] = {};
	BYTE keyState_old[256] = {};

	//XInput�R���g���[���[���͏���ۑ�����ϐ�
	XINPUT_STATE controllerState = {};
	XINPUT_STATE controllerState_old = {};

	//Dinput�̃R���g���[���[�p
	IDirectInput8* m_pDI = nullptr; // DirectInput �{��
	IDirectInputDevice8* m_pKeyboard = nullptr; // �L�[�{�[�h�f�o�C�X

	int VibrationTime; //�U���p�����Ԃ��J�E���g����ϐ�

public:

	Input(); //�R���X�g���N�^
	~Input(); //�f�X�g���N�^
	void Update(); //�X�V

	//�L�[����
	bool GetKeyPress(int key);   //�v���X(�����Ă���Ԃ�����)
	bool GetKeyTrigger(int key); //�g���K�[(�����n�߂���)
	bool GetKeyRelease(int key); //�����[�X(�����I�������)

	//�A�i���O�X�e�B�b�N(�R���g���[���[)
	DirectX::XMFLOAT2 GetLeftAnalogStick(void);
	DirectX::XMFLOAT2 GetRightAnalogStick(void);

	//�g���K�[(�R���g���[���[)
	float GetLeftTrigger(void);
	float GetRightTrigger(void);

	//�{�^������(�R���g���[���[)
	bool GetButtonPress(WORD btn);   //�v���X(�����Ă���Ԃ�����)
	bool GetButtonTrigger(WORD btn); //�g���K�[(�����n�߂���)
	bool GetButtonRelease(WORD btn); //�����[�X(�����I�������)

	//�U��(�R���g���[���[)
	//flame�F�U�����p�����鎞��(�P�ʁF�t���[��)
	//powoe�F�U���̋���(0�`1)
	void SetVibration(int frame = 1, float powor = 1);

	//マウス入力
	void SetHWnd(HWND hWnd); //ウィンドウハンドル設定
	void SetGameResolution(int width, int height); //ゲーム論理解像度を設定
	DirectX::XMFLOAT2 GetMousePosition(); //マウス座標取得（ゲーム座標系に変換済み）
	bool GetMouseLeftPress();   //左クリック押している
	bool GetMouseLeftTrigger(); //左クリック押した瞬間
	bool GetMouseLeftRelease(); //左クリック離した瞬間
};