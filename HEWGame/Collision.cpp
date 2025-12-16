#include "Collision.h"
#include "HewStd.h"
#include <algorithm>
ColRes CollisionRect(const Object& a, const Object& b)
{
	ColRes colres = ColRes::NONE;

	// オブジェクトの座標とサイズを取得
	DirectX::XMFLOAT3 aPos = a.GetPos();
	DirectX::XMFLOAT3 bPos = b.GetPos();
	DirectX::XMFLOAT3 aSize = a.GetSize();
	DirectX::XMFLOAT3 bSize = b.GetSize();

	// AABB外側判定（衝突していない場合）
	if (aPos.x >= bPos.x + bSize.x ||    // aがbの右側
		aPos.x + aSize.x <= bPos.x ||    // aがbの左側
		aPos.y >= bPos.y + bSize.y ||    // aがbの下側
		aPos.y + aSize.y <= bPos.y)      // aがbの上側
	{
		return colres; // 当たっていので早期リターン
	}

//---- 当たっている場合の処理 -------------------------------//
	
	// 接触方向の計算
	// どれくらい重なっているかを計算
	float overlapLeft = (aPos.x + aSize.x) - bPos.x;	// 左側に
	float overlapRight = (bPos.x + bSize.x) - aPos.x;	// 右側に
	float overlapTop = (aPos.y + aSize.y) - bPos.y;		// 上側に
	float overlapBottom = (bPos.y + bSize.y) - aPos.y;	// 下側に

	float minOverlap = (std::min)({ overlapLeft, overlapRight, overlapTop, overlapBottom });


	if (minOverlap == overlapLeft)			colres = ColRes::LEFT;		// 一番重なりが少ないのが左なら左
	else if (minOverlap == overlapRight)	colres = ColRes::RIGHT;		// 一番重なりが少ないのが右なら右
	else if (minOverlap == overlapTop)		colres = ColRes::TOP;		// 一番重なりが少ないのが上なら上
	else if (minOverlap == overlapBottom)	colres = ColRes::BOTTOM;	// 一番重なりが少ないのが下なら下
	
	return colres;  // 当たっている状態を返す
}

ColRes CollisionRect(const Object& a, DirectX::XMFLOAT3& bPos, const DirectX::XMFLOAT3& bSize)
{

	ColRes colres = ColRes::NONE;

	DirectX::XMFLOAT3 aPos = a.GetPos();
	DirectX::XMFLOAT3 aSize = a.GetSize();

	// AABB外側判定（衝突していない場合）
	if (aPos.x >= bPos.x + bSize.x ||    // aがbの右側
		aPos.x + aSize.x <= bPos.x ||    // aがbの左側
		aPos.y >= bPos.y + bSize.y ||    // aがbの下側
		aPos.y + aSize.y <= bPos.y)      // aがbの上側
	{
		return colres; // 当たっていので早期リターン
	}

	//---- 当たっている場合の処理 -------------------------------//

	// 接触方向の計算
	// どれくらい重なっているかを計算
	float overlapLeft = (aPos.x + aSize.x) - bPos.x;	// 左側に
	float overlapRight = (bPos.x + bSize.x) - aPos.x;	// 右側に
	float overlapTop = (aPos.y + aSize.y) - bPos.y;		// 上側に
	float overlapBottom = (bPos.y + bSize.y) - aPos.y;	// 下側に

	float minOverlap = (std::min)({ overlapLeft, overlapRight, overlapTop, overlapBottom });


	if (minOverlap == overlapLeft)			colres = ColRes::LEFT;		// 一番重なりが少ないのが左なら左
	else if (minOverlap == overlapRight)	colres = ColRes::RIGHT;		// 一番重なりが少ないのが右なら右
	else if (minOverlap == overlapTop)		colres = ColRes::TOP;		// 一番重なりが少ないのが上なら上
	else if (minOverlap == overlapBottom)	colres = ColRes::BOTTOM;	// 一番重なりが少ないのが下なら下

	return colres;  // 当たっている状態を返す
}

ColRes CollisionRect(const DirectX::XMFLOAT3& aPos, const DirectX::XMFLOAT3& aSize, const DirectX::XMFLOAT3& bPos, const DirectX::XMFLOAT3& bSize)
{
	ColRes colres = ColRes::NONE;

	// AABB外側判定（衝突していない場合）
	if (aPos.x >= bPos.x + bSize.x ||    // aがbの右側
		aPos.x + aSize.x <= bPos.x ||    // aがbの左側
		aPos.y >= bPos.y + bSize.y ||    // aがbの下側
		aPos.y + aSize.y <= bPos.y)      // aがbの上側
	{
		return colres; // 当たっていので早期リターン
	}

	//---- 当たっている場合の処理 -------------------------------//

	// 接触方向の計算
	// どれくらい重なっているかを計算
	float overlapLeft = (aPos.x + aSize.x) - bPos.x;	// 左側に
	float overlapRight = (bPos.x + bSize.x) - aPos.x;	// 右側に
	float overlapTop = (aPos.y + aSize.y) - bPos.y;		// 上側に
	float overlapBottom = (bPos.y + bSize.y) - aPos.y;	// 下側に

	float minOverlap = (std::min)({ overlapLeft, overlapRight, overlapTop, overlapBottom });


	if (minOverlap == overlapLeft)			colres = ColRes::LEFT;		// 一番重なりが少ないのが左なら左
	else if (minOverlap == overlapRight)	colres = ColRes::RIGHT;		// 一番重なりが少ないのが右なら右
	else if (minOverlap == overlapTop)		colres = ColRes::TOP;		// 一番重なりが少ないのが上なら上
	else if (minOverlap == overlapBottom)	colres = ColRes::BOTTOM;	// 一番重なりが少ないのが下なら下

	return colres;  // 当たっている状態を返す
}