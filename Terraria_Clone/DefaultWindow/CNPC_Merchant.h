#pragma once
#include "CNPC.h"
class CNPC_Merchant : public CNPC
{
public:
	CNPC_Merchant();
	virtual ~CNPC_Merchant();

public:
	void	Initialize()	override;
	int		Update()		override;
	void	Late_Update()	override;
	void	Render(HDC hDC)	override;
	void	Release()		override;

	void	Status_Check()	override;
	void	Move_Frame()	override;
};