#include "Enemy.h"

void Enemy::Phase_Approach()
{
	// 移動 (ベクトルを加算)
	worldTransform_.translation_ += Vector3(0, 0, -0.75f);
	// 規定の位置に到着したら離脱
	if (worldTransform_.translation_.z < 0.0f)
	{
		phase_ = Phase::Leave;
	}
	return;
}
void Enemy::Phase_Leave()
{
	// 移動 （ベクトル加算）
	worldTransform_.translation_ += Vector3(-0.5f, 0.5f, 1.0f);
	return;
}