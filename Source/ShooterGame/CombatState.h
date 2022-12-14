#pragma once

UENUM(BlueprintType)
enum class ECombatState : uint8
{
    ECS_Unoccupied UMETA(DisplayName = "Unoccupied"),
    ECS_Occupied UMETA(DisplayName = "Occupied"),
    ECS_Reloading UMETA(DisplayName = "Reloading"),

    ECS_DefaultMax UMETA(DisplayName = "DefaultMax")
};