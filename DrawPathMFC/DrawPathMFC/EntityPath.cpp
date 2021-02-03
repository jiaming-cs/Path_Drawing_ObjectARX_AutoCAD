#include "stdafx.h"
#include "EntityPath.h"
#include <tchar.h>
#include "Path.h"

Path path_copy;
vector<JD> jd_vector_copy;
vector<QX> qx_vector_copy;
vector<LC> lc_vector_copy;
int qdlc_copy;

ACRX_DXF_DEFINE_MEMBERS(EntityPath, AcDbEntity, AcDb::kDHL_CURRENT, \
	AcDb::kMReleaseCurrent, 0, EntityPath, /*MSG0*/"AutoCAD");

EntityPath::EntityPath() {
	this->qdlc = qdlc_copy;
	this->jd_vector = jd_vector_copy;
	this->qx_vector = qx_vector_copy;
	this->lc_vector = lc_vector_copy;
	for (int i = 0; i < this->jd_vector.size(); i++) {
		AcGePoint3d point = AcGePoint3d(this->jd_vector[i].jd_x, this->jd_vector[i].jd_y, 0);
		jd_acd.push_back(point);
	}
}

EntityPath::EntityPath(Path path, vector<JD> jd, vector<QX> qx, vector<LC> lc, int qdlc) {

	this->qdlc = qdlc;
	this->jd_vector = jd;
	this->qx_vector = qx;
	this->lc_vector = lc;
	jd_vector_copy = jd;
	qx_vector_copy = qx;
	lc_vector_copy = lc;
	for (int i = 0; i < this->jd_vector.size(); i++) {
		AcGePoint3d point = AcGePoint3d(this->jd_vector[i].jd_x, this->jd_vector[i].jd_y, 0);
		jd_acd.push_back(point);
	}

}






Acad::ErrorStatus   EntityPath::dwgInFields(AcDbDwgFiler *pFiler) { // 使对象读入数据
	assertWriteEnabled();

	if (AcDbEntity::dwgInFields(pFiler) != Acad::eOk) {
		return AcDbEntity::dwgInFields(pFiler);
	};

	for (int i = 0; i < jd_acd.size(); i++) {
		pFiler->readItem(&jd_acd[i].x);
		pFiler->readItem(&jd_acd[i].y);
	}

	return	Acad::eOk;
}

Acad::ErrorStatus EntityPath::dwgOutFields(AcDbDwgFiler *pFiler) { // 让对象写数据

	assertReadEnabled();
	if (AcDbEntity::dwgInFields(pFiler) != Acad::eOk) {
		return AcDbEntity::dwgInFields(pFiler);
	};
	// 需要数量和顺序和dagInFields一致
	for (int i = 0; i < jd_acd.size(); i++) {
		pFiler->writeItem(&jd_acd[i].x);
		pFiler->writeItem(&jd_acd[i].y);
	}

	return	Acad::eOk;
}

Acad::ErrorStatus EntityPath::dxfOutFields(AcDbDxfFiler *pFiler) {
	assertReadEnabled();
	if (AcDbEntity::dxfInFields(pFiler) != Acad::eOk)
	{
		return AcDbEntity::dxfInFields(pFiler);
	}
	for (int i = 0; i < jd_acd.size(); i++) {
		int offset = 2 * i;
		pFiler->writeItem(AcDb::kDxfReal + offset, jd_acd[i].x);
		pFiler->writeItem(AcDb::kDxfReal + offset + 1, jd_acd[i].y);
	}

	return	Acad::eOk;
}

Acad::ErrorStatus EntityPath::dxfInFields(AcDbDxfFiler *pFiler) {
	assertWriteEnabled();

	if (AcDbEntity::dxfInFields(pFiler) != Acad::eOk)
	{
		return AcDbEntity::dxfInFields(pFiler);
	}

	struct resbuf rb;
	Acad::ErrorStatus rtevl = pFiler->readItem(&rb);
	int index = (int)rtevl;
	if (index % 2 == 0) {
		jd_acd[index / 2].x = rb.resval.rreal;
	}
	else
	{
		jd_acd[index / 2].y = rb.resval.rreal;
	}

	return	Acad::eOk;
}



Adesk::Boolean EntityPath::subWorldDraw(AcGiWorldDraw* mode) {

	/*
	The AcGiWorldDraw class provides functionality that has the ability to generate geometry
	that doesn't change for different viewports. The geometry may look different in different
	viewports (due to different zoom factors, viewpoints, etc.),
	but only one set of geometry is generated, which is then adjusted by AutoCAD for all the
	different viewports. For generating different geometry in different viewports, see AcGiViewportDraw.

	*/
	path.compute_quxian(jd_vector, qx_vector);
	path.compute_licheng(jd_vector, qx_vector, lc_vector, qdlc / 1000, qdlc % 1000);

	//AcGiSubEntityTraits object sets graphical attribute values using the following set of traits functions:
   /*
   Color
   Description
   0
   Color by block.
   1
   Red
   2
   Yellow
   3
   Green
   4
   Cyan
   5
   Blue
   6
   Magenta
   7
   White (or black if the graphics system considers it easier to see against the background color)
   8
   Dark gray
   9
   Light gray
   10
   Red
   20

   30
   Orange
   40

   50
   Yellow
   60

   70
   Yellow-green
   80

   90
   Green
   100

   110
   Aquamarine
   120

   130
   Cyan
   140

   150
   Turquoise
   160

   170
   Blue
   180

   190
   Purple
   200

   210
   Magenta
   220

   230
   Maroon
   240

   250-255
   A ramp from dark gray to almost white
   256
   Color By Layer
   */

	AcGePoint3d *Verts = new AcGePoint3d[jd_acd.size()];
	AcGePoint3d pts[2];
	copy(jd_acd.begin(), jd_acd.end(), Verts);

	mode->geometry().polyline(jd_acd.size(), Verts); //The AcGiWorldGeometry object writes vectors to AutoCAD's refresh memory
	if (Verts) {
		delete[] Verts;
	}

	mode->subEntityTraits().setColor(2);

	const static int point_num = 1000;
	AcGePoint3d points[point_num];
	double start_lc;
	double end_lc;
	double interval;
	AcGePoint3d pt;
	ZB zb;
	AcGeVector3d norm(0, 0, 1);
	AcGeVector3d vec;
	ACHAR buf[100];
	AcGeVector3d vecV;
	ZB previous_point;

	for (int i = 0; i < jd_vector.size(); i++) {
		if (i == 0) { // 第一个直线连接起点和下一个直缓
			pts[0].x = lc_vector[i].zh_zb.x;
			pts[0].y = lc_vector[i].zh_zb.y;
			pts[1].x = lc_vector[i + 1].zh_zb.x;
			pts[1].y = lc_vector[i + 1].zh_zb.y;
			mode->subEntityTraits().setColor(2);   //黄色 
			mode->subEntityTraits().setLineWeight(AcDb::kLnWt050); //线宽
			mode->geometry().polyline(2, pts);    //画线
			continue;
		}

		if (i == jd_vector.size() - 1) { //最后一个点连接上个缓直和终点
			pts[0].x = lc_vector[i - 1].hz_zb.x;
			pts[0].y = lc_vector[i - 1].hz_zb.y;
			pts[1].x = lc_vector[i].zh_zb.x;
			pts[1].y = lc_vector[i].zh_zb.y;
			mode->subEntityTraits().setColor(2);   //黄色 
			mode->subEntityTraits().setLineWeight(AcDb::kLnWt050); //线宽
			mode->geometry().polyline(2, pts);    //画线
			continue;
		}


		pts[0].x = lc_vector[i].hz_zb.x;
		pts[0].y = lc_vector[i].hz_zb.y;
		pts[1].x = lc_vector[i + 1].zh_zb.x;
		pts[1].y = lc_vector[i + 1].zh_zb.y;

		mode->subEntityTraits().setColor(2);   //黄色 
		mode->subEntityTraits().setLineWeight(AcDb::kLnWt050); //线宽
		mode->geometry().polyline(2, pts);    //画线



		mode->subEntityTraits().setLineWeight(AcDb::kLnWt050); //线宽
		start_lc = lc_vector[i].zh;
		end_lc = lc_vector[i].hy;
		interval = (end_lc - start_lc) / point_num;
		for (int i = 0; i < point_num; i++) {
			zb = path.lc_to_zb(jd_vector, qx_vector, lc_vector, start_lc + i * interval);
			pt.x = zb.x;
			pt.y = zb.y;
			points[i] = pt;
		}
		mode->subEntityTraits().setColor(190);
		mode->geometry().polyline(point_num, points);

		start_lc = lc_vector[i].yh;
		end_lc = lc_vector[i].hz;
		interval = (end_lc - start_lc) / point_num;
		for (int i = 0; i < point_num; i++) {
			zb = path.lc_to_zb(jd_vector, qx_vector, lc_vector, start_lc + i * interval);
			pt.x = zb.x;
			pt.y = zb.y;
			points[i] = pt;
		}
		mode->subEntityTraits().setColor(190);
		mode->subEntityTraits().setLineWeight(AcDb::kLnWt050); //线宽
		mode->geometry().polyline(point_num, points);

		AcGePoint3d PtStart;
		AcGePoint3d PtOn;
		AcGePoint3d PtEnd;
		PtStart.x = lc_vector[i].hy_zb.x;
		PtStart.y = lc_vector[i].hy_zb.y;
		PtOn.x = lc_vector[i].qz_zb.x;
		PtOn.y = lc_vector[i].qz_zb.y;
		PtEnd.x = lc_vector[i].yh_zb.x;
		PtEnd.y = lc_vector[i].yh_zb.y;
		mode->subEntityTraits().setColor(5);
		mode->subEntityTraits().setLineWeight(AcDb::kLnWt050); //线宽
		mode->geometry().circularArc(PtStart, PtOn, PtEnd);

		mode->subEntityTraits().setLineWeight(AcDb::kLnWt005); //线宽

		mode->subEntityTraits().setColor(4);
		pts[1].x = lc_vector[i].zh_zb.x;
		pts[1].y = lc_vector[i].zh_zb.y;
		previous_point = path.lc_to_zb(jd_vector, qx_vector, lc_vector, ((int)lc_vector[i].zh / 100) * 100); //上一个百米柱的坐标
		pts[0].x = previous_point.x;
		pts[0].y = previous_point.y;
		vec = pts[1] - pts[0];
		vec.normalize();
		vecV = vec.crossProduct(AcGeVector3d::kZAxis); //垂直于直线的方向
		vecV = -vecV; //取反
		pts[0] = pts[1];
		pts[1] = pts[0] + vecV * 100;
		mode->geometry().polyline(2, pts);

		_stprintf(buf, L"  ZH+%.2f", path.get_lc2(lc_vector[i].zh));
		mode->geometry().text(pts[0] + 20 * vec, norm, vecV, 12.0, 1.0, 0, buf);

		pts[1].x = lc_vector[i].hy_zb.x;
		pts[1].y = lc_vector[i].hy_zb.y;
		previous_point = path.lc_to_zb(jd_vector, qx_vector, lc_vector, ((int)lc_vector[i].hy / 100) * 100); //上一个百米柱的坐标
		pts[0].x = previous_point.x;
		pts[0].y = previous_point.y;
		vec = pts[1] - pts[0];
		vec.normalize();
		vecV = vec.crossProduct(AcGeVector3d::kZAxis); //垂直于直线的方向
		vecV = -vecV; //取反
		pts[0] = pts[1];
		pts[1] = pts[0] + vecV * 100;
		mode->geometry().polyline(2, pts);
		_stprintf(buf, L"  HY+%.2f", path.get_lc2(lc_vector[i].hy));
		mode->geometry().text(pts[0] + 20 * vec, norm, vecV, 12.0, 1.0, 0, buf);

		pts[1].x = lc_vector[i].yh_zb.x;
		pts[1].y = lc_vector[i].yh_zb.y;
		previous_point = path.lc_to_zb(jd_vector, qx_vector, lc_vector, ((int)lc_vector[i].yh / 100) * 100); //上一个百米柱的坐标
		pts[0].x = previous_point.x;
		pts[0].y = previous_point.y;
		vec = pts[1] - pts[0];
		vec.normalize();
		vecV = vec.crossProduct(AcGeVector3d::kZAxis); //垂直于直线的方向
		vecV = -vecV; //取反
		pts[0] = pts[1];
		pts[1] = pts[0] + vecV * 100;
		mode->geometry().polyline(2, pts);
		_stprintf(buf, L"  YH+%.2f", path.get_lc2(lc_vector[i].yh));
		mode->geometry().text(pts[0] + 20 * vec, norm, vecV, 12.0, 1.0, 0, buf);

		pts[1].x = lc_vector[i].hz_zb.x;
		pts[1].y = lc_vector[i].hz_zb.y;
		previous_point = path.lc_to_zb(jd_vector, qx_vector, lc_vector, ((int)lc_vector[i].hz / 100) * 100); //上一个百米柱的坐标
		pts[0].x = previous_point.x;
		pts[0].y = previous_point.y;
		vec = pts[1] - pts[0];
		vec.normalize();
		vecV = vec.crossProduct(AcGeVector3d::kZAxis); //垂直于直线的方向
		vecV = -vecV; //取反
		pts[0] = pts[1];
		pts[1] = pts[0] + vecV * 100;
		mode->geometry().polyline(2, pts);
		_stprintf(buf, L"  HZ+%.2f", path.get_lc2(lc_vector[i].hz));
		mode->geometry().text(pts[0] + 20 * vec, norm, vecV, 12.0, 1.0, 0, buf);

	}



	for (int i = 0; i < jd_acd.size(); i++) {

		//写交点坐标信息
		if (i == 0) {
			vec = jd_acd[i + 1] - jd_acd[i];
			vec.normalize();
			vecV = vec.crossProduct(AcGeVector3d::kZAxis); //差乘Z坐标轴得到垂直vec的向量
			pt = jd_acd[i] + vecV * 25.0;
			mode->subEntityTraits().setColor(3);
			_stprintf(buf, L"JD %d", i + 1);
			mode->geometry().text(pt, norm, vec, 15.0, 1.0, 0, buf);

			//写交点坐标
			pt = pt + vecV * 25.0;
			_stprintf(buf, L"(%.2f, %.2f)", jd_acd[i].x, jd_acd[i].y);
			mode->geometry().text(pt, norm, vec, 15.0, 1.0, 0, buf);

			//写壁交点缓和曲线长
			pt = pt + vecV * 25.0;
			_stprintf(buf, L"l0: %.2f", jd_vector[i].jd_l0);
			mode->geometry().text(pt, norm, vec, 15.0, 1.0, 0, buf);

			//写交点圆曲线半径
			pt = pt + vecV * 25.0;
			_stprintf(buf, L"R: %.2f", jd_vector[i].jd_r);
			mode->geometry().text(pt, norm, vec, 15.0, 1.0, 0, buf);

			continue;
		}
		else if (i == jd_acd.size() - 1) {
			vec = jd_acd[i] - jd_acd[i - 1];
			vec.normalize();
			vecV = vec.crossProduct(AcGeVector3d::kZAxis); //差乘Z坐标轴得到垂直vec的向量
			mode->subEntityTraits().setColor(3);
			pt = jd_acd[i] + vecV * 25.0;
			mode->subEntityTraits().setColor(3);
			_stprintf(buf, L"JD %d", i + 1);
			mode->geometry().text(pt, norm, vec, 15.0, 1.0, 0, buf);

			//写交点坐标
			pt = pt + vecV * 25.0;
			_stprintf(buf, L"(%.2f, %.2f)", jd_acd[i].x, jd_acd[i].y);
			mode->geometry().text(pt, norm, vec, 15.0, 1.0, 0, buf);

			//写壁交点缓和曲线长
			pt = pt + vecV * 25.0;
			_stprintf(buf, L"l0: %.2f", jd_vector[i].jd_l0);
			mode->geometry().text(pt, norm, vec, 15.0, 1.0, 0, buf);

			//写交点圆曲线半径
			pt = pt + vecV * 25.0;
			_stprintf(buf, L"R: %.2f", jd_vector[i].jd_r);
			mode->geometry().text(pt, norm, vec, 15.0, 1.0, 0, buf);

			continue;
		}
		else {
			AcGeVector3d left = jd_acd[i - 1] - jd_acd[i];
			AcGeVector3d right = jd_acd[i + 1] - jd_acd[i];
			left.normalize();
			right.normalize();
			vecV = left + right;
			vecV.normalize();
			vec = vecV.crossProduct(AcGeVector3d::kZAxis); //差乘Z坐标轴得到垂直vec的向量
			vec = -vec;
			pt = jd_acd[i] + vecV * 400;
		}

		mode->subEntityTraits().setColor(3);
		_stprintf(buf, L"JD %d", i + 1);
		mode->geometry().text(pt, norm, vec, 15.0, 1.0, 0, buf);

		//写转角信息
		pt = pt + vecV * 25.0;
		Angle a = path.to_angle(qx_vector[i].qx_a);
		_stprintf(buf, L"AZ: %d°%02d'%02d''", a.degree, a.minute, a.second);
		mode->geometry().text(pt, norm, vec, 15.0, 1.0, 0, buf);

		//写交点坐标
		pt = pt + vecV * 25.0;
		_stprintf(buf, L"(%.2f, %.2f)", jd_acd[i].x, jd_acd[i].y);
		mode->geometry().text(pt, norm, vec, 15.0, 1.0, 0, buf);

		//写壁交点缓和曲线长
		pt = pt + vecV * 25.0;
		_stprintf(buf, L"l0: %.2f", jd_vector[i].jd_l0);
		mode->geometry().text(pt, norm, vec, 15.0, 1.0, 0, buf);

		//写交点圆曲线半径
		pt = pt + vecV * 25.0;
		_stprintf(buf, L"R: %.2f", jd_vector[i].jd_r);
		mode->geometry().text(pt, norm, vec, 15.0, 1.0, 0, buf);

		//写切线信息
		pt = pt + vecV * 25.0;
		_stprintf(buf, L"T: %.2f", qx_vector[i].qx_T);
		mode->geometry().text(pt, norm, vec, 15.0, 1.0, 0, buf);



		//写交点圆曲线半径
		pt = pt + vecV * 25.0;
		_stprintf(buf, L"L: %.2f", qx_vector[i].qx_L);
		mode->geometry().text(pt, norm, vec, 15.0, 1.0, 0, buf);


	}
	int qdlc = path.get_qdlc1() * 1000 + path.get_qdlc2();
	int zdlc = path.get_zdlc();
	vector<AcGePoint3d> mark_points;
	pt.x = jd_vector[0].jd_x;
	pt.y = jd_vector[0].jd_y;
	mark_points.push_back(pt);
	for (int i = qdlc + 100; i < zdlc; i += 100) {
		ZB zb = path.lc_to_zb(jd_vector, qx_vector, lc_vector, i);
		pt.x = zb.x;
		pt.y = zb.y;
		mark_points.push_back(pt);
	}
	pt.x = jd_vector[jd_vector.size() - 1].jd_x;
	pt.y = jd_vector[jd_vector.size() - 1].jd_y;
	mark_points.push_back(pt);
	mode->subEntityTraits().setColor(140);
	mode->subEntityTraits().setLineWeight(AcDb::kLnWt005);
	for (int i = 0; i < mark_points.size(); i++) { //标注百米/千米柱
		if (i == 0) { //如果是起点
			vec = mark_points[1] - mark_points[0];
			vec.normalize();
			vecV = vec.crossProduct(AcGeVector3d::kZAxis); //垂直于直线的方向
			vecV = -vecV; //取反
			mode->subEntityTraits().setLineWeight(AcDb::kLnWt005);
			mode->subEntityTraits().setColor(4);
			pt.x = mark_points[0].x;
			pt.y = mark_points[0].y;
			pts[0] = pt;
			pts[1] = pt + vecV * 200;
			mode->geometry().polyline(2, pts);
			mode->subEntityTraits().setColor(4);
			_stprintf(buf, L" +%03d    QD DK%03d+%03d", path.get_qdlc2(), path.get_qdlc1(), path.get_qdlc2());
			mode->geometry().text(pt + 25 * vec, norm, vecV, 12.0, 1.0, 0, buf);
			continue;
		}
		if (i == mark_points.size() - 1) {
			vec = mark_points[mark_points.size() - 2] - mark_points[mark_points.size() - 1];
			vec.normalize();
			vecV = vec.crossProduct(AcGeVector3d::kZAxis); //垂直于直线的方向
			vecV = -vecV; //取反
			mode->subEntityTraits().setColor(4);
			mode->subEntityTraits().setLineWeight(AcDb::kLnWt005);
			pt.x = mark_points[mark_points.size() - 1].x;
			pt.y = mark_points[mark_points.size() - 1].y;
			pts[0] = pt;
			pts[1] = pt + vecV * 200;
			mode->geometry().polyline(2, pts);
			int zdlc1 = (int)zdlc / 1000;
			int zdlc2 = zdlc - zdlc1 * 1000;
			mode->subEntityTraits().setColor(4);
			_stprintf(buf, L" +%03d    ZD DK%03d+%03d", zdlc2, zdlc1, zdlc2);
			mode->geometry().text(pt + 25 * vec, norm, vecV, 12.0, 1.0, 0, buf);
			continue;

		}
		vec = mark_points[i] - mark_points[i - 1];
		vecV = vec.crossProduct(AcGeVector3d::kZAxis); //垂直于直线的方向
		vecV = -vecV; //取反
		pt.x = mark_points[i].x;
		pt.y = mark_points[i].y;

		if (i % 10 != 0) {
			_stprintf(buf, L"-%d", i % 10);
			mode->geometry().text(pt, norm, vecV, 12.0, 1.0, 0, buf);
		}
		else {
			mode->subEntityTraits().setColor(200);
			_stprintf(buf, L"-DK%d", path.get_qdlc1() + i / 10);
			mode->geometry().text(pt, norm, vecV, 12.0, 1.0, 0, buf);
			mode->subEntityTraits().setColor(140);
		}
	}

	return	Adesk::kTrue;
}



Acad::ErrorStatus EntityPath::subGetGripPoints(
	AcGePoint3dArray& gripPoints,
	AcDbIntArray & osnapModes,
	AcDbIntArray & geomIds) const {

	AcGePoint3d Pt;
	Pt.x = (jd_acd[0].x + jd_acd[jd_acd.size() - 1].x) / 2;
	Pt.y = (jd_acd[0].y + jd_acd[jd_acd.size() - 1].y) / 2;
	gripPoints.append(Pt);
	for (int i = 0; i < jd_acd.size(); i++) {
		gripPoints.append(jd_acd[i]);
	}

	return	Acad::eOk;
}


Acad::ErrorStatus   EntityPath::subMoveGripPointsAt(const AcDbIntArray & indices,
	const AcGeVector3d& offset) {
	AcGeMatrix3d move;
	move.setToTranslation(offset);
	int index = indices[0];
	if (index == 0)
	{
		for (int i = 0; i < jd_acd.size(); i++) {
			jd_acd[i].transformBy(move);
			jd_vector[i].jd_x = jd_acd[i].x;
			jd_vector[i].jd_y = jd_acd[i].y;
		}
		path.clean_qx(qx_vector);
		path.clean_lc(lc_vector);
		path.compute_quxian(jd_vector, qx_vector);
		path.compute_licheng(jd_vector, qx_vector, lc_vector, qdlc / 1000, qdlc % 1000);
	}
	else {
		jd_acd[index - 1].transformBy(move);
		jd_vector[index - 1].jd_x = jd_acd[index - 1].x;
		jd_vector[index - 1].jd_y = jd_acd[index - 1].y;
		path.clean_qx(qx_vector);
		path.clean_lc(lc_vector);
		path.compute_quxian(jd_vector, qx_vector);
		path.compute_licheng(jd_vector, qx_vector, lc_vector, qdlc / 1000, qdlc % 1000);
	}
	jd_vector_copy.clear();
	jd_vector_copy = jd_vector;
	

	return	Acad::eOk;
}