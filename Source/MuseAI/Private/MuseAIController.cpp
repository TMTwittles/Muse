// Fill out your copyright notice in the Description page of Project Settings.


#include "MuseAIController.h"
#include "MusePathFollowingComponent.h"

AMuseAIController::AMuseAIController(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer.SetDefaultSubobjectClass<UMusePathFollowingComponent>(TEXT("PathFollowingComponent")))
{
}
