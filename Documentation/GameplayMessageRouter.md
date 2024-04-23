# GameplayMessageRouter

- GameplayMessage를 보내면, 미리 등록된 Listener에게 데이터를 전달해주는 시스템이며, GameplayMessageRouter Plugin에 구현되어 있음.

## 개요

- 구조  
    - GameplayMessageSubsystem  
        핵심 기능을 가진 클래스임.
        해당 시스템을 활용하여 리스너를 등록하고, 메세지를 Broadcast하는 등의 작업을 할 수 있음.
        - RegisterListener  
            리스너로 등록을 진행.  
            - GameplayTag
            - UScriptStruct => StructType
            - Callback  
            - MatchType
            
            위 4가지만 전달하여 리스너로 등록할 수 있음.  
            - 델리게이트와는 다르게 굳이 먼저 스피커쪽에 무엇인가를 만들고 할 필요가 없음
            - 구독하고자 하는 GameplayTag와 전달 받고자 하는 StructType, MatchType, 전달받고자 하는 콜백함수만 등록해두면 됨.
        - Broadcast  
            메세지 전송 진행.
            - GameplayTag
            - Message

            두 가지만 전달하면, 메세지를 바로 전송할 수 있음.
        - 중요한 점  
            - GameplayTag 사용  
            리스너 등록, 메세지 전송에 GameplayTag를 사용하기 때문에 이의 계층적인 구조를 활용할 수 있음.  
            또한, 미리 스피커에 정의하거나 할 필요 없이, 등록하고자 하면 바로 등록 가능하고, 메세지 전송을 하고 싶으면 바로 할 수 있음.
            - UScriptStruct  
            메세지를 전송할 때 void*를 통해서 데이터를 바로 전송하고, 리스너 쪽에 도착하기 전에 미리 받아둔 ScriptStruct (StructType)을 통해 해당 데이터를 캐스팅함.  
            이를 통해 어떠한 타입의 Struct가 와도 대응할 수  있음.  
            또한 등록할 당시에 Struct를 미리 받아두기 때문에 아예 잘못된 데이터를 전송한 것이 아니면, 캐스팅쪽에 문제 생길일이 없음. (Struct를 비교하는 안전장치가 있음.)
    - AsyncAction_ListenForGameplayMessage  
    이는 Blueprint에서 GameplayMessageSystem을 활용하기 위한 방법임.  
        - 해당 AsyncAction을 Factory Method인 ListenForGameplayMessages를 활용하여 만들기만 하면 바로 사용 가능함.
        - 알아서 활성화되고, 메세지가 오면 델리게이트가 실행되어서 로직이 진행됨.
        - 전송되는 메세지의 타입 또한 void *를 알아서 변환까지 진행하여 전송되기 때문에 신경쓰지 않아도 됨.
        - 주의할 것.  
            - ListenForGameplayMessages함수는 함수 지정자 BlueprintInternalUseOnly를 통해 직접적으로 그래프에서 바로 부를 수 없도록 막혀있음.
            - HasDedicatedAsyncNode 메타 지정자를 통해 AsyncAction에서 자동으로 만들어주는 UK2Node의 생성을 막아둠.
            - 위 두가지를 막아둔 이유는 아래의 K2Node_AsyncAction_ListenForGameplayMessage 때문임.
    - K2Node_AsyncAction_ListenForGameplayMessage  
        이를 사용하는 이유는 딱 하나임.
        - 메세지를 통해 전송되는 PayloadData는 void *로 전송되는데, 이를 직접 타입 변환을 하는 것이 아니라, 노드 내부에서 타입 변환까지 진행하게 하기 위함임.  
        => 또한 변환된 타입을 OutputPin을 통해 나타내기 위함임.
        - 따라서 기존에 자동으로 생성되는 두 Node를 막아버리고, 이를 커스텀 한 K2Node_AsyncAction_ListenForGameplayMessage 노드만 사용하도록 함.  
        - 따라서 이의 내부 함수의 역할은 두 가지임.
            - void형인 Payload Data가 전송되면, AsyncAction_ListenForGameplayMessage의 GetPayload 함수를 호출하여 Struct 형태로 바꾸는 것
            - Output Pin이 Struct 형태에 따라서 변경되도록 하는 것.
        - 추가적으로 기존 AsyncAction_ListenForGameplayMessage과의 연동은 GetMenuActions를 통해서 진행됨.  
            - 이는 내부적으로 AsyncACtion_ListenForGameplayMessage를 전달하여 해당 클래스의 Factory Method를 찾음.  
            => ListenForGameplayMessage가 자기 자신의 객체를 생성하는 팩토리 메서드이기 때문에 이것이 해당 Node에 등록되는 것임.
        - 이러한 과정을 통해 Blueprint에서도 쉽게 리스너를 등록하고, 메세지를 전송할 수 있게 처리함.     



## 사용처

- 사실 기능만으로 보면, 델리게이트와 크게 다를 것 없어 보였음.  
    블루프린트 쪽에서도 델리게이트를 쓰거나 혹은 AsyncAction만 써도 되지 않을까 라는 생각이 들었음.
    - 그러나 유용한 점이 많았음.

- 기능 요약  
    - 리스너가 받고 싶은 GameplayMessage에 해당하는 GameplayTag, 전달 받고 싶은 Struct Type, Message를 받을 Callback 함수를 등록함.
    - 어느 곳이던, GameplayMessage를 보내고 싶으면, GameInstance의 Subsystem인 GameplayMessageSubsystem을 활용하여 메세지를 보낼 수 있음.
    - 전달하고자 하는 Message의 GameplayTag, 전달할 데이터를 Broadcast하면, 등록되어 있는 리스너 중에 해당 하는 리스너들에게 메세지가 전달됨.

- 장점
    - 가장 큰 장점은, GameplayMessage를 전송하는 객체와 GameplayMessage를 받는 객체 사이의 커플링이 생기지 않는 다는 점임.  
        리스너와 스피커는 서로에 대해 알 필요가 없음.  
        - 리스너는 구독하고 싶은 Message의 Tag 타입과, 전달 받을 Struct의 타입을 정해두면, 이에 해당하는 메세지를 받을 수 있음.
        - 스피커는 전달하려는 Message의 Tag와 Struct를 전달하면 알아서 메세지가 전송됨.  
        => 이것들은 사실 델리게이트의 장점과 유사함.
    - GameplayTag를 통해 전달하기에 사용하기 편하다는 장점이 있음.  
    리스너의 입장에서는 GameplayTag, StructType만 지정해주면, 원하는 메세지를 전달 받을 수 있음.  
    스피커의 입장에서는 GameplayTag와 전달하고자 하는 데이터만 전송하면 원하는 메세지를 전달할 수 있음.
        - 이는 델리게이트와 가장 차이가 많이나는 부분임.  
        델리게이트도 사용이 편리하지만, 이는 스피커쪽에서 델리게이트를 미리 정의해두고, 리스너가 이에 대해서 등록을 해야하는 구조를 띄고 있음.  
        => 이와 다르게 GameMessageSystem은 그냥 바로 스피커가 Tag와 함께 Message를 보내면 되고, 리스너는 스피커의 구현을 기다릴 필요 없이 미리 등록을 해두어도 됨.
            - 어차피 GameplayTag를 통해서 연동되기 때문임.  
    - 어떠한 Struct Type이건 하나의 시스템으로 구동 가능함.  
    GameplayMessageSubsystem은 Struct Type을 UScriptStruct로 받기 때문에 어떠한 타입이 와도 대응할 수 있음.  
    => 전송하는 데이터도 void *로 전송하고, 추후에 Struct Type으로 변환하기 때문에 어떠한 타입이라도 다 전송할 수 있음.
        - 델리게이트와 유사하지만 다른 점임.
            - 전송, 전달 받고자 하는 타입을 정의하는 것은 유사함.  
            MessageSystem => 리스너가 StructType(UScriptStruct)로 지정.  
            델리게이트 => 선언 시에 전달할 매개변수 설정.
            - 그러나 사용 방식에 대해서 차이가 존재.  
            MessageSystem => 따로 무엇을 정의할 필요 없이, 리스너가 등록될 때 받고자 하는 StructType을 전달만 해주면 됨. 해당 시스템을 사용하기만 하면 됨.  
            델리게이트 => 스피커쪽에서 보내고자 하는 데이터를 미리 정의해둬야 함.



-  요약  
GameMessageSystem은 델리게이트와 유사하지만, 약간의 차이를 가짐.  
유사한 점
    - 기능적으로 유사함.
    - 따라서 리스너와 스피커 사이의 커플링을 끊을 수 있다는 장점이 유사함.  

    차이점
    - 사용 방식  
    스피커쪽에서 미리 준비하고, 이후 리스너에서 구독해야하는 델리게이트와는 다르게, GameplayTag, UScriptStruct 만을 활용하여 바로 전송 및 등록을 진행할 수 있음.  
    - 편의성  
    델리게이트는 필요한 클래스마다 모두 이를 정의해줘야함.  
    그러나 이와는 다르게 그냥 GameInstance의 Subsystem인 GameMessageSystem을 활용하여 바로 메세지를 전송하고, 리스너로 등록을 진행할 수 있음.  
        - 즉, 델리게이트에서는 전송하고자 하는 스피커를 추가하거나 변경하려면 코드를 건드리거나 하는 추가 작업이 필요함.
        - 이와는 다르게 그냥 메세지를 보내는 것처럼 기존의 메세지를 보내는 발신자와는 다른 발신자가 메세지를 보내거나 해도 문제 없이 도착함.
    - GameplayTag  
        GameplayTag를 활용하기에 이의 계층적인 구조를 그대로 이용할 수 있음.  
        이에 따라서 GameplayTag MatchType을 설정하여 유연한 구현이 가능함.







        

    

