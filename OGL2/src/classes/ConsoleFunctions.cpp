#include "LocusAFX.h"
#include "./ConsoleFunctions.h"
#include "./Console.h"
#include "./Time.h"
//mmmmmmmmmmmmmmmmmmmm ����� ����������� ������� ������� ����� �������� �� �������
void	Exit			(string	s);		// �����
void	FPS				(string	s);		// ���������� ������� �������� FPS
void	CLS				(string	s);		// ������� ������ �������
void	Echo			(string	s);		// ��������� ����� � �������
void	Lighting		(string	s);		// ���������� ����������
void	Lighting2Side	(string	s);
void	DropHistory		(string s);

void	INIT_CONSOLE_FUNCTIONS	()
{
	//MainCon->OUT_STRING(CONCOL_HELP,"������� ����������� :)");

	MainCon->RegFunc("exit",					&Exit);
	MainCon->RegFunc("quit",					&Exit);
	MainCon->RegFunc("quti",					&Exit);
	MainCon->RegFunc("fps",						&FPS);
	MainCon->RegFunc("cls",						&CLS);
	MainCon->RegFunc("echo",					&Echo);
	MainCon->RegFunc("Lighting",				&Lighting);
	MainCon->RegFunc("Lighting2Side",			&Lighting2Side);
	MainCon->RegFunc("DropCommandHistory",		&DropHistory);
}

void	DropHistory	(string s)
{
	if (upcase_string(s) == "HELP")
	{
		MainCon->OUT_STRING(CONCOL_HELP,"������� ������� ������");
		return;
	}
	MainCon->DropHistory();
}

void	Exit	(string	s)
{
	if (upcase_string(s) == "HELP")
	{
		MainCon->OUT_STRING(CONCOL_HELP,"�����");
		return;
	}
	Globals.Exiting	=	true;
}

void	FPS		(string	s)
{
	if (upcase_string(s) == "HELP")
	{
		MainCon->OUT_STRING(CONCOL_HELP,"����� �������� ���������� ������ � �������");
		return;
	}
	MainCon->OUT_STRING(CONCOL_HELP,"FPS: %5.1lf",GT.GetFPS());
}

void	CLS		(string	s)
{
	if (upcase_string(s) == "HELP")
	{
		MainCon->OUT_STRING(CONCOL_HELP,"������� ������");
		return;
	}
	for ( DWORD i = 0 ; i < 80 ; i++ )
		MainCon->OUT_STRING(CONCOL_HELP,"");
}

void	Echo	(string	s)
{
	if (upcase_string(s) == "HELP")
	{
		MainCon->OUT_STRING(CONCOL_HELP,"������������ ������ � �������");
		return;
	}
	if (s.empty())	return;
	MainCon->OUT_STRING(CONCOL_HELP,"%s",s.c_str());
}

void	Lighting	(string	s)
{
	s = upcase_string(s);
	if ( s == string ("TRUE") || s == string ("ON") || s == string ("1") )
	{
		glEnable(GL_LIGHTING);
		MainCon->OUT_STRING(CONCOL_MESSAGE,"��������� ��������");
	}
	else if ( s == string ("FALSE") || s == string ("OFF") || s == string ("0") )
	{
		glDisable(GL_LIGHTING);
		MainCon->OUT_STRING(CONCOL_MESSAGE,"��������� ���������");
	}
	else
	{
		MainCon->OUT_STRING(CONCOL_HELP,"����� ��� ��������� ��������� ���������");
		MainCon->OUT_STRING(CONCOL_HELP,"������������� - lighting [true|ON|1|false|OFF|0]");
		if (glIsEnabled(GL_LIGHTING))
		{
			MainCon->OUT_STRING(CONCOL_MESSAGE,"��������� ��������");
		}
		else
		{
			MainCon->OUT_STRING(CONCOL_MESSAGE,"��������� ���������");
		}
	}
}

void	Lighting2Side	(string	s)
{
	s = upcase_string(s);
	if ( s == string ("TRUE") || s == string ("ON") || s == string ("1") )
	{
		glLightModelf(GL_LIGHT_MODEL_TWO_SIDE,1.0);
		MainCon->OUT_STRING(CONCOL_MESSAGE,"��������� c 2� ������ ��������");
	}
	else if ( s == string ("FALSE") || s == string ("OFF") || s == string ("0") )
	{
		glLightModelf(GL_LIGHT_MODEL_TWO_SIDE,0.0);
		MainCon->OUT_STRING(CONCOL_MESSAGE,"��������� c 2� ������ ���������");
	}
	else
	{
		MainCon->OUT_STRING(CONCOL_HELP,"����� ��� ��������� ��������� ��������� (1 ��� 2 ������� ���������)");
		MainCon->OUT_STRING(CONCOL_HELP,"������������� - Lighting2Side [true|ON|1|false|OFF|0]");
		GLboolean boolean;
		glGetBooleanv(GL_LIGHT_MODEL_TWO_SIDE,&boolean);
		if (boolean)
		{
			MainCon->OUT_STRING(CONCOL_MESSAGE,"��������� c 2� ������ ��������");
		}
		else
		{
			MainCon->OUT_STRING(CONCOL_MESSAGE,"��������� c 2� ������ ���������");
		}
	}
}