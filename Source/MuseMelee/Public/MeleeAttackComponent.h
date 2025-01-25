// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MeleeComboDataAsset.h"
#include "MeleeAttackComponent.generated.h"

class UAnimNotifyState_MeleeAttackPhase;
enum class EMeleeAnimationPhase : uint8;

DECLARE_LOG_CATEGORY_EXTERN(LogMeleeAttackComponent, Log, All);

USTRUCT()
struct FMeleeAttackContainer
{
  GENERATED_USTRUCT_BODY()

  uint32 ComboID;
  UAnimMontage* AnimMontage;
  TArray<UAnimNotifyState_MeleeAttackPhase*> MeleeAttackAnimPhases;

  bool IsValid()
  {
    return AnimMontage != nullptr
      && MeleeAttackAnimPhases.Num() == 3
      && MeleeAttackAnimPhases[0] != nullptr
      && MeleeAttackAnimPhases[1] != nullptr
      && MeleeAttackAnimPhases[2] != nullptr;
  }
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MUSEMELEE_API UMeleeAttackComponent : public UActorComponent
{
	GENERATED_BODY()

private:
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Data, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UMeleeComboDataAsset> MeleeComboData;

public:	
	// Sets default values for this component's properties
	UMeleeAttackComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
  bool TryTriggerAttack(FVector MovementInput, bool bInAir);
  UFUNCTION()
  EMeleeAnimationPhase GetActiveMeleeAnimationPhase();
  UFUNCTION()
  inline bool MeleeAnimationPlaying() { return CurrMeleeContainer.AnimMontage != nullptr; }
  bool TryCancelMeleeAnimation();

private:
  void ConfigureContainer();
  UFUNCTION()
  void MeleeMontageFinished(UAnimMontage* Montage, bool bInterrupted);
  UFUNCTION()
  void MeleeAnimationPhaseStarted(EMeleeAnimationPhase InPhase);
  void PlayActiveAnimation(FVector MovementInput, bool bInAir);
  void ClearMeleeContainer();

private:
  UAnimInstance* AnimInstance;
  FMeleeAttackContainer CurrMeleeContainer;
};
