// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Components/ActorComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "CesiumRuntime/Public/CesiumGeoreference.h"
#include "DroneActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FLIGHTSIMULATOR_API UDroneActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone")
	ACesiumGeoreference* georeference;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone")
	UTextureRenderTarget2D* cameraRenderTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone")
	UTextureRenderTarget2D* cameraRenderTarget2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone")
	AActor* SceneCaptureActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone")
	AActor* sceneCaptureActor2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone")
	FString IPAddress;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone")
	AActor* flyingPawn;

	// Sets default values for this component's properties
	UDroneActorComponent();

protected:
	// IP and port
	TSharedRef<FInternetAddr> SenderAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	TSharedRef<FInternetAddr> SenderImageAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	TSharedRef<FInternetAddr> SenderImageAddr2 = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	bool bIsValid;
	const int32 Port = 8080;
	const int32 ImagePort = 8081;
	const int32 ImagePort2 = 8082;
	FSocket* SenderSocket;
	FSocket* SenderSocket2;
	FSocket* SenderSocket3;
	TArray<FColor> outBMP;
	TArray<uint8> CompressedBitmap;
	FTextureRenderTargetResource* rtResource;
	FTextureRenderTargetResource* rtResource2;
	FReadSurfaceDataFlags readPixelFlags;
	FRotator NewRotation = FRotator(-90.f, 0.f, 0.f);
	FIntPoint destSize = FIntPoint(128, 128);
	
	// FString filePath = "E:/test.png";

	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
