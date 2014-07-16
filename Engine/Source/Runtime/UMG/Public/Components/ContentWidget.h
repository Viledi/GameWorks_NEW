// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ContentWidget.generated.h"

UCLASS(Abstract)
class UMG_API UContentWidget : public UPanelWidget
{
	GENERATED_UCLASS_BODY()

protected:

	// UPanelWidget
	virtual UClass* GetSlotClass() const override;
	// End UPanelWidget

protected:
	UPanelSlot* GetContentSlot() const;
};
