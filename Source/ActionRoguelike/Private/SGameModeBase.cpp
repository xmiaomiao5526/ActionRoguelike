// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "AI/SAICharacter.h"
#include "SAttributeComponent.h"
#include "EngineUtils.h"
#include "SCharacter.h"
#include "SPlayerState.h"


ASGameModeBase::ASGameModeBase()
{
	SpawnTimeInterval = 2.f;
	//�ȼ�����World Setting���޸�PlayerStateClass
	PlayerStateClass = ASPlayerState::StaticClass();
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this,
		&ASGameModeBase::SpawnBotTimerElapsed, SpawnTimeInterval, true);
	//UPROPERTY(EditDefaultsOnly, Category = "Powerup")		TArray<TSubclassOf<AActor>> PowerupClasses;
	if (ensure(PowerupClasses.Num()>0))
	{
		//��ָ��QuerierʱGameModeʱ�����EQS��ͼ���߼���������Zero VectorΪ���ĵ�λ����������
		//UEnvQuery* PoweupSpawnQuery...EditDefaultsOnly, Category = "Powerup"
		UEnvQueryInstanceBlueprintWrapper* QueryInstance = 
			UEnvQueryManager::RunEQSQuery(this, PoweupSpawnQuery, this, EEnvQueryRunMode::AllMatching, nullptr);

		if (ensure(QueryInstance))
		{
			QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnPowerupSpawnQueryCompleted);
		}
	}
}


static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable spawning of bots via timer"), ECVF_Cheat);
void ASGameModeBase::SpawnBotTimerElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot spawning disabled via cvar 'CVarSpawnBots'"));
		return;
	}

	//��ǰ��ͼ�л��ŵ�AI������
	int32 NrOfAliveBots = 0;
	//TActorIterator<>���Կ�����һ�����ð汾��GetAllActorOfClass
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComp = AttributeComp->GetAttributes(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			++NrOfAliveBots;
		}
	}

	//���
	float MaxBotCount = 10.f;
	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if (NrOfAliveBots >= MaxBotCount)
	{
		return;
	}
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	//ensure����ͼ�а���EQS
	if (ensure(QueryInstance))
	{
		//��ѯ�����󣬽���OnQueryCompleted�������������Դ�Դ���еĴ����鿴
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);
	}
}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS query Failed!!"));
		return;
	}
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	//��ͬ��Locations.Num()>0
	if (Locations.IsValidIndex(0))
	{
		if (MinionClass)
		{
			GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
		}
	}
}


void ASGameModeBase::KillAll()
{
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComp = AttributeComp->GetAttributes(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			AttributeComp->Kill(this);// @fixme: pass in player? for kill credit
		}
	}
}

void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		//��SCaractor�У����ǶԽ�ɫ����������������Ƶ��߼�����û��UnPossess
		//RestartPlayerԴ���У���Controller����Player����ʲôҲ�����������������UnPossess
		Controller->UnPossess();
		RestartPlayer(Controller);
	}
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{

	//����ɱ����AI(������ASAICharacter)����Cast��ʧ�ܣ�����nullptr
	//Castֻ�ܽ�ָ��ת�����������������
	ASCharacter* Player = Cast<ASCharacter>(VictimActor);
	if (Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor die and try respawn!"));
		FTimerHandle TimerHandle_RespawnDelay;
		
		//Ϊ��ʱ�����ô������ĺ���
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());
		float RespawnDelay = 2.f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate,RespawnDelay,false);
	}
	APawn* KillerPawn = Cast<APawn>(Killer);
	if (KillerPawn)
	{
		//Ĭ��AI��PlayerState
		ASPlayerState* PS = KillerPawn->GetPlayerState<ASPlayerState>();
		if (PS)
		{
			//CreditsPerKill : UPROPERTY(EditDefaultsOnly, Category = "Credits")
			PS->AddCredits(CreditsPerKill); 
		}
	}
}

void ASGameModeBase::OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn Powerup EQS Query Failed"));
		return;
	}
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	TArray<FVector> UsedLocations;

	int32 SpawnCounter = 0;

	while (SpawnCounter < DesirePowerCount && Locations.Num() > 0)
	{
		int32 RandomLocationIndex = FMath::RandRange(0, Locations.Num() - 1);

		FVector  PickedLocation = Locations[RandomLocationIndex];
		//Remove to avoid picking again
		Locations.RemoveAt(RandomLocationIndex);

		bool bValidLocation = true;
		//��֤Powerup֮��ľ���
		for (FVector OtherLocation: UsedLocations)
		{
			float DistanceTo = (PickedLocation - OtherLocation).Size();

			if (DistanceTo < RequirePowerupDistance)
			{
				bValidLocation = false;
				break;
			}
		}

		if (!bValidLocation)
		{
			continue;
		}
		//��ѡ�е�λ���������ָ����Powerup����֮һ
		int32 RandomClassIndex = FMath::RandRange(0,PowerupClasses.Num() -1);
		TSubclassOf<AActor> RandomPowerupClass = PowerupClasses[RandomClassIndex];

		GetWorld()->SpawnActor<AActor>(RandomPowerupClass, PickedLocation, FRotator::ZeroRotator);

		UsedLocations.Add(PickedLocation);
		++SpawnCounter;
	}
}
