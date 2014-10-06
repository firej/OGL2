#pragma once

class SoundController;								// ���������� ������
class soundSource;									// �������� �����
class SoundBuf;										// �������� �����

//=========================================================================================================
//====[ �������� ���������� ]==============================================================================
//=========================================================================================================
class SoundController
{
protected:
	ALCdevice	*ALDevice;
	ALCcontext	*ALContext;
	ALboolean	inSuspend;

	ALenum		ALerror;
// Listener properties
	Point3f		LPosition;							// ������� ����
	Vector3f	LVelocity;							// �������� ����
	Vector3f	LDirection;							// ���������� ������� ��������
	Vector3f	LUpVector;							// ���� ��������� ������� ��������
public:
	SoundController();
	~SoundController();
	ALboolean	Init			();					// �������� ��������� ���������� ��� AL
	ALboolean	Suspend			();					// ����� ����� ��� ����� ��������� (������ ��������� ���������)
	ALboolean	Process			();					// ����������� ������������
	ALboolean	DeInit			();					// �������� ��������� ���������� ��� AL

	ALvoid	SetDistModel	( ALenum value );	// ��������� ������ ����� ����������

	ALvoid	SetLPosition	(Point3f	Pos);	// ��������� ������� ����
	ALvoid	SetLVelocity	(Vector3f	Vel);	// ��������� �������� ����
	ALvoid	SetLOrientation	(Vector3f	Direction,// ��������� ���������� ����
							 Vector3f	UpVector);
	ALvoid	SetMasterGain	(ALfloat	MasterGain);// ��������� ��������� GAIN
};
extern SoundController Sound;
//=========================================================================================================
//====[ ���������� ������� ]===============================================================================
//=========================================================================================================
class SoundBuf										// �������� �����
{
protected:
	ALuint		id;									// ID ������
	ALuint		FileType;							// ��� ����� ������������� ��� �������� �����
public:
	SoundBuf();
	~SoundBuf();
	ALvoid		Init		();						// �������������
	ALvoid		DeInit		();						// ���������������
	ALuint		GetID		()						// ��������� �������������� ��� �������� � ��������� �����
		{	return id;	};
	ALboolean LoadWav(const char *FileName);		// "�������� �����"
};

//=========================================================================================================
//====[ ����� "�������� �����" ]===========================================================================
//=========================================================================================================
class soundSource									// �������� �����
{
protected:
	ALuint		id;									// ������������� �����
	ALuint		BufID;								// ������������� ������
	Vector3f	pos;								// ������� ��������� �����
	Vector3f	dir;								// ����������� ��������� �����
	Vector3f	vel;								// �������� ��������� �����
	ALboolean	now_playing;						// ������� ���� ��� ���� � ������ ������ �������������
	ALboolean	now_generated;						// ������������ ����
	ALboolean	linked_with_buffer;					// ������ � �������
	SoundBuf	*Buf;								// ��������� �� ������������ �������� �����

public:
	soundSource();									// �����������
	~soundSource();									// ����������

	ALvoid		Init();								// ������������� ��������� �����
	ALvoid		DeInit();							// ����������������� ��������� �����

	ALboolean	LinkWithBufer		(SoundBuf *Buffer);	// ������� �������� � �������

	ALvoid SetPos(const Vector3f &Pos);				// ��������� �������
		Vector3f GetPos()
		{return pos;};
	ALvoid SetVel(const Vector3f &Vel);				// ��������� ��������
		Vector3f GetVel()
		{return vel;};
	ALvoid SetDir(const Vector3f &Dir);				// ������� ����������� ��������� �����
		Vector3f GetDir()
		{return dir;};
	ALboolean NowPlaying()							// ������ �������������
	{	return now_playing;}
	ALvoid SetPitch		( ALfloat pitch );
	ALvoid SetGain		( ALfloat gain);
	ALvoid SetLooping	( ALboolean Looping);		// ��������� ����� ����������
	ALvoid Play			(ALvoid);					// �������������(������ ���������������)
	ALvoid Pause		(ALvoid);					// �����
	ALvoid Stop			(ALvoid);					// ���������� ���������������
	ALvoid Rewind		(ALvoid);					// ������ �������
};

extern	SoundBuf		TestBuf;
extern	soundSource		TestSource;