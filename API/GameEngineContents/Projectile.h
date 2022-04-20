#pragma once
#include <GameEngine/GameEngineActor.h>
#include <vector>
#include <GameEngineBase/GameEngineSound.h>

enum class BulletType
{
	KNIFE,
	FLAME_BLUE,
	FLAME_RED
};


// �÷��̾��� �߻�ü( Į, ����, ����, ...)
class GameEngineRenderer;
class GameEngineCollision;
class Projectile : public GameEngineActor
{	
public:

	// constrcuter destructer
	Projectile();
	~Projectile();

	// delete Function
	Projectile(const Projectile& _Other) = delete;
	Projectile(Projectile&& _Other) noexcept = delete;
	Projectile& operator=(const Projectile& _Other) = delete;
	Projectile& operator=(Projectile&& _Other) noexcept = delete;

	void SetType(BulletType _BT);

	void SetDir(float4 _Direction);

	GameEngineRenderer* GetProjImage()
	{
		return ProjImage_;
	}


	int GetDamage()
	{
		return Damage_;
	}

	void SetDamage(int _Damage)
	{
		Damage_ = _Damage;
	}

protected:

	void Start() override;
	void Update() override;
	void Render() override;

private:

	GameEngineRenderer* ProjImage_;
	GameEngineCollision* ProjCol_;

	float4 ShootDir_;
	int Damage_;
	float Speed_;

	// �Ѿ� ���ӽð�
	float Duration_;

};

