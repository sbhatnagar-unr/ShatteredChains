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

	attack_animation_montage = nullptr;
	attack_animation_exit_blend = 0.2f;
	attack_damage = 5.f;
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
		UE_LOG(Enemy, Error, LOG_TEXT("Culd not locate target (player)"));
	}


	// Make sure we have an animation montage
	if (attack_animation_montage == nullptr)
	{
		UE_LOG(Enemy, Error, LOG_TEXT("No attack animation montage"));
	}
}


// Called every frame
void ALightEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


AActor* ALightEnemy::get_target()
{
	return target;
}


float ALightEnemy::get_attack_range()
{
	return attack_range;
}


float ALightEnemy::get_attack_damage()
{
	return attack_damage;
}


UAnimMontage* ALightEnemy::get_attack_animation_montage()
{
	return attack_animation_montage;
}

float ALightEnemy::get_attack_animation_exit_blend()
{
	return attack_animation_exit_blend;
}


void ALightEnemy::dead(AActor* killed_by)
{
	UE_LOG(Enemy, Log, LOG_TEXT("I DIED!!!!"));
}