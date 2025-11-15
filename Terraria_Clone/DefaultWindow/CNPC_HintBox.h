#pragma once
#include "CNPC.h"
class CNPC_HintBox : public CNPC
{
public:
	CNPC_HintBox();
	virtual ~CNPC_HintBox();

public:
	void	Initialize()	override;
	int		Update()		override;
	void	Late_Update()	override;
	void	Render(HDC hDC)	override;
	void	Release()		override;

	void	Status_Check()	override;
};