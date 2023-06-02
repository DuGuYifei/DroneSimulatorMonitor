// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneActorComponent.h"

#include "ImageUtils.h"
#include "Components/SceneCaptureComponent2D.h"

// Sets default values for this component's properties
UDroneActorComponent::UDroneActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UDroneActorComponent::BeginPlay()
{
	Super::BeginPlay();

	SenderAddr->SetIp(*IPAddress, bIsValid);
	SenderImageAddr->SetIp(*IPAddress, bIsValid);
	SenderImageAddr2->SetIp(*IPAddress, bIsValid);
	SenderAddr->SetPort(Port);
	SenderImageAddr->SetPort(ImagePort);
	SenderImageAddr2->SetPort(ImagePort2);
	SenderSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_DGram, TEXT("UDP_SenderSocket"));
	SenderSocket2 = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_DGram, TEXT("UDP_SenderSocket2"));
	SenderSocket3 = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_DGram, TEXT("UDP_SenderSocket3"));
	readPixelFlags = FReadSurfaceDataFlags(RCM_UNorm);
	rtResource = cameraRenderTarget->GameThread_GetRenderTargetResource();
	rtResource2 = cameraRenderTarget2->GameThread_GetRenderTargetResource();
}


// Called every frame
void UDroneActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//如果按下esc或者手柄的back键，退出游戏
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Escape) || GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Gamepad_Special_Left))
	{
		GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
	}

	if(flyingPawn == nullptr)
	{
		TArray<AActor*> actors = GetWorld()->GetCurrentLevel()->Actors;
		for (AActor* actor : actors)
		{
			if (actor && actor->GetName().Contains("Flying"))
			{
				this->flyingPawn = actor;
			}
		}
		return;
	}

	this->SceneCaptureActor->SetActorRotation(NewRotation);

	// Get Location:
	FVector position = flyingPawn->GetOwner()->GetActorLocation();
	FVector rotation = flyingPawn->GetOwner()->GetActorRotation().Euler();
	glm::dvec3 positionGlm = glm::dvec3(position.X, position.Y, position.Z);
	glm::dvec3 cesiumPosition = this->georeference->TransformUnrealToLongitudeLatitudeHeight(positionGlm);

	// get image of camera and change to png
	outBMP.Empty();
	CompressedBitmap.Empty();
	outBMP.AddUninitialized(16384);
	rtResource->ReadPixels(outBMP, readPixelFlags);
	FImageUtils::CompressImageArray(destSize.X, destSize.Y, outBMP, CompressedBitmap);
	//UE_LOG(LogTemp, Warning, TEXT("Compressed image size: %d"), CompressedBitmap.Num());

	// save to file
	//bool imageSavedOk = FFileHelper::SaveArrayToFile(CompressedBitmap, *filePath);
	//if(!imageSavedOk)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("Failed to save image"));
	//}
	FString JsonString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonString);

	//JsonObject->SetNumberField("lon", cesiumPosition.x);
	//JsonObject->SetNumberField("lat", cesiumPosition.y);
	//JsonObject->SetNumberField("alt", cesiumPosition.z);
	//JsonObject->SetNumberField("yaw", rotation.Z);
	//JsonObject->SetNumberField("pitch", rotation.Y);
	//JsonObject->SetNumberField("roll", rotation.X);

	JsonWriter->WriteObjectStart();
	JsonWriter->WriteValue("lon", cesiumPosition.x);
	JsonWriter->WriteValue("lat", cesiumPosition.y);
	JsonWriter->WriteValue("alt", cesiumPosition.z);
	JsonWriter->WriteValue("yaw", rotation.Z);
	JsonWriter->WriteValue("pitch", rotation.Y);
	JsonWriter->WriteValue("roll", rotation.X);
	//JsonWriter->WriteArrayStart("image");
	//for(const uint8& byte : CompressedBitmap)
	//{
	//	JsonWriter->WriteValue(byte);
	//}
	//JsonWriter->WriteArrayEnd();
	JsonWriter->WriteObjectEnd();
	JsonWriter->Close();

	//FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);
	//JsonWriter->Close();

	if (!bIsValid)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid IP address: %s"), *IPAddress);
		return;
	}

	int32 BytesSent = 0;
	TArray<uint8> DataArray;
	FTCHARToUTF8 UTF8Data(*JsonString);
	DataArray.Append((uint8*)UTF8Data.Get(), UTF8Data.Length());
	bool bSuccessful = SenderSocket->SendTo(DataArray.GetData(), DataArray.Num(), BytesSent, *SenderAddr);
	if (!bSuccessful)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to send UDP data"));
	}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("%s"), *JsonString);
	//}
	BytesSent = 0;
	bSuccessful = SenderSocket2->SendTo(CompressedBitmap.GetData(), CompressedBitmap.Num(), BytesSent, *SenderImageAddr);
	if(!bSuccessful)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to send UDP image data"));
	}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Sent image data: %d"), BytesSent2);
	//}
	outBMP.Empty();
	CompressedBitmap.Empty();
	outBMP.AddUninitialized(16384);
	rtResource2->ReadPixels(outBMP, readPixelFlags);
	FImageUtils::CompressImageArray(destSize.X, destSize.Y, outBMP, CompressedBitmap);
	BytesSent = 0;
	bSuccessful = SenderSocket3->SendTo(CompressedBitmap.GetData(), CompressedBitmap.Num(), BytesSent, *SenderImageAddr2);
	if (!bSuccessful)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to send UDP image2 data"));
	}
}

