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

	// Create root component
	skeletal_mesh_component = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh Component"));
	RootComponent = skeletal_mesh_component;

	// Locate neccesary assets
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletal_mesh(*skeletal_mesh_asset_path); // Is the star overloaded wtf literally why?!
	static ConstructorHelpers::FClassFinder<UAnimInstance> animation_class(*animation_blueprint_class_path); // Is the star overloaded wtf literally why?!

	// Check that we found the skeletal mesh
	if (!skeletal_mesh.Succeeded())
	{
		UE_LOG(Enemy, Error, TEXT("Unable to locate skeletal mesh for LightEnemy (%s)"), *skeletal_mesh_asset_path);
		return;
	}

	// Check that we found the animation class
	if (!animation_class.Succeeded())
	{
		UE_LOG(Enemy, Error, TEXT("Unable to locate animation class for LightEnemy (%s)"), *animation_blueprint_class_path);
		return;
	}

	// Set the components
	skeletal_mesh_component->SetSkeletalMesh(skeletal_mesh.Object);
	skeletal_mesh_component->SetAnimClass(animation_class.Class);
	UE_LOG(Enemy, Log, TEXT("Mesh and animation assets loaded successfully"));

	// Initialize variables that will be defined in BeginPlay
	ai_controller = nullptr;
	target_actor = nullptr;
	has_target_actor = false;
	has_ai_controller = false;

	// SUCCESS YAY!!!!
	UE_LOG(Enemy, Log, TEXT("New light enemy created (not yet spawned)"));
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

	// Get AI Controller
	ai_controller = Cast<AAIController>(GetController());

	// Validate AI Controller
	if (ai_controller == nullptr)
	{
		has_ai_controller = false;
		UE_LOG(Enemy, Error, TEXT("Unable to get AI Controller"));
	}
	else
	{
		has_ai_controller = true;
		UE_LOG(Enemy, Log, TEXT("AI Controller successfully retrieved"));
	}

	// Get player actor
	target_actor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	// Validate player actor
	if (target_actor == nullptr)
	{
		has_target_actor = false;
		UE_LOG(Enemy, Error, TEXT("Unable to get target actor (player)"));
	}
	else
	{
		has_target_actor = true;
		UE_LOG(Enemy, Log, TEXT("Target actor successfully retrieved (player)"));
	}

	// Adjust log level if an error occured
	if (!has_ai_controller || !has_target_actor)
	{
		UE_LOG(Enemy, Warning, TEXT("Light enemy spawned"));
	}
	else
	{
		UE_LOG(Enemy, Log, TEXT("Light enemy spawned"));
	}
}


// Called every frame
void ALightEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bool succesfull_chase = chase_player();
}


bool ALightEnemy::chase_player()
{
	/*
	Return true if able to chase and false otherwise
	*/

	// If no target or ai controller return false because then we cant chase player
	if (!has_target_actor || !has_ai_controller) return false;

	// Log info for debugging
	FVector enemy_location = this->GetActorLocation();
	FVector target_location = target_actor->GetActorLocation();
	UE_LOG(Enemy, Log, TEXT("Enemy (%s) -> Target (%s)"), *(enemy_location.ToString()), *(target_location.ToString()));

	// Go to target location
	ai_controller->MoveToActor(target_actor);

	// Successful chase
	return true;
}