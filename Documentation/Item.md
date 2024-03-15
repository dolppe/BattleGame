# Item

Lyra의 Item 구현 구조에 대한 설명

## 사용 클래스 설명

- InventoryItemDefinition  
    InventoryItemFragment에 대한 배열을 가지고 있음.  
    Fragment는 InventoryItem에 대해 Actor-Component의 구조와 유사하게 특성을 부여해주는 것임.  
    - ex) Fragment_EquippableItem의 경우에는 해당 InventoryItem이 장착이 가능하다는 특성을 부여해주는 것이며, 이를 적용하기 위해 해당 Fragment 내부에는 EquipmentDefinition이 존재함.
    - ex) Fragment_SetStats는 해당 InventoryItem이 Stat을 가지고 있다는 특성을 부여해주는 것이며, 해당 Fragment 내부에는 총의 경우에는 총알 최대치, 데미지 등을 넣어서 구현할 수 있다.  

    이러한 예시와 같이 Fragment를 사용하여 InventoryItem에 특성을 부여할 수 있다.  
    또한 Fragment를 배열로 가지고 있어서, 여러가지 특성을 동시에 부여할 수 있다.
- InventoryItemInstance  
    말 그대로 InventoryItem의 Instance 클래스임.
    메타 데이터 같은 느낌으로 InventoryItemDefinition을 들고 있어서 해당 아이템이 어떻게 정의된 것인지 알 수 있음.  
    - 이는 InventoryManagerComponent에 의해 생성되고, InventoryManagerComponent 내부에 캐싱된다.
    - 또한, QuickBarComponent의 Slot에 들어가게 된다면, QuickBarComponent 또한 이를 캐싱해서 가지고 있게됨.  
    => 이때 해당 아이템에 대한 장착시도를 할 경우, Definition의 Fragment를 살펴봐서 Fragment_EquippableItem이 있는지 체크하여 장착 여부를 확인하게 된다.
- InventoryManagerComponent  
    ActorComponent를 상속 받았지만, PlayerController의 Component로서 동작하며 Inventory를 관리함.  
    InventoryItemInstance에 대한 List를 가지고 있으며, Add된 InventoryItemInstance를 해당 List에 캐싱해서 관리함.
    - InventoryItemDefninition을 입력으로 하여 InventoryItemInstance를 생성하는 역할을 한다.  
    이때 생성하는 과정에서 해당 Definition의 Fragment의 OnInstanceCreated를 호출함.
- InventoryFragment_EquippableItem  
    InventoryFragment를 상속 받은 클래스들 중 하나이며, 해당 InventoryItem은 장착 가능하다는 특성을 부여한다.  
    이를 위해 EquipmentDefinition을 내부에 가지고 있다.
- EquipmentDefinition  
    장착 아이템에 대한 메타 클래스임.
    - EquipmentInstance 클래스를 가져서, 어떤 Instance를 생성할지를 가지고 있음.
    - EquipmentActorToSpawn이라는 클래스를 배열로 가져서, 어떤 Actor가 어떤 Socket에 부착되고, 그때의 Transform은 무엇인지 가지고 있음.  
    
    중요한 점은 하나의 Equipment에 실제로 Spawn되고 장착될 Actor는 여러가지 일 수 있다는 점이다.
    EquipmentInstance:EquipmentDefinition:SpawnActor 의 비율이 1:1:*이다.  
    이를 통해 여러 종류의 장착물이 한번에 부착되어야하는 Equipment에 대한 처리를 진행할 수 있다. (소켓, Transform도 각각 다 다르게)
- EquipmentInstance  
    EquipmentManagerComponent를 통해서 Equipment가 생성될 때, 실제로 생성되는 Equipment에 대한 Instance 클래스임.  
    - 어떤 InventoryItemInstance에 의해 활성화 됐는지를 Instigator로 가지고 있음. 이는 QuickBarComponent에서 InventoryItemInstance을 사용하여 Equipment 장착을 시도할 때 설정됨.
    - EquipmentDefinition에는 실제로 Spawn될 Actor와 그에 대한 정보들을 가지고 있음. EquipmentInstance는 해당 Actor가 실제로 Spawn 됐을 때 이를 캐싱해서 가지고 있음.
    - 특이한 점으로, 실제 Actor의 Spawn을 진행하는 것은 EquipmentInstance에서 관련 함수로 진행함. (전체 실행 구조 자체는 EquipmentManagerComponent에 있긴함.)
    - 해당 Equipment가 장착되고, 해제될 때에 대한 메서드를 가지고 있음. 해당 로직은 BlueprintImplementableEvent로 구현되어 있어서 주로 BP에서 처리해야함.
- EquipmentManagerComponent  
    Pawn의 Component로서 동작하며, Equipment에 대한 관리를 담당함.  
    장착한 Equipment에 대해 EquipmentDefinition과 EquipmentInstance를 모두 캐싱해서 List로 관리함.
    - EquipmentDefinition을 입력으로 하여 장착을 진행함.  
    해당 과정에서 EquipmetInstance를 생성하여 캐싱해두고, 이를 활용하여 Definition의 스폰해야할 Actor들을 실제로 Spawn하도록 함.  
    추가적으로 해당 EquipmentInstance는 QuickBarComponent에서도 캐싱해서 가지도록 세팅되어 있음.
- QuickBarComponent  
    Controller의 Component로서 동작하며, InventoryItem과 Equipment의 브릿지 역할을 하게 되는 컴포넌트임.  
    Slot이라는 InventoryItemInstance의 Array를 가지고 있으며, 여기에 실제 InventoryItemInstance를 캐싱해서 가지고 있으며, 이를 통해 아이템의 소유, 장착 등을 관리함.  
    또한 Slot에 있는 InventoryItem을 한번에 하나씩만 활성화하여 장착할 수 있는데, 해당 장착물에 대해서는 EquipmentInstance를 캐싱해둠.
    - 원하는 Slot에 대해서 SlotIndex와 InventoryItemInstance를 넣어서 Item을 소유할 수 있음.
    - 하나의 Slot을 정해서 해당 칸에 있는 InventoryItemInstance를 장착할 수 있음.  
    이를 진행하면, 우선 InventoryItemInstance의 Fragments를 확인하여 InventoryFragment_EquippableItem이 있는지 확인하고, 존재하면 내부의 EquipmentDefinition을 활용하여 Equip을 진행함.  
    Equip 자체는 EquipmentManagerComponent를 통해서 진행하며, 진행이 완료되면 EquipmentInstance를 캐싱하고, 해당 Instance의 Instigator를 해당 Slot(InventoryItemInstance)으로 설정함.


Lyra는 위의 클래스를 활용하여 전체적인 Item, Equipment에 대한 틀을 설정해 두었음.

## 구조 설명

InventoryItem과 Equipment를 살펴보면, 유사한 구조가 보이게 된다.

- Definition-Instance의 관계  
    InventoryItem과 Equipment 모두 Definition 클래스와 Instnace 클래스를 가지고 있다.  
    - Definition 클래스의 역할은 대상에 대한 메타 데이터의 역할을 가진다.  
    Equipment의 경우에는 Equipment를 장착하고, Spawn하기 위해 필요한 정보들을 가지고 있다.  
    InventoryItem의 경우에는 Item을 표현하기 위한 Fragments를 가지고 있다.  
        - 이는 장착, Add가 진행될 때도 Instance되지 않으며, 장착, Add를 진행하기 위해 사용된다.
    - Instance 클래스의 역할은 대상에 대해서 실제 Instance되는 역할을 가진다.
    Equipment의 경우에는 어떤 InventoryItemInstance에 의해 활성화 됐는지와 실제로 Spawn되어 부착된 장착물 Actor들을 캐싱해서 가지고 있으며, 실제로 Equip이 진행될 때 해당 Instance가 생성되게 된다.
    InventoryItem의 경우에는 메타 클래스인 Definition을 가지고 있으며, 실제로 InventoryItem이 생성될 때 해당 Instance가 생성되게 된다.
        - 이는 장착, Add가 될 때 Instnace되며, 필요한 정보를 캐싱해서 가지고 있음.
- ManagerComponent의 구조  
    InventoryItem과 Equipment 모두 ManagerComponent를 가지며, 구조상으로 동일한 특징을 가진다.  
    - Entry(구조체), List(구조체), ManagerComponent(클래스)로 구성된다.
    - Entry는 대상의 객체 하나 단위를 나타내며, 여기에는 Instance나 Definition이 속해있음.
    - List는 그러한 Entry를 배열로 관리하며, AddEntry의 형태로 추가하도록 함. 또한 ManagerComponent를 캐싱해두고 필요한 곳에 사용함
    - ManagerComponent는 List를 가지며, 생성자 단계에서 해당 List를 초기화하며 생성함. 또한 초기의 Add, 장착의 명령이 들어오면 해당 List를 활용하여 Entry를 추가하고, 값을 설정하도록 함.

    Equipment, InventoryItem 모두 위의 구조를 가지고 있음.


## 실제 사용 예시

Pistol을 장착하는 예시를 들어보자.

- BP_WeaponInstance_Pisto (EquipmentInstance)
    - AnimationLayer (이는 WeaponInstance에서 추가된 것이긴 함.)
- BP_Pistol (Actor, Equipment에서 부착될 Actor)
    - MeshComponent
- ID_Pistol (InventoryItemDefinition)
    - Fragments (EquippableItem) => ED_Pistol(EquipmentDefinition)
- ED_Pistol (EquipmentDefinition)
    - InstancedType => BP_WeaponInstance_Pistol (EquipmentInstance)
    - ActorToSpawn => BP_Pistol (Actor)

위와 같은 구조로 서로 연결시켜두었으며, 해당 Weapon을 장착하기 위해서는 다음과 같은 과정이 필요하다.

1. InventoryDefinition을 PlayerController의 InventoryManagerComponent에게 넘기며, InventoryInstance를 생성함.
2. 생성된 InventoryInstance를 PlayerController의 QuickBarComponent에게 넘기며, Slot에 캐싱해둠.
3. 이후 QuickBarComponent에게 무기 장착 명령이 오면, Slot에 캐싱해둔 InventoryItemInstance의 Fragment(Equippable)를 활용하여 EquipmentDefinition을 가져오고, 이를 PawnComponent의 EquipmentManagerComponent에게 전달하여 장착을 진행함.
4. 장착의 결과로, EquipmentInstance, SpawnActor가 생성되고, Pawn에게 부착되게 됨. EquipmentInstance는 EquipmentManagerComponent, QuickBarComponent 두개 모두에 캐싱된다.
5. Equip이 진행되며 EquipmentInstnace의 OnEquipped 이벤트가 호출됨. (장착 진행중에)





