#pragma once

//====== ����� �����
#include "./texture.h"
#include "./globals.h"

class SkyBox				// ������ ����
{
protected:
	TextureClass	t;							// �������� ��� ����
	GLuint	listID;								// ID ������
public:
			SkyBox			();					// �����������
			~SkyBox			();					// ����������
	void	LoadTextures	(					// �������� ������� ����
					ILenum	Type,			// ��� �����
					char 	*NameOfTex		// ������ ��� �������
							);
	void	operator()		();					// ���������� ����
	void	Render			();					// ���� ����� ������ ��� �������
};

extern SkyBox	MSB;							// ������� ������ ����