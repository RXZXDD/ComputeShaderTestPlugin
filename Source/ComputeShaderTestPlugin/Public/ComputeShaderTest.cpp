#include "ComputeShaderTest.h"

#include "MeshPassProcessor.h"
#include "RenderGraphBuilder.h"
#include "RenderGraphUtils.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Renderer/Private/ScenePrivate.h"
#include "Slate/SceneViewport.h"

IMPLEMENT_GLOBAL_SHADER(FComputeShaderTest, "/Plugin/ComputeShaderTestPlugin/Private/ComputeShaderTest.usf", "MainCS", SF_Compute);
IMPLEMENT_GLOBAL_SHADER(FComputeShaderTest2,  "/Plugin/ComputeShaderTestPlugin/Private/ComputeShaderTest2.usf", "MainCS", SF_Compute);
//IMPLEMENT_GLOBAL_SHADER(FComputeShaderTest3,  "/Plugin/ComputeShaderTestPlugin/Private/ComputeShaderTest3.usf", "MainCS", SF_Compute);


static void UseComputeShader_RenderThread(FRHICommandListImmediate& RHICmdList, FTextureRenderTargetResource* RenderTargetResource)
{
	check(IsInRenderingThread());

	//set up CS1 group dispatch 
	uint32 GGroupSize = 32;

	uint32 GroupSizeX = FMath::DivideAndRoundUp((uint32)RenderTargetResource->GetSizeX(), GGroupSize);
	uint32 GroupSizeY = FMath::DivideAndRoundUp((uint32)RenderTargetResource->GetSizeY(), GGroupSize);
	FIntVector GroupCount(GroupSizeX,GroupSizeY, 1);

	//get CS1 & CS2's shader maps
	TShaderMapRef<FComputeShaderTest> ComputeShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));
	TShaderMapRef<FComputeShaderTest2> ComputeShader2(GetGlobalShaderMap(GMaxRHIFeatureLevel));

	//declare two pointer for extract CS1 & CS2's Results
	TRefCountPtr<IPooledRenderTarget> PooledRenderTarget;
	TRefCountPtr<IPooledRenderTarget> PooledRenderTarget2;

	{
		FRDGBuilder GraphBuilder(RHICmdList);

		//RDG_EVENT_SCOPE(GraphBuilder,"MyCSPass");
		//RDG_GPU_STAT_SCOPE(GraphBuilder, MyPassGPUStatScope);
		//RDG_CSV_STAT_EXCLUSIVE_SCOPE(GraphBuilder, "MyPass");

		//Set up CS1 Parameter
		auto PassParam =  GraphBuilder.AllocParameters<FComputeShaderTest::FMyParameters>();
		FRDGTexture* RDGTex = GraphBuilder.CreateTexture(FRDGTextureDesc::Create2D(FIntPoint(RenderTargetResource->GetSizeX(),RenderTargetResource->GetSizeY()),RenderTargetResource->GetRenderTargetTexture()->GetFormat(),FClearValueBinding::White, ETextureCreateFlags::RenderTargetable|TexCreate_ShaderResource |TexCreate_UAV),
			TEXT("MyTex"));
		
		FRDGTextureUAVDesc UAVDesc(RDGTex, 0);
		FRDGTextureUAV* UAV = GraphBuilder.CreateUAV(UAVDesc);
		
		PassParam->RWOutputSurface = UAV;
		PassParam->Total = RenderTargetResource->GetSizeX();
		
		GraphBuilder.AddPass(
			RDG_EVENT_NAME("MyCSPass"),
			PassParam,
			ERDGPassFlags::Compute,
			[ComputeShader, PassParam, GroupCount, RenderTargetResource](FRHICommandList& RHICmdList)
			{
				RHICmdList.SetComputeShader(ComputeShader.GetComputeShader());
				SetShaderParameters(RHICmdList, ComputeShader, ComputeShader.GetComputeShader(), *PassParam);
				FComputeShaderUtils::Dispatch(RHICmdList, ComputeShader, *PassParam, GroupCount);
			}
		);


		//Set up CS2 Parameter
		auto PassParam2 =  GraphBuilder.AllocParameters<FComputeShaderTest2::FMyParameters>();
		//FRDGTexture* RDGTex2 = GraphBuilder.RegisterExternalTexture(PooledRenderTarget);
		FRDGTexture* RDGTex2 = GraphBuilder.CreateTexture(
										FRDGTextureDesc::Create2D(FIntPoint(RenderTargetResource->GetSizeX(),
											RenderTargetResource->GetSizeY()),
											RenderTargetResource->GetRenderTargetTexture()->GetFormat(),FClearValueBinding::White,
											ETextureCreateFlags::RenderTargetable|TexCreate_ShaderResource |TexCreate_UAV),
											TEXT("MyTex2"));

		FRDGTextureUAVDesc UAVDesc2(RDGTex2, 0);
		FRDGTextureUAV* UAV2 = GraphBuilder.CreateUAV(UAVDesc2);
		PassParam2->RWOutputSurface = UAV2;
		PassParam2->Total = RenderTargetResource->GetSizeX();
		PassParam2->Sampler = TStaticSamplerState<ESamplerFilter::SF_Bilinear>::GetRHI();
		PassParam2->InputTex = RDGTex;

		GraphBuilder.AddPass(
			RDG_EVENT_NAME("MyCSPass2"),
			PassParam2,
			ERDGPassFlags::Compute,
			[ComputeShader2, PassParam2, GroupCount, RenderTargetResource](FRHICommandList& RHICmdList)
			{
				RHICmdList.SetComputeShader(ComputeShader2.GetComputeShader());
				SetShaderParameters(RHICmdList, ComputeShader2, ComputeShader2.GetComputeShader(), *PassParam2);
				FComputeShaderUtils::Dispatch(RHICmdList, ComputeShader2, *PassParam2, GroupCount);
			}
		);

		//add extract queue
		GraphBuilder.QueueTextureExtraction(RDGTex, &PooledRenderTarget);
		GraphBuilder.QueueTextureExtraction(RDGTex2, &PooledRenderTarget2);

		//execute RDG
		GraphBuilder.Execute();

	}

	//Copy result to pass-in RT 
	//TODO Maybe a issue cause copy RT right after RDG exectue, and CS may not finish at this timing,may be have a delegate?
	RHICmdList.CopyTexture(PooledRenderTarget->GetRenderTargetItem().ShaderResourceTexture,
		RenderTargetResource->GetRenderTargetTexture() ,
		FRHICopyTextureInfo());



	RHICmdList.CopyTexture(PooledRenderTarget2->GetRenderTargetItem().ShaderResourceTexture,
	RenderTargetResource->GetRenderTargetTexture() ,
	FRHICopyTextureInfo());
	
	// //set shader parameters
	// ComputeShader->SetParameters(RHICmdList, RenderTargetTextureRHIRef, TextureUAV);
	// SetShaderParameters(RHICmdList, ComputeShader, ComputeShader.GetComputeShader(), Parameters);
	//
	// //run CS
	// DispatchComputeShader(RHICmdList, ComputeShader, GroupSizeX, GroupSizeY, 1);
	//
	// //CS's UAV copy to RenderTargetTexture
	// RHICmdList.CopyTexture(CreatedRHITexture, RenderTargetTextureRHIRef, FRHICopyTextureInfo());
}

void FComputeShaderRenderer::Render(UTextureRenderTarget2D* RenderTarget2D)
{
	if(RenderTarget2D)
	{
		FTextureRenderTargetResource* RenderTargetResource = RenderTarget2D->GameThread_GetRenderTargetResource();

		ENQUEUE_RENDER_COMMAND(ComputeShaderTestCmd)
		(
			[RenderTargetResource](FRHICommandListImmediate& RHICmdList)
			{
				UseComputeShader_RenderThread(RHICmdList, RenderTargetResource);
			}
		);
	}
}
