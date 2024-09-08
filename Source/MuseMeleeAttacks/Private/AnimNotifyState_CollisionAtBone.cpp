// Fill out your copyright notice in the Description page of Project Settings.
#include "AnimNotifyState_CollisionAtBone.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(AnimNotifyState_CollisionAtBone);

void UAnimNotifyState_CollisionAtBone::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
  if (MeshComp->GetBoneIndex(BoneName) == INDEX_NONE)
  {
    UE_LOG(AnimNotifyState_CollisionAtBone, Warning, TEXT("Unable to locate bone %s in skeletal mesh: %s"), *BoneName.ToString(), *MeshComp->GetName());
  }

  HitActors.Empty();

  CurrentBoneLocation = MeshComp->GetSocketLocation(BoneName);
  PreviousBoneLocation = CurrentBoneLocation;
  FCollisionQueryParams Params;
  Params.AddIgnoredActor(MeshComp->GetOwner());
  PerformSphereTrace(MeshComp->GetWorld(), Params);
}

void UAnimNotifyState_CollisionAtBone::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
  CurrentBoneLocation = MeshComp->GetSocketLocation(BoneName);
  FCollisionQueryParams Params;
  Params.AddIgnoredActor(MeshComp->GetOwner());
  PerformSphereTrace(MeshComp->GetWorld(), Params);
  PreviousBoneLocation = CurrentBoneLocation;
}

void UAnimNotifyState_CollisionAtBone::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
  HitActors.Empty();
}

void UAnimNotifyState_CollisionAtBone::PerformSphereTrace(UWorld* InWorld, const FCollisionQueryParams& InParams)
{
  FHitResult Result;
  bool bHit = InWorld->SweepSingleByChannel(Result,
    CurrentBoneLocation,
    PreviousBoneLocation,
    FQuat::Identity,
    ECC_Pawn,
    FCollisionShape::MakeSphere(CollisionSphereRadius),
    InParams);

  DrawDebugSphere(InWorld, PreviousBoneLocation, CollisionSphereRadius, 10, FColor::Red);
  DrawDebugSphere(InWorld, CurrentBoneLocation, CollisionSphereRadius, 10, FColor::Green);
  
  if (bHit)
  {
    FName HitActorName = FName(*Result.GetActor()->GetName());
    if (!HitActors.Contains(HitActorName))
    {
      DrawDebugSphere(InWorld, Result.ImpactPoint, CollisionSphereRadius, 10, FColor::Blue, true, 10.0f);
      HitActors.Add(HitActorName);
      UGameplayStatics::ApplyDamage(
        Result.GetActor(),
        10.0f, 
        nullptr, 
        nullptr,
        UDamageType::StaticClass()  
        );
    }
  }
}
