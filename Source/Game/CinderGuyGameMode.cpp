#include "CinderGuyGameMode.h"
#include "Global.h"
#include "Characters/CPlayer.h"

ACinderGuyGameMode::ACinderGuyGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("Class'/Script/Game.CPlayer'"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
}

void ACinderGuyGameMode::PostLogin(APlayerController* NewController)
{
	Super::PostLogin(NewController);
//	ACPlayer* Player = Cast<ACPlayer>(GetWorld()->SpawnActor(DefaultPawnClass));	// 다시 소환
//	Player->SpawnDefaultController();
//	if (Player != nullptr)
//	{
//		NewController->Possess(Player);
//	}
}

void ACinderGuyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	// 여기말고, PlayerState에서 세션 만들면서 로그아웃 시 파괴 델리게이트 호출하기
}

void ACinderGuyGameMode::Travel(FString LevelName)
{
	UWorld* world = GetWorld();
	if (!!world)
	{
		FString LevelFullName = "/Game/Maps/" + LevelName + "?Listen";
		world->ServerTravel(LevelFullName);//"/Game/Maps/Dungeon1?Listen"
	}
}