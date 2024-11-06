// Fill out your copyright notice in the Description page of Project Settings.


#include "LightEnemy.h"
#include "ShatteredChains/Logging.h"

#include <stdexcept>

DEFINE_LOG_CATEGORY(Enemy);

// Sets default values
ALightEnemy::ALightEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Create root component
	skeletal_mesh_component = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh Component"));
	RootComponent = skeletal_mesh_component;

	// Locate neccesary assets
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletal_mesh(*skeletal_mesh_asset_path); // Is the star overloaded wtf literally why?!
	static ConstructorHelpers::FClassFinder<UAnimInstance> animation_class(*animation_blueprint_class_path); // Is the star overloaded wtf literally why?!

	// Check that we found the skeletal mesh
	if (!skeletal_mesh.Succeeded()) {
		UE_LOG(Enemy, Error, TEXT("Unable to locate skeletal mesh for LightEnemy (%s)"), *skeletal_mesh_asset_path);
		return;
	}

	// Check that we found the animation class
	if (!animation_class.Succeeded()) {
		UE_LOG(Enemy, Error, TEXT("Unable to locate animation class for LightEnemy (%s)"), *animation_blueprint_class_path);
		return;
	}

	// Set the components
	skeletal_mesh_component->SetSkeletalMesh(skeletal_mesh.Object);
	skeletal_mesh_component->SetAnimClass(animation_class.Class);

	UE_LOG(Enemy, Log, TEXT("New light enemy created (not yet spawned)"));
}

// Called when the game starts or when spawned
void ALightEnemy::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(Enemy, Log, TEXT("Light enemy spawned"));
	
}

// Called every frame
void ALightEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

