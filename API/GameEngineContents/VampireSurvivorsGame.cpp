#include "VampireSurvivorsGame.h"
#include <GameEngineBase/GameEngineWindow.h>
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEngineBase/GameEngineFile.h>
#include <GameEngineBase/GameEngineInput.h>
#include <GameEngine/GameEngineImageManager.h>
#include <GameEngine/GameEngineFolderImage.h>
#include <GameEngineBase/GameEngineSound.h>
#include <Windows.h>

#include "GameInfo.h"
#include "IntroLevel.h"
#include "PlayLevel.h"
#include "ResultLevel.h"
#include "TitleLevel.h"


VampireSurvivorsGame::VampireSurvivorsGame() 
{
}

VampireSurvivorsGame::~VampireSurvivorsGame() 
{
}

void VampireSurvivorsGame::GameInit()
{
	GameEngineWindow::GetInst().SetWindowScaleAndPosition({ 100, 100 }, { 1280, 800 });

	// 리소스 로드
	ResourceLoad();

	// Level을 맵 형태로 관리
	CreateLevel<IntroLevel>("Intro");
	CreateLevel<TitleLevel>("Title");
	CreateLevel<PlayLevel>("Play");
	CreateLevel<ResultLevel>("Result");
	ChangeLevel("Intro");


	// 키 세팅
	if (false == GameEngineInput::GetInst()->IsKey("MoveLeft"))
	{
		GameEngineInput::GetInst()->CreateKey("MoveLeft", 'A');
		GameEngineInput::GetInst()->CreateKey("MoveRight", 'D');
		GameEngineInput::GetInst()->CreateKey("MoveUp", 'W');
		GameEngineInput::GetInst()->CreateKey("MoveDown", 'S');
		GameEngineInput::GetInst()->CreateKey("SpaceBar", VK_SPACE);
		GameEngineInput::GetInst()->CreateKey("Esc", VK_ESCAPE);
		
		// Volume
		/*GameEngineInput::GetInst()->CreateKey("VolumeUp", VK_UP);
		GameEngineInput::GetInst()->CreateKey("VolumeDown", VK_DOWN);*/

		GameEngineInput::GetInst()->CreateKey("LevelUp", 'L');
		GameEngineInput::GetInst()->CreateKey("Num1", '1');
		GameEngineInput::GetInst()->CreateKey("Num2", '2');
		GameEngineInput::GetInst()->CreateKey("Num3", '3');
		GameEngineInput::GetInst()->CreateKey("Num4", '4');
		
		
		GameEngineInput::GetInst()->CreateKey("ColDebugger", 'C');
		GameEngineInput::GetInst()->CreateKey("GodMode", 'G');
		GameEngineInput::GetInst()->CreateKey("SpawnReaper", 'R');

		GameEngineInput::GetInst()->CreateKey("Map", 'M');
		GameEngineInput::GetInst()->CreateKey("FastMode", 'F');

	}


}

void VampireSurvivorsGame::GameLoop()
{

}

void VampireSurvivorsGame::GameEnd()
{
	GameInfo::Destroy();
}

void VampireSurvivorsGame::ResourceLoad()
{


	// 이미지 리소스
	GameEngineDirectory ResourcesDir;
	ResourcesDir.MoveParent("API");
	ResourcesDir.Move("Resources");

	std::string Directories[] = {"characters", "background", "vfx", "TitleUI", "PlayUI", "EndingUI", "enemies", "items"};
	
	for (std::string& Dir : Directories)
	{
		ResourcesDir.Move(Dir);
		std::vector<GameEngineFile> AllImageFileList = ResourcesDir.GetAllFile("Bmp");

		for (size_t i = 0; i < AllImageFileList.size(); i++)
		{
			GameEngineImageManager::GetInst()->Load(AllImageFileList[i].GetFullPath());
		}

		ResourcesDir.MoveParent("Resources");
	}

	// 애니메이션용 이미지는 Cut선행되어야 함
	GameEngineImageManager::GetInst()->Find("CautionToMain.bmp")->Cut({ 1280, 800 });

	GameEngineImageManager::GetInst()->Find("Arrow_L.bmp")->Cut({ 40, 40 });
	GameEngineImageManager::GetInst()->Find("Arrow_R.bmp")->Cut({ 40, 40 });

	GameEngineImageManager::GetInst()->Find("Cavallo_WalkRight.bmp")->Cut({ 56, 56 });
	GameEngineImageManager::GetInst()->Find("Cavallo_WalkLeft.bmp")->Cut({ 56, 56 });

	GameEngineImageManager::GetInst()->Find("Imelda_WalkRight.bmp")->Cut({ 70, 70 });
	GameEngineImageManager::GetInst()->Find("Imelda_WalkLeft.bmp")->Cut({ 70, 70 });

	GameEngineImageManager::GetInst()->Find("Poe_WalkRight.bmp")->Cut({ 70, 70 });
	GameEngineImageManager::GetInst()->Find("Poe_WalkLeft.bmp")->Cut({ 70, 70 });

	GameEngineImageManager::GetInst()->Find("Mud_WalkLeft.bmp")->Cut({ 120, 120 });
	GameEngineImageManager::GetInst()->Find("Mud_WalkRight.bmp")->Cut({ 120, 120 });
	GameEngineImageManager::GetInst()->Find("Mud_Dead.bmp")->Cut({ 120, 120 });



	GameEngineImageManager::GetInst()->Find("Medusa_WalkLeft.bmp")->Cut({ 140, 140 });
	GameEngineImageManager::GetInst()->Find("Medusa_WalkRight.bmp")->Cut({ 140, 140 });
	GameEngineImageManager::GetInst()->Find("Medusa_Dead.bmp")->Cut({ 140, 140 });

	GameEngineImageManager::GetInst()->Find("Mummy_WalkLeft.bmp")->Cut({ 140, 140 });
	GameEngineImageManager::GetInst()->Find("Mummy_WalkRight.bmp")->Cut({ 140, 140 });
	GameEngineImageManager::GetInst()->Find("Mummy_Dead.bmp")->Cut({ 140, 140 });



	GameEngineImageManager::GetInst()->Find("Ecto_WalkLeft.bmp")->Cut({ 140, 140 });
	GameEngineImageManager::GetInst()->Find("Ecto_WalkRight.bmp")->Cut({ 140, 140 });
	GameEngineImageManager::GetInst()->Find("Ecto_Dead.bmp")->Cut({ 140, 140 });

	// Boss 100 x 100
	GameEngineImageManager::GetInst()->Find("ShadeRed_WalkLeft.bmp")->Cut({ 100, 100 });
	GameEngineImageManager::GetInst()->Find("ShadeRed_WalkRight.bmp")->Cut({ 100, 100 });
	GameEngineImageManager::GetInst()->Find("ShadeRed_Dead.bmp")->Cut({ 100, 100 });
	
	GameEngineImageManager::GetInst()->Find("ShadeRed_SmokeDead.bmp")->Cut({ 50, 50 });

	GameEngineImageManager::GetInst()->Find("XLMummy_WalkLeft.bmp")->Cut({ 100, 100 });
	GameEngineImageManager::GetInst()->Find("XLMummy_WalkRight.bmp")->Cut({ 100, 100 });
	GameEngineImageManager::GetInst()->Find("XLMummy_Dead.bmp")->Cut({ 100, 100 });

	GameEngineImageManager::GetInst()->Find("XLReaper_WalkLeft.bmp")->Cut({ 200, 200 });
	GameEngineImageManager::GetInst()->Find("XLReaper_WalkRight.bmp")->Cut({ 200, 200 });
	GameEngineImageManager::GetInst()->Find("XLReaper_Dead.bmp")->Cut({ 200, 200 });


	// 보물상자
	GameEngineImageManager::GetInst()->Find("Box_Bounce.bmp")->Cut({ 456, 620 });
	GameEngineImageManager::GetInst()->Find("Box_Opening.bmp")->Cut({ 456, 620 });
	GameEngineImageManager::GetInst()->Find("Box_Ending.bmp")->Cut({ 456, 620 });
	GameEngineImageManager::GetInst()->Find("Box_End.bmp")->Cut({ 456, 620 });


	// 사운드 리소스
	{
		GameEngineDirectory ResourcesDir;
		ResourcesDir.MoveParent("API");
		ResourcesDir.Move("Resources");
		ResourcesDir.Move("sfx");
		
		// 폴더안에 모든 이미지 파일을 찾는다.
		std::vector<GameEngineFile> AllImageFileList = ResourcesDir.GetAllFile();

		for (size_t i = 0; i < AllImageFileList.size(); i++)
		{
			GameEngineSound::LoadRes(AllImageFileList[i].GetFullPath());
		}

	}
}
