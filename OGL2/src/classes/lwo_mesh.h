#ifndef LWO_MESH_H
#define LWO_MESH_H

#include "./globals.h"
#include "./texture.h"
// ������ ��������� ��� ����
#include <vector>
#include <map>
#include <string>
#include <set>
using namespace std;
namespace Mesh
{
	typedef	enum
	{
		OK,
		FILE_NOT_FOUND,
		NO_MEMORY,
		INVALID_FORMAT,
		NO_LISTS
	} RESULT;

	typedef	struct	lwSurface_tt
	{
		lwSurface_tt()
		{
			DoubleSided		= false;
			texture			= false;
			Smoothing		= false;
		}
		string			name;			// ��� ���������
		string			AncestorName;	// ��� ������������� ��������� (�� ���������)
		Color4f			color;			// ����� ����
		Color4f			Diffuse;		// ��������� ����
		Color4f			Specular;		// ����������� ������������
		bool			DoubleSided;	// ���� ��������������� ��������
		bool			texture;		// ���������� ���������� ��������� ����� �����
		TexturePointer	Texure;			// ��������� ��������
		int				imageIndex;		// ������ �������� � ������� clipVector
		bool			Smoothing;		// ���� ����������� (������������ �� �������)
		float			nSmoothAngle;	// ������������ ���� ��� ����������� (������ ��� �� ������������)
	}	lwSurface_t	;

	typedef	struct clipElement_tt
	{
		string			Name;
		TexturePointer	LoadedTexture;
	}	clipElement_t;

	typedef	vector	<clipElement_t>	clipVector_t;
	typedef	vector	<lwSurface_t>	lwSurfList_t;

	typedef	struct tag_tt
	{
		string		tn;
		WORD		iSurf;
	}	tag_t;
	typedef	vector<tag_t>		tag_v_t;

	typedef	struct lwTagList_tt
	{
	//	int				count;
	//	int				offset;					// only used during reading
		tag_v_t			tag;					// array of strings
	}	lwTagList_t	;

	typedef	struct Poly_tt
	{
		Poly_tt()
		{
			v		=	0;
			vi		=	0;
			ns		=	0;
			iSurf	=	0;
		}
		WORD		v;						// ���������� ������ �� ��������
		WORD		*vi;					// ������ �������� ������
		bool		*ns;					// ������ ������ ������������ �������
		// ���� true - ������� ������ � ������� ����� ����� � ��������
		WORD		iSurf;					// ������ ��������
		Vector3f	normal;					// ������� ��������
	}	Poly_t	;

	typedef	struct BoundingBox_tt
	{
		Point3f		a;
		Point3f		b;
	}	BoundingBox_t	;

	typedef	struct lwVertex_tt
	{
		Point3f		v;						// ���������� ������� �������
		Point3f		n;						// ������� � ������� �������
	}	lwVertex_t	;

	typedef	struct lwLayer_tt
	{
		WORD			number;
		WORD			flags;
		WORD			pivot;
		string			name;
		Point3f			PivotPoint;			// ������� ����� ���� (�� �� ������)
		BoundingBox_t	*bbox;				// ������������ �����
		lwVertex_t	*	Vertexes;			// ������ ������
		DWORD			iVertexes;			// ���������� ������
		Poly_t		*	Pols;				// ������ ���������
		DWORD			iPols;				// ���������� ���������
	}	lwLayer_t	;

	typedef vector<lwLayer_t>	lwLayers;
//mmm ����������� �������� mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
	typedef	struct NormalSmoothPolygonPointer_tt
	{
		NormalSmoothPolygonPointer_tt()
		{
			s = false;
		}
		NormalSmoothPolygonPointer_tt(Poly_t * pp)	// ����������� ����������
		{
			p = pp;
			s = false;
		}
		Poly_t	*	p;		// ��������� �� �������
		bool		s;		// ���� �����������
		WORD		np;		// ����� ������� � ��������
		// ������ ��� ���� ������� ������ ������������ ���������� ������� �� ���� �������
	}	NormalSmoothPolygonPointer_t;
	typedef	struct NormalSmoothVertex_tt
	{
		// ������ ���������, ������� ����������� ������ �������
		vector<NormalSmoothPolygonPointer_t>	polys;
		DWORD									n;		// ����� ������� �������
		Vector3f								normal;	// �������������� �������
	}	NormalSmoothVertex_t;

//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//mmm ����� mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
	class LWOMesh
	{
	private:
		// ������ ���������
		lwLayers		Layers;						// ������ ����
		DWORD			iLayers;					// ������� ����
		GLuint			ListID;
		// ������ ���������� � �. �.
		lwTagList_t		TagList;					// ������ �����
		// ������ �������
		Point3f			Translate;					// ��������, ������� � �������
		Point3f			Rotate;
		Point3f			Scale;
		DWORD			Pols;
		DWORD			Points;
		GLuint			list;
		bool			compiled;
		clipVector_t	clipVector;					// ������ ���� ��� �������
	public:		lwSurfList_t	SurfList;			// ������ ���������
		WORD			iSurfaces;					// ���������� ���������

		//material_t	*	material;				// ������ ����������
		//int				mi;						// ������ ���������
		//Point3f		*	vertexes;				// ������ ������
		//Point2f		*	tc;						// ���������� ����������
		//Mesh::Poly_t	*	Pols;					// ������ ���������
		//DWORD			iPols;						// ���������� ���������
	// ������� ������
	private:
		bool	ReadPNTSChunk		(	FILE	*	f	);
		bool	ReadPOLSChunk		(	FILE	*	f	);
		bool	ReadLAYRChunk		(	FILE	*	f	);
		bool	ReadTAGSChunk		(	FILE	*	f	);
		bool	ReadSURFChunk		(	FILE	*	f	);
		bool	ReadPTAGChunk		(	FILE	*	f	);
		bool	ReadCLIPChunk		(	FILE	*	f	);
	// ������� ���������
		WORD	ReadSURFCOLRSubChunk(	FILE	*	f	);
		WORD	ReadSURFDIFFSubChunk(	FILE	*	f	);
		WORD	ReadSURFSPECSubChunk(	FILE	*	f	);
	// ������������� ����� - ���������� ���������, ����������� �������� � �. �. � �. �.
		void	PostLoadProcessing	(	void			);
		void	DrawFromSource		(	void			);
		void	CompileList			(	void			);
	public:
		LWOMesh			();
		Mesh::RESULT	Load		(char	*FileName);
		GLvoid			ULoad		(GLvoid);
		GLvoid			Render		(GLvoid);
		WORD			GetLayers	();
		DWORD			GetPoints	();
		DWORD			GetPolygons	();
	};

	typedef	Mesh::LWOMesh	mesh;
};
typedef Mesh::LWOMesh* meshPointer;
#endif //#ifndef LWO_MESH_H