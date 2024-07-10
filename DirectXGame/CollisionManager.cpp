#include "CollisionManager.h"
#include <Vector3/calc/vector3calc.h>

void CollisionManager::CheckAllCollisions()
{
	// リスト内のペアを総当り
	std::vector<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA)
	{
		Collider* colliderA = *itrA;
		// イテレータBはイテレータAの次の要素から回す (重複判定を回避)
		std::vector<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB)
		{
			Collider* colliderB = *itrB;
			// ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}
}

void CollisionManager::ClearColliderList()
{
	colliders_.clear();
}

void CollisionManager::CheckCollisionPair(Collider* _colliderA, Collider* _colliderB)
{
	// 衝突フィルタリング
	if (
		!(_colliderA->GetCollisionAttribute() & _colliderB->GetCollisionMask()) ||
		!(_colliderB->GetCollisionAttribute() & _colliderA->GetCollisionMask())
		) return;

	Vector3 positionA = _colliderA->GetWorldPosition();
	Vector3 positionB = _colliderB->GetWorldPosition();
	// 判定に必要な前計算
	float radiusCompd = _colliderA->GetRadius() + _colliderB->GetRadius();
	Vector3 AtoBVect = positionB - positionA;
	float distAtoB = Length(AtoBVect);
	// 球と球の交差判定
	if (distAtoB < radiusCompd)
	{
		// コライダーAの衝突時コールバック呼出
		_colliderA->OnCollision();
		// コライダーBの衝突時コールバック呼出
		_colliderB->OnCollision();
	}
}
