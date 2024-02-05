# Input

Lyra의 Input 처리

## 기본

EnhancedInput을 사용하여 Input 처리를 진행하고 있음.  
- Input Action  
	게임 프로젝트 <-> Enhanced Input System 사이의 연결고리 역할
	- Enhanced Input에서는 InputAction이 Key와 바인딩됨.
		게임 프로젝트는 이에 대해 이벤트 or 콜백을 등록하여 업데이트 받음.  
	
    즉, InputAction을 바인딩하여 입력을 이벤트나 콜백으로 처리하는 것.
- Input Mapping Context  
	복수 개의 InputAction에 대한 Key매핑을 가지고 있는 컨테이너임.  
	Key - InputAction을 매핑함.  
	InputAction을 어떻게 활성화하고, 이에 대해 값을 어떻게할지 대한 룰을 가짐.  
	ex) W키는 IA_Move에 바인딩, 이는 캐릭터가 앞으로 나아가는 값을 업데이트함.
    - Trigger  
    Key 매핑에 대해 Trigger 조건을 설정해둘 수 있음.  
    ex) 키가 눌릴 때 이벤트 발생, 누르고 풀었을 때 이벤트 발생 등 조건을 설정할 수 있음.
    - Modifier  
    입력에 대한 값을 바꿔줌.  
    ex) Vector2D의 경우, 일반적으로 XYZ에 값이 들어오는데, X값이 우선적으로 들어오게 되는데, 이를 Y가 먼저 들어오게 하거나 특정 입력에 대해서는 X값에 -을 붙이게 하는 등의 작업 가능.
- Player Mappable Input Config  
	InputMappingContext를 복수개 가지고 있음.  
    ex) 일반적인 상황, 차를 탄 상황의 입력이 다를때
    일반적인 상황에 대한 InputMappingContext, 차를 탄 상황의 InputMappingContext를 둘 다 가지고 있음.  
	정확히는 그냥 가지고 있는 것 뿐이고, 이를 바꾸거나 하는 것은 HeroComponent가 진행함.  


## 연관 클래스

- MappableConfigPair  
    - UPlayerMappableInputConfig를 가지고 있음.  
    이는 복수 개의 InputMappingContext를 가질 수 있음.  
    따라서 나올 수 있는 입력 상황에 대해서 해당 Context를 가지고 있다가 나중에 InputMappingContext를 변경해야할 상황이 오면 이를 이용하여 변경함.  
    변경 및 적용은 HeroComponent가 담당함.  

    즉, HeroComponent가 MappableConfigPair을 내부에 가지며, 이를 통해 등록되어 있는 InputMappingContext들 중에 현 상황에 맞는 IMC를 선택하여 적용하는 것.
- InputConfig  
    GameplayTag와 InputAction을 연결하는 LyraInputAction이라는 래퍼 클래스에 대한 배열을 가지고 있음.  
    NativeInputActions, AbilityInpuActions이며, 이를 순회하며 InputTag를 이용해서 이에 대한 InputAction을 찾아주는 함수를 가지고 있음.  
    즉, 허용되는 InputAction들과 그에 대한 태그를 가지고 있으며 InputTag를 통해 같이 바인딩된 InputAction을 반환해줌.
- InputComponent  
    EnhancedInputComponent를 상속 받은 클래스이며, InputAction과 Trigger함수를 바인딩 시켜주는 함수를 가지고 있음.  
    NativeAction일때와 AbilityAction일 때에 대한 대응 함수를 가지고 있음.
- HeroComponent  
    DefaultInputConfigs라는 FBattleMappableConfigPair 배열을 가지고 있음.  
    이를 통해서 Input Mapping context를 EnhancedInputLocalPlayerSubsystem에 등록함. (내부적으로 IMC를 추가하는 것으로 추가를 진행해야 Input 바인딩을 제대로 할 수 있음)  
    또한 Input에 대한 함수를 가지며 이를 InputComponent를 통해서 InputAction과 바인딩함.
- PawnData  
    InputConfig를 내부적으로 가지고 있음.  
    이를 통해 해당 Pawn에서 사용할 InputAction과 그에 해당하는 InputTag를 바인딩함.


## 연결

결국, 연결해야하는 것은 다음과 같다.
- 사용자의 키보드 입력
- InputAction
- InputTag
- Input Method

사용자의 키보드 입력 <-> InputAction  
=> Input Mapping Context => UPlayerMappableInputConfig(PMI) => MappableConfigPair => HeroComponent

InputAction <-> InputTag  
=> LyraInputConfig(Tag를 통해서 InputAction을 찾음) => PawnData

InputAction <-> Input Method  
=> HeroComponent => InputComponent => 바인딩 진행

