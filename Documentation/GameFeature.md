# GameFeature

- 게임의 기능을 분할하고, 각 부분을 독립적이고 재사용 가능하도록 만들어진 시스템.  
이를 활용하면, 기존의 게임과는 독립적으로 게임의 모드를 관리하면서 이를 활성화 / 비활성화 시킬 수 잇음.
- 게임플레이 시스템을 모듈화 하는 것.

- 장점 
    1. 이벤트 형식의 컨텐츠를 단발적, 주기적으로 만들어야 할때 재사용하기 쉬워짐.
    2. 게임의 기능을 독립적이고, 재사용 가능하도록 만들 수 있음.
    3. 활성화 비활성화가 가능해서 원하는 기능을 설정, 해제하기 편함.
    4. 기능을 비활성화 시켜두면, 이에 대한 컨텐츠나 코드들이 기존의 게임과 아예 분리되어서 비활성화되기 때문에 에디터나 게임을 더 가볍게 만들 수 있음.

## GameFeature Plugin
기능을 플러그인 단위로 묶어둔 것으로, 이를 활성화 비활성화 하여 플러그인 내부의 컨텐츠를 게임에 부착하거나 떼거나할 수 있음.  
하나의 큰 컨텐츠 덩어리를 의미함.

이를 활용하면, 원하는 게임의 컨텐츠, 모드를 Plugin 단위로 묶어서 모듈화할 수 있음. 모듈화하여 메인 게임에 이를 붙이거나 떼는 것을 편하게 진행할 수 있음.

또한 GameFeatureData, GameFeatureAction 등을 통해 Plugin이 활성화되고, 비활성화 될때 필요한 설정들을 진행할 수 있음.

### GameFeatureData
각 GameFeature Plugin마다 존재하는 Data 객체.  
해당 GameFeature Plugin에 대해 설명하는 데이터라고 생각하면 됨.
아래와 같은 데이터를 가지고 있음.
- FeatureState  
해당 GameFeature Plugin의 활성화 상태에 대한 정보를 나타냄.  
시작 할때의 State부터, 계속 진행되는 상태를 담고 있음.
- Actions  
GameFeaturePlugin이 활성화 될때 같이 활성화 될 GameFeature Action을 정의할 수 있음.  
여기에 등록된 Action들은 Plugin이 활성화 될때 자동으로 상태를 바꿔가며 활성화됨.
- AssetManager  
Project Setting에 존재하는 AssetManager의 Primary Asset Types to Scan은 프로젝트 전체적으로 봤을 때 스캔을 진행할 Primary Asset에 대한 정의를 담고 있음.  
GameFeatureData에 있는 AssetManager는 해당 GameFeature Plugin이 활성화 될때 스캔을 진행할 Primary Asset에 대한 정보를 담고 있음.  
(참고로 이는 GameFeatureData에 대한 상태 경로도 인식함.)


## GameFeature Action

GameFeature Plugin이 하나의 큰 컨텐츠 덩어리를 의미하면, 이는 해당 Plugin이 활성화, 비활성화 되는 등의 과정 속에서 필요한 Action들을 진행함.

ex) 특정 Actor에 컴포넌츠 추가 / 데이터 레지스트리 추가 / InputConfig 추가 등 필요한 모든 작업들을 만들고 적용할 수 있음.

하나의 Action 단위이며, 이를 커스텀하여 원하는 작업을 진행하게 할 수 있음.

이를 활용하여 작업들을 모듈화할 수 있고, GameFeature Plugin이 활성화되거나 혹은 특정 상황이 생겼을 때 필요한 작업들을 호출해서 실행시킬 수 있음.


## ShooterCore Plugin

Lyra의 ShooterCore Plugin은 GameFeature Plugin으로서 총기와 관련된 기능들이 담겨져 있음.  
Experience 내부에는 해당 Experience가 활성화 될 때 같이 활성화 될 GameFeature Plugin에 대한 정보를 담고 있음.  
Plugin이 활성화되는 과정을 살펴보면 다음과 같다.

1. UserFacingExperience가 변경되면서, 해당 맵으로 ServerTravel을 진행함.
2. 이를 진행하면서 넘긴 ExtraArgs에서 Experience 정보를 찾아와서 이를 Loading함.
3. Experience 로딩이 진행되고 난 후에 내부에 있는 GameFeature Plugin이 활성화됨.
4. 이후 GameFeature Plugin, Experience의 내부에 담긴 GameFeature Action들이 활성화되면서 해당 액션들이 수행됨.

위와 같은 과정을 통해 Plugin이 활성화됨.




