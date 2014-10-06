#pragma once

#ifdef WIN32
#define _CRT_SECURE_NO_DEPRECATE
#define _WIN32_WINNT				0x0500
#endif
//#define ENABLE_TEXTURE_RENDERING

#include "./shared/mymath.h"
#include "./shared/ASSERT.h"
#include "./files.h"

extern	LogFile	LF;								// ���-����
extern	CFGFile	CF;								// ���� � �����������

#define	FJC_ERROR		true
#define	FJC_NO_ERROR	false


// ��� Define'�
	// ��������� �������������� �����������
#define HALF_SECOND_FPS_REFRESH					// �������� ������� ���������� FPS � 0,5 �������
#define TIME_TO_CALCULATE_CPU_SPEED 200
// #define DOUBLE_TIME_PRESISION
// ������ ���������� ���������
#define INPUT_MODE_FPS_GAME				0
#define INPUT_MODE_AUTOSYM_GAME			1
#define INPUT_MODE_MENU					2
#define INPUT_MODE_CONSOLE				3

#define	FJC_TEX_NO_FILTERING			0
#define FJC_TEX_ANISOTROPIC_FILTERING	1
#define FJC_TEX_BILINEAR_FILTERING		2
#define FJC_TEX_TRILINEAR_FILTERING		3

#define CAM_LOOKTO_MODE_VECTOR			0					// ������� ����������� ����� ������ �����������
#define CAM_LOOKTO_MODE_POINT			1					// --//-- ����� �� ������� ������� ��������� ������
#define CAM_LOOKTO_MODE_ANGLES			2					// ������� ����� �������� ������ (������������� �����)
#ifndef FJ_INPUT_ENABLED
#define CAM_MOVE_FORWARD				0					// ������ ��������� �����
#define CAM_MOVE_BACKWARD				1					// ������ ��������� �����
#define CAM_MOVE_LEFT					2					// ������ ��������� �����
#define CAM_MOVE_RIGHT					3					// ������ ��������� ������
#endif

// ��� ����
typedef void (*SwapBuffersFunction)(void);
typedef void (*KeyFunc)(void);								// ��� ��������� �� ��������� �������

#ifndef WIN32
typedef unsigned int				UINT;
typedef unsigned char				BYTE;
typedef unsigned char				bool;
typedef unsigned long long			QWORD
#if		sizeof(UINT) == 4
typedef unsigned int				DWORD
#elif	sizeof(unsigned long int) == 4
typedef unsigned long int			DWORD
#else
#error Could not find bilt-in type for DWORD
#endif
#endif

struct CameraSet								// ��������� ������
{
	double	ForwardSpeed,						// �������� ����������� �����
			BackwardSpeed,						// �������� ����������� �����
			StrafeSpeed;						// �������� �������

	double	Sensitivity;						// ���������������� ����
	Point3d	CamStartPosition;					// ��������� ������� ������
};
extern CameraSet	CamS;						// ����������

namespace EStates
{
	enum	type
	{
		RENDER_3D,
		RENDER_2D,
		PLAY_CLIP
	};
}
namespace ERS
{
	namespace Draw
	{
		enum type
		{
			SCENE		=	0,
			TEXTURE		=	1,
			CONSOLE		=	2
		};
	}
	namespace Mesh
	{
		enum type
		{
			NORMAL		=	0,
			WIRE		=	1,
			HALFWIRE	=	2
		};
	}
}
namespace	AR							// Aspect Ratio  - ����������� ������ ������
{
	enum	type
	{
		NORMAL		=	0,				// 4:3
		AR4_3		=	0,
		WIDE_HDTV	=	1,				// ������� ����� ��������� HTDV ��� � �������� �����������
		AR16_9		=	1,
		NORMAL_LCD	=	2,				// �������� ��� LCD ������� � ���������� 1280x1024
		AR1280_1024	=	2,
		WIDE_LDC	=	3,				// ������� ����� LCD (�������� � ��������� ��������)
		AR16_10		=	3,				// � ������������ ������ 16:10 ��� ����������� 1280x800
		AR1280_800	=	3
	};
}
struct GlobalsStruct								// ���������� ��������� � ����������
{
	struct
	{
		bool		IsFullScreen;					// ��������� �������������� ������
		int			Width,Height;					// ������ � ������ ����
		int			Bits,Hzs;						// ������� ����� � �������� ������
		int			Vsync;							// ��������� ������������ ������������� (1 - ��������)
		UINT		ScreenStrings;					// ���������� ����� �� ������
		AR::type	AR;								// ����������� ������ ������� ������
	}	VP;
	unsigned char	TextureFiltering;				// ��� ���������� �������
//	unsigned int	lists;							// ���������� �������������� ������� (������� � ����� ����������)
	struct	tagRESstruct{
		char	TEXTURE_DIR	[128];					// ����� ����������, ��� ����������� �������
		char	FONT_DIR	[128];
		char	MESH_DIR	[128];
	}	RES;
	EStates::type				es;
	struct	tagERSs
	{
		ERS::Draw::type	d;
		ERS::Mesh::type	m;
	}	ERS;
	struct	tagEFlags
	{
		bool	show_stat;
		bool	show_gui;
		bool	LWOCullFace;					// �������� ��������� ������ LWO �������
	}			EFl;
	bool	Exiting;
};
extern GlobalsStruct			Globals;		// ���������� ��������� � ����������

struct KeySettings								// ��������� ���������� ���������� � �������������� ��������
{
	// ������� ���������� �������
	WPARAM CamMoveForward;						// ����������� �����
	WPARAM CamMoveBackward;						// ����������� �����
	WPARAM CamMoveLeft;							// ����������� �����
	WPARAM CamMoveRight;						// ����������� ������
	WPARAM CamBackToCenter;						// ����������� � ��������� �����
	WPARAM MoreFastMove;						// ���������

};
extern KeySettings				KeySet;			// ��������� ���������� ���������� � �������������� ��������

// ��������� � ����������� � ����������
struct VCInfo									// ���������� � ����� ����������
{
	int		MaxAnisotropy;						// ����������� ������������� ������������ ����������
	char	*SupportedGlExtentions;				// �������������� OpenGL ���������� ����������
	char	*RendererName;
	char	*VendorName;
	char	*OpenGL_Version;
};
struct CPUInfo
{
	CPUInfo()
	{
		memset(this,0,sizeof(CPUInfo));
	}
	int	speed;								// �������� ���������� � ���
	char	vendor_id	[13];
	char	id			[64];
	char	name		[64];
	char	SSE;
	char	SSE2;
	char	AMD_3DNow;
	char	AMD_3DNow_Ex;
	char	RDTSC;
	char	MMX;

};
// ��������� ���������� ���������� � ������� � ������� ����������� ����������
struct SysInformationData
{
	VCInfo	video;								// ���������� � ����� ����������
	CPUInfo	cpu;								// ���� � ����������
};

extern SysInformationData		SysInfo;		// ���������� � �������
