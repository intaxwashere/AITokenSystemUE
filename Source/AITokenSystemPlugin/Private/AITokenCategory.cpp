
#include "AITokenCategory.h"
#include "AITokenBase.h"
#include "TokenSubscriberComponent.h"
#include "AITokenSystemPlugin.h"
#include "VisualLogger/VisualLogger.h"

DECLARE_CYCLE_STAT(TEXT("Token Evaluate Time"), STAT_TokenEvaluateTime, STATGROUP_AITokenPlugin);

bool UAITokenCategory::EvaluateTokens(const TArray<UTokenSubscriberComponent*>& Subscribers) const
{
	SCOPE_CYCLE_COUNTER(STAT_TokenEvaluateTime);
	
	if (!bInitialized)
	{
		return false;
	}
	
	for (UAITokenBase* Token : TokenCollection)
	{
		// only evaluate the token if it's not obtained or on cooldown
		if (IsValid(Token) && !Token->HasCooldown() && !Token->IsObtained())
		{
			// filter subscribers - only evaluate the subscribers that does not already obtained this type of a token.
			// @todo i think this might be better, somehow... currently I have doubts about quality of this implementation
			// but I also have no idea how can it be better than it is. 
			const bool bSingleInstancePerSubscriber = Token->IsSingleInstancePerSubscriber();
			TArray<UTokenSubscriberComponent*> FilteredSubscribers;
			if (bSingleInstancePerSubscriber)
			{
				FilteredSubscribers.Reserve(5);
				for (UTokenSubscriberComponent* Subscriber : Subscribers)
				{
					if (IsValid(Subscriber) && !Subscriber->HasTokenOfType(Token->GetClass()))
					{
						FilteredSubscribers.Add(Subscriber);
					}
				}
			}
			
			// let token select best subscriber based on it's own desire and rules.
			UTokenSubscriberComponent* SelectedSubscriber = Token->SelectBestSubscriber(bSingleInstancePerSubscriber ? FilteredSubscribers : Subscribers);
			if (IsValid(SelectedSubscriber)) // if we managed to select one, use set owner and call use() function.
			{
				Token->SetOwner(SelectedSubscriber);
				Token->Use();
			}
		}
	}

	return false;
}

void UAITokenCategory::InitializeTokenCategory()
{
	if (InitialTokenCount > 0)
	{
		for (int32 i = 0; i < InitialTokenCount; ++i)
		{
			UAITokenBase* TokenBase = NewObject<UAITokenBase>(this, TokenClass);
			TokenCollection.Add(TokenBase);
		}

		bInitialized = true;
		return;
	}

	UE_VLOG(this, LogTemp, Error, TEXT("Could not initialize Token Category (%s), see %hs for more details!"), *FriendlyName, __FUNCTION__);
	bInitialized = false;
}

void UAITokenCategory::TickCategory(float DeltaTime)
{
}

