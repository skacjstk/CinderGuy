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
//	ACPlayer* Player = Cast<ACPlayer>(GetWorld()->SpawnActor(DefaultPawnClass));	// �ٽ� ��ȯ
//	Player->SpawnDefaultController();
//	if (Player != nullptr)
//	{
//		NewController->Possess(Player);
//	}
}

void ACinderGuyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	// ���⸻��, PlayerState���� ���� ����鼭 �α׾ƿ� �� �ı� ��������Ʈ ȣ���ϱ�
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