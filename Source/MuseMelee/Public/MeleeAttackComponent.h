// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MeleeComboDataAsset.h"
#include "MeleeAttackComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMeleeAttackComponent, Log, All);

class UAnimNotifyState_MeleeAttackPhase;
enum class EMeleeAnimationPhase : uint8;

USTRUCT()
struct FMeleeAttackContainer
{
  GENERATED_USTRUCT_BODY()

  UAnimMontage* AnimMontage;
  TArray<UAnimNotifyState_MeleeAttackPhase*> MeleeAttackAnimPhases;

  void Clear()
  {
    AnimMontage = nullptr;
    MeleeAttackAnimPhases.Empty();
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
  bool TryTriggerAttack();

private:
  void ConfigureContainer(FMeleeAttackContainer& InContainer, const uint32 MeleeAttackIndex);
  void MeleeMontageFinished(UAnimMontage* Montage, bool bInterrupted);
  EMeleeAnimationPhase GetActiveMeleeAnimationPhase();
  inline bool MeleeAnimationPlaying() { return CurrMeleeContainer.AnimMontage != nullptr; }

private:
  UAnimInstance* AnimInstance;
  FMeleeAttackContainer CurrMeleeContainer;
};
