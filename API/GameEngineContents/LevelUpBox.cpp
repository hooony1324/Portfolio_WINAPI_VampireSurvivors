#include "LevelUpBox.h"
#include <GameEngine/GameEngine.h>
#include <GameEngineBase/GameEngineWindow.h>
#include <GameEngine/GameEngineImageManager.h>
#include <GameEngine/GameEngineRenderer.h>
#include <GameEngine/GameEngineCollision.h>

#include "GameEnum.h"
#include "GameInfo.h"
#include "LevelUpUI.h"

LevelUpBox::LevelUpBox() 
	: Col_(nullptr)
{
}

LevelUpBox::~LevelUpBox() 
{
}

void LevelUpBox::Start()
{
	GameEngineRenderer* Renderer_ = CreateRenderer("BoxOpen.bmp");
	SetScale(Renderer_->GetScale());

	Col_ = CreateCollision("LevelUpBox", Renderer_->GetScale());

	NextLevelOff();
}

void LevelUpBox::Update()
{
	if (false == Col_->CollisionCheck("Player"))
	{
		return;
	}


	// 일단은 LevelUPUI, 추후에 보물상자 에니메이션으로
	LevelUpUI::GetBox_ = true;
	LevelUpUI::CreateCount_ += 1;

	Death();

}

void LevelUpBox::Render()
{
}