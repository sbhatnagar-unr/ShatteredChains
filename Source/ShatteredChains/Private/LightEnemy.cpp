// Fill out your copyright notice in the Description page of Project Settings.


#include "LightEnemy.h"
#include "ShatteredChains/Logging.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY(Enemy);

// Sets default values
ALightEnemy::ALightEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create root component
	skeletal_mesh_component = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh Component"));
	RootComponent = skeletal_mesh_component;

	// Get neccesary assets
	static USkeletalMesh *skeletal_mesh = load_object<USkeletalMesh>(skeletal_mesh_asset_path);
	static TSubclassOf<UAnimInstance> animation_class = load_class<UAnimInstance>(animation_blueprint_class_path);
	static TSubclassOf<AController> ai_controller = load_class<AController>(ai_controller_class_path);

	if (issue_with_asset)
	{
		UE_LOG(Enemy, Error, TEXT("Error loading one or more assets for LightEnemy"));
	}
	else
	{
		// Set the components
		skeletal_mesh_component->SetSkeletalMesh(skeletal_mesh);
		skeletal_mesh_component->SetAnimClass(animation_class);
		UE_LOG(Enemy, Log, TEXT("Mesh and animation assets loaded successfully"));

		// Set the AI Controller
		AIControllerClass = ai_controller;
		// Set when to have AI posses pawn
		AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
		// Adjust settings for smoother rotation
		bUseControllerRotationYaw = 0;
		GetCharacterMovement()->bUseControllerDesiredRotation = 1;

		UE_LOG(Enemy, Log, TEXT("AI Controller succesfully applied."));

		// SUCCESS YAY!!!!
		UE_LOG(Enemy, Log, TEXT("New light enemy created (not yet spawned)"));
	}
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
	UE_LOG(Enemy, Log, TEXT("Light enemy spawned"));
}


// Called every frame
void ALightEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UE_LOG(Enemy, Log, TEXT("%s"), *GetActorLocation().ToString());
}


template <typename T> T* ALightEnemy::load_object(const FString& object_asset_path)
{
	ConstructorHelpers::FObjectFinder<T> object_asset(*object_asset_path); // Is the star overloaded wtf literally why?!

	// Check that we found the asset
	if (!object_asset.Succeeded())
	{
		UE_LOG(Enemy, Error, TEXT("Unable to locate object asset for LightEnemy (%s)"), *object_asset_path);
		issue_with_asset = true;
		return nullptr;
	}

	return object_asset.Object;
}


template <typename T> TSubclassOf<T> ALightEnemy::load_class(const FString& class_asset_path)
{
	ConstructorHelpers::FClassFinder<T> class_asset(*class_asset_path); // Is the star overloaded wtf literally why?!

	// Check that we found the asset
	if (!class_asset.Succeeded())
	{
		UE_LOG(Enemy, Error, TEXT("Unable to locate class asset for LightEnemy (%s)"), *class_asset_path);
		issue_with_asset = true;
		return nullptr;
	}

	return class_asset.Class;
}