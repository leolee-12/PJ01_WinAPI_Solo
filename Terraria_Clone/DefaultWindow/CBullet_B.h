#pragma once
#include "CBullet.h"

class CBullet_B : public CBullet
{
public:
	CBullet_B();
	virtual	~CBullet_B();

	void	Initialize()				override;
	int		Update()					override;
	void	Late_Update()				override;
	void	Render(HDC hDC)				override;
	void	Release()					override;
	void	OnCollision(COLMSG _colMSG)	override;
	void	Gravity()					override;
};

