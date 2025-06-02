// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MVVMViewModelBase.h"
#include "CECollectibleViewModel.generated.h"

/**
 * 
 */
UCLASS()
class COLLECTIBLES_API UCECollectibleViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	bool IsAppraised() const { return PlayerSetGrade > 0 && PlayerSetYear > 0; }
	
	// Base Data
	int32 GetGrade() const { return Grade; }
	FString GetEdition() const { return Edition; }
	FString GetIssuer() const { return Issuer; }
	FGameplayTagContainer GetSpecialTags() const { return SpecialTags; }
	FName GetSubtype() const { return Subtype; }
	FName GetCollectibleName() const { return CollectibleName; }
	int32 GetYear() const { return Year; }
	
	// Player set data
	int32 GetPlayerSetGrade() const { return PlayerSetGrade; }
	int32 GetPlayerSetYear() const { return PlayerSetYear; }
	FGameplayTagContainer GetPlayerSetSpecialTags() const { return PlayerSetSpecialTags; }
	int32 GetAppraisedValue() const { return AppraisedValue; }

	// Fake data for user selection
	int32 GetFakeYear_1() const { return FakeYear_1; }
	int32 GetFakeYear_2() const { return FakeYear_2; }
	int32 GetFakeYear_3() const { return FakeYear_3; }
	int32 GetFakeYear_4() const { return FakeYear_4; }
	int32 GetFakeYear_5() const { return FakeYear_5; }
	
	// Base Data
	void SetCollectibleName(const FName& InCollectibleName);
	void SetSubtype(const FName& InSubtype);
	void SetYear(int32 InYear);
	void SetGrade(int32 InGrade);
	void SetEdition(const FString& InEdition);
	void SetIssuer(const FString& InIssuer);
	void SetSpecialTags(const FGameplayTagContainer& InSpecialTags);

	// Player set data
	void SetPlayerSetGrade(int32 InGrade);
	void SetPlayerSetYear(int32 InYear);
	void SetPlayerSetSpecialTags(const FGameplayTagContainer& InSpecialTags);
	void SetAppraisedValue(int32 InAppraisedValue);

	// Fake data for user selection
	void SetFakeYear_1(int32 InYear);
	void SetFakeYear_2(int32 InYear);
	void SetFakeYear_3(int32 InYear);
	void SetFakeYear_4(int32 InYear);
	void SetFakeYear_5(int32 InYear);
	
private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	FName CollectibleName = FName();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	FName Subtype = FName();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	int32 Year;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	int32 Grade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	FString Edition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess="true"));
	FString Issuer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer SpecialTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	int32 PlayerSetGrade = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	int32 PlayerSetYear = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	int32 AppraisedValue = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer PlayerSetSpecialTags = FGameplayTagContainer();

	// Fake data for user selection
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	int32 FakeYear_1 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	int32 FakeYear_2 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	int32 FakeYear_3 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	int32 FakeYear_4 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	int32 FakeYear_5 = 0;

	
};
