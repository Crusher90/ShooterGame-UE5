// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ShooterHUD.generated.h"

USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()

public:
	class UTexture2D *CrosshairCenter;

	UTexture2D *CrosshairLeft;

	UTexture2D *CrosshairRight;

	UTexture2D *CrosshairTop;

	UTexture2D *CrosshairBottom;
};

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AShooterHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

	UPROPERTY(EditAnywhere, Category = "ShooterHUD")
	TSubclassOf<class UUserWidget> CharacterOverlayClass;

	class UCharacterOverlay *CharacterOverlay;

protected:
	virtual void BeginPlay() override;

	void AddCharacterOverlay();

private:
	FHUDPackage HUDPackage;

	void DrawCrosshair(UTexture2D *TextureToDraw, FVector2D ViewportCenter);

public:
	FORCEINLINE void SetHUDPackage(const FHUDPackage& Package) { HUDPackage = Package; }
};
