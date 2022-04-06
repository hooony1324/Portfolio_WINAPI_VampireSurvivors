#pragma once
#include "Enemy.h"
#include <vector>

// 설명 :
class ShadeRed : public Enemy
{
public:
	// constrcuter destructer
	ShadeRed();
	~ShadeRed();

	// delete Function
	ShadeRed(const ShadeRed& _Other) = delete;
	ShadeRed(ShadeRed&& _Other) noexcept = delete;
	ShadeRed& operator=(const ShadeRed& _Other) = delete;
	ShadeRed& operator=(ShadeRed&& _Other) noexcept = delete;

private:

	void Start() override;
	void Update() override;
	void Render() override;

	// 기본 정보
	float Speed_;
	int Hp_;


	// 피격
	GameEngineRenderer* ShadeRed_;
	GameEngineCollision* ShadeRedCol_;
	GameEngineCollision* AttackCheck_;


	// 서로 밀어내기
	void BlockOther();
	GameEngineCollision* OtherBlockUp_;
	GameEngineCollision* OtherBlockDown_;
	GameEngineCollision* OtherBlockLeft_;
	GameEngineCollision* OtherBlockRight_;
	std::vector<GameEngineCollision*> Others_;

};

