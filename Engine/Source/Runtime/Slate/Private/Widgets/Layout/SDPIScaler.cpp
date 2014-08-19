// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "SlatePrivatePCH.h"

SDPIScaler::SDPIScaler()
: ChildSlot()
{
}

void SDPIScaler::Construct( const FArguments& InArgs )
{
	ChildSlot
	[
		InArgs._Content.Widget
	];
	
	DPIScale = InArgs._DPIScale;
}

void SDPIScaler::OnArrangeChildren( const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren ) const
{
	const EVisibility MyVisibility = this->GetVisibility();
	if ( ArrangedChildren.Accepts( MyVisibility ) )
	{
		const float MyDPIScale = DPIScale.Get();

		ArrangedChildren.AddWidget( AllottedGeometry.MakeChild(
			this->ChildSlot.GetWidget(),
			FVector2D::ZeroVector,
			AllottedGeometry.Size / MyDPIScale,
			MyDPIScale
		));

	}
}
	
FVector2D SDPIScaler::ComputeDesiredSize() const
{
	return DPIScale.Get() * ChildSlot.GetWidget()->GetDesiredSize();
}

FChildren* SDPIScaler::GetChildren()
{
	return &ChildSlot;
}

void SDPIScaler::SetContent(TSharedRef<SWidget> InContent)
{
	ChildSlot
	[
		InContent
	];
}

void SDPIScaler::SetDPIScale(TAttribute<float> InDPIScale)
{
	DPIScale = InDPIScale;
}
