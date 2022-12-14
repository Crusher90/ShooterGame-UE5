// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterHUD.h"
#include "GameFramework/PlayerController.h"
#include "CharacterOverlay.h"

void AShooterHUD::DrawHUD()
{
    Super::DrawHUD();

    FVector2D ViewportSize;
    if(GEngine && GEngine->GameViewport)
    {
        GEngine->GameViewport->GetViewportSize(ViewportSize);
        const FVector2D ViewportCenter(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);

        if(HUDPackage.CrosshairCenter)
        {
            DrawCrosshair(HUDPackage.CrosshairCenter, ViewportCenter);
        }
        if (HUDPackage.CrosshairLeft)
        {
            DrawCrosshair(HUDPackage.CrosshairLeft, ViewportCenter);
        }
        if (HUDPackage.CrosshairRight)
        {
            DrawCrosshair(HUDPackage.CrosshairRight, ViewportCenter);
        }
        if (HUDPackage.CrosshairTop)
        {
            DrawCrosshair(HUDPackage.CrosshairTop, ViewportCenter);
        }
        if (HUDPackage.CrosshairBottom)
        {
            DrawCrosshair(HUDPackage.CrosshairBottom, ViewportCenter);
        }
    }
}

void AShooterHUD::BeginPlay() 
{
    Super::BeginPlay();
    AddCharacterOverlay();
}

void AShooterHUD::AddCharacterOverlay() 
{
    APlayerController *PlayerController = GetOwningPlayerController();
    if(PlayerController && CharacterOverlayClass)
    {
        CharacterOverlay = CreateWidget<UCharacterOverlay>(PlayerController, CharacterOverlayClass);
        CharacterOverlay->AddToViewport();
    }
}

void AShooterHUD::DrawCrosshair(UTexture2D *TextureToDraw, FVector2D ViewportCenter) 
{
    const float TextureWidth = TextureToDraw->GetSizeX();
    const float TextureHeight = TextureToDraw->GetSizeY();

    FVector2D TextureDrawPoint(ViewportCenter.X - (TextureWidth / 2.f), ViewportCenter.Y - (TextureHeight / 2.f));

    DrawTexture(TextureToDraw, TextureDrawPoint.X, TextureDrawPoint.Y, TextureWidth, TextureHeight, 0.f, 0.f, 1.f, 1.f, FLinearColor::White);
}
