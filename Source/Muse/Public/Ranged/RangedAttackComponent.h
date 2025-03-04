// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Equipment/EquipmentManagerComponent.h"
#include "MuseCharacter.h"

#include "RangedAttackComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MUSE_API URangedAttackComponent : public UActorComponent
{
	GENERATED_BODY()

  UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
  bool bIsAiming;

  UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
  TObjectPtr<UEquipmentManagerComponent> EquipmentManagerComponent;

  UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
  AMuseCharacter* OwningCharacter;

public:	
	// Sets default values for this component's properties
	URangedAttackComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

  UFUNCTION(BlueprintCallable)
  void EnterAim();
  UFUNCTION(BlueprintCallable)
  void ExitAim();
  UFUNCTION(BlueprintCallable)
  void FireWeapon();

  UFUNCTION(BlueprintCallable, BlueprintPure)
  inline bool GetIsAiming() { return bIsAiming; }
};
