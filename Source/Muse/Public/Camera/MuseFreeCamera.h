// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MuseFreeCamera.generated.h"

class UCameraComponent;

UCLASS(Blueprintable, BlueprintType)
class MUSE_API AMuseFreeCamera : public AActor
{
	GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
  TObjectPtr<UCameraComponent> Camera;
	
public:	
	// Sets default values for this actor's properties
	AMuseFreeCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
