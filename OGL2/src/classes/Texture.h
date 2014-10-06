#pragma once
//====== ����� �����
#include "./globals.h"

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>


class TextureClass
{
protected:
	char			TextureFileName[_MAX_PATH];			// ��� ����� � ��������
	int				iHeight, iWidth, Bpp;				// ������, ������ � !����������! �������
	ILenum			FType;								// ��� �������� (JPEG,BMP,PNG...)
	int				size;								// ������ ��������
	unsigned int	type;								// ��� �������� ������
	unsigned char*	data;								// ������ ��������
	int				FilterMode;							// ����� ����������� ��������
public:
	GLuint			Number;								// ������ ��������

	TextureClass				(void);
	~TextureClass				(void);

	void	SetFileName		(const char *Name);			// ������������� ��� ����� ��� ��������
	void	SetFileType		(ILenum Type);				// ��������� ���� �����
	void	SetFM			(int FM);					// ���������� ����� ����������
	char*	GetFileName		(void);						// ������� ����� ����� � ���������
	int		GetNumber		(void)	{return Number;};	// ������ ���������� ����� ��������
	bool	Load			();							// ������ �������� � ������
	bool	Load (int FM,ILenum Type,const char* Name);	// ��� ����� �� � �����������
	bool	Load(const char* Name);						// ����� OIL ��� ���������� ��� � ���������� �� ���������
	bool	LoadL	(ILenum Type, void* Lump, ILuint size);// �������� �������� �� ������

	const int		GetWidth		()
	{	return iWidth;};	// No comments :=)
	const int		GetHeight		()
	{	return iHeight;};

	void	bind			()							// ������������� ��������
	{glBindTexture(GL_TEXTURE_2D, Number);}
};

typedef TextureClass * TexturePointer;

#ifdef ENABLE_TEXTURE_RENDERING

#define	FJC_RTEX_NO_FILTERING			0				// ������ ���������� ��� ������-�������
#define FJC_RTEX_BILINEAR_FILTERING		1

class FJCRendererTexture
{
protected:
	GLuint			Number;								// ������ ��������
	int				iHeight, iWidth, Bpp;				// ������, ������ � !����������! �������
	int				size;								// ������ ��������	(� ������)
	unsigned char*	data;								// ������ ��������
	int				FilterMode;							// ����� ����������� ��������
	char			ScreenShot	[64];					// ��� ��� ������
	unsigned int	ScreenIndex;						// ������ ����� ������
public:
	FJCRendererTexture				(void);
	~FJCRendererTexture				(void);

	void	SetSize			(int x,int y)				// ���������� ������
	{	iHeight = y; iWidth = x;	size = x*y*4; };
	void	SetFM			(int FM)					// ���������� ����� ����������
	{ FilterMode = FM; };
	int		GetNumber		(void)						// ������ ���������� ����� ��������
	{return Number;};

	void	Init			(
								int Width,
								int Height,
								int FilterMode = FJC_RTEX_BILINEAR_FILTERING
							);
	void	Init			();							// ������������� ������� (��������� ������ � ��� �����)
	
	void	CopyBackbufferToTexture();					// ���������� backbuffer � ��������

	void	bind			();							// ������������� ��������

	void	Save			();							// ���������� �������� �� ����
};
extern FJCRendererTexture RendTex;						// ���� ��������� ��� ������
#endif