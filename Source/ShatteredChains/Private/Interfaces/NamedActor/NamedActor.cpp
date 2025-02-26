// Fill out your copyright notice in the Description page of Project Settings.


#include "NamedActor.h"


// Add default functionality here for any INamedActor functions that are not pure virtual.
FString INamedActor::get_actor_name() const
{
    return actor_name;
}


void INamedActor::set_actor_name(const FString &new_name)
{
    actor_name = new_name;
}
