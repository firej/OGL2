#pragma once
#ifndef APPLICATIONCLASS_HEADER
#define APPLICATIONCLASS_HEADER

//#define AUX_ENABLED								// ��������� ���������� AUX
#define _CRT_SECURE_NO_DEPRECATE
//====== �������� ���� ����������� ������� � �������
#include "./globals.h"
#include "./input.h"
#include "./Texture.h"
#include "./camera.h"
#include "./objects.h"
#include "./Text.h"
#include "./sound/sound.h"
//#include "./PhysiX.h"
#include "./Logo.h"
#include "./lwo_mesh.h"
#include "./resman.h"
#include "./video/AVI_player.h"
#include "./console.h"
#include "./ParticleSystem.h"

#ifdef WIN32
#include "./../../resource.h"
#else
#include "../Ex/MesaGL/linux/glx.h"
#endif

#ifdef WIN32
#define MY_WINDOW_CLASS_NAME	("FJC_OGL_Window_CLASS")
#define MY_WINDOW_NAME			("FJC_OGL_Window")
typedef BOOL (APIENTRY *wglSwapIntervalEXT_Func)(int);
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);	// ������� ����
#else
	int attributeList[] =
		{
			GLX_USE_GL,
			GLX_RGBA, GL_TRUE,
			GLX_DOUBLEBUFFER
		};

	static Bool WaitForNotify(Display *d,	XEvent *e, char	*arg) 
	{	return (e->type == MapNotify) && (e->xmap.window == (Window)arg);	}

#endif
// ���������� ����������
extern GLfloat LightAmbient[4];			// �������� �������� �����
extern GLfloat LightDiffuse[4];			// �������� ���������� �����
extern GLfloat LightPosition[4];			// ������� �����
extern TextureLogo NVLogo;
extern bool	TextBenchmark;

void	SwapBuffersEXT	();							// ����� ������ - ���� ��� ������� ������

// ����� ����������� ������ ����� ������������ ����������
class Application
{

protected:
	bool			AppLoopFunc	();					// ������� ����� ���������
	Camera			Cam;							// ������ ��� First-Person-Mode
	AVI_player		AVIp;							// ����� �������� )
	ParticleSystem	PS;								// ������� ������
public:
	CConsole		CCons;							// ��������� �������
//protected:

public:
#ifdef WIN32
	struct	Vars_t
	{
		struct App_t {
			HINSTANCE	HInstance;					// ����� ���������� ����������
		} App;
		struct WindowVars_t {
			HDC		DC;								// ����� ��������� ����������
			HGLRC	RC;								// ����� ��������� �������
			HWND	MHWnd;							// ����� �������� ����
			DEVMODE PrevDS,CurrDS;					// ��������� ������ �� � ����� �������
		}		win;

	}		Vars;
#else
	Display			*dpy;							// ���� �� �������
	XVisualInfo		*vi;							// ���� � �������
	Colormap		cmap;							// ����� ������?
	XSetWindowAttributes swa;						// �������� ����
	Window			win;							// ���� ����
	GLXContext		cx;								// ��������
	XEvent			event;							// �������

#endif
	bool	IsFullScreen;							// ���� �������������� ������

	Application				();						// ����������� ������
	~Application			();						// ���������� ������
	bool	ReadConfig		();						// �������� �������� �� �����
	bool	SaveConfig		();						// ���������� �������� � ����
	bool	InitOpenGL		();						// ������������� �������� OpenGL
	bool	InitOpenIL		();						// ������������� ����������� ����������
	bool	GetSysInfo		();						// ����� ������������ �������
	bool	Birth			();						// ������� ������������� ����������
	bool	Suicide			();						// ��������������� ����������
	bool	Work			();						// ���������� ������ ����������
	bool	ReshapeWindow	(int,int);				// ��������� �������� ������������� �������
	bool	RenderScene		();						// ��������� ��������
	void	RenderTexture	();						// ��������� ������� ���������� ��������
	void	ShowMovie		(bool CanBreak);		// ���������� �����
	bool	LoadResources	();						// �������� ��������
	void	ProcessKBInput	();						// ������� �� ������� ������
};

extern Application* MainApplication;				// ��������� �� ������ ����������
#endif	//#ifndef APPLICATIONCLASS_HEADER