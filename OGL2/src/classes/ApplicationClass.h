#pragma once
#ifndef APPLICATIONCLASS_HEADER
#define APPLICATIONCLASS_HEADER

//#define AUX_ENABLED								// Включение библиотеки AUX
#define _CRT_SECURE_NO_DEPRECATE
//====== Загрузка моих собственных классов и функций
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
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);	// Функция окна
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
// Глобальные переменные
extern GLfloat LightAmbient[4];			// Значения фонового света
extern GLfloat LightDiffuse[4];			// Значения диффузного света
extern GLfloat LightPosition[4];			// Позиция света
extern TextureLogo NVLogo;
extern bool	TextBenchmark;

void	SwapBuffersEXT	();							// Смена кадров - одна для быстрой замены

// Класс описывающий работу всего графического приложения
class Application
{

protected:
	bool			AppLoopFunc	();					// Функция приёма сообщений
	Camera			Cam;							// Камера для First-Person-Mode
	AVI_player		AVIp;							// Плеер заставок )
	ParticleSystem	PS;								// Система частиц
public:
	CConsole		CCons;							// Экземпляр консоли
//protected:

public:
#ifdef WIN32
	struct	Vars_t
	{
		struct App_t {
			HINSTANCE	HInstance;					// Хендл экземпляра приложения
		} App;
		struct WindowVars_t {
			HDC		DC;								// Хендл контекста устройства
			HGLRC	RC;								// Хендл контекста рендера
			HWND	MHWnd;							// Хендл главного окна
			DEVMODE PrevDS,CurrDS;					// Настройки экрана до и после запуска
		}		win;

	}		Vars;
#else
	Display			*dpy;							// Линк на дисплей
	XVisualInfo		*vi;							// Инфа о визуале
	Colormap		cmap;							// Карта цветов?
	XSetWindowAttributes swa;						// Атрибуты окна
	Window			win;							// Само окно
	GLXContext		cx;								// Контекст
	XEvent			event;							// Событие

#endif
	bool	IsFullScreen;							// Флаг полноэкранного режима

	Application				();						// Конструктор класса
	~Application			();						// Деструктор класса
	bool	ReadConfig		();						// Загрузка настроек из файла
	bool	SaveConfig		();						// Сохранение настроек в файл
	bool	InitOpenGL		();						// Инициалазация настроек OpenGL
	bool	InitOpenIL		();						// Инициализация графической библиотеки
	bool	GetSysInfo		();						// Узнаём конфигурацию системы
	bool	Birth			();						// Функция инициализации приложения
	bool	Suicide			();						// Деинициализация приложения
	bool	Work			();						// Нормальная работа приложения
	bool	ReshapeWindow	(int,int);				// Изменение размеров рисовательной области
	bool	RenderScene		();						// Отрисовка картинки
	void	RenderTexture	();						// Отрисовка текущей забинденой текстуры
	void	ShowMovie		(bool CanBreak);		// Показывает видик
	bool	LoadResources	();						// Загрузка ресурсов
	void	ProcessKBInput	();						// Реакция на нажатия клавиш
};

extern Application* MainApplication;				// Указатель на объект приложения
#endif	//#ifndef APPLICATIONCLASS_HEADER