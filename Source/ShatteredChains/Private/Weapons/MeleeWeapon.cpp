// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/MeleeWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "Components/HealthComponent/HealthComponent.h"

AMeleeWeapon::AMeleeWeapon()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AMeleeWeapon::BeginPlay()
{
    Super::BeginPlay();
    OwnerPawn = Cast<APawn>(GetOwner());
}

void AMeleeWeapon::Punch()
{
    if (!OwnerPawn || bIsPunching) return; // Prevent spam if already punching

    UAnimInstance* AnimInstance = Cast<ACharacter>(OwnerPawn)->GetMesh()->GetAnimInstance();
    if (!AnimInstance) return;

    UAnimMontage* SelectedMontage = (FMath::RandBool()) ? PunchMontage1 : PunchMontage2;
    if (SelectedMontage)
    {
        bIsPunching = true;

        float Duration = AnimInstance->Montage_Play(SelectedMontage);

        // Reset punch state after montage ends
        FTimerHandle PunchResetTimer;
        GetWorldTimerManager().SetTimer(PunchResetTimer, [this]()
            {
                bIsPunching = false;
            }, Duration, false);

        // Delay damage
        FTimerHandle DamageTimer;
        GetWorldTimerManager().SetTimer(DamageTimer, this, &AMeleeWeapon::ApplyDamage, DamageDelay, false);
    }
}

void AMeleeWeapon::ApplyDamage()
{
    if (!OwnerPawn) return;

    FVector Start = OwnerPawn->GetActorLocation();
    FVector Forward = OwnerPawn->GetActorForwardVector();
    FVector End = Start + Forward * 150.0f; // Punch reach distance

    float Radius = 100.0f;

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



