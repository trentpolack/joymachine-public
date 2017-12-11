// Copyright 2015-2017 Joy Machine, LLC. All rights reserved.

#include "steelhunters.h"

#include "IObjectPooled.h"

#include "ObjectPool.h"

//----------------------------------------------------------------------------------------------------
bool IObjectPooled::GetIsActive( ) const
{
	return IsActive;
}

//----------------------------------------------------------------------------------------------------
void IObjectPooled::Deactivate( )
{
	ReturnToPool.Execute( this );
}