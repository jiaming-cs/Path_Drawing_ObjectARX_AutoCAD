#pragma once
#include "C:\Apps\AutoCAD\objectARX\ObjectARX_for_AutoCAD_2020_Win_64_bit\inc\dbmain.h"
#include <vector>
#include "Path.h"
#include <string>
using namespace std;

class EntityPath :
	public AcDbEntity
{
public:
	EntityPath();
	EntityPath(Path path, vector<JD> jd_vector, vector<QX> qx_vector, vector<LC> lc_vector, int qdlc);
	Path get_path() { return this->path; };
	vector<JD> get_jd_vector() { return this->jd_vector; };
	void set_path(Path path) { this->path = path; };
	void set_jd_vector(vector <JD> jd_vector) { this->jd_vector = jd_vector; };
	void set_qdlc(int qdlc) { this->qdlc = qdlc; };
	void recompute() {
		qx_vector.clear();
		lc_vector.clear();
		path.compute_quxian(jd_vector, qx_vector);
		path.compute_licheng(jd_vector, qx_vector, lc_vector, qdlc / 1000, qdlc % 1000);
	}
	int get_qdlc() { return this->qdlc; };
	ACRX_DECLARE_MEMBERS(EntityPath)
	
protected:

	Acad::ErrorStatus dwgInFields(AcDbDwgFiler *filer);
	Acad::ErrorStatus dwgOutFields(AcDbDwgFiler *filer);
	Acad::ErrorStatus dxfInFields(AcDbDxfFiler *filer);
	Acad::ErrorStatus dxfOutFields(AcDbDxfFiler *filer);
	Adesk::Boolean subWorldDraw(AcGiWorldDraw* mode);
	Acad::ErrorStatus subGetGripPoints(
		AcGePoint3dArray& gripPoints,
		AcDbIntArray & osnapModes,
		AcDbIntArray & geomIds
	)const;
	Acad::ErrorStatus subMoveGripPointsAt(const AcDbIntArray & indices,
		const AcGeVector3d& offset);
	
private:
	Path path = Path();
	vector<JD> jd_vector;
	vector<QX> qx_vector;
	vector<LC> lc_vector;
	vector <AcGePoint3d> jd_acd;
	int qdlc;
};

