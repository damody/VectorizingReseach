// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

UCLASS()
class VECTORIZINGANIMATION_API AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mat")
	UTexture2D* MyTexture2D;
	
	UFUNCTION(BlueprintCallable, Category = "Mat")
	void imshow();


	UFUNCTION(BlueprintImplementableEvent, Category = Tracking)
	void IsActorInJunction(AActor* Actor, bool& Result);

	/** point light component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Switch Components")
	class UPointLightComponent* PointLight1;

	/** sphere component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Switch Components")
	class USphereComponent* Sphere1;

	AMyActor(const FObjectInitializer& ObjectInitializer);

	/** Toggles the light component's visibility*/
	UFUNCTION()
	void ToggleLight();

	/** the desired intensity for the light */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch Variables")
	float DesiredIntensity;

};
