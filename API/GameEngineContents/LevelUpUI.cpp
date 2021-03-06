#include "LevelUpUI.h"
#include <GameEngine/GameEngine.h>
#include <GameEngineBase/GameEngineWindow.h>
#include <GameEngine/GameEngineImageManager.h>
#include <GameEngine/GameEngineRenderer.h>
#include <GameEngineBase/GameEngineInput.h>
#include <GameEngineBase/GameEngineSound.h>
#include <GameEngineBase/GameEngineRandom.h>
#include <string>

#include "GameEnum.h"
#include "GameInfo.h"

#include "StatUI.h"

int LevelUpUI::CreateCount_ = 0;
bool LevelUpUI::IsActivated_ = false;
LevelUpUI::STATE LevelUpUI::State_ = STATE::NONE;

bool LevelUpUI::GetBox_ = false;


LevelUpUI::LevelUpUI() 
	: EvolveSkill_(SkillType::NONE)
{

}

LevelUpUI::~LevelUpUI() 
{
}

void LevelUpUI::Start()
{
	NextLevelOff();

	// 배경 
	BoxBackGround_ = CreateRenderer("LevelUpUI.bmp", static_cast<int>(RENDER_ORDER::UI));
	SetScale(BoxBackGround_->GetScale());
	BoxBackGround_->CameraEffectOff();
	BoxBackGround_->SetPivot(GameEngineWindow::GetScale().Half() + float4{0, 10});
	BoxBackGround_->Off();


	// 스탯 UI 같이 킴
	if (nullptr == StatUI_)
	{
		StatUI_ = GetLevel()->CreateActor<StatUI>(static_cast<int>(ACTOR_ORDER::UI), "UI");
	}
	else
	{
		StatUI_->On();
	}
	
	// 사운드
	GameEngineSound::SoundPlayOneShot("LevelUp.MP3", 0);
	IsActivated_ = true;

	// 박스 얻음
	if (true == GetBox_)
	{
		EvolveSkill_ = GameInfo::SkillEvolveCheck();

		// 스킬이 꽉 차지 않았다면(각성포함)
		bool Full = GameInfo::EvolveSkillLevelFull();

		if (false == Full)
		{
			ChangeState(STATE::TREASURE);
		}
		else
		{
			// 꽉 찼다면
			if (SkillType::NONE != EvolveSkill_)
			{
				ChangeState(STATE::TREASURE);
			}
			else
			{
				ChangeState(STATE::HPMONEY);
			}
		}

	}
	// 레벨업
	else
	{
		if (true == GameInfo::SkillLevelFull())
		{
			ChangeState(STATE::HPMONEY);
		}
		else
		{
			ChangeState(STATE::BOXES);
		}
	}

}

void LevelUpUI::Update()
{
	UpdateState();
}

void LevelUpUI::Render()
{
}



void LevelUpUI::ShowRandomSkills()
{
	if (0 >= SelectNum_)
	{
		MsgBoxAssert("잘못된 LevelUPUI분기로 들어왔습니다");
		return;
	}

	if (1 <= SelectNum_)
	{
		SkillType SelectSkill = RandomSkills_[0];
		Renderer1_->SetImage("LevelUp" + SkillTypeToName(SelectSkill) + ".bmp");
	}


	if (2 <= SelectNum_)
	{
		SkillType SelectSkill = RandomSkills_[1];
		Renderer2_->SetImage("LevelUp" + SkillTypeToName(SelectSkill) + ".bmp");
	}


	if (3 <= SelectNum_)
	{
		SkillType SelectSkill = RandomSkills_[2];
		Renderer3_->SetImage("LevelUp" + SkillTypeToName(SelectSkill) + ".bmp");
	}


	if (4 <= SelectNum_)
	{
		SkillType SelectSkill = RandomSkills_[3];
		Renderer4_->SetImage("LevelUp" + SkillTypeToName(SelectSkill) + ".bmp");
	}


}

void LevelUpUI::SelectSkillBox()
{
	// 아이템 선택
	if (true == GameEngineInput::GetInst()->IsDown("Num1") && SelectNum_ >= 1)
	{
		// 플레이어에게 무기 정보 갱신
		SelectSkill(RandomSkills_[0]);

		RandomSkills_.clear();

		// 종료
		UIEnd();
		return;
	}

	// 아이템 선택
	if (true == GameEngineInput::GetInst()->IsDown("Num2") && SelectNum_ >= 2)
	{
		// 플레이어에게 무기 정보 갱신
		SelectSkill(RandomSkills_[1]);

		RandomSkills_.clear();

		// 종료
		UIEnd();
		return;
	}

	// 아이템 선택
	if (true == GameEngineInput::GetInst()->IsDown("Num3") && SelectNum_ >= 3)
	{
		// 플레이어에게 무기 정보 갱신
		SelectSkill(RandomSkills_[2]);

		RandomSkills_.clear();

		// 종료
		UIEnd();
		return;
	}

	// 아이템 선택
	if (true == GameEngineInput::GetInst()->IsDown("Num4") && SelectNum_ >= 4)
	{
		// 플레이어에게 무기 정보 갱신
		SelectSkill(RandomSkills_[3]);

		RandomSkills_.clear();

		// 종료
		UIEnd();
		return;
	}
}

void LevelUpUI::SelectSkill(SkillType _SkillType)
{
	std::map<SkillType, int>& AllSkillLevel = GameInfo::GetPlayerInfo()->AllSkillLevel_;

	// 새로운 스킬
	if ( 0 == AllSkillLevel[_SkillType])
	{
		if (ACTIVE_MAX > static_cast<int>(_SkillType))
		{
			GameInfo::GetPlayerInfo()->ActiveSkillSlot_.push_back(_SkillType);
		}
		else if (ACTIVE_MAX <= static_cast<int>(_SkillType) && SkillType::MAX > _SkillType)
		{
			GameInfo::GetPlayerInfo()->PassiveSkillSlot_.push_back(_SkillType);
		}

		AllSkillLevel[_SkillType] = 1;
	}
	else
	{
		// 이미 있는 스킬 레벨업
		AllSkillLevel[_SkillType] += 1;
	}
}


void LevelUpUI::UpdateState()
{
switch (State_)
{
case LevelUpUI::STATE::BOXES:
	BoxesUpdate();
	break;
case LevelUpUI::STATE::TREASURE:
	TreasureUpdate();
	break;
case LevelUpUI::STATE::TREASURE_OPENING:
	TreasureOpeningUpdate();
	break;
case LevelUpUI::STATE::TREASURE_END:
	TreasureEndingUpdate();
	break;
case LevelUpUI::STATE::HPMONEY:
	HpMoneyUpdate();
	break;
default:
	break;
}
}

void LevelUpUI::ChangeState(STATE _State)
{
	switch (_State)
	{
	case LevelUpUI::STATE::BOXES:
		BoxesStart();
		break;
	case LevelUpUI::STATE::TREASURE:
		TreasureStart();
		break;
	case LevelUpUI::STATE::TREASURE_OPENING:
		TreasureOpeningStart();
		break;
	case LevelUpUI::STATE::TREASURE_END:
		TreasureEndingStart();
		break;
	case LevelUpUI::STATE::HPMONEY:
		HpMoneyStart();
		break;
	default:
		break;
	}

	State_ = _State;
}

void LevelUpUI::BoxesStart()
{
	BoxBackGround_->On();

	// 무기 선택 박스 1~4에 띄울 스킬(액티브/패시브) 선택
	SelectNum_ = 3;

	// 무기 선택 박스 1
	Renderer1_ = CreateRenderer("LevelupBlank.bmp", static_cast<int>(RENDER_ORDER::UI));
	Renderer1_->CameraEffectOff();
	Renderer1_->SetPivot(GameEngineWindow::GetScale().Half() + float4{ 2, -Renderer1_->GetImageScale().y });


	// 무기 선택 박스 2
	Renderer2_ = CreateRenderer("LevelupBlank.bmp", static_cast<int>(RENDER_ORDER::UI));
	Renderer2_->CameraEffectOff();
	Renderer2_->SetPivot(GameEngineWindow::GetScale().Half() + float4{ 2, 0 });

	// 무기 선택 박스 3
	Renderer3_ = CreateRenderer("LevelupBlank.bmp", static_cast<int>(RENDER_ORDER::UI));
	Renderer3_->CameraEffectOff();
	Renderer3_->SetPivot(GameEngineWindow::GetScale().Half() + float4{ 2, Renderer1_->GetImageScale().y });

	// 무기 선택 박스 4 띄울지 여부
	GameInfo::GetPlayerInfo()->Luck_ += GameInfo::GetPlayerInfo()->AllSkillLevel_[SkillType::CLOVER] * 3;
	float PlayerLuck = GameInfo::GetPlayerInfo()->Luck_;
	int Success = Random.RandomInt(1, 101);

	// 성공
	if (Success <= PlayerLuck)
	{
		// 무기 선택 박스 4
		Renderer4_ = CreateRenderer("LevelupBlank.bmp", static_cast<int>(RENDER_ORDER::UI));
		Renderer4_->CameraEffectOff();
		Renderer4_->SetPivot(GameEngineWindow::GetScale().Half() + float4{ 2, Renderer1_->GetImageScale().y * 2 });

		SelectNum_ = 4;
	}

	// SelectNum_확인
	int MaxLevelCount = 0;

	std::map<SkillType, int>::iterator FindIter = GameInfo::GetPlayerInfo()->AllSkillLevel_.begin();
	std::map<SkillType, int>::iterator EndIter = GameInfo::GetPlayerInfo()->AllSkillLevel_.end();
	for (; FindIter != EndIter; ++FindIter)
	{
		if ((*FindIter).first == SkillType::MAX)
		{
			break;
		}

		if (SKILL_LEVELMAX <= (*FindIter).second || -1 == (*FindIter).second)
		{
			MaxLevelCount++;
		}
	}

	int SelectableCount = static_cast<int>(SkillType::MAX) - MaxLevelCount;
	if (SelectableCount < SelectNum_)
	{
		SelectNum_ = SelectableCount;
	}


	std::set<SkillType> Skills;
	// RandomSkills_에 SelectNum만큼의 스킬 Push
	// 넣을 수 있는 스킬 : 0레벨 && -1레벨X 
	while (Skills.size() < SelectNum_)
	{
		SkillType Skill = SelectBoxSkills();
		if (SkillType::NONE != Skill)
		{
			Skills.insert(Skill);
		}
	}

	for (SkillType Skill : Skills)
	{
		RandomSkills_.push_back(Skill);
	}
	Skills.clear();
	
	ShowRandomSkills();
}

void LevelUpUI::BoxesUpdate()
{
	SelectSkillBox();
}

void LevelUpUI::TreasureStart()
{
	SelectedTreasure_ = SelectTreasure();

	if (SkillType::NONE == SelectedTreasure_)
	{
		ChangeState(STATE::BOXES);
	}

	// 보물상자
	GameEngineSound::SoundPlayOneShot("TreasureFound.mp3", 0);
	BoxAnim_ = CreateRenderer("BoxFound.bmp", static_cast<int>(RENDER_ORDER::UI));
	BoxAnim_->CameraEffectOff();
	BoxAnim_->SetPivot(GameEngineWindow::GetScale().Half() + float4{ 0, 10 });
	BoxAnim_->CreateAnimation("Box_Bounce.bmp", "Box_Bounce", 0, 89, 0.025f, true);
	BoxAnim_->CreateAnimation("Box_Opening.bmp", "Box_Opening", 0, 209, 0.025f, false);
	BoxAnim_->CreateAnimation("Box_Ending.bmp", "Box_Ending", 0, 172, 0.025f, false);
	BoxAnim_->CreateAnimation("Box_End.bmp", "Box_End", 0, 132, 0.025f, false);
	BoxAnim_->ChangeAnimation("Box_Bounce");

	// Treasure
	Treasure_ = CreateRenderer("Box_" + SkillTypeToName(SelectedTreasure_) + ".bmp", static_cast<int>(RENDER_ORDER::UI2));

	Treasure_->CameraEffectOff();
	Treasure_->Off();
	Treasure_->SetPivot(GameEngineWindow::GetScale().Half() + float4{ 0, 10 });

	TreasureOnCounter_.SetCount(6.10f);
}

void LevelUpUI::TreasureUpdate()
{
	if (true == GameEngineInput::GetInst()->IsDown("SpaceBar"))
	{
		ChangeState(STATE::TREASURE_OPENING);
	}
}

void LevelUpUI::TreasureOpeningStart()
{
	GameEngineSound::SoundPlayOneShot("TreasureOpening.mp3", 0);
	BoxAnim_->ChangeAnimation("Box_Opening");
}

void LevelUpUI::TreasureOpeningUpdate()
{
	// 도중에 무기 출력
	if (true == TreasureOnCounter_.Start(GameEngineTime::GetDeltaTime()))
	{
		Treasure_->On();

		if (true == BoxAnim_->IsEndAnimation())
		{
			ChangeState(STATE::TREASURE_END);
		}
	}
}

void LevelUpUI::TreasureEndingStart()
{
	BoxAnim_->ChangeAnimation("Box_Ending");
}

void LevelUpUI::TreasureEndingUpdate()
{
	if (true == BoxAnim_->IsEndAnimation())
	{
		BoxAnim_->ChangeAnimation("Box_End");
	}

	if (true == GameEngineInput::GetInst()->IsDown("SpaceBar"))
	{
		// 무기 획득 
		if (SkillType::NONE != EvolveSkill_)
		{
			GameInfo::PushEvolvedSkill(EvolveSkill_);
		}
		else
		{
			SelectSkill(SelectedTreasure_);
		}

		UIEnd();
	}
}


void LevelUpUI::HpMoneyStart()
{
	BoxBackGround_->On();

	HpRenderer_ = CreateRenderer("LevelUpHp.bmp", static_cast<int>(RENDER_ORDER::UI));
	HpRenderer_->CameraEffectOff();
	HpRenderer_->SetPivot(GameEngineWindow::GetScale().Half() + float4{ 2, -HpRenderer_->GetImageScale().y });
	MoneyRenderer_ = CreateRenderer("LevelUpMoney.bmp", static_cast<int>(RENDER_ORDER::UI));
	MoneyRenderer_->CameraEffectOff();
	MoneyRenderer_->SetPivot(GameEngineWindow::GetScale().Half() + float4{ 2, 2 });
}

void LevelUpUI::HpMoneyUpdate()
{
	if (true == GameEngineInput::GetInst()->IsDown("Num1"))
	{
		GameInfo::GetPlayerInfo()->CurrentHp_ += 30;
		if (GameInfo::GetPlayerInfo()->CurrentHp_ > 100)
		{
			float OverHp = GameInfo::GetPlayerInfo()->CurrentHp_ - 100;
			GameInfo::GetPlayerInfo()->CurrentHp_ = 100;
			GameInfo::GetPlayerInfo()->MaxHp_ += OverHp;
		}
		UIEnd();
	}

	if (true == GameEngineInput::GetInst()->IsDown("Num2"))
	{
		GameInfo::GetPlayerInfo()->EearnedCoin_ += 25;
		UIEnd();
	}
}


SkillType LevelUpUI::SelectTreasure()
{
	// Evolve 할 수 있는 Skill 이 있으면 그 스킬 선택
	if (SkillType::NONE != EvolveSkill_)
	{
		return EvolveSkill_;
	}
	else
	{
		// 아니라면 무작위 "스킬" + 맥스레벨은 뽑으면 안됨 + -1레벨은 뽑으면 안됨
		std::vector<SkillType> Treasures;

		// 레벨 8 미만 -1아닌 뽑을 수 있는 스킬 추가
		// 액티브 슬릇에 각성무기 있으면 추가
		std::map<SkillType, int>& AllSkillLevel = GameInfo::GetPlayerInfo()->AllSkillLevel_;

		for (int i = 0; i < ACTIVE_MAX; i++)
		{
			SkillType Type = static_cast<SkillType>(i);

			if (8 > AllSkillLevel[Type] && -1 != AllSkillLevel[Type])
			{
				Treasures.push_back(Type);
			}
		}

		// 각성 스킬 1레벨 이상인지
		for (int i = static_cast<int>(SkillType::MAX) + 1; i < EVOLVE_MAX; i++)
		{
			SkillType Type = static_cast<SkillType>(i);

			if (8 > AllSkillLevel[Type] && 1 <= AllSkillLevel[Type])
			{
				Treasures.push_back(Type);
			}
		}

		// 스킬이 모두 8레벨, 올릴게 패시브 스킬 밖에 없다면
		if (0 == Treasures.size())
		{
			ChangeState(STATE::HPMONEY);
		}

		int Index = Random.RandomInt(0, static_cast<int>(Treasures.size()) - 1);
		return Treasures[Index];
	}

}

SkillType LevelUpUI::SelectBoxSkills()
{
	std::vector<SkillType> BoxSkills;

	// 레벨 8 미만 -1아닌 뽑을 수 있는 스킬 추가
	std::map<SkillType, int>& AllSkillLevel = GameInfo::GetPlayerInfo()->AllSkillLevel_;

	for (int i = 0; i < static_cast<int>(SkillType::MAX); i++)
	{
		SkillType Type = static_cast<SkillType>(i);

		if (8 > AllSkillLevel[Type] && -1 != AllSkillLevel[Type])
		{
			BoxSkills.push_back(Type);
		}
	}

	// 스킬이 모두 8레벨, 올릴게 패시브 스킬 밖에 없다면
	if (0 == BoxSkills.size())
	{
		return SkillType::NONE;
	}

	// 랜덤 스킬 리턴
	int Index = Random.RandomInt(0, static_cast<int>(BoxSkills.size()) - 1);
	return BoxSkills[Index];
}

void LevelUpUI::UIEnd()
{
	Death();
	CreateCount_--;
	IsActivated_ = false;
	GetBox_ = false;
	SelectedTreasure_ = SkillType::NONE;
	BoxBackGround_->Off();
	StatUI_->Off();
	RandomSkills_.clear();
	return;
}
