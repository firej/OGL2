/*==============================================================================================\\
|| ���� ���� �������� ������ ����������� ��� �������� ������������ �/� ����������� ������� �    ||
|| ��������� ���������                                                                          ||
||  ������� ������ ����� �������� string                                                        ||
||                                                                           ___________________||
|| copyright FJ ART, 2005                                                    |created 10.09.2005||
\\==============================================================================================*/

#pragma once

#include <string>
#include <map>

using namespace std;

typedef void (*ConFunc)(string);							// ��� ��������� �� ��������� �������
typedef map <string,ConFunc,less <string> > ConFuncsMap;	// ������� ��� ������������� ��� � �������

class ConsoleFuncsMap							// ����� ��� ������������� ������� �� ���������� ������
{
private:
	ConFuncsMap MMap;										// ������� ���������� ������
	ConFuncsMap::iterator i;								// �������� ��� ����������� �� �������

public:
	ConsoleFuncsMap();										// ����������� ��-���������
	~ConsoleFuncsMap();										// ���������� ��-���������
	ConFunc GetFunc	(string Name);							// ����� ������� �� �����
	bool AddFunc	(string Name, ConFunc Func);			// ���������� ������� � �������
	bool AddVar		(string Name, void * p);				// ���������� ���������� � �������
//	bool DelFunc	(string Name, ConFunc Func=NULL);		// �������� �� �������
//	int GetQuantity	(void);									// ����� ������ ���������� �������
	bool Exec		(string Name);
};