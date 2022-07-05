// Made by Intax, 2022

#include "AITokenBase.h"
#include "TokenSubscriberComponent.h"

UTokenSubscriberComponent* UAITokenBase::GetCurrentOwner() const
{
	return Owner;
}

bool UAITokenBase::IsSingleInstancePerSubscriber() const
{
	return bSingleInstancePerSubscriber;
}

void UAITokenBase::Use()
{
	// ensure we haven't used already obtained one.
	check(!bObtained);
	bObtained = true;

	if (IsValid(Owner))
	{
		Owner->OnTokenObtained(this);
	}

	OnUsed();
}

void UAITokenBase::Release()
{
	check(bObtained);
	bObtained = false;

	if (IsValid(Owner))
	{
		Owner->OnTokenReleased(this);
	}
	
	if (bTokenHasCooldown)
	{
		bCooldownActive = true;
		if (const UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimer(TimerHandle_UseToken, this, &UAITokenBase::RemoveCooldown, 1.f, false);
		}
	}
	
	OnReleased();
}

void UAITokenBase::ForceToRelease()
{
	// this function might get called anywhere without a check so we have to check if we are obtained or not manually.
	if (bObtained)
	{
		Release();
	}
}

void UAITokenBase::RemoveCooldown()
{
	bCooldownActive = false;
}

UTokenSubscriberComponent* UAITokenBase::SelectBestSubscriber_Implementation(const TArray<UTokenSubscriberComponent*>& PossibleSubscribers) const
{
	return nullptr;
}

void UAITokenBase::OnUsed_Implementation()
{
}

void UAITokenBase::OnReleased_Implementation()
{
	
}

