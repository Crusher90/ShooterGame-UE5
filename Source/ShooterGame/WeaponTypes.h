#pragma once

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    EWT_AssaultRifle UMETA(DisplayName = "AssaultRifle"),
    EWT_Pistol UMETA(DisplayName = "Pistol"),
    EWT_SniperRifle UMETA(DisplayName = "SniperRifle"),
    EWT_SMG UMETA(DisplayName = "SMG"),
    EWT_RocketLauncher UMETA(DisplayName = "RocketLauncher"),
    EWT_GrenadeLauncher UMETA(DisplayName = "GrenadeLauncher"),

    EWT_DefaultMax UMETA(DisplayName = "DefaultMax")
};