// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectibles/Public/CECollectibleData.h"

#include "../Public/CECollectibleViewModel.h"


UCECollectibleData::UCECollectibleData()
{
	CollectibleViewModeClass = UCECollectibleViewModel::StaticClass();
}

void UCECollectibleData::SetPlayerSelectedYear(int32 Year)
{
	AppraisalData.PlayerSetYear = Year;
	CollectibleViewModel->SetPlayerSetYear(AppraisalData.PlayerSetYear);
}

void UCECollectibleData::SetPlayerSelectedGrade(int32 Grade)
{
	AppraisalData.PlayerSetGrade = Grade;
	CollectibleViewModel->SetPlayerSetGrade(AppraisalData.PlayerSetGrade);
}

void UCECollectibleData::AddPlayerSelectedSpecialTag(const FGameplayTag& InTag)
{
	AppraisalData.PlayerSetSpecialTags.AddTag(InTag);
	CollectibleViewModel->SetPlayerSetSpecialTags(AppraisalData.PlayerSetSpecialTags);
}

void UCECollectibleData::RemovePlayerSelectedSpecialTag(const FGameplayTag& InTag)
{
	AppraisalData.PlayerSetSpecialTags.RemoveTag(InTag);
	CollectibleViewModel->SetPlayerSetSpecialTags(AppraisalData.PlayerSetSpecialTags);
}

void UCECollectibleData::SetAppraisedValue(int32 Value)
{
	AppraisalData.AppraisedValue = Value;
	CollectibleViewModel->SetAppraisedValue(AppraisalData.AppraisedValue);
}

void UCECollectibleData::SetupViewModel()
{
	CollectibleViewModel = NewObject<UCECollectibleViewModel>(this, CollectibleViewModeClass);
	UE_LOG(LogTemp, Warning, TEXT("Setting up View Model %s"), *CollectibleViewModel->GetClass()->GetName());

	// Copy data from CollectibleDataRef
	CollectibleViewModel->SetCollectibleName(CollectibleDataRef.CollectibleName);
	CollectibleViewModel->SetGrade(CollectibleDataRef.Grade);

	FGameplayTagContainer AllSpecialTags;
	AllSpecialTags.AppendTags(CollectibleDataRef.SpecialTags);
	AllSpecialTags.AppendTags(CoinCoreDataRef.SpecialTags);
	CollectibleViewModel->SetSpecialTags(AllSpecialTags);

	// Copy data from CoinCoreDataRef
	CollectibleViewModel->SetSubtype(CoinCoreDataRef.Subtype);
	CollectibleViewModel->SetYear(CoinCoreDataRef.Year);
	CollectibleViewModel->SetEdition(CoinCoreDataRef.Edition.ToString());
	CollectibleViewModel->SetIssuer(CoinCoreDataRef.Issuer.ToString());

	// Copy data from AppraisalData
	if (AppraisalData.IsAppraisalValid())
	{
		CollectibleViewModel->SetPlayerSetGrade(AppraisalData.PlayerSetGrade);
		CollectibleViewModel->SetPlayerSetYear(AppraisalData.PlayerSetYear);
		CollectibleViewModel->SetPlayerSetSpecialTags(AppraisalData.PlayerSetSpecialTags);
		CollectibleViewModel->SetAppraisedValue(AppraisalData.AppraisedValue);
	}
}

