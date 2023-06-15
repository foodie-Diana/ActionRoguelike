

#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	
	Health = 100;
}


bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;
	// 用于判断操作是否成功完成，如角色死亡、特殊机制等情况可能失败
	// 目前先返回true

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);

	return true;
}

