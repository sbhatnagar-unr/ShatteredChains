// Fill out your copyright notice in the Description page of Project Settings.


#include "LightEnemy.h"
#include "ShatteredChains/Logging.h"

DEFINE_LOG_CATEGORY(Enemy);

// Sets default values
ALightEnemy::ALightEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	RootComponent = SkeletalMesh;

	UE_LOG(Enemy, Log, TEXT("New light enemy created (not yet spawned)"));

}

// Called when the game starts or when spawned
void ALightEnemy::BeginPlay()
{
	UE_LOG(Enemy, Log, TEXT("Light enemy spawned"));
	Super::BeginPlay();
	
}

// Called every frame
void ALightEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

