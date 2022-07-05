
#include "TokenSubscriberComponent.h"
#include "AITokenBase.h"

UTokenSubscriberComponent::UTokenSubscriberComponent()
{
}

void UTokenSubscriberComponent::OnTokenObtained(UAITokenBase* Token)
{
	ensure(ObtainedTokens.Find(Token) == INDEX_NONE);
	ObtainedTokens.Add(Token);
	OnTokenObtainedEvent.Broadcast(Token);
}

void UTokenSubscriberComponent::OnTokenReleased(UAITokenBase* Token)
{
	ensure(ObtainedTokens.Find(Token) != INDEX_NONE);
	ObtainedTokens.RemoveSwap(Token);
	OnTokenReleasedEvent.Broadcast(Token);
}

bool UTokenSubscriberComponent::ReleaseToken(TSubclassOf<UAITokenBase> TokenType)
{
	for (UAITokenBase* Token : ObtainedTokens)
	{
		if (IsValid(Token) && (Token->GetClass() == TokenType))
		{
			Token->ForceToRelease();
			return true;
		}
	}

	return false;
}

bool UTokenSubscriberComponent::HasTokenOfType(TSubclassOf<UAITokenBase> TokenType) const
{
	if (TokenType)
	{
		for (const UAITokenBase* Token : ObtainedTokens)
		{
			return IsValid(Token) && Token->GetClass() == TokenType;
		}
	}

	return false;
}
