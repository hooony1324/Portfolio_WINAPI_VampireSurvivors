#include "PlayLevel.h"
#include <GameEngineBase/GameEngineWindow.h>
#include <GameEngine/GameEngine.h>
#include <GameEngineBase/GameEngineInput.h>
#include <GameEngineBase/GameEngineTime.h>
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineCollision.h>
#include <GameEngine/GameEngineImageManager.h>

#include "GameEnum.h"
#include "PlayLevelContents.h"
#include "GameInfo.h"

GameEngineImage* PlayLevel::MapColImage_ = nullptr;

PlayLevel::PlayLevel()
	: Player_(nullptr)
	, Map_(nullptr)
	, EnemyController_(nullptr)
	, ExpUI_(nullptr)
	, WeaponUI_(nullptr)
	, TimerUI_(nullptr)
	, CoinUI_(nullptr)
	, LevelUI_(nullptr)
	, KillCountUI_(nullptr)
	, PauseUI_(nullptr)
	, StatUI_(nullptr)
	, LevelSpeed_(1.0f)
{
}

PlayLevel::~PlayLevel() 
{
}

// 미리 로드해두면 좋은
void PlayLevel::Loading()
{

}

// 레벨 변경시 진입 (Title -> Play -> Result)
// 맵, 캐릭터, .. 가 선택 되면 해당하는 맵으로 액터를 생성해야 함
void PlayLevel::LevelChangeStart(GameEngineLevel* _PrevLevel)
{	

	IsDebugModeOff();

	// 시간 설정
	GameEngineTime::GetInst()->SetTimeScale(static_cast<int>(TIME_GROUP::MONSTER), 1.0f);
	GameEngineTime::GetInst()->SetTimeScale(static_cast<int>(TIME_GROUP::PLAYER), 1.0f);
	GameEngineTime::GetInst()->SetTimeScale(static_cast<int>(TIME_GROUP::WEAPON), 1.0f);

	// 맵
	CreateMap();	

	// 몬스터 생성기 
	EnemyController_ = CreateActor<EnemyController>(static_cast<int>(ACTOR_ORDER::MONSTER), "EnemyController");

	// 플레이어
	Player_ = CreateActor<Player>(static_cast<int>(ACTOR_ORDER::PLAYER), "Player");

	// UI
	ExpUI_ = CreateActor<ExpBar>(static_cast<int>(ACTOR_ORDER::UI), "UI");
	WeaponUI_ = CreateActor<WeaponSlots>(static_cast<int>(ACTOR_ORDER::UI), "UI");
	TimerUI_ = CreateActor<TimerUI>(static_cast<int>(ACTOR_ORDER::UI), "UI");
	CoinUI_ = CreateActor<CoinUI>(static_cast<int>(ACTOR_ORDER::UI), "UI");
	LevelUI_ = CreateActor<LevelUI>(static_cast<int>(ACTOR_ORDER::UI), "UI");
	KillCountUI_ = CreateActor<KillCountUI>(static_cast<int>(ACTOR_ORDER::UI), "UI");

	PauseUI_ = CreateActor<PauseUI>(static_cast<int>(ACTOR_ORDER::UI), "UI");
	StatUI_ = CreateActor<StatUI>(static_cast<int>(ACTOR_ORDER::UI), "UI");
	
	PauseUI_->Off();
	StatUI_->Off();

	BgmPlayer = GameEngineSound::SoundPlayControl("bgm_elrond_library_quiet.MP3");

	// 아이템 
	ExpGem* FirstGem = CreateActor<ExpGem>(static_cast<int>(ACTOR_ORDER::ITEM), "ITEM");
	FirstGem->SetPosition({ 1210, 490 });
	FirstGem->SetType(GemType::GREEN);


	// 테스트용 아이템
	Coin* FirstCoin = CreateActor<Coin>(static_cast<int>(ACTOR_ORDER::ITEM), "ITEM");
	FirstCoin->SetPosition({ 1000, 1000 });
	
	for (int i = 0; i < 50; i++)
	{
		LevelUpBox* Box = CreateActor<LevelUpBox>(static_cast<int>(ACTOR_ORDER::ITEM), "ITEM");
		Box->SetPosition(float4{ 1100 , 1100 } + float4{ static_cast<float>(i) * 50, 0 });
	}
	
	ChangeState(LevelState::PLAY);

	
}

void PlayLevel::LevelChangeEnd(GameEngineLevel* _NextLevel)
{
	// BGM 종료
	BgmPlayer.Stop();
}


void PlayLevel::Update()
{
	// 디버깅
	{
		// C
		if (true == GameEngineInput::GetInst()->IsDown("ColDebugger"))
		{
			IsDebugModeSwitch();
		}

		// L
		if (true == GameEngineInput::GetInst()->IsDown("LevelUP"))
		{
			std::map<SkillType, int>& AllSkillLevel = GameInfo::GetPlayerInfo()->AllSkillLevel_;

			// 가지고 있는 스킬만 레벨 8
			for (int i = 0; i < static_cast<int>(GameInfo::GetPlayerInfo()->ActiveSkillSlot_.size()); i++)
			{
				SkillType Type = GameInfo::GetPlayerInfo()->ActiveSkillSlot_[i];
				AllSkillLevel[Type] = 8;
			}

			for (int i = 0; i < static_cast<int>(GameInfo::GetPlayerInfo()->PassiveSkillSlot_.size()); i++)
			{
				SkillType Type = GameInfo::GetPlayerInfo()->PassiveSkillSlot_[i];
				AllSkillLevel[Type] = 8;
			}
		}

		// F
		if (true == GameEngineInput::GetInst()->IsDown("FastMode"))
		{
			SpeedUp(4.0f);
		}

	}

	UpdateState();

}


void PlayLevel::CreateMap()
{
	Map_ = CreateActor<Library>(static_cast<int>(RENDER_ORDER::MONSTER), "Library");
	// 액터의 위치, 크기 설정
	Map_->SetPosition(float4::ZERO);
	Map_->SetScale(Map_->GetRendererScale());

	MapColImage_ = GameEngineImageManager::GetInst()->Find("LibraryColMap.bmp");

}

void PlayLevel::InfiniteMap()
{
	Map_->CheckPlayerOnEnd();
}


void PlayLevel::UpdateState()
{
	switch (LevelState_)
	{
	case LevelState::PLAY:
		PlayUpdate();
		break;
	case LevelState::LEVELUP:
		LevelUpUpdate();
		break;
	case LevelState::PAUSE:
		PauseUpdate();
		break;
	case LevelState::GAMEOVER:
		GameOverUpdate();
		break;
	default:
		break;
	}
}

void PlayLevel::ChangeState(LevelState _State)
{
	switch (_State)
	{
	case LevelState::PLAY:
		PlayStart();
		break;
	case LevelState::LEVELUP:
		LevelUpStart();
		break;
	case LevelState::PAUSE:
		PauseStart();
		break;
	case LevelState::GAMEOVER:
		GameOverStart();
		break;
	default:
		break;
	}

	LevelState_ = _State;
}

void PlayLevel::PlayStart()
{
	FreezeOut();
}

void PlayLevel::LevelUpStart()
{
	Freeze();

	CreateActor<LevelUpUI>(static_cast<int>(ACTOR_ORDER::UI), "UI");
}

void PlayLevel::PauseStart()
{
	PauseUI::Activated_ = true;

	Freeze();
}

void PlayLevel::GameOverStart()
{
	Freeze();
	GameEngineSound::SoundPlayOneShot("GameOver.mp3", 0);
	GameOverUI_ = CreateActor<GameOverUI>(static_cast<int>(ACTOR_ORDER::BACKGROUND));
}

void PlayLevel::PlayUpdate()
{
	if (Player_->IsHpZero())
	{
		ChangeState(LevelState::GAMEOVER);
	}

	if (LevelUpUI::CreateCount_ > 0 && false == LevelUpUI::IsActivated())
	{
		ChangeState(LevelState::LEVELUP);
	}

	if (true == GameEngineInput::GetInst()->IsDown("Esc"))
	{
		ChangeState(LevelState::PAUSE);
		return;
	}


	InfiniteMap();

}

void PlayLevel::LevelUpUpdate()
{
	if (false == LevelUpUI::IsActivated() && LevelUpUI::CreateCount_ <= 0)
	{
		ChangeState(LevelState::PLAY);
		return;
	}

	int a = 0;
}

void PlayLevel::PauseUpdate()
{
	if (false == PauseUI::Activated_)
	{
		PauseUI_->Off();
		StatUI_->Off();
		ChangeState(LevelState::PLAY);
		return;
	}


	if (true == GameEngineInput::GetInst()->IsDown("Esc"))
	{
		PauseUI::Activated_ = false;
	}

	PauseUI_->On();
	StatUI_->On();
}

void PlayLevel::GameOverUpdate()
{
	if (true == GameEngineInput::GetInst()->IsDown("SpaceBar"))
	{
		// 게임 종료
		GameEngineSound::SoundPlayOneShot("ButtonQuit.mp3", 0);
		GameEngine::GetInst().ChangeLevel("Result");
	}
}


void PlayLevel::Freeze()
{
	GameEngineTime::GetInst()->SetTimeScale(static_cast<int>(TIME_GROUP::MONSTER), 0.0f);
	GameEngineTime::GetInst()->SetTimeScale(static_cast<int>(TIME_GROUP::PLAYER), 0.0f);
	GameEngineTime::GetInst()->SetTimeScale(static_cast<int>(TIME_GROUP::WEAPON), 0.0f);
	GameEngineTime::GetInst()->SetTimeScale(static_cast<int>(TIME_GROUP::TIMER), 0.0f);
	GameEngineTime::GetInst()->SetTimeScale(static_cast<int>(TIME_GROUP::ITEM), 0.0f);
	BgmPlayer.PlaySpeed(0.0f);
}

void PlayLevel::FreezeOut()
{
	GameEngineTime::GetInst()->SetTimeScale(static_cast<int>(TIME_GROUP::MONSTER), LevelSpeed_);
	GameEngineTime::GetInst()->SetTimeScale(static_cast<int>(TIME_GROUP::PLAYER), LevelSpeed_);
	GameEngineTime::GetInst()->SetTimeScale(static_cast<int>(TIME_GROUP::WEAPON), LevelSpeed_);
	GameEngineTime::GetInst()->SetTimeScale(static_cast<int>(TIME_GROUP::TIMER), LevelSpeed_);
	GameEngineTime::GetInst()->SetTimeScale(static_cast<int>(TIME_GROUP::ITEM), LevelSpeed_);
	BgmPlayer.PlaySpeed(LevelSpeed_);
}

void PlayLevel::SpeedUp(float _Speed)
{
	if (LevelSpeed_ >= _Speed)
	{
		LevelSpeed_ = 1.0f;
	}
	else
	{
		LevelSpeed_ = _Speed;
	}

	GameEngineTime::GetInst()->SetTimeScale(static_cast<int>(TIME_GROUP::MONSTER), LevelSpeed_);
	GameEngineTime::GetInst()->SetTimeScale(static_cast<int>(TIME_GROUP::PLAYER), LevelSpeed_);
	GameEngineTime::GetInst()->SetTimeScale(static_cast<int>(TIME_GROUP::WEAPON), LevelSpeed_);
	GameEngineTime::GetInst()->SetTimeScale(static_cast<int>(TIME_GROUP::TIMER), LevelSpeed_);
	GameEngineTime::GetInst()->SetTimeScale(static_cast<int>(TIME_GROUP::ITEM), LevelSpeed_);
	BgmPlayer.PlaySpeed(LevelSpeed_);
}

