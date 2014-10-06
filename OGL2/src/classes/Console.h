#pragma once
#include "./globals.h"
#include "./Text.h"
//#include "./ApplicationClass.h"
#include <string>
#include <vector>
#include <map>
#include <list>
using namespace std;

#define		CONCOL_DEFAULT		0
#define		CONCOL_HELP			1
#define		CONCOL_WARNING		2
#define		CONCOL_ERROR		3
#define		CONCOL_CURSOR		4
#define		CONCOL_BACKGROUND	5
#define		CONCOL_INPUTLINE	6
#define		CONCOL_OK			7
#define		CONCOL_MESSAGE		7

string	upcase_string	(	string	s	);
char	upcase			(	char	a	);

namespace	Console
{
	enum	Status
	{
		ST_VISIBLE			=	0,
		ST_INVISIBLE		=	1,
		ST_FADEIN			=	2,
		ST_FADEOUT			=	3
	};
	enum	FadeEffect						// ��������� ������� ���������/������������ �������
	{
		ALFA				=	0			// ������ ����-����������
	};
	struct	ConString
	{
		//BYTE		chars;		// ���������� �������������� �������� (��� ��������)
		BYTE		c;
		string		s;			// ������
	};
	struct	ListElement
	{
		ListElement	*	next;	// ��������� �� ��������� ������� ������
		ConString		data;	// ������
	};

	enum	VarType					// ������������ ��������� ����� ����������
	{
		VT_BYTE			=	14,
		VT_WORD			=	0,
		VT_DWORD		=	1,
		VT_QWORD		=	2,
		VT_CHAR			=	9,
		VT_INT16		=	3,
		VT_INT32		=	4,
		VT_INT64		=	5,
		VT_FLOAT32		=	6,
		VT_FLOAT64		=	7,
		VT_BOOLEAN		=	8,
		VT_STRING		=	10,
		VT_POINT2f		=	11,
		VT_POINT3f		=	12,
		VT_COLOR3f		=	12,
		VT_COLOR4f		=	13
	};
}

typedef void (*ConFunc)(string);					// ��� ��������� �� ��������� �������
typedef map <string,ConFunc,less<string>>	ConFuncsMap;		// ������� ��� ������������� ��� � �������
typedef map <string,ConFunc,less<string>>::iterator	iCFuncsMap;	// ������� ��� ������������� ��� � �������
struct	Var_t
{
	Console::VarType	type;
	void			*	var;
	BYTE				maxsize;
};
typedef map <string,Var_t>				ConVarsMap;	// ������� ��� ������������� ��� � ����������
typedef	map <string,Var_t>::iterator	iCVarsMap;	// ������� ��� ������������� ��� � ����������

class CConsole
{
//private:
protected:
	//BYTE			CurrSubString;					// �����
	Console::ListElement*	list;					// ��������� �� ������ ����� �������
									// (�� ���������������� ��������� �� ������� ������������� ������)
	DWORD					chars;					// ���������� ���� � ������� ������
	DWORD					cursor;					// ������� ������� � ������
	DWORD					ListElements;			// ���������� ��������� ������
	Point3f					Colors[64];				// ������ ��������������� ������
	PFont					ConsoleFont;			// ��������� �� ���������� �����
	Console::Status			State;					// ��������� �������� �������
	double					persent;				// �������� ���������/������������ �������
	double					pers_step;				// ��� ��������� ��������� �������
	double					transparency;			// ������������ ������������ (��� ������������ ���������)
	double					height;					// ������� ����������� �� ������ �����
	double					lastTick;				// ������� ����� �������
	double					Period;					// ������ ������� �������
	bool					CurVisible;				// ����� �� ������
	string					Commands[60];			// ������ ������ ��� ����������
	DWORD					CurCommand;				// ������� ��������� ������� ��� ����������
	WORD					iAComp;					// ���������� ���� ��� ������ � ��������������
	std::list<string>			CommandsHistory;		// ������� �������� ������
	std::list<string>::iterator	iCndHistory;			// �������� � �������
public:
	CConsole			(): ListElements(0),list(0)
	{}
	void	SetColors	(	Point3f	C_Def, Point3f C_Help, Point3f C_Warn, Point3f C_Err)
	{	Colors[0] = C_Def; Colors[1] = C_Help; Colors[2] = C_Warn; Colors[3] = C_Err;	}
	void	SetFont		(PFont	CFont)
	{	ConsoleFont	=	CFont;	}		// ����� ����������� ������	
	void	INIT		();
	void	DEINIT		();
	void	ProcessChar	(char	c);
	void	ProcessKey	(DWORD	key);
	void	OUT_STRING	(BYTE	c, const char *fmt, ...);
	void	Draw		();
	void	Show		();
	void	Hide		();
	void	ChangeState	();
	bool	Visible		();
// ������ � ���������
private:
	ConFuncsMap			CFmap;		// ����� ������� �������
public:
	void	DoString	(string	s);					// ���������� ������, �������� �������������
	string	FindNextSym	(string	s);					// ����� ��������� �� ���� ������ � �����
	void	RegFunc		(string	s,ConFunc cf);		// ����������� ������� ��� ����������� ������
	void	RegFunc		(char*	s,ConFunc cf)
	{	RegFunc(string(s),cf);				}
	void	UnRegFunc	(string	s);					// ������� ����������� �������
	/*void	RegVar		(string	s,Console::VarType	type,void*	var,BYTE	maxsize);
	void	RegVar		(char*	s,Console::VarType	type,void*	var,BYTE	maxsize)
	{	RegVar		(string(s),type,var,maxsize);	}
	void	UnRegVar	(string	s);					// ������� ����������� ����������*/
	ConFuncsMap*	GetMap	(void)
	{	return &CFmap;	}
	void	DropHistory	(	void	);				// ������� ������� ������
};

extern CConsole	*	MainCon;

//void	Console_Set			(string	s);	// ���������� �������� ������������������ ����������
//void	Console_Get			(string	s);	// ������� � ������� �������� ��������� ����������
void	Console_Help		(string	s);	// ������� ������� ������ ��� �� �������