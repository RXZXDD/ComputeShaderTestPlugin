// Fill out your copyright notice in the Description page of Project Settings.


#include "ComputeShaderTestFunctionLibrary.h"
#include "ComputeShaderTest.h"

void UComputeShaderTestFunctionLibrary::DoRender(UTextureRenderTarget2D* RT)
{
	FComputeShaderRenderer Renderer;
	Renderer.Render(RT);
}
