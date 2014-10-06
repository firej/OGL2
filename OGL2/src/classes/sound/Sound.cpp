#include "LocusAFX.h"
#include "./sound.h"

SoundController Sound;
SoundBuf		TestBuf;
soundSource		TestSource;

//=========================================================================================================
//====[ ���������� �������� ����������� ]==================================================================
//=========================================================================================================
SoundController::SoundController()
{
};
SoundController::~SoundController()
{
};
ALboolean SoundController::Init()		// ������������� ����� - �������� ���������� � �������� ���������
{
	char deviceName[256];
		memset(deviceName,0,256);
	char	*DDeviceName = NULL;
	char	*deviceList = NULL;
	char	*devices[12];
		memset(devices,0,12);
	int		numDevices;
	int		numDefaultDevice;
	int		i;

	const char * exts = (const char *) alcGetString ( ALDevice, ALC_EXTENSIONS );
	LF.Logf("Sound::Init","AL extentions:\n %s",exts);

	if (alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT") == AL_TRUE) { // try out enumeration extension
		DDeviceName	= (char *)alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);	// ���������� ��-���������
		deviceList	= (char *)alcGetString(NULL, ALC_DEVICE_SPECIFIER);			// ������ ��������� (� �������)
		for (numDevices = 0; numDevices < 12; numDevices++)		// ��������� ������� ���������
		{	devices[numDevices] = NULL;}
		for (numDevices = 0; numDevices < 12; numDevices++)
		{
			devices[numDevices] = deviceList;
			if (strcmp(devices[numDevices], DDeviceName) == 0)
			{
				numDefaultDevice = numDevices;
			}
			deviceList += strlen(deviceList);					// ����������� ��������� ������ �� ������
			if (deviceList[0] == 0)
			{
					if (deviceList[1] == 0)
					{
						break;
					}
					else
					{
						deviceList += 1;
					}
			}
		}
		if (devices[numDevices] != NULL) {						// ����������� �������� �������� ���������
			numDevices++;
			LF.Logf("Sound :: Init","This sound devices was founded:");
			for (i = 0; i < numDevices; i++) {
				LF.Logf("Sound :: Init","%d. %s", i + 1, devices[i]);
			}
		}
	}
	if (strlen(deviceName) == 0) {
		ALDevice = alcOpenDevice(NULL); // this is supposed to select the "preferred device"
	} else {
		ALDevice = alcOpenDevice(deviceName); // have a name from enumeration process above, so use it...
	}
	//Create context(s)
	ALContext=alcCreateContext(ALDevice,NULL);

	alcMakeContextCurrent(ALContext);


	SetLOrientation	(	Vector3f(0.0f,0.0f,1.0f)	,			// ���� �����
						Vector3f(0.0f,1.0f,0.0f)	);
	SetLPosition	(	Vector3f(0.0f,0.0f,0.0f)	);
	SetLVelocity	(	Vector3f(0.0f,0.0f,0.0f)	);

	return AL_TRUE;
};

ALboolean SoundController::Suspend()		// ����� ����� ��� ����� ��������� (������ ��������� ���������)
{
	if (inSuspend == AL_FALSE)
	{
	alcSuspendContext     ( ALContext );
	return AL_TRUE;
	}
	return AL_FALSE;
};
ALboolean SoundController::Process()		// ����������� ������������ ����� �����
{
	alcProcessContext     ( ALContext );
	return AL_FALSE;
};

ALboolean SoundController::DeInit()		// ��������������� �����
{;
  // ��������� ������� ��������
  alcMakeContextCurrent(NULL);
  // ���������� ��������
  alcDestroyContext(ALContext);
  // ��������� �������� ����������
  alcCloseDevice(ALDevice);
  return AL_TRUE;
};
ALvoid	SoundController::SetDistModel(ALenum value)
{
	alDistanceModel(value);
};
ALvoid SoundController::SetMasterGain	(ALfloat MasterGain)
{
	alListenerf(AL_GAIN,MasterGain);
};
ALvoid SoundController::SetLOrientation	(Vector3f Direction,Vector3f UpVector)
{

	LDirection	=	Direction;
	LUpVector	=	UpVector;
	ALfloat tempVect[6] = 
			{
				Direction.d.c.x,
				Direction.d.c.y,
				Direction.d.c.z,
				UpVector.d.c.x,
				UpVector.d.c.y,
				UpVector.d.c.z
			};
	alListenerfv(AL_ORIENTATION,tempVect);
};
ALvoid SoundController::SetLPosition		(Point3f Pos)
{
	LPosition = Pos;
	alListenerfv(AL_POSITION,Pos.d.v);
};
ALvoid SoundController::SetLVelocity		(Vector3f Vel)
{
	LVelocity = Vel;
	alListenerfv(AL_VELOCITY,Vel.d.v);
};

//=========================================================================================================
//====[ ������� ���������� ������� ]=======================================================================
//=========================================================================================================
SoundBuf::SoundBuf()
{
};
SoundBuf::~SoundBuf()
{
};
ALvoid SoundBuf::Init()
{
};
ALvoid SoundBuf::DeInit()
{
	if (!id) return;
	alDeleteBuffers(1,&id);
};
ALboolean SoundBuf::LoadWav(const char *FileName)			// �������� WAV ����� � ����� "Buffer"
{
	static ALenum err;
	err = AL_NO_ERROR;
	ALenum    format;		//�������� ����� �����
	ALvoid    *data;		// ��������� �� ������ ������ �����
	ALsizei    size;		// ������ ����� �������
	ALsizei    freq;		// ������� ����� � ������
	ALboolean  loop;		// ������������� ������������ ���������������
	
	alGenBuffers(1,&id);

	alutLoadWAVFile( (ALbyte *)FileName, &format, &data, &size, &freq, &loop);	// �������� ���������� �����
	err = alGetError();
	if (err != AL_NO_ERROR) LF.Logf("Snd::LoadWAV","Error %d",err);

	alBufferData(id, format, data, size, freq);							// ���������� ������ �������
	err = alGetError();
	if (err != AL_NO_ERROR) LF.Logf("Snd::LoadWAV","Error %d",err);
	alutUnloadWAV(format, data, size, freq);									// �������� ����� �� ������
    err = alGetError();
	if (err != AL_NO_ERROR) LF.Logf("Snd::LoadWAV","Error %d",err);
	return AL_TRUE;
};
//=========================================================================================================
//====[ ����� "�������� �����" ]===========================================================================
//=========================================================================================================
soundSource::soundSource()
{
};
soundSource::~soundSource()
{
};
ALvoid soundSource::Init()
{
	alGenSources(1, &id);
	alSourcef (id, AL_GAIN,    1.0f);

	alSource3f(id, AL_POSITION, 0.0f, 0.0f, 0.0f);
	alSource3f(id, AL_VELOCITY, 0.0f, 0.0f, 0.0f);

};
ALvoid soundSource::DeInit()
{
	if (!id) return;
	alDeleteSources(1, &id);
};
ALboolean soundSource::LinkWithBufer(SoundBuf *Buffer)
{
	if (!Buffer->GetID()) return AL_FALSE;
	BufID = Buffer->GetID();
	alSourcei (id, AL_BUFFER, BufID);
	return AL_TRUE;
};
//====[ ���������� ����������� ]===========================================================================
inline ALvoid soundSource::SetPos(const Vector3f &Pos)
{
	alSource3f(id, AL_POSITION,  Pos.d.c.x, Pos.d.c.y, Pos.d.c.z);
};
inline ALvoid soundSource::SetVel(const Vector3f &Vel)
{
	alSource3f(id, AL_VELOCITY,  Vel.d.c.x, Vel.d.c.y, Vel.d.c.z);
};
inline ALvoid soundSource::SetPitch( ALfloat pitch )
{
	alSourcef(id, AL_PITCH, pitch);
};
inline ALvoid soundSource::SetGain( ALfloat gain)
{
	alSourcef(id, AL_GAIN, gain);
};
//====[ ���������� ���������������� ]======================================================================
ALvoid soundSource::Play()
{	
	now_playing = AL_TRUE; 
	alSourcePlay	(id);//this->BufID);
};
ALvoid soundSource::Pause()
{
	alSourcePause	(id);
};
ALvoid soundSource::Stop()
{
	now_playing = AL_FALSE;
	alSourceStop	(id);
};
ALvoid soundSource::Rewind()
{
	alSourceRewind(id);
};
ALvoid soundSource::SetLooping(ALboolean Looping)
{
	alSourcei (id, AL_LOOPING,  Looping);
};