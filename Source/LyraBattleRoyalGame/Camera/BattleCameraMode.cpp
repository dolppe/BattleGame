#include "BattleCameraMode.h"
#include "BattleCameraComponent.h"
#include "BattlePlayerCameraManager.h"
#include "GameplayTagContainer.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleCameraMode)

PRAGMA_DISABLE_OPTIMIZATION


FBattleCameraModeView::FBattleCameraModeView()
	: Location(ForceInit)
	, Rotation(ForceInit)
	, ControlRotation(ForceInit)
	, FieldOfView(BATTLE_CAMERA_DEFAULT_FOV)
{
}

void FBattleCameraModeView::Blend(const FBattleCameraModeView& Other, float OtherWeight)
{
	// 카메라모드 스택의 바텀부터 블랜딩시작함.
	// 바텀의 View(기본변수들)에 바텀-1의 Other이 가장 최초로 Blend되는 것.
	if (OtherWeight <= 0.0f)
	{
		return;
	}
	else if (OtherWeight >= 1.0f)
	{
		// Weight가 1.0이면 Other을 덮어씀
		*this = Other;
		return;
	}

	// Location + OtherWieght*(Other.Location - Location)
	Location = FMath::Lerp(Location, Other.Location, OtherWeight);

	const FRotator DeltaRotation = (Other.Rotation - Rotation).GetNormalized();
	Rotation = Rotation + (OtherWeight * DeltaRotation);

	const FRotator DeltaControlRotation = (Other.ControlRotation - ControlRotation).GetNormalized();
	ControlRotation = ControlRotation + (OtherWeight * DeltaControlRotation);

	FieldOfView = FMath::Lerp(FieldOfView, Other.FieldOfView, OtherWeight);
}

UBattleCameraMode::UBattleCameraMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	FieldOfView = BATTLE_CAMERA_DEFAULT_FOV;
	ViewPitchMax = BATTLE_CAMERA_DEFAULT_PITCH_MAX;
	ViewPitchMin = BATTLE_CAMERA_DEFAULT_PITCH_MIN;

	BlendTime = 0.0f;
	BlendAlpha = 1.0f;
	BlendWeight = 1.0f;

	BlendFunction = EBattleCameraModeBlendFunction::EaseOut;
	BlendExponent = 4.0f;
}

UBattleCameraComponent* UBattleCameraMode::GetBattleCameraComponent() const
{
	// UBattleCameraMode를 생성하는 곳은 UBattleCameraModeStack이었다.
	// - 생성할 때 Outer 설정을 GetOuter로 해줬으므로 CameraModeStack의 Outer인 CameraComponent가 Outer이다.
	// - UHakCameraModeStack::GetCameraModeInstance() 확인
	return CastChecked<UBattleCameraComponent>(GetOuter());
}

AActor* UBattleCameraMode::GetTargetActor() const
{
	const UBattleCameraComponent* BattleCameraComponent = GetBattleCameraComponent();
	return BattleCameraComponent->GetTargetActor();
}

FVector UBattleCameraMode::GetPivotLocation() const
{
	// 카메라를 구성하는 캐릭터가 타켓
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		return TargetPawn->GetPawnViewLocation();
	}

	return TargetActor->GetActorLocation();
}

FRotator UBattleCameraMode::GetPivotRotation() const
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		// Pawn의 ControlRotation 반환
		return TargetPawn->GetViewRotation();
	}

	return TargetActor->GetActorRotation();
}

void UBattleCameraMode::UpdateView(float DeltaTime)
{
	// 카메라모드를 가지고 있는 CameraComponent의 Owner인 Character(Pawn)을 활용하여,
	// 해당 캐릭터의 View를 가져옴 (캐릭터의 시야)
	FVector PivotLocation = GetPivotLocation();
	FRotator PivotRotation = GetPivotRotation();

	// Pitch값의 한계에 따라 Clamp함.
	PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);

	// 카메라모드의 View에 Pivot값들을 설정
	View.Location = PivotLocation;
	View.Rotation = PivotRotation;

	// PivotRotation을 ControlRotation으로 설정.
	// 이는 특수한 카메라 움직임을 구현할때는 달라질 수 있음.
	View.ControlRotation = View.Rotation;
	View.FieldOfView = FieldOfView;

	// 정리하면, Character의 Location과 ControlRotation을 활용하여 View를 업데이트함.
}

void UBattleCameraMode::UpdateBlending(float DeltaTime)
{
	if (BlendTime > 0.f)
	{
		// BlendTime은 블랜드 과정의 총 시간임.
		// - BlendAlpha는 0->1로 변화하는 과정임.
		// 진행도를 누적하는 느낌
		BlendAlpha += (DeltaTime / BlendTime);
		BlendAlpha = FMath::Min(BlendAlpha, 1.0f);
	}
	else
	{
		BlendAlpha = 1.0f;
	}

	const float Exponent = (BlendExponent > 0.0f) ? BlendExponent : 1.0f;
	switch (BlendFunction)
	{
	case EBattleCameraModeBlendFunction::Linear:
		BlendWeight = BlendAlpha;
		break;
	case EBattleCameraModeBlendFunction::EaseIn:
		BlendWeight = FMath::InterpEaseIn(0.0f, 1.0f, BlendAlpha, Exponent);
		break;
	case EBattleCameraModeBlendFunction::EaseOut:
		BlendWeight = FMath::InterpEaseOut(0.0f, 1.0f, BlendAlpha, Exponent);
		break;
	case EBattleCameraModeBlendFunction::EaseInOut:
		BlendWeight = FMath::InterpEaseInOut(0.0f, 1.0f, BlendAlpha, Exponent);
		break;
	default:
		checkf(false, TEXT("UpdateBlending: Invalid Blending"));
		break;
	}
}

void UBattleCameraMode::UpdateCameraMode(float DeltaTime)
{
	// Actor를 활용해서, Pivot의 Location, Rotation을 계산하여 View를 업데이트
	UpdateView(DeltaTime);

	UpdateBlending(DeltaTime);
}

UBattleCameraModeStack::UBattleCameraModeStack(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UBattleCameraMode* UBattleCameraModeStack::GetCameraModeInstance(TSubclassOf<UBattleCameraMode>& CameraModeClass)
{
	check(CameraModeClass);

	// CameraModInstances에서 먼저 생성되었는지 확인 후, 반환
	for (UBattleCameraMode* CameraMode : CameraModeInstances)
	{
		// CameraMode는 UClass를 비교함
		// Class에 따라 하나씩만 생김 CameraMode는
		if ((CameraMode != nullptr) && (CameraMode->GetClass() == CameraModeClass))
		{
			return CameraMode;
		}
	}
	// CameraMode의 인스턴스가 없으면 생성
	UBattleCameraMode* NewCameraMode = NewObject<UBattleCameraMode>(GetOuter(), CameraModeClass, NAME_None, RF_NoFlags);
	check(NewCameraMode);

	// CameraModeInstances는 CameraModeClass에 맞는 CameraMode 인스턴스를 가지고 있는 컨테이너라는 걸 알 수 있음
	CameraModeInstances.Add(NewCameraMode);

	return NewCameraMode;
}

void UBattleCameraModeStack::PushCameraMode(TSubclassOf<UBattleCameraMode>& CameraModeClass)
{
	// heroComponent에게 전달받은 카메라 모드를 스택에 넣어주는 함수
	// blend 가중치를 보고 보간을 계산하는 것은 다른 함수에서 진행 (EvalueateStack)
	
	if (!CameraModeClass)
	{
		return;
	}

	// 카메라 모드의 Instance를 가져옴.
	UBattleCameraMode* CameraMode = GetCameraModeInstance(CameraModeClass);

	// 만약 이미 최근에 CameraMode를 STacking했으면 리턴함. (가장 최근에 이미 했어서)
	int32 StackSize = CameraModeStack.Num();
	if ((StackSize >0 ) && (CameraModeStack[0] == CameraMode))
	{
		return;
	}

	// CameraMode에 맞는 Index를 찾음
	// 최종 BlendWeight값을 찾기 위해 초기값으로 1.0설정.
	int32 ExistingStackIndex = INDEX_NONE;
	float ExistingStackContribution = 1.0f;
	
	/*
	 * 내가 찾는 카메라 모드가 나올 때까지 계속 할당된 가중치를 줄여가며 찾는 것임.
	 * 처음 가지는 가중치는 1.0f이며 이를 계속 나눠 가지는 것.
	 * BlendWeight	|	다음으로 넘길 남은 가중치						|	현재 카메라 모드가 가질 가중치 
	 * 0.1f			|	남은가중치(1.0f) * (1.0f - 0.1f) = 0.9f		|	0.1f	= 1.0f * 0.1f
	 * 0.3f			|	남은가중치(0.9f) * (1.0f - 0.3f) = 0.63f		|	0.27f	= 0.9f * 0.3f
	 * 0.6f			|	남은가중치(0.63f) * (1.0f - 0.6f) = 0.252f	|	0.378f	= 0.63f * 0.6f
	 * 1.0f			|	남은가중치(0.252f) * (1.0f - 1.0f) = 0.0f		|	0.252f = 0.252f * 1.0f
	 * 
	 */
	for (int32 StackIndex = 0; StackIndex < StackSize; ++StackIndex)
	{
		if (CameraModeStack[StackIndex] == CameraMode)
		{
			ExistingStackIndex = StackIndex;
			ExistingStackContribution *= CameraMode->BlendWeight;
			break;
		}
		else
		{
			ExistingStackContribution *= (1.0f - CameraModeStack[StackIndex]->BlendWeight);
			
		}
	}

	// 찾은 카메라 모드는 top으로 이동해야 하기에 우선 빼줌
	if (ExistingStackIndex != INDEX_NONE)
	{
		CameraModeStack.RemoveAt(ExistingStackIndex);
		StackSize--;
	}
	else
	{
		// 카메라 모드가 없는 경우 현재 가진 가중치 0.0f로 설정.
		ExistingStackContribution = 0.0f;
	}

	// blendTime이 0보다 작으면 블랜드를 안하고, 만약 스택에 남은게 없으면 현재 가진 카메라 모드만 실행하기에 false 처리
	const bool bShouldBlend = ((CameraMode->BlendTime > 0.0f) && (StackSize > 0));
	// blend를 진행한다면, 구한 가중치를 가져와서 적용해줌.
	// 안한다면 1.0f로 설정해서 해당 카메라 모드만 나오게 함.
	const float BlendWeight = (bShouldBlend ? ExistingStackContribution : 1.0f);
	CameraMode->BlendWeight = BlendWeight;

	// top (index == 0)에 카메라 모드 설정.
	// 마지막은 항상 1.0f이 되도록 해야함.
	CameraModeStack.Insert(CameraMode, 0);
	CameraModeStack.Last()->BlendWeight = 1.0f;

	
}

void UBattleCameraModeStack::EvaluateStack(float DeltaTime, FBattleCameraModeView& OutCameraModeView)
{
	// Top -> Bottom까지 (0->Num)까지 순차적으로 Stack의 CameraMode를 업데이트함
	UpdateStack(DeltaTime);

	// Bottom -> Top까지 CameraModeStack에 대해 Blending 진행
	BlendStack(OutCameraModeView);
}

void UBattleCameraModeStack::UpdateStack(float DeltaTime)
{
	const int32 StackSize = CameraModeStack.Num();
	if (StackSize <= 0)
	{
		return;
	}

	// 카메라 모드 스택을 순회하며 카메라 모드를 업데이트함
	
	int32 RemoveCount = 0;
	int32 RemoveIndex = INDEX_NONE;
	for (int32 StackIndex = 0; StackIndex< StackSize; ++StackIndex)
	{
		UBattleCameraMode* CameraMode = CameraModeStack[StackIndex];
		check(CameraMode);

		// 카메라 모드별 Weight, View를 업데이트 해줌.
		CameraMode->UpdateCameraMode(DeltaTime);

		// 만약 카메라 모드의 BlendWeight가 1.0에 도달하면 그 이후는 다 제거함.
		if (CameraMode->BlendWeight >= 1.0f)
		{
			RemoveIndex = (StackIndex+1);
			RemoveCount = (StackSize - RemoveIndex);
			break;
		}
	}

	if (RemoveCount >0)
	{
		CameraModeStack.RemoveAt(RemoveIndex, RemoveCount);
	}

	// BlendTime의 누적을 1.0f으로 맞추기 위한 것.
}

void UBattleCameraModeStack::BlendStack(FBattleCameraModeView& OutCameraModeView) const
{
	const int32 StackSize = CameraModeStack.Num();
	if (StackSize <= 0)
	{
		return;
	}

	// Bottom -> Top 순서로 블랜드 진행
	const UBattleCameraMode* CameraMode = CameraModeStack[StackSize -1];
	check(CameraMode);

	// 제일 밑에 있는 (Bottom)은 BlendWeight가 1.0임
	OutCameraModeView = CameraMode->View;

	// 이미 Bottom은 OutCameraModeView로 지정했기에 다음것부터 순회함.
	for (int32 StackIndex = (StackSize -2); StackIndex >=0 ; --StackIndex)
	{
		CameraMode = CameraModeStack[StackIndex];
		check(CameraMode);

		OutCameraModeView.Blend(CameraMode->View, CameraMode->BlendWeight);
		
	}
	
}

void UBattleCameraModeStack::GetBlendInfo(float& OutWeightOfTopLayer, FGameplayTag& OutTagOfTopLayer) const
{
	if (CameraModeStack.Num() == 0)
	{
		OutWeightOfTopLayer = 1.0f;
		OutTagOfTopLayer = FGameplayTag();
		return;
	}
	else
	{
		UBattleCameraMode* TopEntry = CameraModeStack.Last();
		check(TopEntry);
		OutWeightOfTopLayer = TopEntry->GetBlendWeight();
		OutTagOfTopLayer = TopEntry->GetCameraTypeTag();
	}
}

PRAGMA_ENABLE_OPTIMIZATION
