// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "SAttributeComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);
	
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	WalkSpeed = 1000.0f;
	
}


//MoveForward(Backward)
void ASCharacter::MoveForward(float value)
{

	FRotator ControBotl = GetControlRotation();
	ControBotl.Pitch = 0.0f;
	ControBotl.Roll = 0.0f;

	AddMovementInput(ControBotl.Vector(), value);
}

//MoveRight(Left)
void ASCharacter::MoveRight(float value)
{
	FRotator ControlBot = GetControlRotation();
	ControlBot.Pitch = 0.0f;
	ControlBot.Roll = 0.0f;

	FVector Rightvector = FRotationMatrix(ControlBot).GetScaledAxis(EAxis::Y);

	AddMovementInput(Rightvector, value);
}


// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	// 设置角色的移动速度
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	OriginalWalkSpeed = WalkSpeed;
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);

	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);

	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ASCharacter::Attack);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASCharacter::StartSprinting);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASCharacter::StopSprinting);


}


void ASCharacter::StartSprinting()
{
	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = 20000.0f;
}
// 当 Shift 键释放时调用，恢复原始的最大行走速度值
void ASCharacter::StopSprinting()
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = OriginalWalkSpeed;
}

void ASCharacter::Attack()
{
	PlayAnimMontage(AttackAnim);
	// 0.38s延迟后触发魔法粒子生成
	GetWorldTimerManager().SetTimer(TimerHandle_Attack, this, &ASCharacter::Attack_TimeElapsed, 0.18);

}

void ASCharacter::Attack_TimeElapsed() 
{
	// 获取模型右手位置
	FVector RightHandLoc = GetMesh()->GetSocketLocation("Muzzle_01");

	// 获取相机位置和朝向
	FVector TraceStart = CameraComp->GetComponentLocation();
	FVector TraceEnd = TraceStart + (GetControlRotation().Vector() * 5000);

	// 检测半径
	FCollisionShape Shape;
	Shape.SetSphere(20.0f);

	// 不要检测自己角色
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	// 碰撞设置
	FCollisionObjectQueryParams ObjParams;
	ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjParams.AddObjectTypesToQuery(ECC_Pawn);

	FHitResult Hit;
	if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params)) {
		TraceEnd = Hit.ImpactPoint;
	}

	// 尾向量 - 头向量 = 方向向量 eg：起点(0,0) 终点(1,1)，方向向量为(1,1)
	FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - RightHandLoc).Rotator();
	// 朝向检测到的落点方向，在角色的右手位置生成
	FTransform SpawnTM = FTransform(ProjRotation, RightHandLoc);

	// 此处设置碰撞检测规则为：即使碰撞也总是生成
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);

}
