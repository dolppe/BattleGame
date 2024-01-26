# GameplayFramework

게임의 틀을 이루는 게임 규칙, 플레이어 입력과 컨트롤, 카메라 등의 코어 시스템.
내부 요소로는
- Pawn, PlayerController, GameMode 등이 있음.

## GameFeature
 - 게임의 기능을 최대한 분할하고, 각 부분을 독립적이고 재사용 가능하도록 만드는 것.
- GameFeature Plugin은 컨텐츠 단위로 보면 편함.  
    ex) MMORPG에는 할로윈, 어린이날 등의 시즌별 기존 게임과 독립적인 게임모드가 존재할 수 있음.  
    => 이러한 모드를 손쉽게 제공하면서, 기존 게임 코드에 영향 없이 반영 가능.  
    해당 시즌을 껐다가 켰다 하면서 활용 가능.  
    이벤트 형식의 컨텐츠를 단발적, 주기적으로 만들어야 할때, 재사용하기 쉬워짐.

게임플레이 시스템을 모듈화 하는 것에 가까움

### Lyra에서 GameFeature 예시
Lyra는 다양한 게임 모드들이 존재함. 이를 적용할 때 GameFeaturePlugin으로 나눠두고 활성화시켜서 적용함.  
ex) ShootCore 플러그인  
이는 총을 쏘고, 장비를 장착하는 등의 기능들이 포함되어 있는 플러그인이며, 내부에는 이를 구현하기 위한 데이터들이 포함되어 있음.

## GameFeature Action
GameFeature의 기능을 구현하기 위해 실행할 것들임.

ex) 치트추가, 구성요소 추가, 데이터 레지스트리 추가

- 중요한 점은 내가 원하는 액션을 커스텀할 수 있다는 점.  
원하는 컴포넌트를 특정 액터에 추가하거나 InputConfig를 추가하는 등의 작업도 가능함.

이를 활용하면 특정 GameFeature Plugin이 활성화되거나 특정 상황이 벌어졌을 때 진행되어야 할 액션들을 미리 만들고 설정해두어서 자동으로 실행되도록 할 수 있음.

## GameFrameworkComponentManager
 - GameInstance의 Subsystem이며, GameFeature을 활성화, 비활성화 할때 Action 수행을 하는데, 이를 진행하는 중추 시스템.  
 즉, GameFeature의 기능을 진행하기 위해 GameInstance에서 이를 지원해주는 역할임.
 - 간단히 GameFeature plugin을 사용하기 위한 GameInstacne Subsystem.

 주요 기능은
 1. Extension Handlers  
    Game Feature가 활성화 됐을 때, Componet와 같은 Actor를 추가하거나 제거 수정하는 등의 일을 가능하게 해줌.
2. Initialization States  
    네트워크 게임의 Component 초기화는 순서가 중요하고 복잡한 경우가 많음.  
    => 이러한 복잡한 단계를 더 깔끔하게 나눠서 관리할 수 있도록 기능을 제공해줌.

### InitState

GameFrameworkComponentManager에서 제공하는 기능 중 Initialization States 기능.

- 주요 목적이 Component를 깔끔하게 초기화 하는 것.
- 이를 활용하면 원하는 흐름대로 컴포넌트의 초기화, 설정이 이루어 지게 됨.
- 중요한 점은 InitState는 선형적으로 상태를 업데이트 하도록 구현해야 함. => 중간에 끊어지지 않도록 해야함.
- 또한 PawnExtensionComponent와 같이 마스터 컴포넌트를 만들어서 InitState의 흐름 제어를 진행할 Component가 필요함.  
=> 이때 중앙 관리 컴포넌트의 경우 다른 컴포넌트들보다 더 빠르게 초기화, 설정을 진행해야 관리가 편함 => Simple Construction Script를 활용하면 편함. (중앙은 CDO로 바로 가지고 있게 만들고, 나머지는 SCS를 활용하면 순서가 자동으로 설정됨.)


이를 사용하기 위해 필수적으로 진행해야 하는 행위
- GameplayTags에 원하는 InitState를 생성 및 등록 해야함.
- GameInstance의 GameFrameworkComponentManager에 만들어 둔 InitState를 등록 해야함.
- GameInstance에서 GameFrameworkComponentManager에 InitState를 등록하기 전에 InitState Tag 자체를 생성하고 등록해야 하기 때문에 이를 빠르게 진행하기 위해 AssetManager를 이용함 (빠르게 생성 및 설정 가능하기 때문)

=> 기본적인 준비는 끝.

InitState를 적용하기 위한 준비.
- 적용하길 원하는 Component가 IGameFrameworkInitStateInterface를 상속 받아야 함.
- GetFeatureName을 재정의하여 해당 Component의 FeatureName을 설정 해야함.
- 해당 Component를 InitStateFeature로 사용하는 것을 등록 해야함 (RegisterInitStateFeature)
- 이후 아래 있는 InitState를 다루기 위해 제공되는 함수들을 적절히 재정의하여 InitState 단계를 잘 다루면 됨.


InitState를 다루기 위해 제공되는 함수들
- CheckDefaultInitialization => 상태 조건을 달성하는 한 최대로 InitState를 업데이트함.
- OnActorInitStateChanged => 바인딩 해둔 GameFeature Component의 InitState가 변경되면 호출되는 함수.
- CanChangeInitState => 조건 체크 함수.
- HandleChangeInitState => 조건 체크 후 InitState가 변경됐을 때, 실행되는 함수.
- BindOnActorInitStateChanged => 다른 GameFeature Component에 바인딩을 거는 것.

잡다
 - InitState가 등록되는 것은 UGameFrameworkComponentManager에게 결국 등록되는 것.  
 => 이는 Actor별로 ActorFeatureMap이라는 TMap에 저장됨.  
 Value에서 Actor와 등록된 컴포넌트들을 확인할 수 있음.  
 ex) PawnActor => PawnExtensionComponent, HeroComponent 둘다 등록 되어 있는 것.


 ### Lyra의 InitState 초기화 순서

우선 Lyra Character의 PawnExtensionComponent, HeroComponent를 통해 예시를 설명할 것임.

- PawnExtensionComponent  
캐릭터의 초기화를 담당하는 중앙 제어형 컴포넌트.  
등록된 GameFeatureComponent들의 InitState 초기화를 중앙에서 제어하는 컴포넌트임.  
초기화 순서 등 흐름을 전반적으로 관장함.
- HeroComponent  
카메라, 입력 등 플레이어가 제어하는 시스템들의 초기화를 담당하는 컴포넌트.

InitState 초기화 진행 순서

- Character 초기화 과정  
    현재 Character에 PawnExtensionComponent는 생성자에서 생성됨(SubObject). HeroComponent는 SCS를 활용하여 추가됨.
    1. GameMode => Pawn Spawn
    2. RegisterAllComponent => SubObject로 추가된 PawnExtensionComponent => OnRegister 실행  
    => InitState에 PawnExtComp 등록.
    3. GameMode => Set PawnData  
    이때, CheckDefaultInitialization을 호출하지만, CurrentState가 None이라서 상태 변화 x
    4. FinishingSpawning => SCS->ExecuteConstruction 실행
    => HeroComponent => OnRegister 실행.
    5. PostActorConstruction => BeginPlay  
    PawnExtComp에서 InitState를 Spawned로 변경.  
    이후 추가 업데이트에서 PawnData는 이미 있고, Controller가 Possess 안되어 있어서 DataAvailable로 변경.  
    (Controller가 없어서 그냥 체크 없이 넘어감)
    6. HeroComponent도 BeginPlay를 통해 Spawned로 변경.  
    추가 업데이트는 PC가 없어서 PS도 없으니 진행 x
    7. GameMode => FinishRestartPlayer를 진행하며 PC가 Possess하게 됨. => 이때 SetupPlayerInputComponent가 실행되는데, 이를 활용해 PawnExtComp의 업데이트를 진행함.  
    (해당 과정이 없으면 InitState의 선형적 초기화가 끊어짐.)
    8. PawnExtComp는 다른 GameFeature Component의 InitState 업데이트를 먼저 진행시키기에 HeroComponent => DataAvailable로 변경.  
    (추가 진행은 PawnExtComp가 아직 Spawned라 진행 안함)
    9. PawnExtComp의 업데이트 진행.  
    DataInitialized로 변경. (HeroComp가 DataAvailable이기에)  
    이후 추가 업데이트에서 GameplayReady까지 업데이트 진행.
    10. PawnExtComp가 업데이트 될때 Broadcast되어 HeroComp에게 변경사항이 전달 => 이를 통해 업데이트 진행하여 DataAvailable => DataInitialized => GameplayReady 까지 업데이트 됨.

주의 깊게 봐야하는 부분은 세 가지임.  
1. PawnExtComp(초기화 흐름 관리 Comp)는 SubObject로 붙이고, 이외의 HeroComp는 Simple Construction Script르 붙임.  
=> 이 결과 PawnExtComp가 HeroComp보다 더 빠르게 업데이트가 진행되어서 흐름을 더 편하게 제어할 수 있어짐.
2. HeroComp가 DataAvailable => DataInitialized로 업데이트 시도할 때 PawnExtComp의 조건을 살피며 진행.  
=> PawnExtComp를 통해 전체 다른 GameFeature Component의 Sync를 맞췄다가 진행하는 것.
3. Controller가 Possess되기 전에 InitState의 흐름이 끊김. 이를 Possess한 이후에 PawnExtComp의 InitState 업데이트를 호출하도록 끼워 넣으면서 InitState의 업데이트 흐름을 이어줌.  
=> 결국 InitState는 선형적인 업데이트가 중요하기에 끊어지지 않도록 원할 때 연결시켜주며 업데이트가 되도록 해야함.
