// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilitySystemInterface.h"
#include "MuseCharacterGameplayAbilitySystemComponent.generated.h"

UENUM(BlueprintType)
enum class ECharacterAbilityInput : uint8
{
  UseAbility1 UMETA(DisplayName = "Use Ability01"),
  UseAbility2 UMETA(DisplayName = "Use Ability02"),
  UseAbility3 UMETA(DisplayName = "Use Ability03")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MUSEGAS_API UMuseCharacterGameplayAbilitySystemComponent : public UActorComponent, public IAbilitySystemInterface
{
	GENERATED_BODY()

  //** Ability system */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilties, meta = (AllowPrivateAccess = "true"))
  class UAbilitySystemComponent* AbilitySystem;

  //** Ability */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Abilities, meta=(AllowPrivateAccess="true"))
  TSubclassOf<class UGameplayAbility> Ability;

public:	
	// Sets default values for this component's properties
	UMuseCharacterGameplayAbilitySystemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
  // Bind character abilities to input component
  void BindAbilitySystemInputs(UInputComponent* PlayerInputComponent);

  // Called whenever character/actor changes, i.e during possession.
  void RefreshAbilitySystem();

  // Returns ability system component
  virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystem; }
};
