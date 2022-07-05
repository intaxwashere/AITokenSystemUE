// Copyright Epic Games, Inc. All Rights Reserved.

#include "AITokenSystemPlugin.h"

DECLARE_CYCLE_STAT(TEXT("Token Obtain Time"), STAT_TokenObtainTime, STATGROUP_AITokenPlugin);
DECLARE_CYCLE_STAT(TEXT("Find Token From Collection Time"), STAT_TokenFindTime, STATGROUP_AITokenPlugin);

#define LOCTEXT_NAMESPACE "FAITokenSystemPluginModule"

void FAITokenSystemPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FAITokenSystemPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAITokenSystemPluginModule, AITokenSystemPlugin)