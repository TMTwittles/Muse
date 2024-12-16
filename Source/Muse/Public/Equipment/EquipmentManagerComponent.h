// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentAnimationDataAsset.h"
#include "EquipmentManagerComponent.generated.h"

UENUM(BlueprintType)
enum class EWeapon : uint8
{
  SWORD UMETA(DisplayName = "Sword"),
  RIFLE UMETA(DisplayName = "Rifle"),
  NONE UMETA(DisplayName = "None")
};

UENUM(BlueprintType)
enum class EEquipmentState : uint8
{
  HOLSTERED UMETA(DisplayName = "Holstered"),
  EQUIPPED UMETA(DisplayName = "Equipped")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MUSE_API UEquipmentManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
  DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActiveEquipmentChanged, UEquipmentAnimationDataAsset*, NewEquipmentAnimationData);
  UPROPERTY(BlueprintAssignable)
  FActiveEquipmentChanged ActiveEquipmentChanged;

  DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEquipmentStateChanged, EEquipmentState, EquipmentActive);
  UPROPERTY(BlueprintAssignable)
  FEquipmentStateChanged EquipmentStateChanged;

private:
  UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="True"))
  TObjectPtr<UStaticMeshComponent> SwordStaticMesh;

  UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="True"))
  TObjectPtr<UEquipmentAnimationDataAsset> SwordEquipmentAnimationData;

  UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "True"))
  TObjectPtr<UStaticMeshComponent> RifleStaticMesh;

  UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "True"))
  TObjectPtr<UEquipmentAnimationDataAsset> RifleEquipmentAnimationData;

  EWeapon ActiveWeapon;
  EEquipmentState ActiveEquipmentState;
  UStaticMeshComponent* ActiveEquipmentMesh;
  UEquipmentAnimationDataAsset* ActiveEquipmentAnimationData;
  bool bTickCurrentEquipmentActiveDuration = false;
  float CurrentEquipmentActiveDuration = 0.0f;

public:	
	// Sets default values for this component's properties
	UEquipmentManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

  UFUNCTION(BlueprintCallable)
  void SetActiveEquipment(const EWeapon WeaponType);

  UFUNCTION(BlueprintCallable)
  void SetActiveEquipmentForDuration(const EWeapon InWeapon, const float EquipDuration);

  UFUNCTION(BlueprintCallable)
  void SetEquipmentState(const EEquipmentState InEquipmentState);

  UFUNCTION(BlueprintCallable, BlueprintPure)
  inline bool IsEquipped() const { return ActiveEquipmentState == EEquipmentState::EQUIPPED; }

private:
  void ResetActiveEquipment();
};
