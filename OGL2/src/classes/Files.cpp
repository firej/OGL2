#include "LocusAFX.h"
#include "./Files.h"
#include "./shared/ASSERT.h"

// ���������� �������� ������ File
File::File()
{
};

File::File(	char* Fname,
			char  Omode,
			char  Ftype,
			bool  isExtended)
{
	Open(Fname,Omode,Ftype,isExtended);
};

bool File::Open(	char* Fname,
					char  Omode,
					char  Ftype,
					bool  isExtended)
{
	memset (modestr,0,4);
	modestr [0] = Omode;
	if (isExtended)	{						// �������� ����� �� + � fopen
						modestr [1] = '+';
						modestr [2] = Ftype;
					}
			else	{
						modestr	[1] = Ftype;
					}
	isOpen = true;
	f = fopen(Fname,modestr);
	FASSERT(f);
//	return (bool)f;
	return true;   // �� ����� ������ ��-�� FASSERT'� ����
};

void File::Close()
{
//	FASSERT(f);
	if (f) fclose(f);
};
File::~File()
{
	if (f) fclose(f);
}
/*=[ �����: ����������: ]=======================================================================\\
||	1) ��� ����
||	2) ���������������� ����
||			  
\\==============================================================================================*/

void FJCGetTD (char mode,char* str)						// ������� ����� ��� ������ � ����
{	// mode = 0 - ����� ���� ��� �������
	SYSTEMTIME LocalTime;
	GetLocalTime(&LocalTime);
    if (mode & FJC_NEED_TIME)	// ����� �����
			if (mode & FJC_NEED_DATE)	// ����� ����
					sprintf(	str,
								"(%02d:%02d:%02d) %02d.%02d.%04d",
								LocalTime.wHour,		// ����
								LocalTime.wMinute,		// ������
								LocalTime.wSecond,		// �������
								LocalTime.wDay,			// ����(�����)
								LocalTime.wMonth,		// �����
								LocalTime.wYear			// ���
							);
			else	// if (mode && FJC_NEED_DATE)
					sprintf(	str,
								"(%02d:%02d:%02d)",
								LocalTime.wHour,		// ����
								LocalTime.wMinute,		// ������
								LocalTime.wSecond		// �������
							);
			else	// if (mode && FJC_NEED_TIME)
					sprintf(	str,
								"%02d.%02d.%04d",
								LocalTime.wDay,			// ����(�����)
								LocalTime.wMonth,		// �����
								LocalTime.wYear			// ���
							);
}

bool LogFile::Start(char *message)
{
	if (!f) return NULL;
	msgLength = (unsigned int)strlen(message);				  
	memset(msg,0,256);
	strcpy(msg,message);
	FJCGetTD(FJC_NEED_TIME|FJC_NEED_DATE,buf2);
	sprintf(	buf,
				"%s: %31.31s %s",
				msg,
				"Session sucessfully started at",
				buf2);
	Msg(buf);
	return true;
}
bool LogFile::Msg(char* msg)
{
	if (!f) return NULL;
	fprintf (f,"%s\n",msg);							// ������� ����������� ������ � ����
	return true;
}
bool LogFile::Logf(char *block, char* str, ...)
{
	if (!f) return NULL;
	FJCGetTD(FJC_NEED_TIME,buf);
	sprintf(	buf2,
				"\t%s : <%15.15s> :: %s\n",
				buf,
				block,
				str
			);
	va_list    ap;									// ��������� �� ������ ����������
    va_start(ap, str);								// ������ ������ ����������
	static char formatbuf [512];
	vsprintf(formatbuf,buf2,ap);
	va_end(ap);										// ��������� ���������� � ������
	fprintf(f,"%s", formatbuf);						// � ��������������� �������� � �������� ����
 	return true;
}

bool LogFile::Log(char *block, char* msg, bool ok)
{
	if (!f) return NULL;
	FJCGetTD(FJC_NEED_TIME,buf);
	sprintf(	buf2,
				"\t%s : <%15.15s> :: %50.50s\t\t\t ",
				buf,
				block,
				msg);
	if (ok)	strcat(buf2, "[   OK   ]");
		else strcat(buf2,"[ FAILED ]");
	Msg(buf2);
 	return true;
}
bool LogFile::End(bool is_normal)
{
	if (!f) return NULL;
	FJCGetTD(FJC_NEED_TIME|FJC_NEED_DATE,buf2);
	if (is_normal)
		sprintf(	buf,
					"%s: %31.31s %s",
					msg,
					"Session sucessfully ended at",
					buf2);
	else	// �� ��������� :=)
		sprintf(	buf,
					"%s: Fatal error at %s",
					msg,
					buf2);
	Msg(buf);																					
	return true;
}
//==================================================================================================================
char* CFGFile::Gets(char* string)
{
	if (!f) return NULL;
	do
	{
		fgets(string,256,f);
		CleanString(string);
		if (feof(f))
		{
			if ( (string[0]!=0) &&(string[0]!=10) ) return string;
			return NULL;
		};
	}
	while	((string[0]==10)||(string [0] == ';')||(( string [0] == '/' )&&( string [1] == '/' ) ) );
	return string;
}

void CleanString(char *s)
{
	static char s2[512];							// �������� ������
	static bool	word;								// ���������� ��� ������ ���������� ������� �����(�� �������)
	memset(s2,0,sizeof(s2));
	int i,j;										// ��������� �� �������
	for (i=0,j=0;(s[i]==' ')||(s[i]==9);i++);		// ������� �������� � ���� � ������
	for (;(s[i]!=0)&&(s[i]!=10);)
	{
		if ((s[i]==' ')||(s[i]==9))
		{
			s2[j] = ' '; j++;i++;
			for (;(s[i]==' ')||(s[i]==9);i++);
		}
		else
		{
			s2[j]=s[i];i++;j++;
		}
	}
	memset (s,0,strlen(s)+1);
	memcpy ( s,s2,strlen(s2) );
}