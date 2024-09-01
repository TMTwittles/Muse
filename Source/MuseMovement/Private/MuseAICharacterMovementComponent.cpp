// Fill out your copyright notice in the Description page of Project Settings.
#include "MuseAICharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "AI/Navigation/NavigationDataInterface.h"

bool UMuseAICharacterMovementComponent::FSavedMove_MuseAICharacter::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const
{
  FSavedMove_MuseAICharacter* NewAICharacterMove = static_cast<FSavedMove_MuseAICharacter*>(NewMove.Get());

  // TODO: Check new move variables with custom movement variables to see if movements can be combined.

  return FSavedMove_Character::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void UMuseAICharacterMovementComponent::FSavedMove_MuseAICharacter::Clear()
{
  FSavedMove_Character::Clear();

  // TODO: Add code for clearing custom movement variables here.
}

uint8 UMuseAICharacterMovementComponent::FSavedMove_MuseAICharacter::GetCompressedFlags() const
{
  uint8 Result = Super::GetCompressedFlags();

  // TODO: Add custom bits to the result based off custom movement variables.

  return Result;
}

void UMuseAICharacterMovementComponent::FSavedMove_MuseAICharacter::SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
  FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

  UMuseAICharacterMovementComponent* AICharacterMovement = Cast<UMuseAICharacterMovementComponent>(C->GetCharacterMovement());

  // TODO: Add logic for setting saved moved data based off custom movement variables.
}

void UMuseAICharacterMovementComponent::FSavedMove_MuseAICharacter::PrepMoveFor(ACharacter* C)
{
  FSavedMove_Character::PrepMoveFor(C);

  UMuseAICharacterMovementComponent* AICharacterMovement = Cast<UMuseAICharacterMovementComponent>(C->GetCharacterMovement());

  // TODO: Add logic for setting saved moved to class's custom movement variables.
}

UMuseAICharacterMovementComponent::FNetworkPredictionData_Client_MuseAICharacter::FNetworkPredictionData_Client_MuseAICharacter(const UCharacterMovementComponent& ClientMovement)
  : Super(ClientMovement)
{
}

FSavedMovePtr UMuseAICharacterMovementComponent::FNetworkPredictionData_Client_MuseAICharacter::AllocateNewMove()
{
  return FSavedMovePtr(new FSavedMove_MuseAICharacter());
}

FNetworkPredictionData_Client* UMuseAICharacterMovementComponent::GetPredictionData_Client() const
{
  check(PawnOwner != nullptr);

  if (ClientPredictionData == nullptr)
  {
    UMuseAICharacterMovementComponent* MutableThis = const_cast<UMuseAICharacterMovementComponent*>(this);
    MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_MuseAICharacter(*this);
    MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
    MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.0f;
  }
  return ClientPredictionData;
}

void UMuseAICharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
  Super::UpdateFromCompressedFlags(Flags);
  // Add custom flag logic muse AI character custom movement logic here. 
}

void UMuseAICharacterMovementComponent::PhysNavWalking(float deltaTime, int32 Iterations)
{
  // TODO: Add custom stats for muse AI character.
  //SCOPE_CYCLE_COUNTER(STAT_CharPhysNavWalking);

  if (deltaTime < MIN_TICK_TIME)
  {
    return;
  }

  if ((!CharacterOwner || !CharacterOwner->Controller) && !bRunPhysicsWithNoController && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
  {
    Acceleration = FVector::ZeroVector;
    Velocity = FVector::ZeroVector;
    return;
  }

  const EMovementMode StartingMovementMode = MovementMode;
  const uint8 StartingCustomMovementMode = CustomMovementMode;

  RestorePreAdditiveRootMotionVelocity();

  // Ensure velocity is horizontal.
  MaintainHorizontalGroundVelocity();
  //devCode(ensureMsgf(!Velocity.ContainsNaN(), TEXT("PhysNavWalking: Velocity contains NaN before CalcVelocity (%s)\n%s"), *GetPathNameSafe(this), *Velocity.ToString()));

  //bound acceleration
  Acceleration.Z = 0.f;
  if (!HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
  {
    CalcVelocity(deltaTime, GroundFriction, false, GetMaxBrakingDeceleration());
    //devCode(ensureMsgf(!Velocity.ContainsNaN(), TEXT("PhysNavWalking: Velocity contains NaN after CalcVelocity (%s)\n%s"), *GetPathNameSafe(this), *Velocity.ToString()));
  }

  ApplyRootMotionToVelocity(deltaTime);

  if (MovementMode != StartingMovementMode || CustomMovementMode != StartingCustomMovementMode)
  {
    // Root motion could have taken us out of our current mode
    StartNewPhysics(deltaTime, Iterations);
    return;
  }

  Iterations++;

  FVector DesiredMove = Velocity;
  DesiredMove.Z = 0.f;

  const FVector OldLocation = GetActorFeetLocation();
  const FVector DeltaMove = DesiredMove * deltaTime;
  const bool bDeltaMoveNearlyZero = DeltaMove.IsNearlyZero();

  FVector AdjustedDest = OldLocation + DeltaMove;
  FNavLocation DestNavLocation;

  bool bSameNavLocation = false;
  if (CachedNavLocation.NodeRef != INVALID_NAVNODEREF)
  {
    if (bProjectNavMeshWalking)
    {
      const float DistSq2D = (OldLocation - CachedNavLocation.Location).SizeSquared2D();
      const float DistZ = FMath::Abs(OldLocation.Z - CachedNavLocation.Location.Z);

      const float TotalCapsuleHeight = CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.0f;
      const float ProjectionScale = (OldLocation.Z > CachedNavLocation.Location.Z) ? NavMeshProjectionHeightScaleUp : NavMeshProjectionHeightScaleDown;
      const float DistZThr = TotalCapsuleHeight * FMath::Max(0.f, ProjectionScale);

      bSameNavLocation = (DistSq2D <= UE_KINDA_SMALL_NUMBER) && (DistZ < DistZThr);
    }
    else
    {
      bSameNavLocation = CachedNavLocation.Location.Equals(OldLocation);
    }

    if (bDeltaMoveNearlyZero && bSameNavLocation)
    {
      if (const INavigationDataInterface* NavData = GetNavData())
      {
        if (!NavData->IsNodeRefValid(CachedNavLocation.NodeRef))
        {
          CachedNavLocation.NodeRef = INVALID_NAVNODEREF;
          bSameNavLocation = false;
        }
      }
    }
  }

  if (bDeltaMoveNearlyZero && bSameNavLocation)
  {
    DestNavLocation = CachedNavLocation;
    //UE_LOG(LogNavMeshMovement, VeryVerbose, TEXT("%s using cached navmesh location! (bProjectNavMeshWalking = %d)"), *GetNameSafe(CharacterOwner), bProjectNavMeshWalking);
  }
  else
  {
    //SCOPE_CYCLE_COUNTER(STAT_CharNavProjectPoint);

    // Start the trace from the Z location of the last valid trace.
    // Otherwise if we are projecting our location to the underlying geometry and it's far above or below the navmesh,
    // we'll follow that geometry's plane out of range of valid navigation.
    if (bSameNavLocation && bProjectNavMeshWalking)
    {
      AdjustedDest.Z = CachedNavLocation.Location.Z;
    }

    // Find the point on the NavMesh
    const bool bHasNavigationData = FindNavFloor(AdjustedDest, DestNavLocation);
    if (!bHasNavigationData)
    {
      SetMovementMode(MOVE_Walking);
      return;
    }

    CachedNavLocation = DestNavLocation;
  }

  if (DestNavLocation.NodeRef != INVALID_NAVNODEREF)
  {
    FVector NewLocation(AdjustedDest.X, AdjustedDest.Y, DestNavLocation.Location.Z);
    if (bProjectNavMeshWalking)
    {
      //SCOPE_CYCLE_COUNTER(STAT_CharNavProjectLocation);
      const float TotalCapsuleHeight = CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.0f;
      const float UpOffset = TotalCapsuleHeight * FMath::Max(0.f, NavMeshProjectionHeightScaleUp);
      const float DownOffset = TotalCapsuleHeight * FMath::Max(0.f, NavMeshProjectionHeightScaleDown);
      NewLocation = ProjectLocationFromNavMesh(deltaTime, OldLocation, NewLocation, UpOffset, DownOffset);
    }

    FVector AdjustedDelta = NewLocation - OldLocation;

    if (!AdjustedDelta.IsNearlyZero())
    {
      FHitResult HitResult;
      SafeMoveUpdatedComponent(AdjustedDelta, UpdatedComponent->GetComponentQuat(), bSweepWhileNavWalking, HitResult);
    }

    // Update velocity to reflect actual move
    if (!bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasVelocity())
    {
      Velocity = (GetActorFeetLocation() - OldLocation) / deltaTime;
      MaintainHorizontalGroundVelocity();
    }

    bJustTeleported = false;
  }
  else
  {
    StartFalling(Iterations, deltaTime, deltaTime, DeltaMove, OldLocation);
  }
}
