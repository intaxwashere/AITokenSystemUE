
#pragma once

#include "CoreMinimal.h"

DECLARE_STATS_GROUP(TEXT("AI Token Plugin"), STATGROUP_AITokenPlugin, STATCAT_ADVANCED);

class FAITokenSystemPluginModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
