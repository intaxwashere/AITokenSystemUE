
#pragma once

#include "CoreMinimal.h"
#include "AITokenBase.generated.h"

class UTokenSubscriberComponent;
UCLASS(Abstract, BlueprintType, Blueprintable)
class AITOKENSYSTEMPLUGIN_API UAITokenBase : public UObject
{
	GENERATED_BODY()

	friend class UAITokenCategory;

protected:
	/** @todo this function requires BP assets to run their own for loop. It might be better to
	 * implement for loop in C++ and just call "ScoreSubscriber()" function that only scores single element for performance.
	 * but on the other hand, this will require us to create a "scoring" system specific to this plugin, which is also bad.
	 * so it's best to just leave TryToGetToken as virtual and move on.
	 * ---------------------------------------------------------------------------------------------------------------------------
	 * Select best subscriber to own this token from given array. This is an abstract function that lets developers implement their own desired logic. */
	UFUNCTION(BlueprintNativeEvent)
	UTokenSubscriberComponent* SelectBestSubscriber(const TArray<UTokenSubscriberComponent*>& PossibleSubscribers) const;

	/** Called when this token is obtained by a subscriber. */
	UFUNCTION(BlueprintNativeEvent)
	void OnUsed();

	/** Called when this token is released by a subscriber */
	UFUNCTION(BlueprintNativeEvent)
	void OnReleased();

	UFUNCTION(BlueprintPure)
	UTokenSubscriberComponent* GetCurrentOwner() const;

	bool IsSingleInstancePerSubscriber() const;

private:
	void Use();
	void Release();
	void SetOwner(UTokenSubscriberComponent* NewOwner) { Owner = NewOwner; }
	bool HasCooldown() const { return bCooldownActive; }
	bool IsObtained() const { return bObtained; }
	void RemoveCooldown();
	
public:
	/** Force this token to release back to pool. You usually only need to use this when your AI pawn died, BehaviorTree changed etc.
	 * TryObtainToken service can not check those conditions so when you need to manually handle the releasing, use this.
	 * Do not forget to invalidate your token reference after you called this. */
	UFUNCTION(BlueprintCallable)
	void ForceToRelease();

private:
	UPROPERTY(Transient, DuplicateTransient)
	UTokenSubscriberComponent* Owner;

	/** Whether this token "type" can ever get obtained multiple times by same subscriber or not.
	  * Normally this concept is not a thing around industry and always 'false'.
	  * But this can be converted to a gameplay mechanic if extended. */
	UPROPERTY(EditAnywhere, Category="Token")
	bool bSingleInstancePerSubscriber;

	/** If we should add a cooldown effect to token after used? */
	UPROPERTY(EditAnywhere, Category="Token")
	bool bTokenHasCooldown;

	UPROPERTY(EditAnywhere, Category="Token", meta=(EditCondition="bTokenHasCooldown", ClampMin=0.1, UIMin=0.1))
	float CooldownTime;
	
	bool bCooldownActive;
	bool bObtained;

	FTimerHandle TimerHandle_UseToken;
};
