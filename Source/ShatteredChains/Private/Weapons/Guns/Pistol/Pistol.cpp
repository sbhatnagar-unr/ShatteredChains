// Fill out your copyright notice in the Description page of Project Settings.


#include "Pistol.h"
#include "ShatteredChains/Logging.h"

// Called when the game starts or when spawned
void APistol::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(Weapon, Log, LOG_TEXT("Pistol stock pile size: %d\tPistol mag size: %d"), max_ammo_stock_pile_count, magazine_size);
}