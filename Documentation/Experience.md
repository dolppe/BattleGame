# Experience


## 개요
기존 UE의 GameMode와 유사한 개념.  
게임에 대한 룰을 설정함.  
- Default로 사용할 Pawn은 무엇인지.
- Experience가 활성화 됐을 때, 어떤 GameFeature Plugin을 활성화 할 것인지.
- 어떤 GameFeatureAction들을 실행할 것인지.  

이와 같은 설정들을 진행함.

## 특징

Experience는 활성화 비활성화가 가능함.  
여러 개의 Experience를 중첩하는 것도 가능함.


## 사용 예시

FPS게임에서 점령전을 진행할 때와 섬멸전을 진행할 때는 게임의 규칙이 달라짐.
- 점령전 => 땅을 점령해야 승리
- 섬멸전 => 적을 일정 수 이상 죽여야 승리

이와 같이 여러 다른 종류의 모드를 구현해야 할 때, Experience로 나눠서 구현을 진행함.

점령전용 Experience, 섬멸전용 Experience를 구현하고, 필요시 이를 활성화하고 비활성화 할 수 있음.

> 이를 통해 사용자가 점령전 모드를 선택하면, 점령전용 Expreience를 활성화 시키는 등의 유연한 선택이 가능해짐.


## GameMode

GameMode는 UE의 핵심 로직을 담당하기 때문에 이를 변경하려면 많은 설정, 코드의 변경이 필요함.  
=> 복잡하고, 성능이 좋지 않음.

ex) 잠시 3D게임을 2D로 전환하는 경우 GameMode를 변경하면 성능이 좋지 않음.

=> Experience는 모든 것을 다 바꾸는 것이 아니라 필요한 부분을 부분적으로 교체하는 것이기 때문에 교체 성능이 더 좋음.

따라서 과거의 모드 변경을 GameMode에서 진행 했던 것을 Experience를 교체하는 것으로 대체 가능함.

Experience 정보를 담고 관리하는 것은 ExperienceManagerComponent 클래스임.
게임의 상태를 담고 있는 것은 GameState기 때문에 GameState가 컴포넌트로 해당 클래스를 가지고 있음.

## Lyra에서

- LyraExperienceDefinition  
Experience에 대한 정의 클래스
- LyraUserFacingExperienceDefinition  
실제로 모드 변경을 진행할 때 사용 하는 UserFacingExpeirence에 대한 정의 클래스.  
Map, Experience 등이 포함되어 있어서, 이를 변경하여 실제 모드 변경을 진행함. => 변경할 때 맵, Experience 등을 전달.
- ExperienceManagerComponent  
 Experience 정보를 담고 관리하는 클래스.  
 Experience를 Load하고, CurrentExperience로 가지고 있음.  
 => 해당 컴포넌트는 GameState의 컴포넌트로 붙어 있음. (소유)
- BP_TeleportToUserFacingExperience  
내부적으로 UserFacingExperienceDefinition을 가지고 있으며, Player와 Overlap되면 UserFacingExperience를 이용해 Hosting Request를 생성하고, CommonUser 플러그인의 CommonSeesionSubsystem의 HostSeesion을 호출하여 Map Load를 진행함.

