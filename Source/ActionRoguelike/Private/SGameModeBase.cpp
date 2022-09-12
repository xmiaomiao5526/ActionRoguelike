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
	//等价于在World Setting中修改PlayerStateClass
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
		//当指定Querier时GameMode时，结合EQS蓝图的逻辑，会在以Zero Vector为中心的位置生成网格
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

	//当前地图中活着的AI的数量
	int32 NrOfAliveBots = 0;
	//TActorIterator<>可以看做是一个更好版本的GetAllActorOfClass
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComp = AttributeComp->GetAttributes(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			++NrOfAliveBots;
		}
	}

	//最大
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
	//ensure在蓝图中绑定了EQS
	if (ensure(QueryInstance))
	{
		//查询结束后，进入OnQueryCompleted函数，参数可以从源码中的代理宏查看
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

	//等同于Locations.Num()>0
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
		//在SCaractor中，我们对角色死亡仅作了脱离控制的逻辑，并没有UnPossess
		//RestartPlayer源码中，若Controller下有Player，会什么也不做，因此在这里先UnPossess
		Controller->UnPossess();
		RestartPlayer(Controller);
	}
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{

	//若被杀的是AI(类名：ASAICharacter)，则Cast会失败，返回nullptr
	//Cast只能将指针转换到其自身或父类类型
	ASCharacter* Player = Cast<ASCharacter>(VictimActor);
	if (Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor die and try respawn!"));
		FTimerHandle TimerHandle_RespawnDelay;
		
		//为计时器设置带参数的函数
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());
		float RespawnDelay = 2.f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate,RespawnDelay,false);
	}
	APawn* KillerPawn = Cast<APawn>(Killer);
	if (KillerPawn)
	{
		//默认AI无PlayerState
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
		//保证Powerup之间的距离
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
		//在选中的位置随机生成指定的Powerup其中之一
		int32 RandomClassIndex = FMath::RandRange(0,PowerupClasses.Num() -1);
		TSubclassOf<AActor> RandomPowerupClass = PowerupClasses[RandomClassIndex];

		GetWorld()->SpawnActor<AActor>(RandomPowerupClass, PickedLocation, FRotator::ZeroRotator);

		UsedLocations.Add(PickedLocation);
		++SpawnCounter;
	}
}
