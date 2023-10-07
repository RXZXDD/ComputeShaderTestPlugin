// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ComputeShaderTestFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class COMPUTESHADERTESTPLUGINFUNCTIONLIB_API UComputeShaderTestFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, meta=(Category = "COMPUTE SHADER TEST"))
	static void DoRender(UTextureRenderTarget2D* RT);
};
