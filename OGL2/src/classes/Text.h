#ifndef TEXT_H
#define TEXT_H

//OpenGL Header Files
#include "./globals.h"
#include "./texture.h"

#define	DISPLAY_LINES			40.0
#define	STRING(str_number)		(str_number-1)/DISPLAY_LINES

#ifndef _WIN32						// �������������� ��� ��������
typedef struct _ABC { 
  int     abcA; 
  UINT    abcB; 
  int     abcC; 
} ABC, *PABC;
#endif

namespace Text
{
	enum	Align
	{
		left,						// ������������ �� ����������� �� ������ ����
		center,						// �� ������ (h/v)
		right,						// �� ������� ����
		top,						// �� ��������� �� �����
		bottom						// �� ����
	};
	enum	RESULT
	{
		OK,
		FILE_NOT_EXIST,
		BAD_FORMAT
	};
};
struct	LFontGlyph_t
{
	float	A;			// �������� �� �������
	float	B;			// �������� ����� �������
};
	class Font
	{
	protected:
//		bool			list_builded	;	// ���� ������� ��������� �������
		Text::Align		ha				;	// �������������� �����������
		Text::Align		va				;	// ������������
		GLuint			Base			;	// ������ ������� � ���������
		Vector3f		Color			;	// ���� ������
		float			alfa			;	// ������������
		LFontGlyph_t	lfg[256]		;	// ������ � ����������� � ����� ������
		double			Size			;	// ������ ������ (�������������)
	TexturePointer		T				;	// �������� ������
	static	GLdouble	scr_width		;	// ������� �� ����������� ������ ������
	public:
		Font(void);
		~Font(void);
static	void	SetAspectRatio();			// ��������� ����������� ������ ������
		void	Print	(GLdouble X, GLdouble Y, const char *fmt, ...);	// �������� �����
Text::RESULT	LOAD	(const char *name);	// ������� �����
		void	ULOAD	();
		void	SetStyle(Text::Align halign, Text::Align valign, double size);
Text::Align		GetHA	(void)	{return ha;};
Text::Align		GetVA	(void)	{return va;};
		double	GetSize	(void)	{return Size;}
		void	SetColor(Vector3f C);
		void	SetColor(float r,float g,float b);
		void	SetColor(Vector3f C, float alfa);
		void	SetColor(float r,float g,float b, float alfa);
		float	GetStrWidth(const char	*	str);
	};
typedef Font* PFont;

#endif	//#ifndef TEXT_H