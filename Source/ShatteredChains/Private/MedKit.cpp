// Fill out your copyright notice in the Description page of Project Settings.


#include "MedKit.h"
#include "Player/MyCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ShatteredChains/Logging.h"

// Sets default values
AMedKit::AMedKit()
{
    // Create mesh and set it as root
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MedkitMesh"));
    RootComponent = MeshComponent;

    // Enable physics
    MeshComponent->SetSimulatePhysics(true);
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    MeshComponent->SetCollisionObjectType(ECC_PhysicsBody);
    MeshComponent->SetCollisionResponseToAllChannels(ECR_Block);

    // Create sphere component and attach to mesh
    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    SphereComponent->SetupAttachment(MeshComponent); // Attach to mesh now
    SphereComponent->InitSphereRadius(50.0f);
    SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
    SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AMedKit::OnOverlapBegin);

}

// Called when the game starts or when spawned
void AMedKit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Use Medkit
void AMedKit::Use(AMyCharacter* Character)
{
    if (Character)
    {
        UHealthComponent* HealthComp = Character->get_health_component();
        if (HealthComp)
        {
            HealthComp->set_health(HealthComp->get_max_health()); // Fully heal player
            UE_LOG(LogTemp, Log, TEXT("Medkit used: Restored full health"));
        }

        // Restore normal movement speed
        UCharacterMovementComponent* MoveComp = Character->GetCharacterMovement();
        if (MoveComp)
        {
            MoveComp->MaxWalkSpeed = NormalWalkSpeed;
            UE_LOG(LogTemp, Log, TEXT("Medkit used: Restored normal movement speed"));
        }

        // Reset movement penalties from leg/foot injuries
        Character->ResetMovementDebuffs();


        UE_LOG(LogTemp, Log, TEXT("Medkit used: Reset all movement debuffs"));

        // Destroy medkit after use
        Destroy();
    }
}

// Medkit Pickup on overlap logic
void AMedKit::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AMyCharacter* Character = Cast<AMyCharacter>(OtherActor);
    if (Character)
    {
        if (UInventoryComponent* Inv = Character->get_inventory_component())
        {
            if (Inv->AddItem("MedKit", EItemType::HealthKit, 1))
            {
                UE_LOG(LogTemp, Log, TEXT("Picked up MedKit and added to inventory"));
                Destroy(); // Remove from world
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Could not add MedKit to inventory"));
            }
        }
    }
}

