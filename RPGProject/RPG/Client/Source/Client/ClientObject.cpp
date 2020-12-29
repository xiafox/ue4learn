// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientObject.h"

UClientObject::UClientObject()
{

}

UClientObject::~UClientObject()
{

}

void UClientObject::PostInitProperties()
{
	Super::PostInitProperties();
}

void UClientObject::BeginDestroy()
{
	Super::BeginDestroy();
}

void UClientObject::ShutdownAfterError()
{
	Super::ShutdownAfterError();
}
