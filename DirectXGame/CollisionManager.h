#pragma once
#include <vector>
#include <Collider.h>

class CollisionManager
{
public:
	void CheckAllCollisions();
	void RegisterCollider(Collider* _collider) { colliders_.push_back(_collider); };
	void ClearColliderList();

private:
	std::vector<Collider*> colliders_;
	
	void CheckCollisionPair(Collider*, Collider*);
};