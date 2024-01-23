# Character

## ���

- Character Ŭ����  
GameFramworkComponent�� ������ ������ ���� => ��� ����  
ex) PawnExtensionComponent => GameFeature�� InitState�� Ȱ���ϴ� Component�̸� �ٸ� ������Ʈ���� �ʱ�ȭ, ���� ���߱⸦ ���.  
ex) HeroComponent => GameFeature�� InitState�� Ȱ���ϸ� Input, Camera ���� �ʱ�ȭ, ���ε� ���� ����ϰ� ����.
- PawnData Ŭ����  
Character�� ����� �����͸� ������ �ִ� Ŭ����.  
UPrimaryDataAsset�� ��� ���� Data Ŭ�����̱� ������ � ��Ȳ������ ���� PawnData�� �ٲ㰡�� Character�� ���� ������ ������ �� ����. => UserFacingExperience�� PawnData���� ������ ����. (��� ����� �� ���� ����)  
���ο��� PawnClass, DefaultCameraMode, InputConfig, AbilitySets ���� ����.

## Character Class
������ �÷����ϰ� �Ǵ� Character�� ���� ���εǴ� Class��.  
�ش� Ŭ������ �����ִ� ������Ʈ, �پ� �ִ� ���� ���͵��� ��ȣ�ۿ� ������ Character�� �����ϰ� ��.

���ο� PawnExtensionComponent, HeroComponent ���� ������Ʈ�� ������.   
=> �̴� GameFeature�� InitState�� Ȱ���Ͽ� ���������� �ʱ�ȭ�� ����Ǵ� ������Ʈ����.

### PawnExtensionComponent
=> ���� �Ŀ� �߰������� ����.


### HeroComponent
=> ���� �Ŀ� �߰������� ����.

## PawnData Class
ĳ���Ͱ� ����� �����͸� ������ �ִ� Ŭ����.  
UPrimaryDataAsset�� ��� ���� Data Ŭ������.  

���ο�
- PawnClass => ����� ���� ĳ���� Ŭ����
- DefaultCameraMode => �⺻ ī�޶� ���
- InputConfig => ����� InputConfig
- AbilitySets => AbilitySystem�� ActivatableAbilities�� �� AbilitySets

�� ������ ����.

PawnData�� �����Ͽ� �ս��� ĳ����, ī�޶� ���, �Է� ���, Ȱ��ȭ ������ �����Ƽ ���� ������ �� ����.

=> Experience�� �� �ִ� ������ �� �ϳ���.
��, Experience�� �����ϸ� ���� ĳ����, �Է� ��� ���� ������ �� �ִٴ� ��.



