#pragma once

#include <functional>
#include <cstdint>

/// <summary>
/// 時限発動
/// </summary>
class TimedCall
{
public:
	// コンストラクタ
	TimedCall(std::function<void(void)> _function, uint32_t _time);
	// 更新
	void Update();
	// 完了ならtrueを返す
	bool IsFinished() { return isFinished_; }

private:
	// コールバック
	std::function<void(void)> function_;
	// 残り時間
	uint32_t time_;
	// 完了フラグ
	bool isFinished_ = false;
};