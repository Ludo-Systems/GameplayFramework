// Fill out your copyright notice in the Description page of Project Settings.


#include "CECollectibleViewModel.h"

#include "CECollectibleData.h"


void UCECollectibleViewModel::SetCollectibleName(const FName& InCollectibleName)
{
	UE_MVVM_SET_PROPERTY_VALUE(CollectibleName, InCollectibleName);
}

void UCECollectibleViewModel::SetSubtype(const FName& InSubtype)
{
	UE_MVVM_SET_PROPERTY_VALUE(Subtype, InSubtype);
}

void UCECollectibleViewModel::SetYear(int32 InYear)
{
	UE_MVVM_SET_PROPERTY_VALUE(Year, InYear);
}

void UCECollectibleViewModel::SetGrade(int32 InGrade)
{
	UE_MVVM_SET_PROPERTY_VALUE(Grade, InGrade);
}

void UCECollectibleViewModel::SetEdition(const FString& InEdition)
{
	UE_MVVM_SET_PROPERTY_VALUE(Edition, InEdition);
}

void UCECollectibleViewModel::SetIssuer(const FString& InIssuer)
{
	UE_MVVM_SET_PROPERTY_VALUE(Issuer, InIssuer);
}

void UCECollectibleViewModel::SetSpecialTags(const FGameplayTagContainer& InSpecialTags)
{
	UE_MVVM_SET_PROPERTY_VALUE(SpecialTags, InSpecialTags);
}

void UCECollectibleViewModel::SetPlayerSetGrade(int32 InGrade)
{
	//Cast<UCECollectibleData>(GetOuter())->SetPlayerSelectedGrade(InGrade);
	UE_MVVM_SET_PROPERTY_VALUE(PlayerSetGrade, InGrade);
}

void UCECollectibleViewModel::SetPlayerSetYear(int32 InYear)
{
	//Cast<UCECollectibleData>(GetOuter())->SetPlayerSelectedYear(InYear);
	UE_MVVM_SET_PROPERTY_VALUE(PlayerSetYear, InYear);
}

void UCECollectibleViewModel::SetPlayerSetSpecialTags(const FGameplayTagContainer& InSpecialTags)
{
	//Cast<UCECollectibleData>(GetOuter())->SetPlayerSetSpecialTags(InSpecialTags);
	UE_MVVM_SET_PROPERTY_VALUE(PlayerSetSpecialTags, InSpecialTags);
}

void UCECollectibleViewModel::SetAppraisedValue(int32 InAppraisedValue)
{
	//Cast<UCECollectibleData>(GetOuter())->SetAppraisedValue(InAppraisedValue);
	UE_MVVM_SET_PROPERTY_VALUE(AppraisedValue, InAppraisedValue);
	
}

void UCECollectibleViewModel::SetFakeYear_1(int32 InYear)
{
	UE_MVVM_SET_PROPERTY_VALUE(FakeYear_1, InYear);
}

void UCECollectibleViewModel::SetFakeYear_2(int32 InYear)
{
	UE_MVVM_SET_PROPERTY_VALUE(FakeYear_2, InYear);
}

void UCECollectibleViewModel::SetFakeYear_3(int32 InYear)
{
	UE_MVVM_SET_PROPERTY_VALUE(FakeYear_3, InYear);
}

void UCECollectibleViewModel::SetFakeYear_4(int32 InYear)
{
	UE_MVVM_SET_PROPERTY_VALUE(FakeYear_4, InYear);
}

void UCECollectibleViewModel::SetFakeYear_5(int32 InYear)
{
	UE_MVVM_SET_PROPERTY_VALUE(FakeYear_5, InYear);
}

