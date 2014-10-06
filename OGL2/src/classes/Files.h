/*=[ �����: ]===================================================================================\\
||	1) ���������������� ����
||	2) ��� ����
||
\\==============================================================================================*/
#pragma once

//#pragma warning(disable:4996)
#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <string.h>
#include <windows.h>

void CleanString(char *s);						// ������� ����� �� ����� � ������ �������� (� �������� ����)

class File 
{
protected:
	unsigned int FilenameLenght;				// ����� ������ � ������ ����� (��� ��������)
	FILE*		f;								// ��������� �� ������������� �����
    char		type;							// ��� �����		(t|b)
	char		mode;							// ������ ��������	(r|w|a)
	bool		Extended;						// ������ + ��� open
	char		modestr[4];						// ������ ���������� ��� ����� � ������ ��������
	bool		isOpen;							// ����������, ������ �� ����
public:
	File		(void);							// ����������� �� ���������
	File		(	char*	Fname,			  	// ����������� � ��������� ������ ������
					char	Omode	=	'r',	//		���� ������������� �����������
					char	Ftype	=	't',		
					bool	isExtended = true);
	~File		(void);							// ���������� �����������

	bool Open	(	char*	Fname,				// �������� �����
					char	Omode	=	'r',
					char	Ftype	=	't',
					bool	isExtended = true);
	void Close	( void );						// �������� �����	
};

/*=[ �����: ]===================================================================================\\
||	����� � ����:
||	���������: <Message>: Session started at hh:mm:ss dd.mm.yyyy
||	(hh:mm:ss)	<<Block>>::<Message>
||	����� - 2 �������� - ���������� � ������������
||	1) ���������� - <Message>: Session ended at hh:mm:ss dd.mm.yyyy
||	2) ������������ - <Message>: Fatal error at hh:mm:ss dd.mm.yyyy
\\==============================================================================================*/

#define FJC_NEED_TIME	0x0001
#define FJC_NEED_DATE	0x0002
//#define MAX_TIME_PRESISION		// ���������� ������ ��������� ���� ������� � ���� (�� �������� ����)

void FJCGetTD(char mode,char* str);				// ������� ����� �/��� ���� ��� ������ � ����

class LogFile: public File
{
// ����� ��� � � �� ��������� ��� ������ ������� ���������� �� ������ ��������� ������� �� �� ������ :=)
protected:
	char	msg [256];							// ��������� ���������
	char	buf	[256];							// ����� ��� ��������� ��������� � ���
	char	buf2[256];							// 2� ����� ��� ��������� ��������� � ���
	unsigned int msgLength;						// ����� ��������� (��� ��������)
public:
	bool	Start	(char* message);			// ������ ����
	bool	Logf	(char* block, char* str, ...);						// �������������� ������ � ���
	bool	Log		(char* block, char* msg, bool ok = true);			// ������ � ���
	bool	Msg		(char* msg);				// ������ ��������� ��� �������� �������
	bool	End		(bool is_normal);			// ����� ����
};

class CFGFile: public File
{
protected:
public:
	char*	Gets	(char* string);				// ����������� ��������� ������ �� ����� (����������� ������������)
};
