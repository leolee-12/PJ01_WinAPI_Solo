#pragma once
#include "CNPC.h"

class CNPC_Guide : public CNPC
{
public:
	CNPC_Guide();
	virtual ~CNPC_Guide();

public:
	void	Initialize()	override;
	int		Update()		override;
	void	Late_Update()	override;
	void	Render(HDC hDC)	override;
	void	Release()		override;

	void	Status_Check()	override;
	void	Move_Frame()	override;
};