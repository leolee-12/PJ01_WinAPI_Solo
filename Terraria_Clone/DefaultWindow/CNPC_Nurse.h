#pragma once
#include "CNPC.h"

class CNPC_Nurse : public CNPC

{
public:
	CNPC_Nurse();
	virtual ~CNPC_Nurse();

public:
	void	Initialize()	override;
	int		Update()		override;
	void	Late_Update()	override;
	void	Render(HDC hDC)	override;
	void	Release()		override;

	void	Status_Check()	override;
	void	Move_Frame()	override;
};