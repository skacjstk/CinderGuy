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
