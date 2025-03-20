// Fill out your copyright notice in the Description page of Project Settings.


#include "MedKit.h"
#include "Player/MyCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ShatteredChains/Logging.h"

// Sets default values
AMedKit::AMedKit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    // Create and initialize collision component
    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    SphereComponent->InitSphereRadius(50.0f);
    SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
    SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AMedKit::OnOverlapBegin);
    RootComponent = SphereComponent;
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
        UE_LOG(LogTemp, Log, TEXT("Medkit picked up by player"));
        Use(Character);
    }
}