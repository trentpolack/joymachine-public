// Copyright 2015-2017 Joy Machine, LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "RendererInterface.h"

#include "PostProcess/RenderingCompositionGraph.h"

// FRCPassPostProcessAnamorphicFlares Class Definition.
//
//	ePId_Input0: Half res HDR scene color
//	ePId_Input1: EyeAdaptation
class FRCPassPostProcessAnamorphicFlares : public TRenderingCompositePassBase< 2, 1 >
{
public:
	FRCPassPostProcessAnamorphicFlares( bool bInIsComputePass )
	{
		bIsComputePass = bInIsComputePass;
		bPreferAsyncCompute = false;
	}

private:
	template< typename TRHICmdList >
	void DispatchCS( TRHICmdList& RHICmdList, FRenderingCompositePassContext& Context, const FIntRect& DestRect, FUnorderedAccessViewRHIParamRef DestUAV, FTextureRHIParamRef EyeAdaptationTex );

	FComputeFenceRHIRef AsyncEndFence;

public:
	/*
	 *	FRenderingCompositePass Interface Methods.
	*/
	virtual void Process( FRenderingCompositePassContext& Context ) override;
	virtual FPooledRenderTargetDesc ComputeOutputDesc( EPassOutputId InPassOutputId ) const override;
	virtual void Release( ) override { delete this; }

	virtual FComputeFenceRHIParamRef GetComputePassEndFence( ) const override { return AsyncEndFence; }
};