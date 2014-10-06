#include "LocusAFX.h"
#include "./Text.h"
#include "./resman.h"
#include "../../../ssrc/iff.h"

Font::Font(void)
{
	memset(this,0,sizeof(Font));
	Color	=	Vector3f(1.0f,1.0f,1.0f);
	alfa	=	1.0f;
	Size	=	1.0;
}
Font::~Font(void)
{
		glDeleteLists(Base, 256);
}
void Font::SetStyle(Text::Align halign,Text::Align valign, double size)
{
	this->va	= valign;
	this->ha	= halign;
	this->Size	= size;
}
void Font::Print(GLdouble X, GLdouble Y, const char *fmt, ...)
{
	static char		text[512];	// ����� ��� ����� ������
	//memset(text,0,256);
	va_list    ap;				// ��������� �� ������ ����������
	if (fmt == NULL || IsBadStringPtr(fmt,200))		// ���� ��� ������
		return;					// ������ �� ������
    va_start(ap, fmt);			// ������ ������ ����������
    vsprintf(text, fmt, ap);	// � ��������������� �������� � �������� ����
    va_end(ap);					// ��������� ���������� � ������

glPushAttrib(GL_LIST_BIT|GL_DEPTH_BUFFER_BIT|GL_ENABLE_BIT|GL_CURRENT_BIT);		// ���������� ��������
glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	T->bind();
	glDisable(GL_DEPTH_TEST);							// Disables Depth Testing
	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glPushMatrix();										// Store The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	glOrtho(0,Font::scr_width,480,0,-1,1);				// Set Up An Ortho Screen
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glPushMatrix();										// Store The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix

	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(Color.d.c.x,Color.d.c.y,Color.d.c.z,alfa);// ��������� ����� ������
	glTranslated(X*Font::scr_width,Y*480.0,0);			// Position The Text (0,0 - Bottom Left)
	switch(this->ha)
	{
	case Text::center:
		{
			size_t		strl = strlen (text);
			GLdouble	shift = 0.0;
			for (size_t i = 0 ; i < strl ; i++ )
			{
				shift  +=	lfg	[(unsigned char)text[i]].A +
							lfg	[(unsigned char)text[i]].B;
			}
			glTranslated(-shift/2 * ( float ) Size * 30 / Globals.VP.ScreenStrings ,0.0,0.0);
		}
		break;
	case Text::right:
		{
			size_t		strl = strlen (text);
			GLdouble	shift = 0.0;
			for (size_t i = 0 ; i < strl ; i++ )
			{
				shift  +=	lfg	[(unsigned char)text[i]].A +
							lfg	[(unsigned char)text[i]].B;
			}
			glTranslated(-shift * ( float ) Size * 30 / Globals.VP.ScreenStrings,0.0,0.0);
		}
		break;
	}
	glScaled(Size*30/Globals.VP.ScreenStrings,Size*30/Globals.VP.ScreenStrings,1.0);
	glListBase(this->Base);								// Choose The Font Base
	glCallLists((GLsizei)strlen(text),GL_UNSIGNED_BYTE,text);	// Write The Text To The Screen
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glPopMatrix();										// Restore The Old Projection Matrix
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glPopMatrix();										// Restore The Old Projection Matrix
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
glPopAttrib();											// �������
}
Text::RESULT Font::LOAD( const char*FileName )
{
	iff::tag_t	tagb;								// �������� ����������
	DWORD		sizeb;								// ����� ��� ������ �������
	void	*	texture	=	NULL;					// ����� ��� �������� ��������
	DWORD		tsize	=	NULL;

	FILE * f = fopen (FileName,"rb");
	if (!f)	return Text::FILE_NOT_EXIST;
	tagb.i = iff::GetTAG(f);
	if (tagb.i != ID_FORM)	return Text::BAD_FORMAT;
	sizeb = iff::GetDWORD(f);						// ������ ������

	tagb.i = iff::GetTAG(f);						// ������ ������ �����
	if (tagb.i != iff::FONTFORMATNAME.i)	return Text::BAD_FORMAT;
	do	{
		tagb.i = iff::GetTAG(f);
		if (feof(f)) break;
		if (tagb.i == iff::FONTTEXTURE.i)
		{
			sizeb	=	iff::GetDWORD(f);
			texture	=	new char[sizeb];
			tsize	=	sizeb;
			iff::GetBuffer(f,texture,sizeb);
		}
		else
		if (tagb.i == iff::FONTGLYPHBLOCK.i)
		{
			sizeb = iff::GetDWORD(f);
			iff::GetBuffer(f,this->lfg,sizeb);
		}
		else iff::read_unknow_chunk(f);
	}
	while(!feof(f));
	fclose(f);										// ���� ���������!
// �������� ������ ������
	// �������� ��������
		// �������� ���� � �������� �� ���� � ������ (��� ������ ���� � ����� �����)
	if (!texture)	return Text::BAD_FORMAT;
	T = new TextureClass;
	if (T->LoadL(IL_PNG,texture,tsize))
	{
		delete	T;
		delete	[]	texture;
		return Text::BAD_FORMAT;
	}
	delete	[]	texture;
	// �������� ���������� �������
	Base = glGenLists(256);
	int sym;
	for ( int i = 0 ; i < 16 ; i++ )
	{
		for ( int j = 0 ; j < 16 ; j++ )
		{
			sym = i*16+j+1;
			glNewList(Base+sym,GL_COMPILE);
			glTranslatef(lfg[sym].A,0,0);
			glBegin(GL_QUADS);
				glTexCoord2d((j  )*0.0625,(i+15.0/16.0)*0.0625f);		// ����� � �������� (����� ������)
				glVertex2i	( 0,15);									// ���������� ������� (����� ������)
				glTexCoord2d((j+15.0/16.0)*0.0625,(i+15.0/16.0)*0.0625);// ����� �� �������� (������ ������)
				glVertex2i	(15,15);									// ���������� ������� (������ ������)
				glTexCoord2d((j+15.0/16.0)*0.0625,(i  )*0.0625);		// ����� �������� (������� ������)
				glVertex2i	(15, 0);									// ���������� ������� (������� ������)
				glTexCoord2d((j  )*0.0625,(i  )*0.0625);				// ����� �������� (������� �����)
	            glVertex2i	( 0, 0);									// ���������� ������� (������� �����)
			glEnd();
			glTranslatef(lfg[sym].B,0,0);
			//glTranslated(10.0,0.0,0.0);
			glEndList();
		};
	};
	return Text::OK;
};

float	Font::GetStrWidth	(	const char *str	)
{
	size_t		strl = strlen (str);
	float		shift = 0.0;
	for (size_t i = 0 ; i < strl ; i++ )
	{
		shift  +=	lfg	[(unsigned char)str[i]].A +
					lfg	[(unsigned char)str[i]].B;
	}
	return shift / ( float ) Font::scr_width * ( float ) Size * 30 / Globals.VP.ScreenStrings;
}

void Font::ULOAD(void)
{
	glDeleteLists(Base, 256);
	delete	T;
}

void Font::SetColor(Vector3f C)
{
	Color = C;
}
void Font::SetColor(float r,float g,float b)
{
	Color = Vector3f(r,g,b);
}
void Font::SetColor(Vector3f C, float alfap)
{
	Color = C; alfa = alfap;
}
void Font::SetColor(float r,float g,float b, float alfap)
{
	Color = Vector3f(r,g,b); alfa = alfap;
}

GLdouble	Font::scr_width;	// ����������� ����������

void Font::SetAspectRatio()
{
	switch(Globals.VP.AR)
	{
	case AR::NORMAL:
		Font::scr_width = 640;					// ����� ���������� ����� ������������
		break;
	case AR::NORMAL_LCD:
		Font::scr_width = 1280.0*480.0/1024.0;	// ������ � �������� ���
		break;									// ���������� 1280x1024 � ���������������� ���
	case AR::WIDE_HDTV:
		Font::scr_width = 16.0*480.0/9.0;		// HDTV ���������� 1920x1080
		break;
	case AR::WIDE_LDC:
		Font::scr_width = 16.0*480.0/10.0;		// WIDE LCD - 1280x800
		break;
	}
}