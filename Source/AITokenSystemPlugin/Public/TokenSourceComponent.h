// Copyright INTAX Interactive, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "TokenSourceComponent.generated.h"

class UAITokenCategory;
class UTokenSubscriberComponent;
USTRUCT(BlueprintType)
struct FTokenSubscriberArrayWrapper
{
	GENERATED_BODY()

	TArray<UTokenSubscriberComponent*>& Get() { return Array; }

private:

	UPROPERTY()
	TArray<UTokenSubscriberComponent*> Array;
};

/**
 * Token Source Component, also can be called as "token pool". This component manages
 * the token categories and their lifetime. UTokenSubscriberComponent's register theirselves
 * to this "source" component's token categories and this component evaluates the tokens.
 *  (i.e. tries to find best subscriber for them)
 */
UCLASS(meta=(BlueprintSpawnableComponent))
class AITOKENSYSTEMPLUGIN_API UTokenSourceComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UTokenSourceComponent();

	UFUNCTION(BlueprintCallable)
	bool AddNewSubscriber(UTokenSubscriberComponent* Subscriber, TSubclassOf<UAITokenCategory> Category);
	UFUNCTION(BlueprintCallable)
	bool RemoveSubscriber(UTokenSubscriberComponent* Subscriber);

protected:
	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void DestroyComponent(bool bPromoteChildren) override;

private:
	UPROPERTY(Instanced, EditAnywhere, Category="Token Source Component")
	TArray<UAITokenCategory*> TokenCategories;

	UPROPERTY(VisibleAnywhere, Category="Token Source Component", meta=(ForceInlineRow))
	TMap<TSubclassOf<UAITokenCategory>, FTokenSubscriberArrayWrapper> SubscribersMap;

	UPROPERTY()
	TMap<TSubclassOf<UAITokenCategory>, UAITokenCategory*> InternalTokenCategoryMap;
	
};

