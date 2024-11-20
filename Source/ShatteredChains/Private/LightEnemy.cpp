// Fill out your copyright notice in the Description page of Project Settings.


#include "LightEnemy.h"
#include "ShatteredChains/Logging.h"
#include "Kismet/GameplayStatics.h"


DEFINE_LOG_CATEGORY(Enemy);

// Sets default values
ALightEnemy::ALightEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Default attack range
	attack_range = 100;
}


AActor* ALightEnemy::get_target()
{
	return target;
}


float ALightEnemy::get_attack_range()
{
	return attack_range;
}


// Called to bind functionality to input
void ALightEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


// Called when the game starts or when spawned
void ALightEnemy::BeginPlay()
{
	Super::BeginPlay();

	// Get the target
	target = (TObjectPtr<AActor>) UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (target == nullptr)
	{
		UE_LOG(Enemy, Error, TEXT("Light enemy could not locate target (player)"));
	}

}


// Called every frame
void ALightEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ALightEnemy::dead()
{
	UE_LOG(Enemy, Log, TEXT("I DIED!!!!"));
}