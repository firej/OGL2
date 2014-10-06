#ifndef	RESMAN_HEADER
#define	RESMAN_HEADER

#include "./globals.h"
#include "./text.h"
#include "./texture.h"
#include "./lwo_mesh.h"
#include <hash_map>
using namespace stdext;
#include <string>
using namespace std;

/*
		Resource manadger for OGL2 project LOCUS Engine
			by FJ													*/

enum	ResourceType		// ��� �������
{
	Res_FONT,				// ������ - ������
	Res_TEXTURE,			// ������ - ��������
	Res_MESH				// ������ - ������� 3D ������
};

struct	ResourceObject
{
	ResourceObject()
	{memset(this,0,sizeof(ResourceObject));};
	void*			Res;		// ��������� �� ����������� ������
	int				used;		// ���������� �������������� �����������
	ResourceType	RType;		// ��� �������
};
typedef hash_map <string,ResourceObject>	ResMapL_t;			// ������� ��� ������������� ��� � �������
typedef	ResMapL_t :: iterator				ResMapL_Iter_t;		// �������� ��� �������
typedef	ResMapL_t :: const_iterator			ResMapL_cIter_t;	// ����������� ��������

struct	ResPointer
{
	void*				Res;
};
class	FindRes_ret_t
{
public:
	FindRes_ret_t(bool	Need_Pass,ResMapL_Iter_t	Data)
	{	need_pass	=	Need_Pass;	data	=	Data;	}
	bool			need_pass;
	ResMapL_Iter_t	data;
};

typedef hash_map <string,ResPointer>		ResMapH_t;			// ������� ��� ������������� ��� � �������
typedef	ResMapH_t :: iterator				ResMapH_Iter_t;		// �������� ��� �������
typedef	ResMapH_t :: const_iterator			ResMapH_cIter_t;	// ����������� ��������


class	ResMan
{
protected:
	ResMapL_t	mapL;			// ������ ����������� (�������) ������
ResMapL_Iter_t	mapLi;
	ResMapH_t	mapH;			// ������ ��������������� ������� (�����������) ������
ResMapH_Iter_t	mapHi;
	PFont		defFont;		// ����� ����������� 
TexturePointer	defTexture;
	meshPointer	defMesh;
//	int			Resources;		// ���������� ����������� ��������
FindRes_ret_t	FindRes(ResMapH_Iter_t	mapHi);	/*	����� �������� � mapL, �����-�� ������� �������	*/
public:
	ResMan();
	~ResMan();

	void INIT				(	char*	DefFont,		// ������������� ��������� ��������
								char*	DefTexture,
								char*	DefMesh);
	void DEINIT				(void);			// ����� ������ ��������� ��������
// LOAD functions
	void			LOAD_Font		(char* ResName,char* FontName);	// �������� ������
	PFont			SELECT_Font		(char* ResName);				// ����� ������
	void			ULOAD_Font		(char* ResName);
	void			LOAD_Texture	(char *ResName,char *FileName);	// �������� �������
TexturePointer		SELECT_Texture	(char* ResName);
	void			ULOAD_Texture	(char* ResName);
	void			LOAD_Mesh		(char *ResName,char *FileName);	// �������� 3� ��������
	meshPointer		SELECT_Mesh		(char* ResName);
	void			ULOAD_Mesh		(char* ResName);

};
extern ResMan		rm;
bool InitFonts();
bool DeInitFonts();

#endif