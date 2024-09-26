// Fill out your copyright notice in the Description page of Project Settings.
#include "Muse0CharacterMovementComponent.h"
#include "Engine/OverlapResult.h"

bool UMuse0CharacterMovementComponent::FSavedMove_MuseCharacter::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const
{
  FSavedMove_MuseCharacter* NewCharacterMove = static_cast<FSavedMove_MuseCharacter*>(NewMove.Get());

  // TODO: Check new move variables with custom movement variables to see if movements can be combined.

  return FSavedMove_Character::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void UMuse0CharacterMovementComponent::FSavedMove_MuseCharacter::Clear()
{
  FSavedMove_Character::Clear();

  // TODO: Add code for clearing custom movement variables here.
}


uint8 UMuse0CharacterMovementComponent::FSavedMove_MuseCharacter::GetCompressedFlags() const
{
  uint8 Result = Super::GetCompressedFlags();

  // TODO: Add custom bits to the result based off custom movement variables.

  return Result;
}

void UMuse0CharacterMovementComponent::FSavedMove_MuseCharacter::SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
  FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

  UMuse0CharacterMovementComponent* CharacterMovement = Cast<UMuse0CharacterMovementComponent>(C->GetCharacterMovement());

  Saved_bInMeleeAttack = CharacterMovement->Safe_bInMeleeAttack;
}

void UMuse0CharacterMovementComponent::FSavedMove_MuseCharacter::PrepMoveFor(ACharacter* C)
{
  FSavedMove_Character::PrepMoveFor(C);

  UMuse0CharacterMovementComponent* CharacterMovement = Cast<UMuse0CharacterMovementComponent>(C->GetCharacterMovement());

  Saved_bInMeleeAttack = CharacterMovement->Safe_bInMeleeAttack;
}

UMuse0CharacterMovementComponent::FNetworkPredictionData_Client_MuseCharacter::FNetworkPredictionData_Client_MuseCharacter(const UCharacterMovementComponent& ClientMovement)
  : Super(ClientMovement)
{
}

FSavedMovePtr UMuse0CharacterMovementComponent::FNetworkPredictionData_Client_MuseCharacter::AllocateNewMove()
{
  return FSavedMovePtr(new FSavedMove_MuseCharacter());
}

void UMuse0CharacterMovementComponent::InitializeComponent()
{
  Super::InitializeComponent();
}

FNetworkPredictionData_Client* UMuse0CharacterMovementComponent::GetPredictionData_Client() const
{
  check(PawnOwner != nullptr);

  if (ClientPredictionData == nullptr)
  {
    UMuse0CharacterMovementComponent* MutableThis = const_cast<UMuse0CharacterMovementComponent*>(this);
    MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_MuseCharacter(*this);
    MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
    MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.0f;
  }
  return ClientPredictionData;
}

void UMuse0CharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
  Super::UpdateFromCompressedFlags(Flags);
  // Add custom flag logic muse AI character custom movement logic here. 
}

void UMuse0CharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
  Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);
}

void UMuse0CharacterMovementComponent::PhysCustom(float DeltaTime, int32 Iterations)
{
  Super::PhysCustom(DeltaTime, Iterations);

  switch (CustomMovementMode)
  {
  case CMOVE_MELEE_ATTACK:
    PhysMeleeAttack(DeltaTime, Iterations);
    break;
  default:
    UE_LOG(LogTemp, Fatal, TEXT("Invalid movement mode."));
    break;
  } 
}

void UMuse0CharacterMovementComponent::EnterMeleeAttack()
{
  if (TrySetMeleeTargetActor())
  {
    UpdateTargetMeleePosition();
  }
  bInMeleeAttack = true;
  SetMovementMode(MOVE_Custom, CMOVE_MELEE_ATTACK);
}

void UMuse0CharacterMovementComponent::EndMeleeAttack()
{
  ExitMeleeAttack();
}

void UMuse0CharacterMovementComponent::ExitMeleeAttack()
{
  bInMeleeAttack = false;
  FQuat NewRotation = FRotationMatrix::MakeFromXZ(UpdatedComponent->GetForwardVector().GetSafeNormal2D(), FVector::UpVector).ToQuat();
  FHitResult Hit;
  SafeMoveUpdatedComponent(FVector::ZeroVector, NewRotation, true, Hit);
  SetMovementMode(MOVE_Walking);
}

void UMuse0CharacterMovementComponent::PhysMeleeAttack(float DeltaTime, int32 Iterations)
{
  if (DeltaTime < UCharacterMovementComponent::MIN_TICK_TIME)
  {
    return;
  }

  if (!TargetActorFound())
  {
    return;
  }
  // We need this, for some reason.
  RestorePreAdditiveRootMotionVelocity();

  UpdateTargetMeleePosition();

  const float SuckToMeleePositionSpeed = 300.0f;
  const FVector SuckMovementDirection = (TargetAttackPosition - GetOwner()->GetActorLocation()).GetSafeNormal();
  const FVector TargetVelocity = SuckMovementDirection * SuckToMeleePositionSpeed;

  // TODO: Add acceleration / input code.

  // We cant probably just ignore this.
  //if (!HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
  //{
  //  CalcVelocity(DeltaTime, GroundFriction, true, GetMaxBrakingDeceleration());
  //}
  //ApplyRootMotionToVelocity(DeltaTime);
  
  // Perform movement
  Iterations++;
  bJustTeleported = false;

  FVector OldLocation = UpdatedComponent->GetComponentLocation();
  FQuat OldRotation = UpdatedComponent->GetComponentRotation().Quaternion();
  FHitResult Hit(1.f);

  FVector Delta = TargetVelocity * DeltaTime;

  // TODO: Will need to raycast to determine ground normal to project onto the current
  // ground surface.
  //FVector PlaneNormal = FVector::UpVector; 
  //FVector VelocityPlaneDirection = FVector::VectorPlaneProject(Velocity, PlaneNormal).GetSafeNormal();
  FQuat NewRotation = FRotationMatrix::MakeFromXZ(TargetVelocity.GetSafeNormal(), FVector::UpVector).ToQuat();

  SafeMoveUpdatedComponent(Delta, NewRotation, true, Hit);

  if (Hit.Time < 1.f)
  {
    HandleImpact(Hit, DeltaTime, Delta);
    SlideAlongSurface(Delta, (1.f - Hit.Time), Hit.Normal, Hit, true);
  }

  // Update outgoing velocity
  if (!bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
  {
    Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / DeltaTime;
  }

  return;
}

bool UMuse0CharacterMovementComponent::TrySetMeleeTargetActor()
{
  TargetActor = nullptr;
  TArray<APawn*> NearbyPawns;

  FCollisionShape Sphere = FCollisionShape::MakeSphere(1000.0f);
  FCollisionQueryParams CollisionParams;
  CollisionParams.AddIgnoredActor(GetOwner()); // Ignore self

  TArray<FOverlapResult> OverlapResults;
  bool bHit = GetWorld()->OverlapMultiByObjectType(
    OverlapResults,
    GetOwner()->GetActorLocation(),
    FQuat::Identity,
    FCollisionObjectQueryParams(ECC_Pawn),
    Sphere,
    CollisionParams
  );

  //DrawDebugSphere(GetWorld(), GetOwner()->GetActorLocation(), 1000.0f, 12, FColor::Red, false, 2.0f);

  float ClosestDistance = MAX_flt;
  for (const FOverlapResult& Result : OverlapResults)
  {
    if (APawn* Pawn = Cast<APawn>(Result.GetActor()))
    {
      //DrawDebugSphere(GetWorld(), Pawn->GetActorLocation(), 10.0f, 12, FColor::Red, false, 2.0f);
      float Distance = FVector::Dist(GetOwner()->GetActorLocation(), Pawn->GetActorLocation());
      if (Distance < ClosestDistance)
      {
        ClosestDistance = Distance;
        TargetActor = Pawn;
      }
    }
  }

  return TargetActor != nullptr;
}

void UMuse0CharacterMovementComponent::UpdateTargetMeleePosition()
{
  check(TargetActorFound());

  FVector Direction = GetOwner()->GetActorLocation() - TargetActor->GetActorLocation();
  Direction = Direction.GetSafeNormal();

  //DrawDebugLine(GetWorld(), TargetActor->GetActorLocation(), TargetActor->GetActorLocation() + Direction * 250.0f, FColor::Red, false, -1.0f, 0, 10.0f);

  TargetAttackPosition = TargetActor->GetActorLocation() + Direction * 75.0f;

  DrawDebugSphere(GetWorld(), TargetAttackPosition, 50.0f, 10, FColor::Red, false, -1.0f);
}

bool UMuse0CharacterMovementComponent::IsCustomMovementMode(EMuseMovementMode InCustomMovementMode) const
{
  return MovementMode == MOVE_Custom && CustomMovementMode == InCustomMovementMode;
}
