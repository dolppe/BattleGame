# GameplayAbilitySystem


## 개요

- GameplayAbilitySystem  
    액터가 소유하고, 발동할 수 있는 GameplayAbility라는 것을 활용하여, 액터간의 인터렉션 기능을 제공하는 프레임 워크.  
    이를 활용하면, 버프 스킬, 데미지 스킬, 데미지 방식은 어떻게 할건지, 이펙트는 어떤것으로 할지를 유연하게 구현 가능함.
    - 장점  
        - 다양하고 복잡한 기능에 대응할 수 있도록 범용적으로 설계할 수 있음.  
        => 다양한 스킬과 같은 것들을 유연하고, 범용성 있게 구현할 수 있음.  
        - 각 기능에 대한 의존성을 줄일 수 있음.
        - 데이터 기반으로 동작할 수 있도록 설계할 수 있음.



- 구성요소
    - AbilitySystemComponent
    - GameplayAbility
    - GameplayCue
    - GameplayEffect
    - Attribute
    - GameplayTag  
        정확히 태그는 GAS 외에도 여러 곳에서 사용됨.  
        그러나 GAS에서 서로 연동되거나 하는 부분에서 잘 사용됨.


## AbilitySystemComponent

- AbilitySystemComponent (ASC)
    - GAS를 관리하는 핵심 컴포넌트  
        GA를 처리하고, Attribute를 관리하는 등 GAS에 관련된 다양한 작업을 관리하고, 처리하는 중앙 처리 장치임.
    - Actor에 부착되는 컴포넌트이며, 한 Actor에 하나만 부착됨.  
        Actor는 부착된 ASC를 통해 어빌리티를 발동시키는 등의 작업을 할 수 있음.  
        또한, ASC가 부착된 다른 Actor와 GAS 시스템에 대한 상호작용을 할 수 있도록 함.


- 부착되는 Actor  
    PlayerState, GameState, Character 등 모든 Actor에 다 부착될 수 잇으나, 일반적으로 State 쪽에 부착하는 경우가 많음.  
    - ASC에는 OwnerActor와 AvatarActor를 지정해줘야함.  
        이때, 캐릭터에게 GAS를 적용하고 싶다면 다음과 같이하는 게 일반적임.  
        OwnerActor => PlayerState  
        AvatarActor => Pawn (Character)  
        - 주의해야 할 점은 초기에 Possess하기 전에는 Pawn이 없기에 AvatarActor에도 OwnerActor로 설정되어 있음.

## GameplayAbility

- GameplayAbility (GA)  
    ASC에 등록되고, 발동될 수 있는 액션 명령.  
    ex) 공격, 마법, 스킬, 특수 공격, 버프 등등
    상황에 따른 복잡한 액션도 수행 가능함.
    - 설정된 옵션에 따라서 Instancing되는 타이밍이 달라짐.  
        ex) NonInstanced => 인스턴싱 없이 CDO에서 처리
        InstancedPerActor => 액터마다 Instancing 진행해서 처리함.
        InstancedPerExecution => 실행마다 Instancing 진행.

- GameplayAbilitySpec  
    GameplayAbility의 Runtime용으로 필요한 데이터를 분리하여 소유하고 있는 객체.
    내부적으로 AbilityHandle, SourceObject, AbilityClass 등을 가지고 잇음.
    - Runtime에 필요한 데이터를 묶어둔 것임.
    - ASC는 Spec 정보를 가지고 있는 것임.  
        => ASC에서 이를 다룰려면 Spec의 Handle 정보를 활용하여 컨트롤하면 됨.
    - GameplayAbilitySpec => 어빌리티에 대한 정보
    - GameplayAbilitySpecHandle => 어빌리티의 인스턴스에 대한 레퍼런스.


- 어빌리티 사용 절차  
    GA를 사용하고자하는 Actor에 등록을 해야 필요할 때 GA를 발동시킬 수 있다.
    - ASC에 GA를 등록  
        ASC의 ActivatableAbilities에 등록을 해야, 사용 가능한 GA로 등록이 되는 것임.  
        GiveAbility를 통해 ASC에 GA를 전달하여 등록함.  
        ActivatableAbilities에 등록되어 있어야 추후 해당 GA를 발동시킬 수 있음.
    - ASC에게 GA를 발동하라고 요청  
        ASC에게 발동시킬 GA에 대한 정보를 전달.  
        ActivatableAbilities에 등록되어 있으면 발동됨.  
        또한 발동된 GA의 Instance에는 발동한 Actor 정보, 실행 정보가 기록됨.  
        SpecHandle, ActorInfo, ActivationInfo 등.
    - GA 내부에서 여러 작업이 진행됨.
        - CanActivateAbility => GA가 발동될 수 있는지 체크
        - ActivateABility => GA가 발동될 때 호출
        - CancelAbility => GA가 취소될 때 호출
        - EndAbility => GA를 마무리할 때 호출
    

- GameplayTag와 연동  
    GA에 GameplayTag를 연동시켜서 유연하게 활용 가능하다.
    - GA가 발동 됐을 때 특정 Tag를 활성화시킬 수 있음.  
    이를 통해 GA가 발동된 이후 진행해야 할 작업을 Tag를 통해 진행시킬 수 있음.
    - Tag가 활성화 됐을 때, GA가 발동하도록 설정할 수도 있음.  
    
    이와 같이 Tag를 통해 GA의 상태에 대해 전달 받거나 GA를 실행시키는 등의 작업이 가능함.  
    이를 활용하면, GA를 활용하면서 생길 수 있는 커플링들을 끊을 수 있음.

- Lyra에서의 활용  
    여러 GA가 존재하지만, 총을 쏘는 GameplayAbility에 대해 예시를 들겠음.  
    - 우선 Lyra에서는 PlayerState가 ASC를 가지고 있고, AvatarActor로 Character가 설정되어 있음.  
    - 총을 쏘는 GameplayAbility가 존재하며 이는 Equipment가 가지고 있음.   
    즉, 총에 Ability가 부여되어 있음.  
    - GA가 부여되어 있는 총을 장착하면, EquipmentManagerComponent를 통해 Character, PlayerState에 접근하여 GA를 등록하게됨.
    - 또한 GA와 사용자의 입력을 GameplayTag를 통해 연동함.
    - InputAction이 활성화되면, GameplayTag가 활성화되고, 이를 통해 GA가 발동됨.
    - GA가 발동되면서 등록해둔 GameplayTag가 활성화되고, 이는 Animation의 Property를 변경시켜서 사격 애니메이션이 발동됨.
    - 또한 GameplayCueTag가 활성화되고, GameplayCue가 발동되며 이펙트가 실행됨.


- AbilityTask (AT)  
    GA의 실행은 한 프레임에 이루어지는데, 시간이 소요되거나 상태 관리가 필요한 작업이 있을 경우 AbilityTask를 활용하면 됨.  
    - 비동기적으로 작업을 수행하고, 끝나면 이에 대한 결과를 받는 것임.
    - 활용 요약
        - AT에 작업이 끝나면 Broadcast할 델리게이트 선언.
        - GA가 발동되면, AT를 생성하고, 델리게이트를 구독.
        - AT를 구동
        - AT의 작업이 끝나면 GA의 콜백함수가 실행되며 마무리함.
        - GA가 EndAbility를 통해 종료함.


- GameplayAbilityTargetActor (TA)  
    GAS에서 물리 판정 처리를 담당함.
    - 타겟을 선정하는 다양한 방법에 대응할 수 있음.  
        ex) Trace 등으로 즉각적으로 타겟 검출하는 방법.
        타겟 범위를 보여주고 사용자의 최종 확인을 거치는 방법.
    - GameplayAbilityTargetData  
        TA에서 판정한 결과를 담는 데이터.  
        HitResult, 시작 지점, 끝 지점 등.
    - TA의 주요 함수
        - StartTargeting
        - ConfirmTargetingAndContinue
        - ConfirmTargeting
        - CancleTargeting
    - TA 활용 요약
        - AT에서 TA를 생성하고, 초기화함.
        - AT가 TA의 타겟팅 결과 델리게이트를 구독.
        - AT가 TA의 생성을 마무리하고, 타겟팅 실행.
        - TA가 타겟팅을 진행하고, 필요한 경우 Confirm받음.
        - TA가 델리게이트로, 최종 타겟 데이터 전달함.
        - AT가 이를 받아서 처리.


## GameplayCue

- GameplayCue  
    시각 이펙트, 사운드와 같은 게임 로직과 무관한 시각, 청각적 기능을 담당함.  
    GameplayCue가 선두에 있는 GameplayTag를 활용하여 쉽게 이펙트를 실행시킬 수 있음.  
    ex) GameplayCue.Weapon.Pistol.Fire  
    이와 같은 Tag를 활용하여 손쉽게 GA와 연동할 수 있음.
    - GAS와는 다르게 GameplayCueManager라는 것이 존재하여, 이를 통해서 GameplayCue를 관리함.

- GameplayCueNotifyData  
    GameplayCueTag, GameplayCue가 위치한 Path, 로딩된 해당 Instance 등 GameplayCue에 대한 정보를 가지고 있음.  
    - GameplayCueManager  
        GameplayCue는 GameplayCueManager를 통해 관리됨.  
        - GameplayCue는 엔진 초기에 미리 스캔해서 로딩을 진행해둠. => 이를 GameplayCueManager에서 진행.  
        GameplayCue에 대한 정보를 설정된 위치에서 스캔을 하여 GameplayCue가 존재하면 이를 캐싱하고, 로딩해둠.  
        이때 Map을 통해 GameplayTag를 Key로, Value를 GameplayCueNotifyData가 담긴 Array에 대한 Index로 설정하여 캐싱해둠.
        - 이 외에도 GameplayCue의 재생을 관리하는 등의 역할을 진행함.

- Lyra에서의 GameplayCue  
    GameplayCue는 엔진 초기에 정해진 구역에 대해서 GameplayCueManager가 스캔을 진행하여 캐싱과 로딩을 진행함.  
    그러나 Lyra는 GameFeature Plugin을 통해서 특정 컨텐츠를 활성화, 비활성화를 진행함.  
    => 따라서 GameFeature Plugin을 통해서 추가되는 GameplayCue는 따로 스캔을 진행 해야함.  
    - 이를 위해 세가지 클래스를 커스텀하여 만들어야 함.
        - GameplayCueManager  
            추가된 GameplayCue의 Path에 대해서 로딩을 진행하는 함수를 구현하여, 원하는 타이밍에 스캔, 로딩할 수 있도록 함.
        - GameFeaturePolicy  
            원래 GameFeature Plugin의 상태를 추적하고, 관리하는 클래스임.  
            이를 활용하여 GameFeature Plugin이 Registering되거나 Unregistering될 때를 모니터링 함. 그때 GameFeatureAction_AddGameplayCuePath가 GameFeature Plugin에 있는 경우, GameplayCue Path를 추가하거나 제거하고, 다시 스캔하고 로딩하도록 함.
        - GameFeatureAction_AddGameplayCuePath  
            스캔을 진행할 GameplayCue의 Path를 등록하는 GameFeatureAction임.  
            이를 통해 GameFeature Plugin이 등록될 때, 스캔을 진행해야 하는 GameplayCue가 담긴 Path 정보를 가져옴.


## AttributeSet

- AttributeSet  
    단일 Attribute 데이터인 GameplayAttributeData의 묶음.  
    - GameplayAttributeData  
        BaseValue => 기본값, 영구히 적용되는 고정 스탯 값을 관리.  
        CurrentValue => 변동값, 버프 등으로 임시적으로 변동된 값을 처리하는데 사용.  

    이를 묶어서 AttributeSet으로 관리함.  
    - 주요함수
        - PreAttributeChange (+Post) => Attribute 변경 전후 호출  
        - PreGameplayEffectExecute (+Post) => GameplayEffect 적용 전후 호출
    - AttributeSet 접근자 매크로  
        Attribute에 대해서 많이 수행하는 기능을 매크로로 만들어서 제공해줌.  
        => ATTRIBUTE_ACCESSORS  
        ex) ValueGetter, PropertyGetter, ValueSetter, ValueIniter
    - ASC는 초기화될 때 같은 Actor에 있는 Attribute타입의 객체를 자동으로 찾아서 등록함.

이를 활용하여 게임에서의 스탯, 버프, 데미지, 디버프 등을 쉽게 구현 가능함.  
ASC를 통해서 Attribute에 대해 서로 다른 액터들이 상호작용 가능.  
Attribute는 GameplayEffect와 함께 동작하도록 구성되어 있음.

- MetaAttribute  
    Attribute 설정을 위해 사전에 미리 설정하는 임시 어트리뷰트.  
    - ex) 데미지를 입을 때, 체력 Attribute를 바로 깎는 것이 아니라, 데미지라는 Attribute를 통해 체력을 감소하도록 설정시키는 것.  
    => 체력은 일반 어트리뷰트, 데미지는 메타 어트리뷰트임.  

    데미지를 사용하는 경우 기획 추가에 유연한 대처가 가능해짐.
    - 무적 기능 => 데미지를 0으로 처리함 (무적인 동안)
    - 실드 기능 => 실드값으로 데미지 값을 낮춤.
    - 콤보 공격시 공격력 보정 => 데미지 계수를 높여서 적용하도록.  

    메타 어트리뷰트 사용시 주의사항.
    - 사실상 일반 AttributeData와 동일하지만, PostGameplayEffectExecute를 통해 메타 어트리뷰트 값이 변경되는 경우 다시 값을 0으로 바꿔줘야함.
    - 즉, 메타 어트리뷰트는 적용 후 바로 0으로 값을 초기화하도록 설정해야함.
    - 또한 리플리케이션에서 일반적으로는 제외시킴.


## GameplayEffect

GameplayCue는 게임 로직에 영향을 주지 않는 시각적이나 청각적인 기능들을 담당함.  
GameplayEffect는 게임에 영향을 주는 기능을 담당함. (게임 데이터를 변경한다는 의미임.)  

- GameplayEffect  
    GE는 Attribute와 함께 동작함.
    - GE의 DurationType  
        Instant => 즉각적으로 한프레임에 Attribute에 적용됨.  
        Duration => 지정한 시간동안 동작.  
        Infinite => 명시적으로 종료하지 않는 한 계속 동작.
    - GameplayEffect Modifier  
        GE에서 Attribute의 변경 방법을 지정.  
        - 적용할 Attribute 지정.
        - 적용 방식 설정. (곱하기, 더하기, 덮어쓰기 등)  

        또한 Modifier 계산 방법을 정할 수도 있음.  
        - ScalableFloat => 실수 (데이터 테이블과 연동 가능)
        - AttributeBased => 특정 Attribute 기반으로 계산
        - CustomCalculationClass => 계산을 진행하는 전용 클래스를 활용
        - SetByCaller => 데이터 태그를 활용하여 데이터 전달  

        Modier 없이 자체 계산 로직 만드는 방법 => GameplayEffectExecutionCalculation을 커스텀 구현.  
    
    GE 또한 BP와 연동해서 사용하는 것이 권장됨.  
    ex) GE 자체를 BP로 제작, 복잡한 계산이나 적용 관련은 C++로 진행.

    - GameplayTag와 연동  
    GE에 GameplayTag를 연동하여, GE가 발동할 때 해당 Tag를 활성화하도록 할 수 있음.  
    이를 통해 GA 등 다른 GAS 시스템과 연동 가능함.  
    이 외에도 특정 태그가 있을 때만 GE가 발동하게 하거나 특정 태그가 활성화시 GE를 발동하지 못하게 하는 등 여러가지 구현 가능.  
    

    
- 기간형 게임플레이 이펙트  
    Instance 타입이 아닌 GameplayEffect를 의미함.  
    Insstance 타입은 한 프레임에 종료되기 때문에 상태를 가질 수 없음.  
    - 그러나, 기간형 GE는 유효 기간동안 태그와 같은 상태를 가질 수 있음.
    - 또한 Stack처럼 중첩되도록 설정할 수 있음.  
    - Current값을 변경하고 추후에 원래대로 돌려놓음으로서 여러 스킬, 버프 등을 구현하기 쉬움.  

    주의사항
    - 일반적으로 기간형 타입은 Current 값을 변경하는 것임.
    - 그러나 Period를 넣어서 주기적으로 발동하게 하면 Base값을 건드리게 됨.

