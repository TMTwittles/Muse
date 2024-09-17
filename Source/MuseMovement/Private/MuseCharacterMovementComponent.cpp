// Fill out your copyright notice in the Description page of Project Settings.
#include "MuseCharacterMovementComponent.h"

bool UMuseCharacterMovementComponent::FSavedMove_MuseCharacter::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const
{
  FSavedMove_MuseCharacter* NewCharacterMove = static_cast<FSavedMove_MuseCharacter*>(NewMove.Get());

  // TODO: Check new move variables with custom movement variables to see if movements can be combined.

  return FSavedMove_Character::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void UMuseCharacterMovementComponent::FSavedMove_MuseCharacter::Clear()
{
  FSavedMove_Character::Clear();

  // TODO: Add code for clearing custom movement variables here.
}


uint8 UMuseCharacterMovementComponent::FSavedMove_MuseCharacter::GetCompressedFlags() const
{
  uint8 Result = Super::GetCompressedFlags();

  // TODO: Add custom bits to the result based off custom movement variables.

  return Result;
}

void UMuseCharacterMovementComponent::FSavedMove_MuseCharacter::SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
  FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

  UMuseCharacterMovementComponent* CharacterMovement = Cast<UMuseCharacterMovementComponent>(C->GetCharacterMovement());

  Saved_bInMeleeAttack = CharacterMovement->Safe_bInMeleeAttack;
}

void UMuseCharacterMovementComponent::FSavedMove_MuseCharacter::PrepMoveFor(ACharacter* C)
{
  FSavedMove_Character::PrepMoveFor(C);

  UMuseCharacterMovementComponent* CharacterMovement = Cast<UMuseCharacterMovementComponent>(C->GetCharacterMovement());

  Saved_bInMeleeAttack = CharacterMovement->Safe_bInMeleeAttack;
}

UMuseCharacterMovementComponent::FNetworkPredictionData_Client_MuseCharacter::FNetworkPredictionData_Client_MuseCharacter(const UCharacterMovementComponent& ClientMovement)
  : Super(ClientMovement)
{
}

FSavedMovePtr UMuseCharacterMovementComponent::FNetworkPredictionData_Client_MuseCharacter::AllocateNewMove()
{
  return FSavedMovePtr(new FSavedMove_MuseCharacter());
}

void UMuseCharacterMovementComponent::InitializeComponent()
{
  Super::InitializeComponent();
}

FNetworkPredictionData_Client* UMuseCharacterMovementComponent::GetPredictionData_Client() const
{
  check(PawnOwner != nullptr);

  if (ClientPredictionData == nullptr)
  {
    UMuseCharacterMovementComponent* MutableThis = const_cast<UMuseCharacterMovementComponent*>(this);
    MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_MuseCharacter(*this);
    MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
    MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.0f;
  }
  return ClientPredictionData;
}

void UMuseCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
  Super::UpdateFromCompressedFlags(Flags);
  // Add custom flag logic muse AI character custom movement logic here. 
}

void UMuseCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
  Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);
}

void UMuseCharacterMovementComponent::PhysCustom(float DeltaTime, int32 Iterations)
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

void UMuseCharacterMovementComponent::EnterMeleeAttack()
{
  bInMeleeAttack = true;
  SetMovementMode(MOVE_Custom, CMOVE_MELEE_ATTACK);
}

void UMuseCharacterMovementComponent::ExitMeleeAttack()
{
  bInMeleeAttack = false;
  FQuat NewRotation = FRotationMatrix::MakeFromXZ(UpdatedComponent->GetForwardVector().GetSafeNormal2D(), FVector::UpVector).ToQuat();
  FHitResult Hit;
  SafeMoveUpdatedComponent(FVector::ZeroVector, NewRotation, true, Hit);
  SetMovementMode(MOVE_Walking);
}

void UMuseCharacterMovementComponent::PhysMeleeAttack(float DeltaTime, int32 Iterations)
{

}

bool UMuseCharacterMovementComponent::IsCustomMovementMode(EMuseMovementMode InCustomMovementMode) const
{
  return MovementMode == MOVE_Custom && CustomMovementMode == InCustomMovementMode;
}
