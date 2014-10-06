/* ������ : ====================================================================================\\
||	����� ������
\\==============================================================================================*/
#pragma once
#define _CRT_SECURE_NO_DEPRECATE
#include <windows.h>
#include <stdio.h>
#include "./globals.h"
#include "./input.h"
#include "./objects.h"

class Camera
{
public:
	Point3d		Position	;								// ������� ������ � ������� �����������
	Vector3d	Direction	;								// ����������� ���� ������� ������ (������������ �����)
	Vector3d	Strafe		;								// ������ ������� (�����)
	Vector3d	Angles		;								// ���� ����� ��� ����������� ���� ������� ������
	/*===[ ���� : ]============================================(��� ����� ��� ������ �����������)===\\
	||	���� X - ���������� �������� �����|���� (������ ��� OX)
	||	���� Y - ���������� ������� �����|����� (������ ��� OY)
	||	���� Z - �� ������������ ( ���� �����-������ ������ ��� OZ)
	\\==============================================================================================*/
	Point2i		center;										// ����� ������


	Camera			();								// Constructor
	~Camera			();								// Destructor

	Vector3d			GetDirection	(void);				// ���������� ����������� ������
	Vector3d			GetStrafeLeft	(void);				// ���������� ������� ������� (�����)
	Point3d				GetPosition		(void);				// ���������� ������� ������ (� ������� �����������)

	void Init			(void);								// ������������
	void SetPosition	(Point3d Pos);						// ������������� ������� ������
//	void LookTo			(Point3f VAR, char MODE);				// ������� ����������� �������
							//		MODE:
						// CAM_LOOKTO_MODE_ANGLES	-	������ � ���� �����
						// CAM_LOOKTO_MODE_POINT	-	������ � ���� ����� (���� �������)
						// CAM_LOOKTO_MODE_VECTOR	-	������ � ���� ������� �����������
	void Move			(char MODE, double speed);			// ����������� ������
	void Look			(void);								// ���������� ������� � OpenGL
	void MouseLook		(void);								// ������������ ����������� ����
    void KeybMove		(void);								// ������������ ������� ������ => ����������� ������

// FJ Input functions
		// ��� ������� ���� �������� ����� ��������� ������ (����� ����������� ��� �� �����)
//#ifdef FJ_INPUT_ENABLED
	void RecalculateDirection	(void);						// �������� ������� �����
	void RecalculateStrafe		(void);						// �������� ������� ������������� �����

	void MoveModelViewMatrix	(void);						// ����������� ������� ��������� ��� ����������
	void RotateModelViewMatrix	(void);						// ������� ������� ���������
//#endif
};
