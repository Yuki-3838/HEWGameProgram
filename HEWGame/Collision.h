#pragma once
#include "GameObject.h"

//	// 書式
// 
//  //---- 判定の取得 ----//
// 
//    ColRes hit = CollisionRect(player, enemy); // オブジェクト同士の判定
//    ColRes hit = CollisionRect(player, PosB, SizeB);  // オブジェクトと座標・サイズで判定
//    ColRes hit = CollisionRect(PosA, SizeA,PosB, SizeB);  // 座標・サイズ同士で判定
//
//	//---- 条件式 ----//
//	if (Col::None(hit)) { /* 非接触 */ }
//	if (Col::Any(hit)) { /* 接触 */ }
//	if (Col::IsLeft(hit)) { /* 左側で接触 */ }
//	if (Col::IsRight(hit)) { /* 右側で接触 */ }
//	if (Col::IsTop(hit)) { /* 上側で接触 */ }
//	if (Col::IsBottom(hit)) { /* 下側で接触 */ }
//	if (Col::Has(hit, ColRes::TOP | ColRes::LEFT)) { /* 左か上か両方か */ } // 任意の2方向の内どちらか、または両方からの接触
//	if (Col::HasAll(hit, ColRes::TOP | ColRes::LEFT)) { /* 左上の時 */ } // 任意の2方向から同時に
//	if (!Col::IsLeft(hit) && Col::Any(hit)) { /* 左以外の方向に当たっている（右・上・下のいずれか) */ } // 組み合わせ



// 接触したかとその方向 (上下方向より左右方向を優先、左右方向で同じなら左方向を優先)
enum class ColRes : unsigned
{
	NONE = 0u,	// 接触してない	if(!NONE) = 接触している
	TOP = 1u << 0,		//0001 上から接触
	BOTTOM = 1u << 1,	// 0010 下から接触
	LEFT = 1u << 2,		// 0100 左から接触
	RIGHT = 1u << 3,	// 1000 右から接触
	//lihio
};

// オペレーター演算子

// 合成（TOP | LEFT など）→ ColRes に返す。複数方向の条件を組み合わせを可能に
inline ColRes operator|(ColRes a, ColRes b) {
	return static_cast<ColRes>(static_cast<unsigned>(a) | static_cast<unsigned>(b));
}

// 包含チェック（hit & LEFT）を if に直接書けるように、unsigned を返す
inline unsigned operator&(ColRes a, ColRes b) {
	return static_cast<unsigned>(a) & static_cast<unsigned>(b);
}

// 反転（未使用の上位ビットも反転するから注意）多分使わない
inline ColRes operator~(ColRes a) {
	return static_cast<ColRes>(~static_cast<unsigned>(a));
}

// 糖衣：読みやすいヘルパ
namespace Col {
	inline bool None(ColRes col) { return col == ColRes::NONE; }							// 非接触
	inline bool Any(ColRes col) { return col != ColRes::NONE; }								// 接触
	inline bool IsTop(ColRes colRes) { return (colRes & ColRes::TOP) != 0u; }				// 上から
	inline bool IsBottom(ColRes colRes) { return (colRes & ColRes::BOTTOM) != 0u; }			// 下から
	inline bool IsLeft(ColRes colRes) { return (colRes & ColRes::LEFT) != 0u; }				// 左から
	inline bool IsRight(ColRes colRes) { return (colRes & ColRes::RIGHT) != 0u; }			// 右から
	inline bool Has(ColRes colRes1, ColRes colRes2) { return (colRes1 & colRes2) != 0u; }	// 引数に入れた任意の二方向のどちらか
	inline bool HasAll(ColRes col, ColRes bits) { return (col & bits) == static_cast<unsigned>(bits);} // // 引数に入れた任意の二方向の両方が
}

//長方形(AABB)での接触判定、返り値はbool型の接触しているか、enumでAがBに接触した方向
ColRes CollisionRect(const GameObject& a, const GameObject& b); // オブジェクト同士の判定
ColRes CollisionRect(const GameObject& a, const DirectX::XMFLOAT2& bPos, const DirectX::XMFLOAT2& bSize); // オブジェクトと座標・サイズで判定
ColRes CollisionRect(const DirectX::XMFLOAT2& aPos, const DirectX::XMFLOAT2& aSize, const DirectX::XMFLOAT2& bPos, const DirectX::XMFLOAT2& bSize); // 座標・サイズ同士で判定