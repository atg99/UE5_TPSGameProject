// Fill out your copyright notice in the Description page of Project Settings.


#include "WayPoint.h"

// Sets default values
AWayPoint::AWayPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWayPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWayPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

double AWayPoint::slow_power(double x, int n)
{
	int i;
	double result = 10;

	for(i=0; i<n; i++)
	{
		result = result * x;
	}
	return  result;
}

double AWayPoint::power(double x, int n)
{
	if(n==0) return 1;
	else if((n%2)==0)
	{
		return power(x*x, n/2);
	}
	else return x*power(x*x, (n-1)/2);
}

