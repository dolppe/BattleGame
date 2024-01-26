# AssetManager

기본적으로 Unreal Engine에서 제공해줌. 필요 시 원하는 AssetManager를 구현한 후 이를 적용하게 할 수 있음.

역할: 에셋(리소스)의 로딩과 캐싱을 담당.  
- 메모리에 에셋을 올려두고, 이를 캐싱해서 가지고 있거나 언로드하는 등 관리함.

기존에 많이 사용하는 StaticConstructor => 에셋(리소스)와 관련이 없으면 이를 사용해도 무방함.

## 로딩

SynchronousLoad => 동기적으로 로딩하는 것 (정적로딩)  
동기적으로 진행하기에 맵 이동시나 초창기에 사용하면 좋은 로딩
- 오래 걸리는 경우 동적로딩 사용하는게 좋음.

로딩시 StaticFindObject를 통해 이미 있는 지 확인하기에 중복 로딩은 일어나지 않음.

## 캐싱

GetAsset => BP Class나 Object 등을 로딩하여 캐싱
GetSubClass => 클래스 자체를 로딩하여 캐싱

## 특징

에셋 매니저는 ENGINE에 있는 UEngine 클래스의 멤버 함수임.  
=> 싱글톤과 같이 딱 하나만 존재함.
- 일반적인 싱글톤 => 글로벌에 스태틱 멤버 변수를 넣는 것.
- 에셋매니저 엔진에 있는 것을 리턴하면 됨.


이를 활용하여 인스턴스 관리시에 에셋의 Load, unLoad 시점, 정책을 잘 관리하면 됨.  
ex) 맵 변경시 로딩 / 여러번 나눠서 로딩 / 주변의 맵 크기에 따라서 로딩, 언로딩 하는 등의 최적화 가능.



## PrimaryDataAsset

PrimaryDataAsset만 로딩 진행함.

PrimaryDataAsset => 정말 사용하고자 하는 에셋만 모아둔 것
UDataAsset => 사용이 될지 안될지 모름.

따라서 PrimaryDataAsset만 취급함.

- PrimaryDataAsset  
DataAsset에 여러 기능을 추가했지만 주된 것은
    - GetPrimaryAssetId(), Asset Bundle에 대한 구현이 추가된 DataAsset임.
=> 이러한 기능은 AssetManager를 통해 load/unload 관련 메서드 호출로 활성화 할 수 있음.

- PrimaryAssetId
    - AssetManager에서 PrimaryAsset들을 찾을 수 있도록 하는 Id, Type과 같은 느낌이라고 생각하면 됨.
    - GetPrimaryAssetId를 통해 AssetId를 가져올 수 있음.

- GetPrimaryAssetID  
 Class에 대한 정보를 받으면, 이에 해당하는 PrimaryAssetId를 찾아서 리턴해줌. 이때, 제대로 된 PrimaryAssetId를 찾기 위해서 몇가지 규칙을 지키는 것이 좋음.
    - CDO를 넘겨야 함.
    - 해당 CDO는 UPrimaryDataAsset을 상속 받은 클래스를 다시 상속 받은 구체적인 클래스의 COD가 필요함.  
    - ex) 가장 일반적인 상황의 예시를 살펴보면,  
    UPrimaryDataAsset => 이를 상속 받은 NativeClassA
    NativeClassA => 를 상속 받은 구체적인 클래스나 Instance  
    ex) UprimaryDataAsset -> UExperienceDefinition -> BP_DefaultExperience  
    와 같은 구조가 일반적이다.  

 위와 같이 설정하는 이유 => GetPrimaryAssetId 내부 구조를 보면알 수 있음.
  - 내부 구조요약  
  1. 전달 받은 클래스가 CDO 인지 확인.
  2. CDO라면, 두 가지 경우를 찾아서 리턴  
    - Native C++ Class를 만나면, 이를 가져옴.
    - 최상위 BP Class를 만나면, 이를 가져옴.
    만날 때 까지 계속 상위 클래스를 체크함.  
이를 진행하는 의도  
    - 결국 찾고자 하는 것은 UPrimaryDataAsset을 상속 받은 클래스의 구체적인 클래스임.  
    - 이를 찾는 이유는 UPrimaryDataAsset을 상속받은 클래스만으로는 의미가 없기 때문.  
    해당 클래스는 정의만 존재하고, 데이터는 채워지지 않은 상태이기 때문임.
    ex) ExpereinceDefinition 클래스 자체로는 데이터가 없고 정의만 있음.  
    따라서 이를 상속 받아서 데이터를 넣은 상태인 클래스를 찾는 것임.

즉, UPrimaryDataAsset을 상속 받은 클래스가 있을 때, 이를 상속 받아서 데이터를 넣은 구체적인 클래스가 존재해야 Data가 있기에 이를 찾는 것이 목적이 되는 것.

이를 위해서 Experience Loading을 진행할 때, BP_DefaultExpereince를 통해서 CDO를 가져와서 이를 통해 PrimaryAssetId를 가져오는 것임.
    



