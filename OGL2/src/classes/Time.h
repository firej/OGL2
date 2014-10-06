#pragma once
//--------------------------------------------------------------------------------------//
//  Created with help of	Yuan, Feng								www.fengyuan.com	//
//							(Windows Graphics Programming: Win32 GDI and DirectDraw)	//
//--------------------------------------------------------------------------------------//

#pragma warning(disable : 4035)
#include "./globals.h"

inline unsigned __int64 GetCycleCount(void)
{
    _asm    _emit 0x0F
    _asm    _emit 0x31
}

class Timer
{
protected:
	unsigned __int64	last_cycle;					// ��������� ���������� ����
	unsigned __int64	now_cycle;					// "���������" ���������� ����
	double				q;							// ����������� ���������

    double				FrameTime;					// ����� ����� ���������� �������
	double				GFrameTime;					// Game frame time - ����� ��� ������� �������� (+�����)
	double				WorldTime;					// ����� ������� �����
	double				TimeToShowFPS;				// ����� ��������� � ���������� ��������� �������� FPS
	double				FPS;						// ���-�� ������ � �������
	int					FramesToRefresh;			// ���������� ������� ����� ������������
			// ��������� ������ ������������ ��� �������� ����� FPS'� ����������� �������� 2 ���� � �������
	int					i;							// ������� ������� ��� FPS
	bool				pause;						// ��������� �� �� ��� � ���� ����� �����!

public:
    Timer()							  
	{									   
		last_cycle = 0;
		now_cycle = 0;
		FramesToRefresh = 50;
		i = 0;
	}
	void Start()
	{
		q = ((double)1.0) / ((double)(SysInfo.cpu.speed) * ((double)1000000.0));
		last_cycle = now_cycle = GetCycleCount();
		WorldTime = 0;
		pause = false;
	}
    void NewFrame	(void)								// ������ ������ �����
	{
		now_cycle = GetCycleCount();
		FrameTime = (double)(now_cycle - last_cycle) * q;
		if (!pause) GFrameTime = FrameTime;
					else GFrameTime = 0;
		last_cycle = now_cycle;
		WorldTime		+= FrameTime;
			i++;
			TimeToShowFPS	+= FrameTime;
			FramesToRefresh = (int)(0.25/FrameTime);
			if (i >= FramesToRefresh)
			{
				FPS = FramesToRefresh/TimeToShowFPS;
				i = 0;	TimeToShowFPS = 0;
			}
	};
	void	SetFramesToRefresh(const int Frames)
	{	FramesToRefresh = Frames;
	};
	double GetFrameTime		(void)
	{	return FrameTime;
	};
	double GetGameFrameTime	(void)
	{	return GFrameTime;
	};
	double GetFPS	(void)
	{	return FPS;
	};
	double GetWorldTime()
	{	return WorldTime;
	};
	int GetFramesToRefresh()
	{	return FramesToRefresh;
	};
};

extern Timer GT;	// Global Timer