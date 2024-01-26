# GameplayTag

계층적으로 구성된 태그임.
- GameFeature, GameplayAbilitySystem, Input, Animation 등 많은 곳에서 활용 가능함.  
ex) 특정 Ability를 활성화 하기 위해서, 어떤 입력이 들어와야 하는 상황에서 이 둘을 연결해주는 것을 GameplayTag가 할 수 있음.  
=> A 입력이 들어오면, A GameplayTag를 활성화 시킨다, A GameplayTag가 활성화되면 A Ability를 실행한다.  
ex) 특정 GameplayTag가 활성화되면 특정 변수의 값을 true로 바꾼다.

위와 같이 정말 다양하게 많은 용도로 사용할 수 있음.  
주로 GAS, GameFeature과 엮어서 많이 사용함.

- GameFeature Component에서 활용한 InitState의 경우에도 GameplayTags에 직접 등록해서 사용한 것임.

## 사용법

GameplayTag를 사용하기 위해서는 GameInstance의 GameFrameworkComponentManager에 등록하거나 에디터상에서 직접 추가하는 등의 작업이 필요함.

간단하게 나누면 아래의 방법이 존재.
- Native C++로 Tags를 생성하여 GameFrameworkComponentManager에 직접 등록.
- 에디터 상에서 직접 추가


