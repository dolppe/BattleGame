# Camera

UE에는 카메라를 관리하기 위한 여러 클래스가 존재함.  

Lyra에는 카메라 모드라는 것을 이용해서 다양한 카메라 상황을 관리함.  
ex) 차량 탑승 카메라, 3인칭 카메라, 1인칭 카메라 등  
카메라 모드를 변경하는 것으로 게임 카메라의 시점을 변경할 수 있는데, 변경 시에 바로 바뀐다면 끊기는 것이 느껴질 수 밖에 없음.  
=> 이를 처리하기 위해 카메라 블랜딩을 진행하여 서서히 바뀌도록 설정함.

## 카메라 관련 클래스

- CameraComponent  
 실제 카메라를 의미하며, Character의 Component로 붙음.  
 카메라의 Blending 기능을 지원하기 위해 CameraModeSatck을 가지고 있음.
   - HeroComponent의 PawnData를 이용하여 현재 CameraMode를 가져와서 이를 CameraModeStack에 넣어서 적용시킴.
   - 현재 카메라 모드, 블랜딩 등 이와 관련된 계산을 진행하여 완료된 결과물 View를 이용하여 카메라의 Location, Rotation, FOV, ControlRotation에 적용함.  
   또한 실제 렌더링에 적용되도록 FMinimalViewInfo에도 전달.

- PlayerCameraManager  
 PlayerController와 1:1 관계를 이루며, 카메라를 관리하는 역할을 가짐. 카메라의 PITCH MAX, MIN 값, DEFAULT FOV 값 등을 가지고 관리함.

- CameraMode  
	DeltaTime에 따라서 View를 업데이트하고, BlendWeight를 업데이트를 진행함.
	- 내부에 자체적인 View를 가지고 있으며, Blend에 대한 변수들 (BlendTime, BlendAlpha, BlendWeight)를 가지고 있음.
	-  View에 대한 PitchMin, Max, 카메라 모드의 FOV 값을 가짐. 또한 카메라 모드마다 BlendingFunction을 다르게 가짐.  
	
    즉, 정말 카메라 모드라는 말에 맞게, Blending을 위한 변수, 실제로 보여줄 View에 대한 데이터와 함수를 가지고 있는 것.  
	어떻게 카메라가 동작할지를 정하면, 현 상황(DeltaTime, BlendWeight)에 맞는 View를 설정하는 것.

- CameraModeSatck  
 현재 Blend를 진행하고 있는 CameraMode들을 관리함.  
 이에 대해 Stack에 CameraMode를 추가하거나 Blending을 진행하기 위해 값을 업데이트하고, Blending을 진행시킴.  
 카메라 블랜딩을 위해 카메라 모드들을 가지고 관리함.


- CameraModeView  
 카메라 모드의 실제 카메라가 가져야하는 값들임.
    - Location
    - Rotation
    - ControlRotation
    - FieldOfView  
 
    총 4가지 변수를 가지고 있으며, 다른 View와 Weight을 전달 받으면, 이를 토대로 Blend를 진행함.(보간)  
    각 카메라 모드들은 자신만의 CameraModeView를 소유하고 있음.


## 블랜딩 예시

A 카메라 모드 -> B 카메라 모드로 전환 시  
애니메이션 블랜딩과 유사하게 자연스럽게 만들어 주는 것.

ex) 일반 3인칭 시점 <-> 차 탑승 시점  
이를 바로 변하게 하면 어색하니, 시간에 따라 비율적으로 변할 수 있도록 함.

- Blend Time  
    얼마동안 Blend를 진행할 지에 대한 시간.  
    ex) 1초이면, 1초가 지나면 해당 카메라 모드로 전환되는 것.
- Blend Alpha(0~1값)  
    선형적으로 나타낸 blend 값.  
    DeltaTime과 BlendTime을 이용하여 현재 어느정도 시간적 비율인지 나타내는 것.  
    ex) DeltaTime이 1초, BlendTime이 5초이면, 현재 Blend Alpha값은 0.2임.
- Blend Weight(0~1값)  
    블랜딩에 실제 적용하는 가중치임.  
    현재 카메라에 어느정도 비율을 차지할 것인지를 나타냄.  
    블랜딩 함수를 통해 Blend weight을 정하는 것임.  
    ex) 블랜딩 함수는 Blend Alpha가 x축이라면, y축은 Blend Wegiht을 가지며 시간 변화에 따라 Alpha값이 변화하면, 그에 따른 가중치 값을 가져오는 것임.  
    만약, BlendWeight이 1.0f가 되고, 스택의 Top으로 설정 됐다면, 해당 카메라모드만 적용되는 것임.

결론적으로, 각 CameraMode에 맞는 View를 시간, 블랜딩 함수에 따라서 잘 섞어서 현재 적용할 View를 계산하는 역할임.


## CameraModeStack의 BlendWeight

기본적으로 BlendWeight은 0~1사이의 값을 가지며, 1이 되면 해당 카메라 모드는 다 끝나서 종료 예정인 모드임.

- 가장 최근에 들어온 카메라 모드를 Top으로 올린다.
- BlendStack의 모든 가중치의 할당량을 합치면 1.0이 나와야 하기에 Stack의 가장 마지막인 카메라 모드의 BlendWeight은 1.0f임.

실질적인 예시)  
차량에 탑승해 있다가 내린 후에 엎드렸을 때.  
차량 카메라 모드 => 일반 카메라 모드 => 엎드림 카메라 모드  
- 이때 Top은 엎드림 카메라 모드, Bottom은 차량 카메라 모드이며, 차량 카메라 모드가 DeltaTime 누적이 가장 큰 상태.
- BlendWeight이 1.0이 되면(DeltaTime == BlendTime), 해당 카메라 모드는 빠지게 됨.
- 블랜딩이 Bottom부터 진행되는데, Bottom View에 각 Weight에 따라 View를 점점 섞어 가는 것. => 이를 진행하다 보면, 결국 엎드림 카메라 모드가 최종적으로 현재 Camera의 View가 되는 것.


## 카메라 업데이트 순서

카메라의 틱은 월드틱이 모두 다 돈 이후에 진행됨.

- PlayerController => PlayerCameraManager->UpdateCameraManager()
- PlayerCameraManager => Character->UpdateCamera()
- Character => CameraComponent->CalcCamera()
- CameraComponent->GetCameraView()  

위의 순서로 카메라 업데이트가 진행되며, CameraComponent 내부에서 CameraModeStack을 업데이트하여 최종 View를 카메라, FMinimalViewInfo에 적용하는 것임.

CameraComponent 내부의 진행 순서는 아래와 같음.
- UpdateCameraModes() 
    - PawnData를 가져와서 DefaultCameraMode를 가져옴.
    - CameraModeStack->PushCameraMode(DefaultCameraMode)  
        전달한 카메라 모드의 Instance를 생성하거나 가져와서 스택에 넣고, BlendWeight를 설정하는 것.
    - CameraModeStack->EvaluateStack()
        - UpdateStack  
            스택의 Top -> Bottom까지 순차적으로 업데이트를 진행함. CameraMode 별 View 값과, BlendWeight를 설정해줌.
            - UpdateView => 현재 Character의 상태에 따라 자신의 View 값을 설정해줌.
            - UpdateBlending => 설정된 BlendFunction, DeltaTime에 따라 BlendWeight을 설정해줌.
        - BlendStack  
            Bottom -> Top 순서로 Blend를 진행함.  
            Blend를 진행하며 스택의 카메라 모드의 View값을 점점 누적해가는 방식으로 Blend 시킴.  
            결과적으로 현 상황에 맞는 View로 설정되는 것.
    - PC->SetControlRotation, SetWorldLocationAndRotation 등을 통해 결과로 나온 View를 적용함 (Location, Rotation, ControlRotation, FOV)
    - FMinimalViewInfo에 적용  
        렌더링 되는 View 정보, 반영할 카메라의 정보를 담고 있음. 여기에 결과로 나온 View를 적용해서 최종 렌더링에 반영함.


    

