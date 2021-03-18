// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shoot.h"
#include "AIController.h"
#include "SimpleShooter/Characters/ShooterCharacter.h"


UBTTask_Shoot::UBTTask_Shoot() 
{
    NodeName = TEXT("Shoot");
}
EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemeroy) 
{
    Super::ExecuteTask(OwnerComp, NodeMemeroy);

    if (!OwnerComp.GetAIOwner())
    {
        return EBTNodeResult::Failed;
    }

    AShooterCharacter* AIPawn = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if (!AIPawn)
    {
        return EBTNodeResult::Failed;
    }
    AIPawn->Shoot();

    return EBTNodeResult::Succeeded;
}

