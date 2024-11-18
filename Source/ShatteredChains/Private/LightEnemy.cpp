// Fill out your copyright notice in the Description page of Project Settings.


#include "LightEnemy.h"
#include "ShatteredChains/Logging.h"


DEFINE_LOG_CATEGORY(Enemy);

// Sets default values
ALightEnemy::ALightEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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
}


// Called every frame
void ALightEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}