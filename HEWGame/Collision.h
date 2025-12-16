#pragma once
#include "Object.h"

// 接触したかとその方向 (上下方向より左右方向を優先、左右方向で同じなら左方向を優先)
enum class ColRes
{
	NONE,	// 接触してない	if(!NONE) = 接触している
	TOP,	// 上から接触
	BOTTOM,	// 下から接触
	LEFT,	// 左から接触
	RIGHT,	// 右から接触
};

//長方形(AABB)での接触判定、返り値はbool型の接触しているか、enumでAがBに接触した方向
ColRes CollisionRect(const Object& a, const Object& b); // オブジェクト同士の判定
ColRes CollisionRect(const Object& a, DirectX::XMFLOAT3& bPos, const DirectX::XMFLOAT3& bSize); // オブジェクトと座標・サイズで判定
ColRes CollisionRect(const DirectX::XMFLOAT3& aPos, const DirectX::XMFLOAT3& aSize, const DirectX::XMFLOAT3& bPos, const DirectX::XMFLOAT3& bSize); // 座標・サイズ同士で判定