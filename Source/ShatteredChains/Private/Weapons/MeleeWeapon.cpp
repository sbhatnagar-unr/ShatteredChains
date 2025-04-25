// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/MeleeWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "Components/SphereComponent.h"  
#include "Components/StaticMeshComponent.h"
#include "Player/MyCharacter.h"   
#include "Components/HealthComponent/HealthComponent.h"

AMeleeWeapon::AMeleeWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

    // Mesh as root
    MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SwordMesh"));
    RootComponent = MeshComponent;
    MeshComponent->SetSimulatePhysics(false);   // Don't simulate physics
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);  // Only for overlap/pickup
    MeshComponent->SetCollisionResponseToAllChannels(ECR_Block);       // Block walls, floors, etc
    MeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // Allow player overlap


    // Overlap sphere
    PickupSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PickupSphere"));
    PickupSphere->SetupAttachment(MeshComponent);
    PickupSphere->InitSphereRadius(50.0f);
    PickupSphere->SetGenerateOverlapEvents(true);
    PickupSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    PickupSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
    PickupSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    PickupSphere->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeapon::HandleBeginOverlap);
    PickupSphere->OnComponentEndOverlap.AddDynamic(this, &AMeleeWeapon::HandleEndOverlap);
}

void AMeleeWeapon::BeginPlay()
{
    Super::BeginPlay();
    OwnerPawn = Cast<APawn>(GetOwner());
}

void AMeleeWeapon::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (Cast<AMyCharacter>(OtherActor))
    {
        bIsPlayerInRange = true;
    }
}

void AMeleeWeapon::HandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (Cast<AMyCharacter>(OtherActor))
    {
        bIsPlayerInRange = false;
    }
}

void AMeleeWeapon::Punch()
{
    if (!OwnerPawn || bIsPunching) return;

    UAnimInstance* AnimInstance = Cast<ACharacter>(OwnerPawn)->GetMesh()->GetAnimInstance();
    if (!AnimInstance) return;

    bIsPunching = true;

    UAnimMontage* SelectedMontage = nullptr;

    if (bIsSword)
    {
        // Sword logic
        SelectedMontage = (FMath::RandBool()) ? SwordSlashMontage1 : SwordSlashMontage2;

        if (SwordSwingSound)
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), SwordSwingSound, OwnerPawn->GetActorLocation());
        }
    }
    else
    {
        // Fist logic
        SelectedMontage = (FMath::RandBool()) ? PunchMontage1 : PunchMontage2;

        if (PunchSound)
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), PunchSound, OwnerPawn->GetActorLocation());
        }
    }

    if (SelectedMontage)
    {
        float Duration = AnimInstance->Montage_Play(SelectedMontage);

        FTimerHandle PunchResetTimer;
        GetWorldTimerManager().SetTimer(PunchResetTimer, [this]()
            {
                bIsPunching = false;
            }, Duration, false);

        FTimerHandle DamageTimer;
        GetWorldTimerManager().SetTimer(DamageTimer, this, &AMeleeWeapon::ApplyDamage, DamageDelay, false);
    }
}




void AMeleeWeapon::ApplyDamage()
{
    if (!OwnerPawn) return;

    FVector Start = OwnerPawn->GetActorLocation();
    FVector Forward = OwnerPawn->GetActorForwardVector();
    FVector End = Start + Forward * MeleeRange; // Punch reach distance
    float Radius = MeleeRadius;

    TArray<FHitResult> HitResults;
    FCollisionShape CollisionShape = FCollisionShape::MakeSphere(Radius);

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
    Params.AddIgnoredActor(OwnerPawn); // Don't hit self

    bool bHit = GetWorld()->SweepMultiByChannel(
        HitResults,
        Start,
        End,
        FQuat::Identity,
        ECC_Pawn,
        CollisionShape,
        Params
    );

    if (bHit)
    {
        for (const FHitResult& Hit : HitResults)
        {
            AActor* HitActor = Hit.GetActor();
            if (HitActor)
            {
                UHealthComponent* HealthComp = HitActor->FindComponentByClass<UHealthComponent>();
                if (HealthComp && !HealthComp->dead())
                {
                    HealthComp->deal_damage(OwnerPawn, Damage);
                    UE_LOG(LogTemp, Log, TEXT("Punched %s for %f damage"), *HitActor->GetName(), Damage);
                }
            }
        }
    }
    if (PunchSound && OwnerPawn)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), PunchSound, OwnerPawn->GetActorLocation());
    }

}



