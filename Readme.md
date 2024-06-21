# LyraBattleRoyalGame

언리얼 엔진 5의 Lyra 샘플 프로젝트를 분석하여 이를 기반으로 만든 배틀로얄 게임.

Lyra 샘플 프로젝트에 대한 분석도 겸하여 진행.

## 



## Documentation Structure

```sh
Documentation  
 ┣ 게임 설정 관련                 -> 만들고자 하는 배틀로얄 게임에 대한 설정  
 ┃ ┃ ┗ GameRules.md              -> 게임의 규칙  
 ┃ ┃ ┗ GameStructures.md         -> 게임의 구조  
 ┃ ┗ ┗ GameSystems.md            -> 게임에 존재하는 시스템, 기능  
 ┣ AssetManager.md               -> Lyra의 AssetManager에 대한 분석  
 ┣ Camera.md                     -> Lyra의 Camera에 대한 분석  
 ┣ Character.md                  -> Lyra의 Character에 대한 분석  
 ┣ Experience.md                 -> Lyra의 Expereince에 대한 분석  
 ┣ GameFeature.md                -> GameFeatrue에 대한 분석.  
 ┣ GameplayFramework.md          -> Lyra의 GameplayFramework에 대한 분석(GameFeature, Action, InitState 등)  
 ┣ GameplayTags.md               -> Lyra의 GameplayTags에 대한 분석  
 ┣ Input.md                      -> Lyra의 InputSystem에 대한 분석  
 ┣ Item.md                       -> Lyra의 Item 관리에 대한 분석 (Inventory, Equipment 등)   
 ┗ LyraClassDiagram.mdj          -> 분석한 내용을 토대로 작성한 Lyra Class Diagram  
```


### ClassDiagram



## Requirements

- Unreal Engine 5 (v5.1)
- Data Version Control (https://dvc.org/)

## install

-  Git Clone
    ```sh
    git clone https://github.com/dolppe/LyraBattleRoyalGame.git
    ```

- DVC 적용 (Content 폴더 가져오는 방법)
    - DVC Install  
    https://dvc.org/doc/install

    - DVC 적용 (git이 적용된 상태에서 가능.)
    ```sh
    dvc pull
    ```

- Unreal Engine Version 세팅
    - LyraBattleRoyalGame.uproject 파일에 우클릭 하여 Swith Unreal Engine Version 선택.
    - 5.1 버전 선택 => Generate Project File 진행.
    - 이후 LyraBattleRoyalGame.uproject을 더블 클릭하고, rebuild 진행. (기다리면 자동 실행됨.)  
    => 혹은 직접 빌드하여 실행하는 것도 가능.

    



모든 문서, 파일은 계속 업데이트가 진행되며 내용이 추가 및 변경될 수 있음.
