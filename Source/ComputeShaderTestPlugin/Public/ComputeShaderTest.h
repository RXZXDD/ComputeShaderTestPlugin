#pragma once

#include "CoreMinimal.h"
#include "GlobalShader.h"
#include "MaterialShader.h"
#include "ShaderParameterStruct.h"

class FComputeShaderTest : public FGlobalShader
{

	DECLARE_GLOBAL_SHADER(FComputeShaderTest);
	SHADER_USE_PARAMETER_STRUCT(FComputeShaderTest, FGlobalShader);

	BEGIN_SHADER_PARAMETER_STRUCT(FMyParameters,)
		SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D, RWOutputSurface)
		SHADER_PARAMETER(float, Total)
	END_SHADER_PARAMETER_STRUCT()

	using FParameters = FMyParameters;
	

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
	}
	static void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
	{
		FGlobalShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);
		OutEnvironment.SetDefine(TEXT("THREADGROUP_SIZE_X"), 32);
		OutEnvironment.SetDefine(TEXT("THREADGROUP_SIZE_Y"), 32);
		OutEnvironment.SetDefine(TEXT("THREADGROUP_SIZE_Z"), 1);
	}
	
	//  FComputeShaderTest() {}
	//  FComputeShaderTest(const ShaderMetaType::CompiledShaderInitializerType& Initializer) : FGlobalShader(Initializer)
	// {
	// 	//OutputSureface.Bind(Initializer.ParameterMap, TEXT("OutputSurface"));
	// }
	//
	// void SetParameters(
	// 	FRHICommandList& RHICmdList,
	// 	FTexture2DRHIRef& InOutputSurfaceValue,
	// 	FUnorderedAccessViewRHIRef& UAV
	// 	)
	// {
	// 	FRHIComputeShader* ShaderRHI = RHICmdList.GetBoundComputeShader();
	// 	OutputSureface.SetTexture(RHICmdList, ShaderRHI, InOutputSurfaceValue,UAV);
	// }

// private:
// 	LAYOUT_FIELD(FRWShaderParameter, OutputSureface);
};



class FComputeShaderTest2 : public FGlobalShader
{

	DECLARE_GLOBAL_SHADER(FComputeShaderTest2);
	SHADER_USE_PARAMETER_STRUCT(FComputeShaderTest2, FGlobalShader);

	BEGIN_SHADER_PARAMETER_STRUCT(FMyParameters,)
		SHADER_PARAMETER_RDG_TEXTURE(Texture2D, InputTex)
		SHADER_PARAMETER_SAMPLER(SamplerState, Sampler)
		SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D, RWOutputSurface)
		SHADER_PARAMETER(float, Total)
	END_SHADER_PARAMETER_STRUCT()

	using FParameters = FMyParameters;
	

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
	}
	static void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
	{
		FGlobalShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);
		OutEnvironment.SetDefine(TEXT("THREADGROUP_SIZE_X"), 32);
		OutEnvironment.SetDefine(TEXT("THREADGROUP_SIZE_Y"), 32);
		OutEnvironment.SetDefine(TEXT("THREADGROUP_SIZE_Z"), 1);
	}
};

//
// class FComputeShaderTest3 : public FMaterialShader
// {
//
// 	DECLARE_SHADER_TYPE(FComputeShaderTest3, MaterialShader);
// 	SHADER_USE_PARAMETER_STRUCT(FComputeShaderTest3, FGlobalShader);
//
// 	BEGIN_SHADER_PARAMETER_STRUCT(FMyParameters,)
// 		SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D, RWOutputSurface)
// 	END_SHADER_PARAMETER_STRUCT()
//
// 	using FParameters = FMyParameters;
// 	
//
// 	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
// 	{
// 		return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
// 	}
// 	static void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
// 	{
// 		FGlobalShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);
// 		OutEnvironment.SetDefine(TEXT("THREADGROUP_SIZE_X"), 32);
// 		OutEnvironment.SetDefine(TEXT("THREADGROUP_SIZE_Y"), 32);
// 		OutEnvironment.SetDefine(TEXT("THREADGROUP_SIZE_Z"), 1);
// 	}
// 	
// 	//  FComputeShaderTest() {}
// 	//  FComputeShaderTest(const ShaderMetaType::CompiledShaderInitializerType& Initializer) : FGlobalShader(Initializer)
// 	// {
// 	// 	//OutputSureface.Bind(Initializer.ParameterMap, TEXT("OutputSurface"));
// 	// }
// 	//
// 	// void SetParameters(
// 	// 	FRHICommandList& RHICmdList,
// 	// 	FTexture2DRHIRef& InOutputSurfaceValue,
// 	// 	FUnorderedAccessViewRHIRef& UAV
// 	// 	)
// 	// {
// 	// 	FRHIComputeShader* ShaderRHI = RHICmdList.GetBoundComputeShader();
// 	// 	OutputSureface.SetTexture(RHICmdList, ShaderRHI, InOutputSurfaceValue,UAV);
// 	// }
//
// 	// private:
// 	// 	LAYOUT_FIELD(FRWShaderParameter, OutputSureface);
// };


class COMPUTESHADERTESTPLUGIN_API FComputeShaderRenderer
{
public:
	FComputeShaderRenderer() {};

	void Render(class UTextureRenderTarget2D* RenderTarget2D);
};