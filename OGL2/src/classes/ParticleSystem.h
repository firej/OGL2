#pragma once
//====== ����� �����
#include "./globals.h"


class ParticleSystem
{
	Point3f		*Poss;		// �������
	Vector3f	*Vels;		// ��������
	WORD		Particles;	// ���������� ������
public:
	Point3f		wind;		// �����
	Point3f		windSource;
	Point3f		grav;
	bool		windActive;	// ���������� �����
public:
	ParticleSystem();
	~ParticleSystem();
	void	INIT();			// �������������
	void	DRAW();			// ���������
	void	ANIM();			// ��������
	void	SetGravity	(Point3f	g)	// ����������
	{	grav	=	g;	}
	void	SetWind		(Point3f	w, Point3f source)	// �����
	{	wind	=	w;	windSource = source;	}
	void	SetWindStatus	(bool	wA)
	{	windActive	=	wA;	}
};
