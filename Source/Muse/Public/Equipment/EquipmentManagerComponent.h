// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentInstance.h"
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

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MUSE_API UEquipmentManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
  DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActiveEquipmentChanged, const UEquipmentInstance*, NewEquipmentAnimationData);
  UPROPERTY(BlueprintAssignable)
  FActiveEquipmentChanged ActiveEquipmentChanged;

  DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEquipmentStateChanged, const EEquipmentState, EquipmentActive);
  UPROPERTY(BlueprintAssignable)
  FEquipmentStateChanged EquipmentStateChanged;

  DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEquipmentAnimStateChanged, const bool, bUseFullBodyAnims);
  UPROPERTY(BlueprintAssignable)
  FEquipmentAnimStateChanged EquipmentAnimStateChanged;

private:
  UPROPERTY()
  TMap<EWeapon, UEquipmentInstance*> MappedEquipment;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EquipmentData, meta=(AllowPrivateAccess="true"))
  TObjectPtr<UEquipmentDataAsset> SwordEquipmentData;

  UPROPERTY()
  TObjectPtr<AEquipment> Sword;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EquipmentData, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UEquipmentDataAsset> RifleEquipmentData;

  UPROPERTY()
  TObjectPtr<AEquipment> Rifle;

  EWeapon ActiveWeapon;
  EEquipmentState ActiveEquipmentState;
  UEquipmentInstance* ActiveEquipmentInstance;

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

  void ConstructEquipment(USkeletalMeshComponent* InSkeletalMesh, const FName& InSocketName);

  void SetEquipment(const EWeapon WeaponType, UEquipmentInstance* InEquipmentInstance);

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
