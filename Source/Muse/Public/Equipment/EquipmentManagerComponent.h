// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentDataAsset.h"
#include "EquipmentManagerComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogEquipmentManagerComponent, Log, All);

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

USTRUCT(BlueprintType)
struct FEquipmentInstance
{
  GENERATED_USTRUCT_BODY()

  UPROPERTY()
  UEquipmentComponent* Equipment;

  UPROPERTY()
  UEquipmentDataAsset* Data;

public:
  inline UEquipmentComponent* GetEquipment() const { return Equipment; }
  inline const UEquipmentDataAsset* GetEquipmentData() const { return Data; }
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

  DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEquipmentAnimStateChanged, bool, bUseFullBodyAnims);
  UPROPERTY(BlueprintAssignable)
  FEquipmentAnimStateChanged EquipmentAnimStateChanged;

private:
  UPROPERTY()
  TMap<EWeapon, FEquipmentInstance> MappedEquipment;

  EWeapon ActiveWeapon;
  EEquipmentState ActiveEquipmentState;
  FEquipmentInstance* ActiveEquipmentInstance;

  bool bTickCurrentEquipmentActiveDuration = false;
  float CurrentEquipmentActiveDuration = 0.0f;
  bool bUseFullBodyAnims = false;

public:	
	// Sets default values for this component's properties
	UEquipmentManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

  void SetEquipment(const EWeapon WeaponType, const FEquipmentInstance& InEquipmentInstance);

  UFUNCTION(BlueprintCallable)
  void SetActiveEquipment(const EWeapon WeaponType);

  UFUNCTION(BlueprintCallable)
  void SetActiveEquipmentForDuration(const EWeapon InWeapon, const float EquipDuration);

  UFUNCTION(BlueprintCallable)
  void SetEquipmentState(const EEquipmentState InEquipmentState);

  UFUNCTION(BlueprintCallable)
  void SetUseFullBodyAnims(const bool bInUseFullBodyAnims);

  UFUNCTION(BlueprintCallable, BlueprintPure)
  inline bool IsEquipped() const { return ActiveEquipmentState == EEquipmentState::EQUIPPED; }

private:
  void ResetActiveEquipment();
};
