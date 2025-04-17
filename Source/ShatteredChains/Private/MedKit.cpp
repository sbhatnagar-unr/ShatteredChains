#include "MedKit.h"
#include "Player/MyCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/HealthComponent/HealthComponent.h"



AMedKit::AMedKit()
{
    PrimaryActorTick.bCanEverTick = false;

    // Mesh as root
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MedkitMesh"));
    RootComponent = MeshComponent;
    MeshComponent->SetSimulatePhysics(true);
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    MeshComponent->SetCollisionResponseToAllChannels(ECR_Block);

    // Overlap sphere
    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    SphereComponent->SetupAttachment(MeshComponent);
    SphereComponent->InitSphereRadius(50.0f);
    SphereComponent->SetGenerateOverlapEvents(true);
    SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
    SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AMedKit::HandleBeginOverlap);
    SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AMedKit::HandleEndOverlap);
}

void AMedKit::BeginPlay()
{
    Super::BeginPlay();
}

void AMedKit::Use(AMyCharacter* Character)
{
    if (!Character) return;

    if (UHealthComponent* HealthComp = Character->get_health_component())
    {
        HealthComp->set_health(HealthComp->get_max_health());
    }

    if (UCharacterMovementComponent* MoveComp = Character->GetCharacterMovement())
    {
        MoveComp->MaxWalkSpeed = NormalWalkSpeed;
    }

    Character->ResetMovementDebuffs();
    Destroy();
}

void AMedKit::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (Cast<AMyCharacter>(OtherActor))
    {
        bIsPlayerInRange = true;
    }
}

void AMedKit::HandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (Cast<AMyCharacter>(OtherActor))
    {
        bIsPlayerInRange = false;
    }
}
