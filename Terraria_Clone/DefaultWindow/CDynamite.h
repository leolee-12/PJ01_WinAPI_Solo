#pragma once
#include "CBullet.h"

class CDynamite : public CBullet
{
public:
	CDynamite();
	virtual	~CDynamite();

	void	Initialize()				override;
	int		Update()					override;
	void	Late_Update()				override;
	void	Render(HDC hDC)				override;
	void	Release()					override;
	void	Move_Frame()				override;
};