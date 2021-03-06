#pragma once
#include <string>
#include <GameEngineBase/GameEngineMath.h>

#include "GameEnum.h"

enum class CharacterType
{
	Imelda,
	Cavallo,
	Old,
	MAX
};

// 설명 : 
class Character
{
public:

public:
	// constrcuter destructer
	Character();
	Character(CharacterType _CharacterType);
	~Character();

	// delete Function
	Character(const Character& _Other) = delete;
	Character(Character&& _Other) noexcept = delete;
	Character& operator=(const Character& _Other) = delete;
	Character& operator=(Character&& _Other) noexcept = delete;

	void SetCharacter(CharacterType _CharacterType);


	//캐릭터 생성시 정보
public:
	std::string ImageName_;
	std::string Name_;
	std::string WalkRightAnim_;
	std::string WalkLeftAnim_;

	// 패시브 기본 능력치 (체력, 스피드, 공격력, ...)
	float Speed_;			// 이동속도
	float MaxHp_;			
	float CurrentHp_;		// 체력 깎여있는 채로 시작하는 캐릭터 있음 
	float Recovery_;		// 체력 재생
	float Guard_;			// 방어력
	float Power_;			// 추가 데미지, 괴력
	float MeleeRange_;		// 근접 공격 범위(성서, 마늘, 성수, 번개 등 범위 공격만 해당)
	float ShootSpeedRatio_;	// 전체 투사체 속도 증가비율
	float Duration_;		// 스킬 지속시간
	float AddShootNum_;		// 추가 투사체 수
	float CoolTime_;


	// 패시브 서브 능력치
	float Luck_;			// 아이템 확률 3개 -> 4개
	float Growth_;			// 경험치 획득시 추가 경험치
	float Greed_;			// 돈 획득시 추가 돈
	float Magnet_;			// 아이템 획득 범위
	int Revival_;			// 부활 횟수

	SkillType BaseSkill_;
};

