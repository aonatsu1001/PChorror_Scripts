// Fill out your copyright notice in the Description page of Project Settings.

#include "ChaserAIController.h"
#include "Kismet/GameplayStatics.h"  //GetPlayerPawn（Get Player Character)のために追加
#include "Navigation/PathFollowingComponent.h"
#include "ChaserCharacter.h"  //AChaserCharacterを使うために追加

void AChaserAIController::BeginPlay()
{
	Super::BeginPlay();
	CurrentPatrolPointIndex = 0;  //巡回地点の初期化
}

void AChaserAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Chaserキャラクターの取得
	AChaserCharacter* Chaser = Cast<AChaserCharacter>(GetPawn());

	//プレイヤーキャラクターの取得（Get Player Characterノード）
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (PlayerPawn)
	{
		bool bCanSeePlayer = LineOfSightTo(PlayerPawn);  //Chaserの視界内にプレイヤーがいるかどうかを確認

		UE_LOG(LogTemp, Warning, TEXT("Can See Player: %s"), bCanSeePlayer ? TEXT("True") : TEXT("False"));

		if (bCanSeePlayer)  //視界内にプレイヤーがいるとき
		{
			if (bIsChasing == false)
			{
				bIsChasing = true;
				OnPlayerSpotted();  //プレイヤーを発見したときのイベントを呼び出す
			}

			//プレイヤーを追跡
			//MoveToActor(PlayerPawn, 5.0f);  //5.0fは停止距離
		}
		else  //視界内にプレイヤーがいないとき
		{
			if (bIsChasing == true)
			{
				bIsChasing = false;
				OnPlayerLost();  //プレイヤーを見失ったときのイベントを呼び出す
			}

			//プレイヤーを見失った場合のパトロール処理
			if (Chaser->PatrolPoints.Num() > 0)
			{
				//現在の移動ステータスを取得
				EPathFollowingStatus::Type MoveStatus = GetMoveStatus();

				//目的地に到着したか，まだ移動していないなら，次の目的地を設定
				if (MoveStatus == EPathFollowingStatus::Idle)
				{
					//次のパトロール地点インデックスを計算
					CurrentPatrolPointIndex = (CurrentPatrolPointIndex + 1) % Chaser->PatrolPoints.Num();
				
					//次のパトロール地点インデックスを取得
					AActor* NextPatrolPoint = Chaser->PatrolPoints[CurrentPatrolPointIndex];

					if (NextPatrolPoint)
					{
						MoveToActor(NextPatrolPoint, 10.0f);  //10.0fは停止距離
					}
				}

				UE_LOG(LogTemp, Warning, TEXT("Moving to Patrol Point %d"), CurrentPatrolPointIndex);


			}
		}
	}
}


