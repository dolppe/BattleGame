# UI

Lyra의 UI는 결국 기본 프레임워크를 구성해 둔 뒤로, GameFeature를 활용하여 관리함.  
GameFeature Plugin마다 필요한 UI가 다르기 때문에 이에 대응하기 위함임.

## 개요

Lyra의 UI는 GameInstance의 UIManagerSubsystem이 관리하며, 이는 UI Policy라는 정책을 바꿈으로서 UI를 관리함.  
또한 LocalPlayer에게 적용되는 UI의 방식은 Layer를 활용하는 방식임.  
메인 UI의 레이아웃을 담당하는 UMG가 있으며, 각 LocalPlayer는 이를 하나씩 가지게 되고, 이는 Layer 단위로 쪼개져서 관리되며 Layer마다 원하는 UI를 쌓아 올려서 관리할 수 있음.

- CommonGame Plugin  
    UI를 관리하기 위한 클래스들을 정의하는 플러그인.
    - CommonGameInstance  
    UI는 LocalPlayer마다 부여되는데, GameInstance가 LocalPlayer를 관리하기 때문에 이를 활용함.
    - GameUIManagerSubsystem  
    GameInstance의 Subsystem으로 등록되며, UI를 전체적으로 관리하는 역할을 가짐.  
        - 내부적으로 GameUIPolicy라는 UI 정책을 가지고 있음. 이를 토대로 UI를 관리하는 것임.
        - UI Policy를 바꾼다면, UI 적용을 바꿀 수 있음.
    - GameUIPolicy  
    LocaPlayer와 RootLayout (PrimaryGameLayout)을 묶어서 관리하고, Default로 적용할 RootLayout 클래스를 가지고 있음.
    - PrimaryGameLayout  
        인게임에서 메인 UI 레이아웃을 담당하는 UMG.  
        로컬플레이어당 하나씩 가지는 최상위 UI 레이아웃.  
        레이어를 기반으로 UI를 관리함.  
        레이어마다 GameplayTag를 부여하여 추후 Layout을 추가할 때 어떤 Layer에 추가할지 지정할 수 있음.
        - 추후 Layer에 해당하는 Tag와 ActivatableWidget을 전달 받아서 해당 Layer에 Widget (Layout)을 추가하는 작업을 진행함.
    - CommonUIExtensions  
    LocalPlayer, LayerId (Tag), ActivatableWidget Class를 전달 받아서 Layout을 생성하는 함수를 가짐.
        - 해당 LocalPlayer의 LayerTag에 해당하는 Layer에 Layout을 추가하는 것임.
    
- CommonActivatableWidget  
    UE에 기본적으로 있는 Widget이며, Layout으로 활용되기 때문에 실제 게임에서 보이는 것은 없으며, 대신 Slot이라는 것으로 실제 Widget이 부착될 위치를 나타냄.  
    정말 Layout이기 때문에 해당 Slot에 Widget이 잘 부착될 수 있도록 도와주는 역할을 하며, 해당 Slot에는 SlotTag를 통해 부착할 수 있도록 함.  
    해당 Widget은 두 가지 특징이 있음.
    - Widget Layout을 정의하는 데 사용됨.  
    Widget Instance와 Widget Layout을 구분하는 용도로 사용됨.   
    따라서 이는 런타임에 WidgetTree에서 제거되는 것이 아니라, 활성화, 비활성화만 진행하는 것임.  
    이를 통해 Widget Instance와는 다르게 오버헤드 없이 추가 제거가 편함. (활성, 비활성 이기 때문에)
    - 사용자의 Input 처리 방법을 정의할 수 있음.  
    해당 Widget이 있을 때 사용자의 Input을 게임에도 적용할 지, UI에만 적용할지, 둘 다 적용할 지 등을 정의할 수 있음.


- Layout과 Widget 구조(GameFeatureAction)  
    Lyra에서 GameFeature Plugin마다 필요한 UI가 다르기 때문에 이를 GameFeatureAction을 통해 필요한 UI를 생성, 제거를 진행함.  
    - GameFeatureAction_AddWidget  
    위젯을 추가할 때 사용되는 GameFeatureAction임.  
    특이한 점은, Layout과 Widget을 따로 받으며, GameplayTag를 통해 적용시키는 것임.  
        - Layout  
        CommonActivatableWidget을 받으며, 이를 Layer Id (GameplayTag)와 함께 전달 받음.  
        이를 통해 LocalPlayer의 RootLayout에 Layer Id에 해당하는 Layer에 해당 Layout 위젯을 생성하여 추가함.  
        이는 실제 게임에서 보이는 것이 없으며, 실제 Widget이 부착될 위치인 Slot을 제공하는 역할을 함.  
        Slot은 SlotTag를 통해서 편의성 있게 제공함.
        - Widget  
        UserWidget을 받으며, 실제로 사용자에게 보여질 Widget을 의미함.  
        Widget과 함께 Slot Id (GameplayTag)를 전달 받아서, 해당 Widget이 어떤 Slot에 부착될 지 정함.  
        이를 통해 해당하는 Layout의 SlotTag에 맞는 Slot에 Widget을 생성하여 추가함.


- UIExtension Plugin  
    UIExtension이라는 개념을 구현하기 위해 만들어진 Plugin.  
    - UIExtension  
     SlotTag, WidgetClass, ContextObject를 가지며, 해당 Slot에 들어갈 WidgetClass와 이를 발동시킨 Instigator로 ContextObject를 가지고 있는 것임.  
     즉, 실제로 Layout의 Slot에 부착될 위젯에 대한 정보를 가지고 있는 것임.
    - UIExtensionPoint  
    UIExtension과 동일하게 SlotTag, WidgetClass, ContextObject를 가지고 있으며, 추가적으로 Callback함수를 가지고 있음.  
    해당 Callback 함수를 통해 Widget을 추가하거나 제거함.  
    이는 UIExtension의 Context를 보전하기 위해 필요함.  
        - UIExtensionPointWidget이 생성(보통 Layout에)되고, Rebuild될 때, ContextObject(nullptr, PlayerState, LocalPlayer)별로 미리 생성되며, 추후에 Widget이 부착되어 UIExtension이 등록될 때, 해당 UIExtension과 동일한 ContextObject를 가진 UIExtensionPoint의 Callback이 실행되며 Widget이 부착됨.
    - UIExtensionPointWidget  
    실제로 Layout의 Slot 역할로서 부착되는 Widget임.  
    UDynamicEntryBoxBase를 상속 받았으며, 이는 동적으로 UI 위젯을 추가하고 제거할 때 사용하기 좋은 클래스임.  
    즉, 해당 Slot에 Widget을 부착하기 위해 사용되는 클래스임.  
        - Layout에 Slot으로서 부착되면, Rebuild가 진행되며 ContextObject별로 미리 UIExtensionPoint를 생성해두고, 추후 UIExtension이 등록되면, 해당하는 UIExtensionPoint의 콜백함수를 통해 Widget을 부착함.
    - UIExtensionSystem  
        WorldSubsystem이기 때문에 자동으로 세팅됨.  
        이는 UIExtension, UIExtensionPoint를 관리하는 역할을 진행함.
    - 요약  
    UIExtenionPointWidget은 Layout의 Slot 역할로서 실제로 부착되는 Widget임.  
    UIExtensionPointWidget은 부착된 Layout이 생성될 때 Rebuild를 진행하며, 미리 ContextObject별로 UIExtensionPoint를 생성하여 캐싱해둠.  
    나중에 실제 사용자에게 보여질 Widget에 해당하는 UIExtension이 UISubsystem을 통해 등록되면, 생성된 UIExtensionPoint 중 등록된 UIExtension과 같은 ContextObject를 가진 UIExtensionPoint의 Callback함수가 실행됨.  
    해당 Callback 함수를 통해 실제 Widget을 생성하고, Slot에 부착함.

## UI 적용 흐름

- UI는 GameInstance의 Subsystem인 GameUIManagerSubsystem이 관리하며, 이는 내부에 UI 정책을 관리하는 UIPolicy가 존재함.
- UIPolicy는 LocalPlayer가 추가될 때, 해당 LocalPlayer가 가질 Default RootLayout을 생성하여 둘을 묶어서 관리함.
- GameFeature Plugin이 활성화될 때, GameFeatureAction이 실행되며, 이중 GameFeatureAction_AddWidget이 존재한다면, 이에 해당하는 Widget 추가를 진행하게 됨.
- 이때 Layout과 Widget을 전달받아서 생성을 진행함.
- 먼저 Layout이 추가되는데, 이는 전달받은 Tag에 맞는 RootLayout의 Layer에 해당 Layout이 추가됨.
- Layout이 추가되면서, 해당 Layout의 Slot 역할을 하는 UIExtensionPointWidget이 Rebuild를 진행하며 초기화를 진행함.
- UIExtensionPointWidget은 UIExtensionSystem을 활용하며 ContextData만 다른 UIExtensionPoint를 총 3개 생성함. (nullptr, PlayerSTate, LocalPlayer)
- 이후 Widget 추가를 진행하며 UIExtensionSubsystem을 통해 UIExtension을 생성함.
- 생성된 UIExtension과 같은 ExtensionPoint(ContextData, SlotTag)를 찾고, 이의 Callback 함수를 실행함.
- 이를 통해 Widget을 해당 Slot 위치에 생성하며 Widget 생성을 마무리함.



