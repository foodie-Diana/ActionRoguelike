

#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	
	Health = 100;
}


bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;
	// �����жϲ����Ƿ�ɹ���ɣ����ɫ������������Ƶ��������ʧ��
	// Ŀǰ�ȷ���true

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);

	return true;
}

