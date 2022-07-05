
#pragma once

#include "CoreMinimal.h"
#include "AITokenCategory.generated.h"

class UAITokenBase;
class UTokenSubscriberComponent;
class UTokenSourceComponent;
/**
 * Token categories are collection of token types. They hold the subscribers and evaluate the tokens
 * for possible obtainers on tick. Token categories live inside the UTokenSourceComponent's and cause errors
 * if constructed by another Outer (because of Within specifier on UCLASS macro).
 */
UCLASS(Abstract, Within="TokenSourceComponent", BlueprintType, EditInlineNew, DefaultToInstanced, Blueprintable)
class AITOKENSYSTEMPLUGIN_API UAITokenCategory : public UObject
{
	GENERATED_BODY()

	friend UTokenSourceComponent;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FString GetFriendlyName() const { return FriendlyName; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetInitialTokenCount() const { return InitialTokenCount; }

	/** Return a token from collection. */
	bool EvaluateTokens(const TArray<UTokenSubscriberComponent*>& Subscribers) const;

	virtual void InitializeTokenCategory();

protected:
	virtual void TickCategory(float DeltaTime);

private:
	/** Token type to populate. */
	UPROPERTY(EditDefaultsOnly, Category="Token Category")
	TSubclassOf<UAITokenBase> TokenClass;

	/* Set your friendly name for this token category. (Editor & Log usage only) */
	UPROPERTY(EditDefaultsOnly, Category="Token Category")
	FString FriendlyName;

	/* How many tokens this category should generate on initial start? */
	UPROPERTY(EditAnywhere, Category="Token Category", meta=(ClampMin=1, UIClampMin=1, UIClampMax=10))
	int32 InitialTokenCount;

	UPROPERTY(Transient, DuplicateTransient)
	TArray<UAITokenBase*> TokenCollection;

	bool bInitialized;
};
