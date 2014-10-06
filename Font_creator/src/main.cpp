#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_DEPRECATE
#define _WIN32_WINNT	0x500
#include <windows.h>
#include <shlobj.h>
#include <commdlg.h>
#include <stdio.h>
#include <string.h>
#include "../resource.h"
#include <math.h>
#include "./il/il.h"
#include "./il/ilu.h"
#include "./il/ilut.h"
#include "../../ssrc/iff.h"


#pragma comment(lib,"src/il/DevIL.lib")
#pragma comment(lib,"src/il/ilu.lib")
#pragma comment(lib,"src/il/ilut.lib")

char		TexSizes[5][16]	=
						{	"128x128",
							"256x256",
							"512x512",
							"1024x1024",
							"2048x2048"/*,
							"4096x4096"	*/	};
char		TestString[]	=	"ABCDabcd �������� 123 [!,.;:]";
#define		FONT_SIZES	8
char		FontSizes[FONT_SIZES][16]={	"10","12","14","16","18","20","24","30"};
bool		ForcedMono		= false			;
HWND		hWin;
HWND		hDlg;
int			Boldings[10]	= {0,100,200,300,400,500,600,700,800,900};
char		Bs		[10][32]= {	"  0 - FW_DONTCARE",
								"100 - FW_THIN",
								"200 - FW_EXTRALIGHT",
								"300 - FW_LIGHT",
								"400 - FW_NORMAL",
								"500 - FW_MEDIUM",
								"600 - FW_SEMIBOLD",
								"700 - FW_BOLD",
								"800 - FW_EXTRABOLD",
								"900 - FW_HEAVY"};
struct	LFontGlyph_t
{
	float	A;			// �������� �� �������
	float	B;			// �������� ����� �������
};
struct	renderVars_t
	{
		int		TexSizeN;						// ����� ������� ��������
		int Width,Height;
		struct	Font_t
		{
			DWORD				Italic;
			int					Bolding;		// �������
			DWORD				UnderLine;
			DWORD				StrikeOut;
			DWORD				Mono;
			char				Name	[128];	// ��� ������
			int					Format;			// ����� ������� � ������� ���
			void			*	TBuf;			// ����� ��� ���������
			DWORD				TSize;
		} Font;									// ��������� � ����������� ������������� ����
	}	RenderVars;								// --//--//--//--//--//--- ��� ����������
char				SaveFolder	[512]	=	"Fonts";
char				buf			[1024]	=	{NULL};
void				RenderFont			(void);	// ������������ �����
// ��������� ����� � ����
void				WriteFontToFile		(char* FileName, HDC hdc, int FontSize, void* Texture, DWORD	TLength);
void				RefreshDialogVars	(void);	// ���������� ������ �� �������
float				PointSizetoLogicalX	(HDC	hDC, float points);
void				UpdatePreview		(	void	);
INT_PTR CALLBACK	DialogProc			(HWND hDlg,	UINT message,	WPARAM wParam,	LPARAM lParam);

int APIENTRY WinMain( HINSTANCE,
					  HINSTANCE,
					  LPSTR,
					  int)
{
	strcpy(RenderVars.Font.Name,"Consolas");

	int ret = (int) DialogBox(GetModuleHandle(0),MAKEINTRESOURCE(IDD_FCD),0,(DLGPROC)DialogProc);
	if (ret == -1)
	{
		int err = GetLastError();
		char buf[128];
		sprintf(buf, "In application detected error #%d",err);
		MessageBox(NULL,buf,"ERROR!",16);
	}
	return 0;
}


INT_PTR CALLBACK	DialogProc	(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_CLOSE		:
		DestroyWindow(hDlg);
		EndDialog(hDlg,0);
		break;
	//case WM_PAINT		:
	//	break;
	case WM_INITDIALOG	:
		{
			OleInitialize(NULL);
			CoInitialize(NULL);
			ilInit();
			iluInit();
			ilutRenderer(ILUT_WIN32);
			::hDlg = hDlg;
			for (int i=0;i<5;i++)				// ���������� ������ ������
				SendDlgItemMessage(hDlg,IDC_COMBOTEXSIZE,CB_INSERTSTRING,i,(LPARAM)TexSizes[i]);
			SendDlgItemMessage(hDlg,IDC_COMBOTEXSIZE,CB_SETCURSEL,3,NULL);	// �������� 4-� ������ ��������
			for (int i=0;i<10;i++)				// ���������� ����� � �������� ��������
				SendDlgItemMessage(hDlg,IDC_COMBO_BOLD,CB_INSERTSTRING,i,(LPARAM)Bs[i]);
			SendDlgItemMessage(hDlg,IDC_COMBO_BOLD,CB_SETCURSEL,5,NULL);	// �������� ���������� �������
			// ����� ��� ������
			SendDlgItemMessage(hDlg,IDC_EDIT1,WM_SETTEXT,NULL,(LPARAM)RenderVars.Font.Name);
			SendDlgItemMessage(hDlg,IDC_EDIT2,WM_SETTEXT,NULL,(LPARAM)SaveFolder);
			UpdatePreview	();
		}//case WM_INITDIALOG:
		return NULL;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case IDC_CHECK_ITALIC:case IDC_CHECK_L:case IDC_CHECK_SO:
			UpdatePreview();
			break;
		case IDC_EDIT1:
			if (wmEvent == EN_CHANGE)
			{
				UpdatePreview	();
			}
			break;
		case IDC_EXIT:
			SendMessage(hDlg,WM_CLOSE,NULL,NULL);
			break;
		case IDC_RENDER:
			RenderFont();
			break;
		case IDC_BUTTON_BROUSE:
			BROWSEINFO	bi;
			bi.hwndOwner		=	hDlg;
			bi.pidlRoot			=	NULL;
			bi.pszDisplayName	=	NULL;
			bi.lpszTitle		=	"�������� ����� ��� ���������� ������ ������...";
			bi.lpfn				=	NULL;
			bi.lParam			=	NULL;
			bi.iImage			=	NULL;
			bi.ulFlags			=	BIF_NEWDIALOGSTYLE;
			LPCITEMIDLIST pidl	=	SHBrowseForFolder(&bi);
			if (!pidl) break;
			SHGetPathFromIDList(pidl,SaveFolder);
			SendDlgItemMessage(hDlg,IDC_EDIT2,WM_SETTEXT,NULL,(LPARAM)SaveFolder);
			break;
		}
		break;//WM_COMMAND
	case	WM_PAINT:
		UpdatePreview	();
		break;
	}//switch (message)
	return FALSE;
}
void		RefreshDialogVars(void)
{
	SendDlgItemMessage(hDlg,IDC_EDIT1,WM_GETTEXT,128,(LPARAM)RenderVars.Font.Name);
	SendDlgItemMessage(hDlg,IDC_COMBOTEXSIZE,CB_GETLBTEXT,SendDlgItemMessage(hDlg,IDC_COMBOTEXSIZE,CB_GETCURSEL,NULL,NULL),(LPARAM)buf);
	sscanf(buf,"%ix%i",&RenderVars.Width,&RenderVars.Height);
	RenderVars.Font.Bolding		= Boldings[SendDlgItemMessage(hDlg,IDC_COMBO_BOLD,CB_GETCURSEL,NULL,NULL)];
	RenderVars.Font.Italic		= IsDlgButtonChecked(hDlg,IDC_CHECK_ITALIC);
	RenderVars.Font.UnderLine	= IsDlgButtonChecked(hDlg,IDC_CHECK_L);
	RenderVars.Font.StrikeOut	= IsDlgButtonChecked(hDlg,IDC_CHECK_SO);
	RenderVars.Font.Mono		= IsDlgButtonChecked(hDlg,IDC_CHECKMONO);
}

void			RenderFont	(void)
{
	RefreshDialogVars();

	int PixelPerSymbol = RenderVars.Width/16;		// ����� �������� �� ���� ����

	BYTE data[2000]={0};
	BITMAPINFOHEADER *bh=(BITMAPINFOHEADER*)data;
	HBITMAP MyBmp;
	BYTE *MyBmpPixels;

		bh->biSize=sizeof(BITMAPINFOHEADER);
		bh->biWidth			=	RenderVars.Width;	// ������ X
		bh->biHeight		=	RenderVars.Height;	// ������ Y, upside down (line 0=bottom)
		bh->biPlanes		=	1;
		bh->biBitCount		=	24;
		bh->biClrUsed		=
		bh->biClrImportant	=	256;


		// ������� Bitmap
		MyBmp=CreateDIBSection(0,(BITMAPINFO*)bh,DIB_RGB_COLORS,(void **)&MyBmpPixels,NULL,0);
		DWORD dwRet = GetLastError();
		UNREFERENCED_PARAMETER(dwRet);
		HDC Bmp_hDC = CreateCompatibleDC( NULL );					// �������� DC �� ��������
		HBITMAP hbmOld = (HBITMAP)SelectObject( Bmp_hDC, MyBmp );	// � ���������� ������ GDI

		// ������� �����, �������� �� ����� ��������
		HFONT hFont =	CreateFont(	PixelPerSymbol,
									0,
									0,
									0,
									RenderVars.Font.Bolding,
									RenderVars.Font.Italic,
									RenderVars.Font.UnderLine,
									RenderVars.Font.StrikeOut,
									RUSSIAN_CHARSET,
									OUT_TT_PRECIS,
									CLIP_DEFAULT_PRECIS,
									ANTIALIASED_QUALITY,
									DEFAULT_PITCH | FF_DONTCARE,
									RenderVars.Font.Name);

		// �������� ��� ����� � �������� ��������
		SelectObject( Bmp_hDC, hFont );

		SetTextColor( Bmp_hDC, RGB(0,0,0) );
		SetBkColor(   Bmp_hDC, 0x00000000 );

		// ��������� ������� ��� ��� ������.
		SetBkMode(Bmp_hDC, TRANSPARENT);
		// ������������ ������
		SetTextAlign( Bmp_hDC, TA_TOP );

		// �������� ����
		SetTextColor( Bmp_hDC, RGB(255,255,255) );

		// � ��������.
		for (int i = 0;i<16;i++)
			for (int j = 0;j<16;j++)
			{
				if (i*16+j+1>256) continue;
				sprintf(buf,"%c",i*16+j+1);
				ExtTextOut( Bmp_hDC,j*PixelPerSymbol,i*PixelPerSymbol,0,NULL,buf,	1,		NULL );
			};
// �������� 32� ������� ������� �� 24� �������
		BYTE data32[2000]={0};
		BITMAPINFOHEADER *bh32=(BITMAPINFOHEADER*)data32;
		HBITMAP MyBmp32;
		BYTE *MyBmpPixels32;
		
		bh32->biSize=sizeof(*bh32);
		bh32->biWidth			=	RenderVars.Width;	// ������ X
		bh32->biHeight			=	RenderVars.Height;	// ������ Y, upside down (line 0=bottom)
		bh32->biPlanes			=	1;
		bh32->biBitCount		=	32;
		bh32->biClrUsed			=
		bh32->biClrImportant	=	256;


		// ������� Bitmap
		MyBmp32=CreateDIBSection(0,(BITMAPINFO*)bh32,DIB_RGB_COLORS,(void **)&MyBmpPixels32,NULL,0);
		HDC Bmp_hDC32 = CreateCompatibleDC( NULL );					// �������� DC �������
		HBITMAP hbmOld32 = (HBITMAP)SelectObject( Bmp_hDC32, MyBmp32 );	// � ���������� ������ GDI

		BYTE *src_pxls=MyBmpPixels;
		BYTE *dst_pxls=MyBmpPixels32;

		// �������� ��� �������� ����������
		for(int y=0;y<RenderVars.Height;y++)
			for(int x=0;x<RenderVars.Width;x++)
			{
				//memcpy(dst_pxls,src_pxls,3);	// �������� RGB
				//if (*src_pxls != 0x00)
					memset(dst_pxls,0xFF,3);	// ���������� �������� ��������� � ����� ����
				//else
				//	memset(dst_pxls,0x00,3);	// ������������� ������ ����
				dst_pxls[3] = src_pxls[0];		// ���������� ����� �� R ������
				dst_pxls	+= 4;	// ������������ �� ��������� �������
				src_pxls	+= 3;
			}
// ���������� ����������� � ����
		sprintf (buf,"%s/%s",SaveFolder,"TempTextre.png");
		DeleteFile(buf);
		int ret;
		ret = ilutWinSaveImage(buf,MyBmp32);
		ret = ilGetError();
		if (ret == IL_FILE_ALREADY_EXISTS)
		{
			MessageBox(hDlg,"����� ������ � ����������� ��������","�������!!!",16);
			return;
		}
		FILE	*	t = fopen(buf,"rb");
		fseek(t,0,SEEK_END);	// �������� ������ �����
		RenderVars.Font.TSize = ftell(t);
		fseek(t,0,SEEK_SET);	// ��������� � ������
		RenderVars.Font.TBuf = new char [RenderVars.Font.TSize];
		if (RenderVars.Font.TBuf == NULL)
		{
			MessageBox(hDlg,"����� ������ c ���������� ������","�������!!!",16);
			fclose(t);
			return;
		}
		fread(RenderVars.Font.TBuf,1,RenderVars.Font.TSize,t);
		fclose(t);
		DeleteFile(buf);
		sprintf (buf,"%s/%s.LFont",SaveFolder,RenderVars.Font.Name);
		WriteFontToFile(buf,Bmp_hDC,abs(PixelPerSymbol-2),RenderVars.Font.TBuf,RenderVars.Font.TSize);
		delete [] RenderVars.Font.TBuf;
		// ��������������� ���������
		SelectObject( Bmp_hDC, hbmOld );
		DeleteObject( hFont );
		ReleaseDC(NULL,Bmp_hDC);
		DeleteDC(Bmp_hDC);
		DeleteObject(MyBmp);

		SelectObject( Bmp_hDC32, hbmOld32 );
		ReleaseDC(NULL,Bmp_hDC32);
		DeleteDC(Bmp_hDC32);
		DeleteObject(MyBmp32);
}

void	WriteFontToFile	(char* FileName, HDC hdc, int FontSize, void* Texture, DWORD	TLength)
{
	ABCFLOAT	abc_ss[256];		// ��������� ���� ABC
	GetCharABCWidthsFloat(hdc,0,255,abc_ss);

	for (int i = 0 ; i<256 ; i++)
	{
		abc_ss[i].abcfA =	12.0f*PointSizetoLogicalX(hdc,abc_ss[i].abcfA)/FontSize;
		abc_ss[i].abcfB =	12.0f*PointSizetoLogicalX(hdc,abc_ss[i].abcfB)/FontSize;
		abc_ss[i].abcfC =	12.0f*PointSizetoLogicalX(hdc,abc_ss[i].abcfC)/FontSize;
	}
	LFontGlyph_t	lfg[256];
	for (int i = 0 ; i < 256; i++ )
	{
		lfg[i].A = abc_ss[i].abcfA;
		lfg[i].B = abc_ss[i].abcfB + abc_ss[i].abcfC;
	}

	FILE *f = fopen (FileName,"wb");
	if (!f)
	{
		MessageBox(NULL,"���������� ������� ���� ��� ������","������ ��� ���������� ������",16);
		return;
	}
	iff::PutTAG(f,ID_FORM);						// ������ �������������� FORM
	// ������� ������ �����
	size_t	fsize = 0;
	fsize += sizeof(iff::FONTFORMATNAME);		// ����� ��� ID �������
	// ����� ��� ��� �����, ID ����� � ������
	fsize += sizeof(iff::FONTNAME)		+ 4 + strlen(RenderVars.Font.Name) +
		iff::TestString(RenderVars.Font.Name,(DWORD)strlen(RenderVars.Font.Name));
	// ����� ��� ID �����, ������ � ������ ������
	fsize += sizeof(iff::FONTSIZE)		+ 4 + 4;
	// ID �����, ������ ����� � ������ ����� ���������
	fsize += sizeof(iff::FONTTEXTURE)	+ 4 + TLength;
	// ID �����, ������ ����� � ������ ������� ������
	fsize += sizeof(iff::FONTGLYPHBLOCK)+ 4 + sizeof(LFontGlyph_t)*256;
	iff::PutDWORD	(f,(DWORD)fsize);
// ����� ���������� �����-�����
	iff::PutTAG		(f,iff::FONTFORMATNAME.i);
	// ���������� ���� ����� ������ (����� ��� ���������� ��� �� �������)
	iff::PutTAG		(f,iff::FONTNAME.i);
	iff::PutDWORD	(f,(DWORD)strlen(RenderVars.Font.Name) +
		iff::TestString(RenderVars.Font.Name,(DWORD)strlen(RenderVars.Font.Name)));
	iff::PutString	(f,RenderVars.Font.Name,(DWORD)strlen(RenderVars.Font.Name));
	// ���������� ���� ������� ������ (���� ��� ����������)
	iff::PutTAG		(f,iff::FONTSIZE.i);
	iff::PutDWORD	(f,4);
	iff::PutDWORD	(f,FontSize);
	// ���������� ���� ��������
	iff::PutTAG		(f,iff::FONTTEXTURE.i);
	iff::PutDWORD	(f,TLength);
	iff::PutBuffer	(f,Texture,TLength);
	// ���������� ���� ������ ��� ������
	iff::PutTAG		(f,iff::FONTGLYPHBLOCK.i);
	iff::PutDWORD	(f,256*sizeof(LFontGlyph_t));
	iff::PutBuffer	(f,lfg,256*sizeof(LFontGlyph_t));

	fclose(f);
}


float	PointSizetoLogicalX(HDC	hDC, float points)
{
	POINT	P[2] =
	{
		{ 0									,0 },
		{ ::GetDeviceCaps(hDC,LOGPIXELSX)	,0 }
	};
	DPtoLP(hDC,P,2);
	return abs(P[1].x - P[0].x) * points / 72.0f;
}

void	UpdatePreview	(void)
{
	RECT		rect;
	HDC dc = GetDC(GetDlgItem(hDlg,IDS_PREVIEW));
	SendDlgItemMessage(hDlg,IDC_EDIT1,WM_GETTEXT,128,(LPARAM)RenderVars.Font.Name);
	RenderVars.Font.Bolding		= Boldings[SendDlgItemMessage(hDlg,IDC_COMBO_BOLD,CB_GETCURSEL,NULL,NULL)];
	RenderVars.Font.Italic		= IsDlgButtonChecked(hDlg,IDC_CHECK_ITALIC);
	RenderVars.Font.UnderLine	= IsDlgButtonChecked(hDlg,IDC_CHECK_L);
	RenderVars.Font.StrikeOut	= IsDlgButtonChecked(hDlg,IDC_CHECK_SO);
	GetClientRect(GetDlgItem(hDlg,IDS_PREVIEW),&rect);
	Rectangle(dc,rect.left,rect.top,rect.right,rect.bottom);
	HFONT hFont =	CreateFont(	-20,
						0,
						0,
						0,
						RenderVars.Font.Bolding,
						RenderVars.Font.Italic,
						RenderVars.Font.UnderLine,
						RenderVars.Font.StrikeOut,
						RUSSIAN_CHARSET,
						OUT_TT_PRECIS,
						CLIP_DEFAULT_PRECIS,
						CLEARTYPE_QUALITY,
						DEFAULT_PITCH | FF_DONTCARE,
						RenderVars.Font.Name);
	SelectObject( dc, hFont );
	SetTextColor( dc, RGB(0,0,0) );
	SetBkColor(   dc, 0x00000000 );
	// ��������� ������� ��� ��� ������.
	SetBkMode(dc, TRANSPARENT);

	DrawText(dc,TestString,(int)strlen(TestString),&rect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	DeleteObject( hFont );
	ReleaseDC(GetDlgItem(hDlg,IDS_PREVIEW),dc);
}
