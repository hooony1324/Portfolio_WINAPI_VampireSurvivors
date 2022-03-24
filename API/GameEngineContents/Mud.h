#pragma once
#include <GameEngine/GameEngineActor.h>

// ���� :
class Mud : public GameEngineActor
{
public:
	// constrcuter destructer
	Mud();
	~Mud();

	// delete Function
	Mud(const Mud& _Other) = delete;
	Mud(Mud&& _Other) noexcept = delete;
	Mud& operator=(const Mud& _Other) = delete;
	Mud& operator=(Mud&& _Other) noexcept = delete;

protected:

	void Start() override;
	void Update() override;
	void Render() override;

private:

};
