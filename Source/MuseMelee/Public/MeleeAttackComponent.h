// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MeleeComboDataAsset.h"
#include "MeleeAttackComponent.generated.h"

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
  void SetMeleeComboData(UMeleeComboDataAsset* InMeleeComboData);

private:
  UAnimInstance* AnimInstance;
  UAnimMontage* CurrAnimMontage;
};
