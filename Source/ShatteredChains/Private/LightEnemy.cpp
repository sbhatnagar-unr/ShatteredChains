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
	UE_LOG(Enemy, Log, TEXT("Trying to load mesh and animation assets...."));
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
	UE_LOG(Enemy, Log, TEXT("Mesh and animation assets loaded successfully"));

	// SUCCESS YAY!!!!
	UE_LOG(Enemy, Log, TEXT("New light enemy created (not yet spawned)"));
}


// Called when the game starts or when spawned
void ALightEnemy::BeginPlay()
{
	Super::BeginPlay();

	bool error_occured = false;

	// Get AI Controller
	UE_LOG(Enemy, Log, TEXT("Trying to get AI Controller...."));
	ai_controller = Cast<AAIController>(GetController());

	// Validate AI Controller
	if (ai_controller == nullptr) {
		UE_LOG(Enemy, Error, TEXT("Unable to get AI Controller"));
		error_occured = true;
	} else {
		UE_LOG(Enemy, Log, TEXT("AI Controller successfully retrieved"));
	}

	// Get player actor
	target_actor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	// Validate player actor
	if (target_actor == nullptr) {
		UE_LOG(Enemy, Error, TEXT("Unable to get target actor (player)"));
		error_occured = true;
	} else {
		UE_LOG(Enemy, Log, TEXT("Target actor successfully retrieved (player)"));
	}

	// Adjust log level if an error occured
	if (error_occured) {
		UE_LOG(Enemy, Warning, TEXT("Light enemy spawned"));
	} else {
		UE_LOG(Enemy, Log, TEXT("Light enemy spawned"));
	}
}


// Called every frame
void ALightEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ALightEnemy::go_to_location(const FVector &location)
{

}