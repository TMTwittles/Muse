// Fill out your copyright notice in the Description page of Project Settings.
#include "MeleePositioningSlotService.h"
#include "MeleePositioningSlotConfig.h"
#include "MeleeSlotPositioningComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "AIManager.h"

void UMeleePositioningSlotService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
  // TODO: Move this code into a static method for MuseAI that just gets the requested component of the AI manager. Also does
  // appropriate error checking and messaging.
  TArray<AActor*> FoundActors;
  UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIManager::StaticClass(), FoundActors);
  check(FoundActors.Num() == 1);
  AAIManager* AIManager = Cast<AAIManager>(FoundActors[0]);
  UMeleeSlotPositioningComponent* MeleeSlotPositioning = AIManager->GetMeleeSlotPositioningComponent();
  OwnerComp.GetBlackboardComponent()->SetValueAsVector(
    TargetPosition.SelectedKeyName,
    MeleeSlotPositioning->GetMeleePosition(FName(*OwnerComp.GetOwner()->GetName())));
}
