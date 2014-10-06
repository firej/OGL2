/*==============================================================================================\\
|| Main program file for FJ Code 3D Demo                                                        ||
|| File version 0.1.5                                                                           ||
||                                                                                              ||
||    ||        //*****\  //******#  ||      |  //*******                                       ||
||    ||        ||     |  ||         ||      |  ||                                              ||
||    ||        ||     |  ||         ||      |  \\######\      /**** |\  | /***  | |\  | /****  ||
||    ||        ||     |  ||         ||      |          |      |---  | \ | |  __ | | \ | |---   ||
||    \\______  \\_____/  \\_______  \\______/  \\______/      \____ |  \| \___/ | |  \| \____  ||
||                                                                                              ||
|| copyright FJ ART / FJC, 2005 - 2007                                                          ||
\\==============================================================================================*/
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#endif
#include "LocusAFX.h"
//====== ����������� Header'��
#include "./MAIN.h"

Application MyApplication;
// ������� ������� ����������!
int APIENTRY WinMain( HINSTANCE,
					  HINSTANCE,
					  LPSTR,
					  int)
{
	//  �������� �� �������� ������
	//_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_LEAK_CHECK_DF );
	MyApplication.Birth();								// ������������� ����������
	MyApplication.Work();								// ���� ��������� ���������
	MyApplication.Suicide();							// �������� ����������
	return 0;
};
