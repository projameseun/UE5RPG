// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGGameModeBase.h"

ARPGGameModeBase::ARPGGameModeBase()
{
	//UClass : UObject를 상속받은 언리얼 엔진 클래스들은 모두 UClass를 가지고 있다.
	//UClass는 자신이 어떤클래스 타입인지에  대한 클래스 정보를 ㄱ ㅏ지고 있다.
	//언리얼 엔진의  객체를 생성할때는 어떤 타입인지 UClass정보를  넘겨서 생성하도록 되어  있다.
	//GameMode에서 PlayerController 와 DefaultPawn의 경우 모두 UClass를 지정하게 만들어져 있다.

	//Object를 그대로 경로를 갖고와도 되지만 Class는 반드시 뒤에_를 붙여줘야 된다. 

    //레펀러스복사했을때 조심해야된다 경로가 다르다는거 지워야될곳을 잘체크 
    /// Script / Engine.Blueprint'/Game/Player/BP_Shinbi.BP_Shinbi'
    ConstructorHelpers::FClassFinder<APawn> PawnClass(TEXT("/Script/Engine.Blueprint'/Game/Player/BP_Shinbi.BP_Shinbi_C'"));
   
    if (PawnClass.Succeeded()) 
    {
        DefaultPawnClass = PawnClass.Class;
        // 성공 로그 추가
        UE_LOG(LogTemp, Warning, TEXT("DefaultPawnClass set to %s"), *DefaultPawnClass->GetName());
    }
    else {
        // 실패 로그 추가
        UE_LOG(LogTemp, Error, TEXT("Failed to find BP_Shinbi class!"));
    }
}