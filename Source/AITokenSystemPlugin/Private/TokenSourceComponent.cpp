
#include "TokenSourceComponent.h"
#include "AITokenBase.h"
#include "AITokenCategory.h"
#include "TokenSubscriberComponent.h"

UTokenSourceComponent::UTokenSourceComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.2f;

	bWantsInitializeComponent = true;
}

void UTokenSourceComponent::InitializeComponent()
{
	Super::InitializeComponent();

	for (UAITokenCategory* TokenCategory : TokenCategories)
	{
		if (IsValid(TokenCategory))
		{
			TokenCategory->InitializeTokenCategory();
			
			TSubclassOf<UAITokenCategory> Class = TokenCategory->GetClass();
			InternalTokenCategoryMap.Add(Class, TokenCategory);
		}
	}
}

void UTokenSourceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	// iterate over all subscribers map and update token states.
	for (TTuple<TSubclassOf<UAITokenCategory>, FTokenSubscriberArrayWrapper>& Elem : SubscribersMap)
	{
		TSubclassOf<UAITokenCategory> TokenClass = Elem.Key;
		TArray<UTokenSubscriberComponent*> SubscribedPawns = Elem.Value.Get();
		if (const UAITokenCategory* TokenCategory = InternalTokenCategoryMap.FindRef(TokenClass))
		{
			const bool bObtained = TokenCategory->EvaluateTokens(SubscribedPawns);
		}
	}

	// add a random deviation to the tick interval but clamp it to 0.1f
	const float CurrentInterval = PrimaryComponentTick.TickInterval;
	const float RandValue = FMath::RandRange(CurrentInterval - 0.1f, CurrentInterval + 0.1f);
	const float NewInterval = FMath::Clamp(RandValue, CurrentInterval - 0.1f, CurrentInterval + 0.1f);
	PrimaryComponentTick.TickInterval = NewInterval;
}

bool UTokenSourceComponent::AddNewSubscriber(UTokenSubscriberComponent* Subscriber, TSubclassOf<UAITokenCategory> Category)
{
	if (IsValid(Subscriber) && Category)
	{
		FTokenSubscriberArrayWrapper& ArrayWrapper = SubscribersMap.FindOrAdd(Category);
		return ArrayWrapper.Get().Add(Subscriber) != INDEX_NONE;
	}

	return false;
}

bool UTokenSourceComponent::RemoveSubscriber(UTokenSubscriberComponent* Subscriber, TSubclassOf<UAITokenCategory> FromCategory)
{
	int32 RemovedCount = 0;
	if (IsValid(Subscriber) && FromCategory)
	{
		for (TTuple<TSubclassOf<UAITokenCategory>, FTokenSubscriberArrayWrapper>& Elem : SubscribersMap)
		{
			const TSubclassOf<UAITokenCategory> CurrentCategory = Elem.Key;
			if (CurrentCategory == FromCategory)
			{
				RemovedCount += Elem.Value.Get().RemoveSwap(Subscriber);
			}
		}
	}

	return RemovedCount > 0;
}

bool UTokenSourceComponent::RemoveSubscriberFromAllCategories(UTokenSubscriberComponent* Subscriber)
{
	int32 RemovedCount = 0;
	if (IsValid(Subscriber))
	{
		for (TTuple<TSubclassOf<UAITokenCategory>, FTokenSubscriberArrayWrapper>& Elem : SubscribersMap)
		{
			RemovedCount += Elem.Value.Get().RemoveSwap(Subscriber);
		}
	}

	return RemovedCount > 0;
}

void UTokenSourceComponent::DestroyComponent(bool bPromoteChildren)
{
	for (const TTuple<TSubclassOf<UAITokenCategory>, UAITokenCategory*> Elem : InternalTokenCategoryMap)
	{
		if (UAITokenCategory* TokenCategory = Elem.Value)
		{
			for (UAITokenBase* Token : TokenCategory->TokenCollection)
			{
				Token->ForceToRelease();
			}
		}
	}
	
	Super::DestroyComponent(bPromoteChildren);
}
