#pragma once
#include "CObjectBase.h"

// アイテムのベースクラス
class CItem : public CObjectBase
{
public:
	// コンストラクタ
	CItem();
	// デストラクタ
	virtual ~CItem();

	// アイテムをアタッチする行列を設定
	void SetAttachMtx(const CMatrix* mtx);

	// 行列を取得
	CMatrix Matrix() const override;

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// コライダーを取得
	CCollider* Collider() const;

	//更新
	void Update() override;

	// 行列を更新
	void UpdateMtx();

protected:
	CCollider* mpCollider;		// アイテムのコライダー
	const CMatrix* mpAttachMtx;	// くっつける行列のポインター
	CMatrix mAttachMtx;			// くっつける行列の本体

	bool mIsGravity;
	bool mIsGrounded;
	float mMoveSpeedY;
};