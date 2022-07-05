// Copyright INTAX Interactive, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "TokenSubscriberComponent.generated.h"

class UAITokenBase;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTokenSubscriberEvent, const UAITokenBase*, Token);

/**
 * Token subscriber component that registers itself to token source component(s). Subscriber components
 * only acts like a container of obtained tokens and provide info about their state. They do not determine
 * if tokens can or should be obtained or not. They subscribe to a source component or request removal of their
 * subscription.
 */
UCLASS(BlueprintType, meta=(BlueprintSpawnableComponent))
class AITOKENSYSTEMPLUGIN_API UTokenSubscriberComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTokenSubscriberComponent();

	virtual void OnTokenObtained(UAITokenBase* Token);

	virtual void OnTokenReleased(UAITokenBase* Token);

	UFUNCTION(BlueprintCallable)
	bool ReleaseToken(TSubclassOf<UAITokenBase> TokenType);

	/** Returns true if we have a token of given type. Will perform a lookup on ObtainedTokens property. */
	UFUNCTION(BlueprintPure)
	bool HasTokenOfType(TSubclassOf<UAITokenBase> TokenType) const;

	/** Broadcasted when any token obtained. */
	UPROPERTY(BlueprintAssignable)
	FTokenSubscriberEvent OnTokenObtainedEvent;

	/** Broadcasted when any token we obtained released. */
	UPROPERTY(BlueprintAssignable)
	FTokenSubscriberEvent OnTokenReleasedEvent;
private:
	/** Tokens that this subscriber component currently obtained. */
	UPROPERTY(VisibleAnywhere, Category="Token Subscriber Component")
	TArray<UAITokenBase*> ObtainedTokens;
};
