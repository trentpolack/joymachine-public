// Copyright 2015-2017 Joy Machine, LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

#include "SharedPointer.h"

/*
 *	FJoySerializationManager Class Definition.
*/
class UJoySerializationManager
{
private:
	const static FName JSONRootFolder;

public:
	/*
	 *	JSON Utilities.
	*/
	UFUNCTION( )
	static bool LoadJSON( FString& JSONContentsOut, const FName& SubfolderName, const FName& Filename );
	UFUNCTION( )
	static bool LoadJSON( FString& JSONContentsOut, const FName& FilePath );

	UFUNCTION( )
	static bool SaveJSON( const FString& JSONContents, const FName& SubfolderName, const FName& Filename );
	UFUNCTION( )
	static bool SaveJSON( const FString& JSONContents, const FName& FilePath );

	/*
	 *	Struct Serialization.
	*/
	UFUNCTION( )
	template< typename T > static bool SerializeStructToFile( const T& StructInstance, const FName& SubfolderName, const FName& Filename );
	UFUNCTION( )
	template< typename T > static bool SerializeStructToFile( const T& StructInstance, const FName& FilePath );

	/*
	 *	Struct Deserialization.
	*/
	UFUNCTION( )
	template< typename T > static bool DeserializeStructFromFile( T* StructInstance, const FName& SubfolderName, const FName& Filename );
	UFUNCTION( )
	template< typename T > static bool DeserializeStructFromFile( T* StructInstance, const FName& FilePath );

	/*
	 *	Utilities.
	*/

	// Get the root folder for all JSON files (most JSON is stored in various subfolders).
	UFUNCTION( )
	static const FName GetJSONRootFolder( );

	// Build the full JSON file path (do *not* include the extension in the file name).
	UFUNCTION( )
	static FString BuildJSONPath( const FName& SubfolderName, const FName& Filename );

	UFUNCTION( )
	static bool CheckFileExists( const FName& SubfolderName, const FName& Filename );
	UFUNCTION( )
	static bool CheckFileExists( const FName& FilePath );
};
