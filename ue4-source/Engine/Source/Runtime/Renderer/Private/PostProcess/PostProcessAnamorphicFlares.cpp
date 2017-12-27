// Copyright 2015-2017 Joy Machine, LLC. All rights reserved.

#include "PostProcess/PostProcessAnamorphicFlares.h"
#include "StaticBoundShaderState.h"
#include "SceneUtils.h"

#include "PostProcess/SceneRenderTargets.h"
#include "PostProcess/SceneFilterRendering.h"
#include "PostProcess/PostProcessing.h"
#include "PostProcess/PostProcessEyeAdaptation.h"

#include "ClearQuad.h"
#include "PipelineStateCache.h"

const int32 GAnamorphicFlaresComputeTileSizeX = 8;
const int32 GAnamorphicFlaresComputeTileSizeY = 8;

// FPostProcessAnamorphicFlaresPS Class Definition.
class FPostProcessAnamorphicFlaresPS : public FGlobalShader
{
	DECLARE_SHADER_TYPE( FPostProcessAnamorphicFlaresPS, Global );

	//------------------------------------------------------------------------------------------------
	static bool ShouldCache( EShaderPlatform Platform )
	{
		return( IsFeatureLevelSupported( Platform, ERHIFeatureLevel::SM4 ) );
	}

	//------------------------------------------------------------------------------------------------
	static void ModifyCompilationEnvironment( EShaderPlatform Platform, FShaderCompilerEnvironment& OutEnvironment )
	{
		FGlobalShader::ModifyCompilationEnvironment( Platform, OutEnvironment );

		if( !IsFeatureLevelSupported( Platform, ERHIFeatureLevel::SM5 ) )
		{
			//Need to hack in exposure scale for < SM5
			OutEnvironment.SetDefine( TEXT( "NO_EYEADAPTATION_EXPOSURE_FIX" ), 1 );
		}
	}

	//------------------------------------------------------------------------------------------------
	FPostProcessAnamorphicFlaresPS( ) { }

public:
	FPostProcessPassParameters PostprocessParameter;

	FShaderParameter LensTintColor;
	FShaderParameter LensPackedData_01;
	FShaderParameter LensPackedData_02;

	//------------------------------------------------------------------------------------------------
	FPostProcessAnamorphicFlaresPS( const ShaderMetaType::CompiledShaderInitializerType& Initializer )
	: FGlobalShader( Initializer )
	{
		PostprocessParameter.Bind( Initializer.ParameterMap );

		LensTintColor.Bind( Initializer.ParameterMap, TEXT( "LensTintColorPS" ) );
		LensPackedData_01.Bind( Initializer.ParameterMap, TEXT( "LensPackedDataPS_01" ) );
		LensPackedData_02.Bind( Initializer.ParameterMap, TEXT( "LensPackedDataPS_02" ) );
	}

	//------------------------------------------------------------------------------------------------
	template< typename TRHICmdList >
	void SetPS( TRHICmdList& RHICmdList, const FRenderingCompositePassContext& Context )
	{
		const FPixelShaderRHIParamRef ShaderRHI = GetPixelShader( );
		const FPostProcessSettings& Settings = Context.View.FinalPostProcessSettings;

		FGlobalShader::SetParameters< FViewUniformShaderParameters >( RHICmdList, ShaderRHI, Context.View.ViewUniformBuffer );

		PostprocessParameter.SetPS( RHICmdList, ShaderRHI, Context, TStaticSamplerState< SF_Bilinear, AM_Clamp, AM_Clamp, AM_Clamp >::GetRHI( ) );

		// Set the shader parameters.
		float ExposureScale = FRCPassPostProcessEyeAdaptation::ComputeExposureScaleValue( Context.View );

		FLinearColor valueTintColor( Settings.AnamorphicFlaresTint );

		FVector4 valuesLensPackedData_01( Settings.AnamorphicFlaresLuminanceBase, Settings.AnamorphicFlaresThreshold, Settings.AnamorphicFlaresIntensity, ExposureScale*Settings.AnamorphicFlaresExposureModifier );
		FVector4 valuesLensPackedData_02( Settings.AnamorphicFlaresSampleCount, Settings.AnamorphicFlaresLength, 0.0f, 0.0f );

		SetShaderValue( RHICmdList, ShaderRHI, LensTintColor, valueTintColor );
		SetShaderValue( RHICmdList, ShaderRHI, LensPackedData_01, valuesLensPackedData_01 );
		SetShaderValue( RHICmdList, ShaderRHI, LensPackedData_02, valuesLensPackedData_02 );
	}

	/*
	 *	FShader Interface Methods.
	*/

	//------------------------------------------------------------------------------------------------
	virtual bool Serialize( FArchive& Ar ) override
	{
		bool bShaderHasOutdatedParameters = FGlobalShader::Serialize( Ar );

		Ar << PostprocessParameter << LensTintColor << LensPackedData_01 << LensPackedData_02;
		return bShaderHasOutdatedParameters;
	}
};

IMPLEMENT_SHADER_TYPE( , FPostProcessAnamorphicFlaresPS, TEXT( "/Engine/Private/PostProcessAnamorphicFlares.usf" ), TEXT( "MainPS" ), SF_Pixel );


/** Encapsulates the post processing bloom setup vertex shader. */
class FPostProcessAnamorphicFlaresVS : public FGlobalShader
{
	DECLARE_SHADER_TYPE( FPostProcessAnamorphicFlaresVS, Global );

	//------------------------------------------------------------------------------------------------
	static bool ShouldCache( EShaderPlatform Platform )
	{
		return( IsFeatureLevelSupported( Platform, ERHIFeatureLevel::SM4 ) );
	}

	//------------------------------------------------------------------------------------------------
	FPostProcessAnamorphicFlaresVS( ) { }

public:
	FPostProcessPassParameters PostprocessParameter;

	FShaderResourceParameter EyeAdaptation;

	//------------------------------------------------------------------------------------------------
	FPostProcessAnamorphicFlaresVS( const ShaderMetaType::CompiledShaderInitializerType& Initializer )
	: FGlobalShader( Initializer )
	{
		PostprocessParameter.Bind( Initializer.ParameterMap );

		EyeAdaptation.Bind( Initializer.ParameterMap, TEXT( "EyeAdaptation" ) );
	}

	//------------------------------------------------------------------------------------------------
	void SetVS( const FRenderingCompositePassContext& Context )
	{
		const FVertexShaderRHIParamRef ShaderRHI = GetVertexShader( );

		FGlobalShader::SetParameters<FViewUniformShaderParameters>( Context.RHICmdList, ShaderRHI, Context.View.ViewUniformBuffer );

		PostprocessParameter.SetVS( ShaderRHI, Context, TStaticSamplerState< SF_Bilinear, AM_Clamp, AM_Clamp, AM_Clamp >::GetRHI( ) );

		if( EyeAdaptation.IsBound( ) )
		{
			if( Context.View.HasValidEyeAdaptation( ) )
			{
				IPooledRenderTarget* EyeAdaptationRT = Context.View.GetEyeAdaptation( Context.RHICmdList );
				SetTextureParameter( Context.RHICmdList, ShaderRHI, EyeAdaptation, EyeAdaptationRT->GetRenderTargetItem( ).TargetableTexture );
			}
			else
			{
				SetTextureParameter( Context.RHICmdList, ShaderRHI, EyeAdaptation, GWhiteTexture->TextureRHI );
			}
		}
	}

	/*
	 *	FShader Interface Methods.
	*/

	//------------------------------------------------------------------------------------------------
	virtual bool Serialize( FArchive& Ar ) override
	{
		bool bShaderHasOutdatedParameters = FGlobalShader::Serialize( Ar );

		Ar << PostprocessParameter << EyeAdaptation;
		return bShaderHasOutdatedParameters;
	}
};

IMPLEMENT_SHADER_TYPE( , FPostProcessAnamorphicFlaresVS, TEXT( "/Engine/Private/PostProcessAnamorphicFlares.usf" ), TEXT( "MainVS" ), SF_Vertex );

/** Encapsulates the post processing bloom threshold compute shader. */
class FPostProcessAnamorphicFlaresCS : public FGlobalShader
{
	DECLARE_SHADER_TYPE( FPostProcessAnamorphicFlaresCS, Global );

	//------------------------------------------------------------------------------------------------
	static bool ShouldCache( EShaderPlatform Platform )
	{
		return( IsFeatureLevelSupported( Platform, ERHIFeatureLevel::SM5 ) );
	}

	//------------------------------------------------------------------------------------------------
	static void ModifyCompilationEnvironment( EShaderPlatform Platform, FShaderCompilerEnvironment& OutEnvironment )
	{
		FGlobalShader::ModifyCompilationEnvironment( Platform, OutEnvironment );

		OutEnvironment.SetDefine( TEXT( "THREADGROUP_SIZEX" ), GAnamorphicFlaresComputeTileSizeX );
		OutEnvironment.SetDefine( TEXT( "THREADGROUP_SIZEY" ), GAnamorphicFlaresComputeTileSizeY );
	}

	//------------------------------------------------------------------------------------------------
	FPostProcessAnamorphicFlaresCS( ) { }

public:
	FPostProcessPassParameters PostprocessParameter;

	FShaderResourceParameter EyeAdaptation;

	FShaderParameter LensTintColor;
	FShaderParameter LensPackedData_01;
	FShaderParameter LensPackedData_02;

	FShaderParameter OutComputeTex;

	//------------------------------------------------------------------------------------------------
	FPostProcessAnamorphicFlaresCS( const ShaderMetaType::CompiledShaderInitializerType& Initializer )
	: FGlobalShader( Initializer )
	{
		PostprocessParameter.Bind( Initializer.ParameterMap );

		EyeAdaptation.Bind( Initializer.ParameterMap, TEXT( "EyeAdaptation" ) );

		LensTintColor.Bind( Initializer.ParameterMap, TEXT( "LensTintColorCS" ) );
		LensPackedData_01.Bind( Initializer.ParameterMap, TEXT( "LensPackedDataCS_01" ) );
		LensPackedData_02.Bind( Initializer.ParameterMap, TEXT( "LensPackedDataCS_02" ) );

		OutComputeTex.Bind( Initializer.ParameterMap, TEXT( "OutComputeTex" ) );
	}

	//------------------------------------------------------------------------------------------------
	template< typename TRHICmdList >
	void SetParameters( TRHICmdList& RHICmdList, const FRenderingCompositePassContext& Context, const FIntPoint& DestSize, FUnorderedAccessViewRHIParamRef DestUAV, FTextureRHIParamRef EyeAdaptationTex )
	{
		const FComputeShaderRHIParamRef ShaderRHI = GetComputeShader( );
		const FPostProcessSettings& Settings = Context.View.FinalPostProcessSettings;

		FGlobalShader::SetParameters< FViewUniformShaderParameters >( RHICmdList, ShaderRHI, Context.View.ViewUniformBuffer );
		PostprocessParameter.SetCS( ShaderRHI, Context, RHICmdList, TStaticSamplerState< SF_Bilinear, AM_Clamp, AM_Clamp, AM_Clamp >::GetRHI( ) );
		RHICmdList.SetUAVParameter( ShaderRHI, OutComputeTex.GetBaseIndex( ), DestUAV );

		SetTextureParameter( RHICmdList, ShaderRHI, EyeAdaptation, EyeAdaptationTex );

		// Set the shader parameters.
		FLinearColor valueTintColor( Settings.AnamorphicFlaresTint );

		FVector4 valuesLensPackedData_01( Settings.AnamorphicFlaresLuminanceBase, Settings.AnamorphicFlaresThreshold, Settings.AnamorphicFlaresIntensity, Settings.AnamorphicFlaresExposureModifier );
		FVector4 valuesLensPackedData_02( Settings.AnamorphicFlaresSampleCount, Settings.AnamorphicFlaresLength, ( 1.0f/( float )DestSize.X ), 1.0f/( ( float )DestSize.Y ) );

		SetShaderValue( RHICmdList, ShaderRHI, LensTintColor, valueTintColor );
		SetShaderValue( RHICmdList, ShaderRHI, LensPackedData_01, valuesLensPackedData_01 );
		SetShaderValue( RHICmdList, ShaderRHI, LensPackedData_02, valuesLensPackedData_02 );
	}

	//------------------------------------------------------------------------------------------------
	template< typename TRHICmdList >
	void UnsetParameters( TRHICmdList& RHICmdList )
	{
		const FComputeShaderRHIParamRef ShaderRHI = GetComputeShader( );
		RHICmdList.SetUAVParameter( ShaderRHI, OutComputeTex.GetBaseIndex( ), NULL );
	}

	/*
	 *	FShader Interface Methods.
	*/

	//------------------------------------------------------------------------------------------------
	virtual bool Serialize( FArchive& Ar ) override
	{
		bool bShaderHasOutdatedParameters = FGlobalShader::Serialize( Ar );

		Ar << PostprocessParameter << EyeAdaptation << LensTintColor << LensPackedData_01 << LensPackedData_02 << OutComputeTex;
		return bShaderHasOutdatedParameters;
	}
};

IMPLEMENT_SHADER_TYPE( , FPostProcessAnamorphicFlaresCS, TEXT( "/Engine/Private/PostProcessAnamorphicFlares.usf" ), TEXT( "MainCS" ), SF_Compute );

//----------------------------------------------------------------------------------------------------
void FRCPassPostProcessAnamorphicFlares::Process( FRenderingCompositePassContext& Context )
{
	const FPooledRenderTargetDesc* InputDesc = GetInputDesc( ePId_Input0 );
	AsyncEndFence = FComputeFenceRHIRef( );

	if( !InputDesc )
	{
		// input is not hooked up correctly
		return;
	}

	const FSceneView& View = Context.View;
	const FSceneViewFamily& ViewFamily = *( View.Family );

	FIntPoint SrcSize = InputDesc->Extent;
	FIntPoint DestSize = PassOutputs[0].RenderTargetDesc.Extent;

	// e.g. 4 means the input texture is 4x smaller than the buffer size
	uint32 ScaleFactor = FMath::DivideAndRoundUp( FSceneRenderTargets::Get( Context.RHICmdList ).GetBufferSizeXY( ).Y, SrcSize.Y );

	FIntRect SrcRect = View.ViewRect / ScaleFactor;
	FIntRect DestRect = SrcRect;

	SCOPED_DRAW_EVENTF( Context.RHICmdList, PostProcessAnamorphicFlares, TEXT( "PostProcessAnamorphicFlares%s_%dx%d" ), bIsComputePass ? TEXT( "Compute" ) : TEXT( "" ), DestRect.Width( ), DestRect.Height( ) );

	const FSceneRenderTargetItem& DestRenderTarget = PassOutputs[0].RequestSurface( Context );

	if( bIsComputePass )
	{
		DestRect = { View.ViewRect.Min, View.ViewRect.Min + DestSize };

		// Common setup
		SetRenderTarget( Context.RHICmdList, nullptr, nullptr );
		Context.SetViewportAndCallRHI( DestRect, 0.0f, 1.0f );

		static FName AsyncEndFenceName( TEXT( "AsyncAnamorphicFlaresEndFence" ) );
		AsyncEndFence = Context.RHICmdList.CreateComputeFence( AsyncEndFenceName );

		FTextureRHIRef EyeAdaptationTex = GWhiteTexture->TextureRHI;
		if( Context.View.HasValidEyeAdaptation( ) )
		{
			EyeAdaptationTex = Context.View.GetEyeAdaptation( Context.RHICmdList )->GetRenderTargetItem( ).TargetableTexture;
		}

		if( IsAsyncComputePass( ) )
		{
			// Async path
			FRHIAsyncComputeCommandListImmediate& RHICmdListComputeImmediate = FRHICommandListExecutor::GetImmediateAsyncComputeCommandList( );
			{
				SCOPED_COMPUTE_EVENT( RHICmdListComputeImmediate, AsyncAnamorphicFlares );
				WaitForInputPassComputeFences( RHICmdListComputeImmediate );

				RHICmdListComputeImmediate.TransitionResource( EResourceTransitionAccess::ERWBarrier, EResourceTransitionPipeline::EGfxToCompute, DestRenderTarget.UAV );
				DispatchCS( RHICmdListComputeImmediate, Context, DestRect, DestRenderTarget.UAV, EyeAdaptationTex );
				RHICmdListComputeImmediate.TransitionResource( EResourceTransitionAccess::EReadable, EResourceTransitionPipeline::EComputeToGfx, DestRenderTarget.UAV, AsyncEndFence );
			}
			FRHIAsyncComputeCommandListImmediate::ImmediateDispatch( RHICmdListComputeImmediate );
		}
		else
		{
			// Direct path
			WaitForInputPassComputeFences( Context.RHICmdList );

			Context.RHICmdList.TransitionResource( EResourceTransitionAccess::ERWBarrier, EResourceTransitionPipeline::EGfxToCompute, DestRenderTarget.UAV );
			DispatchCS( Context.RHICmdList, Context, DestRect, DestRenderTarget.UAV, EyeAdaptationTex );
			Context.RHICmdList.TransitionResource( EResourceTransitionAccess::EReadable, EResourceTransitionPipeline::EComputeToGfx, DestRenderTarget.UAV, AsyncEndFence );
		}
	}
	else
	{
		WaitForInputPassComputeFences( Context.RHICmdList );

		// Set the view family's render target/viewport.
		SetRenderTarget( Context.RHICmdList, DestRenderTarget.TargetableTexture, FTextureRHIRef( ) );

		// is optimized away if possible (RT size=view size, )
		DrawClearQuad( Context.RHICmdList, true, FLinearColor::Black, false, 0, false, 0, DestSize, DestRect );

		Context.SetViewportAndCallRHI( 0, 0, 0.0f, DestSize.X, DestSize.Y, 1.0f );

		FGraphicsPipelineStateInitializer GraphicsPSOInit;
		Context.RHICmdList.ApplyCachedRenderTargets( GraphicsPSOInit );

		// set the state
		GraphicsPSOInit.BlendState = TStaticBlendState< >::GetRHI( );
		GraphicsPSOInit.RasterizerState = TStaticRasterizerState< >::GetRHI( );
		GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState< false, CF_Always >::GetRHI( );

		TShaderMapRef< FPostProcessAnamorphicFlaresVS > VertexShader( Context.GetShaderMap( ) );
		TShaderMapRef< FPostProcessAnamorphicFlaresPS > PixelShader( Context.GetShaderMap( ) );

		GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = GFilterVertexDeclaration.VertexDeclarationRHI;
		GraphicsPSOInit.BoundShaderState.VertexShaderRHI = GETSAFERHISHADER_VERTEX( *VertexShader );
		GraphicsPSOInit.BoundShaderState.PixelShaderRHI = GETSAFERHISHADER_PIXEL( *PixelShader );
		GraphicsPSOInit.PrimitiveType = PT_TriangleList;

		SetGraphicsPipelineState( Context.RHICmdList, GraphicsPSOInit );

		VertexShader->SetVS( Context );
		PixelShader->SetPS( Context.RHICmdList, Context );

		DrawPostProcessPass( Context.RHICmdList,
							 DestRect.Min.X, DestRect.Min.Y,
							 DestRect.Width( ), DestRect.Height( ),
							 SrcRect.Min.X, SrcRect.Min.Y,
							 SrcRect.Width( ), SrcRect.Height( ),
							 DestSize, SrcSize,
							 *VertexShader,
							 View.StereoPass,
							 false,					// Disabled for correctness
							 EDRF_UseTriangleOptimization );

		Context.RHICmdList.CopyToResolveTarget( DestRenderTarget.TargetableTexture, DestRenderTarget.ShaderResourceTexture, false, FResolveParams( ) );
	}
}

//----------------------------------------------------------------------------------------------------
template< typename TRHICmdList >
void FRCPassPostProcessAnamorphicFlares::DispatchCS( TRHICmdList& RHICmdList, FRenderingCompositePassContext& Context, const FIntRect& DestRect, FUnorderedAccessViewRHIParamRef DestUAV, FTextureRHIParamRef EyeAdaptationTex )
{
	auto ShaderMap = Context.GetShaderMap( );
	TShaderMapRef< FPostProcessAnamorphicFlaresCS > ComputeShader( ShaderMap );
	RHICmdList.SetComputeShader( ComputeShader->GetComputeShader( ) );

	FIntPoint DestSize( DestRect.Width( ), DestRect.Height( ) );
	ComputeShader->SetParameters( RHICmdList, Context, DestSize, DestUAV, EyeAdaptationTex );

	uint32 GroupSizeX = FMath::DivideAndRoundUp( DestSize.X, GAnamorphicFlaresComputeTileSizeX );
	uint32 GroupSizeY = FMath::DivideAndRoundUp( DestSize.Y, GAnamorphicFlaresComputeTileSizeY );
	DispatchComputeShader( RHICmdList, *ComputeShader, GroupSizeX, GroupSizeY, 1 );

	ComputeShader->UnsetParameters( RHICmdList );
}

//----------------------------------------------------------------------------------------------------
FPooledRenderTargetDesc FRCPassPostProcessAnamorphicFlares::ComputeOutputDesc( EPassOutputId InPassOutputId ) const
{
	FPooledRenderTargetDesc Ret = GetInput( ePId_Input0 )->GetOutput( )->RenderTargetDesc;

	Ret.Reset( );
	Ret.DebugName = TEXT( "AnamorphicFlares" );
	Ret.TargetableFlags &= ~( TexCreate_RenderTargetable | TexCreate_UAV );
	Ret.TargetableFlags |= bIsComputePass ? TexCreate_UAV : TexCreate_RenderTargetable;
	Ret.AutoWritable = false;
	return Ret;
}