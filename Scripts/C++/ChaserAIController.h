// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ChaserAIController.generated.h"

UCLASS()
class VRPROJECT_API AChaserAIController : public AAIController
{
	GENERATED_BODY()

public:
	
	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

	//Chaserの視界内にプレイヤーが存在するかどうかを記憶する変数
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	bool bIsChasing;

	//C++から呼び出す，ブループリント側で実装されるイベント
	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnPlayerSpotted();  //プレイヤーを発見したときに呼び出されるイベント

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnPlayerLost();  //プレイヤーを見失ったときに呼び出されるイベント

	//現在の巡回地点のインデックス
	int32 CurrentPatrolPointIndex;

};
