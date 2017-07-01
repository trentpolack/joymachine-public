// Copyright 2015-2017 Joy Machine, LLC. All rights reserved.

#include "steelhunters.h"

#include "Object.h"

#include "JsonWriter.h"
#include "JsonSerializer.h"
#include "JsonObjectConverter.h"

#include "Paths.h"
#include "FileHelper.h"

#include "JoySerializationManager.h"

// Static Definitions (class-scope).
const FName UJoySerializationManager::JSONRootFolder = TEXT( "json" );

//----------------------------------------------------------------------------------------------------
bool UJoySerializationManager::LoadJSON( FString& JSONContentsOut, const FName& SubfolderName, const FName& Filename )
{
	FName filePath = *BuildJSONPath( SubfolderName, Filename );
	return( LoadJSON( JSONContentsOut, filePath ) );
}

//----------------------------------------------------------------------------------------------------
bool UJoySerializationManager::LoadJSON( FString& JSONContentsOut, const FName& FilePath )
{
	if( !CheckFileExists( FilePath ) )
	{
		// File doesn't exist yet.
		return false;
	}

	// Build the path to the file and load it into the passed-in string.
	if( !FFileHelper::LoadFileToString( JSONContentsOut, *FilePath.ToString( ), EFileWrite::FILEWRITE_AllowRead | EFileWrite::FILEWRITE_EvenIfReadOnly | EFileWrite::FILEWRITE_NoFail ) )
	{
		UE_LOG( SteelHuntersLog_Critical, Error, TEXT( "[UJoySerializationManager] ERROR: Failed to load JSON (%s)." ), *FilePath.ToString( ) );
		return false;
	}

	UE_LOG( SteelHuntersLog, VeryVerbose, TEXT( "[UJoySerializationManager] Loaded JSON (%s)." ), *FilePath.ToString( ) );
	return true;
}

//----------------------------------------------------------------------------------------------------
bool UJoySerializationManager::SaveJSON( const FString& JSONContents, const FName& SubfolderName, const FName& Filename )
{
	FName filePath = *BuildJSONPath( SubfolderName, Filename );
	return( SaveJSON( JSONContents, filePath ) );
}

//----------------------------------------------------------------------------------------------------
bool UJoySerializationManager::SaveJSON( const FString& JSONContents, const FName& FilePath )
{
	// Take a pre-populated JSON string and save it out to a file.
	if( !FFileHelper::SaveStringToFile( JSONContents, *FilePath.ToString( ), FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get( ), EFileWrite::FILEWRITE_None ) )
	{
		UE_LOG( SteelHuntersLog_Critical, Error, TEXT( "[UJoySerializationManager] ERROR: Failed to load JSON (%s)." ), *FilePath.ToString( ) );
		return false;
	}

	UE_LOG( SteelHuntersLog, VeryVerbose, TEXT( "[UJoySerializationManager] Loaded JSON (%s)." ), *FilePath.ToString( ) );
	return true;
}

//----------------------------------------------------------------------------------------------------
template< typename T >bool UJoySerializationManager::SerializeStructToFile( const T& StructInstance, const FName& SubfolderName, const FName& Filename )
{
	FName filePath = *BuildJSONPath( SubfolderName, Filename );
	return( SerializeStructToFile< T >( StructInstance, filePath ) );
}

//----------------------------------------------------------------------------------------------------
template< typename T >bool UJoySerializationManager::SerializeStructToFile( const T& StructInstance, const FName& FilePath )
{
	FString json;
	if( !FJsonObjectConverter::UStructToJsonObjectString< T >( StructInstance, json ) )
	{
		UE_LOG( SteelHuntersLog, Error, TEXT( "[UJoySerializationManager] ERROR: Failed to serialize passed-in UStruct to JSON string (%s)." ), *FilePath.ToString( ) );
		return false;
	}
	
	// Save the JSON string to a file. Hopefully.
	if( !SaveJSON( json, FilePath ) )
	{
		UE_LOG( SteelHuntersLog_Critical, Error, TEXT( "[UJoySerializationManager] ERROR: Failed to save file from JSON during UStruct serialization (%s)." ), *FilePath.ToString( ) );
		return false;
	}

	return true;
}

//----------------------------------------------------------------------------------------------------
template< typename T > bool UJoySerializationManager::DeserializeStructFromFile( T* StructType, const FName& SubfolderName, const FName& Filename )
{
	FName filePath = *BuildJSONPath( SubfolderName, Filename );
	return( DeserializeStructFromFile< T >( StructType, filePath ) );
}

//----------------------------------------------------------------------------------------------------
template< typename T > bool UJoySerializationManager::DeserializeStructFromFile( T* StructInstance, const FName& FilePath )
{
	if( !CheckFileExists( FilePath ) )
	{
		UE_LOG( SteelHuntersLog_Critical, Error, TEXT( "[UJoySerializationManager] ERROR: File intended for deserialization does not exist (%s)." ), *FilePath.ToString( ) );
		return false;
	}

	// Save the JSON string to a file. Hopefully.
	FString json;
	if( !LoadJSON( json, FilePath ) )
	{
		UE_LOG( SteelHuntersLog_Critical, Error, TEXT( "[UJoySerializationManager] ERROR: Failed to load JSON during deserialization for UStruct (%s)." ), *FilePath.ToString( ) );
		return false;
	}

	if( !FJsonObjectConverter::JsonObjectStringToUStruct< T >( json, StructInstance, 0, CPF_Deprecated | CPF_Transient ) )
	{
		UE_LOG( SteelHuntersLog, Error, TEXT( "[UJoySerializationManager] ERROR: Failed to deserialize UStruct from JSON string (%s)." ), *FilePath.ToString( ) );
		return false;
	}


	return true;
}

//----------------------------------------------------------------------------------------------------
const FName UJoySerializationManager::GetJSONRootFolder( )
{
	return JSONRootFolder;
}

//----------------------------------------------------------------------------------------------------
FString UJoySerializationManager::BuildJSONPath( const FName& SubfolderName, const FName& Filename )
{
	return( FString::Printf( TEXT( "%s/%s/%s/%s.json" ), *FPaths::GameDir( ), *GetJSONRootFolder( ).ToString( ), *SubfolderName.ToString( ), *Filename.ToString( ) ) );
}

//----------------------------------------------------------------------------------------------------
bool UJoySerializationManager::CheckFileExists( const FName& SubfolderName, const FName& Filename )
{
	return( CheckFileExists( *BuildJSONPath( SubfolderName, Filename ) ) );
}

//----------------------------------------------------------------------------------------------------
bool UJoySerializationManager::CheckFileExists( const FName& FilePath )
{
	return( FPaths::FileExists( FilePath.ToString( ) ) );
}