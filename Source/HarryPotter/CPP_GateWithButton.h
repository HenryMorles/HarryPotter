// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_BaseGate.h"
#include "CPP_GateWithButton.generated.h"

UCLASS()
class HARRYPOTTER_API ACPP_GateWithButton : public ACPP_BaseGate
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_GateWithButton();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* ButtonCollider_1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* ButtonCollider_2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* ButtonCollider_3;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bButtonState_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bButtonState_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bButtonState_3;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
