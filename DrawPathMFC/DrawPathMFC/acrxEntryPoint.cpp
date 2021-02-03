// (C) Copyright 2002-2012 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "Dlg.h"
#include <string>
#include "EntityPath.h"
//-----------------------------------------------------------------------------
#define szRDS _RXST("")
EntityPath* path;
Dlg dlg;
//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CDrawPathMFCApp : public AcRxArxApp {

public:
	CDrawPathMFCApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: Add your initialization code here

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}
	
	// The ACED_ARXCOMMAND_ENTRY_AUTO macro can be applied to any static member 
	// function of the CDrawPathMFCApp class.
	// The function should take no arguments and return nothing.
	//
	// NOTE: ACED_ARXCOMMAND_ENTRY_AUTO has overloads where you can provide resourceid and
	// have arguments to define context and command mechanism.
	
	// ACED_ARXCOMMAND_ENTRY_AUTO(classname, group, globCmd, locCmd, cmdFlags, UIContext)
	// ACED_ARXCOMMAND_ENTRYBYID_AUTO(classname, group, globCmd, locCmdId, cmdFlags, UIContext)
	// only differs that it creates a localized name using a string in the resource file
	//   locCmdId - resource ID for localized command

	// Modal Command with localized name
	// ACED_ARXCOMMAND_ENTRY_AUTO(CDrawPathMFCApp, MyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL)
	static void MyGroupMyCommand () {
		//AcGePoint3d ptStart(500, 500, 0);
		//AcGePoint3d ptEnd(560, 500, 0);
		//AcGePoint3d ptStart1(0, 500, 0);
		//AcGePoint3d ptEnd1(0, 500, 0);
		//AcDbLine *pLine = new AcDbLine(ptStart, ptEnd);

		////获得指向块表的指针
		//AcDbBlockTable *pBlockTable;
		//acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForRead);

		////获得指向特定的块表记录(模型空间)的指针
		//AcDbBlockTableRecord *pBlockTableRecord;
		//pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord,
		//	AcDb::kForWrite);

		////将AcDbLine类的对象添加到块表记录中
		//AcDbObjectId lineId;
		//pBlockTableRecord->appendAcDbEntity(lineId, pLine);
		//pLine->setColorIndex(1); //设置直线为红色
		//int color;

		//
		///*pLine = new AcDbLine(ptStart1, ptEnd1);
		//actrTransactionManager->flushGraphics();
		//acedUpdateDisplay();*/
		////关闭图形数据库的各种对象
		////pBlockTable->close();
		////pBlockTableRecord->close();
		////pLine->close();

		//acedGetInt(L"请输入起点里程(数字部分):\n", &color);
		//
		////acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForRead);

		//////获得指向特定的块表记录(模型空间)的指针
	
		////pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord,
		////	AcDb::kForWrite);

		////将AcDbLine类的对象添加到块表记录中
		//
		//pBlockTableRecord->appendAcDbEntity(lineId, pLine);
		//pLine->setColorIndex(2); //设置直线为红色
		///*pLine = new AcDbLine(ptStart1, ptEnd1);
		//actrTransactionManager->flushGraphics();
		//acedUpdateDisplay();*/
		//actrTransactionManager->flushGraphics();
		//acedUpdateDisplay();
		////关闭图形数据库的各种对象
		//pBlockTable->close();
		//pBlockTableRecord->close();
		//pLine->close();
		/*string prompt = "请输入起点里程(数字部分):";
		acedGetInt(L"请输入起点里程(数字部分):\n", &qdlc);*/
		// Put your command code here

		
		
		
		if (path == NULL) {
			if (dlg.DoModal() == IDOK) {
				path = new EntityPath(dlg.path, dlg.jd_vector, dlg.qx_vector, dlg.lc_vector, dlg.path.get_qdlc1() * 1000 + dlg.path.get_qdlc2());
				path->rxInit();
				acrxBuildClassHierarchy();
				AcDbBlockTable *pBlockTable;
				acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlockTable,
					AcDb::kForRead);
				pBlockTable->close();
				AcDbBlockTableRecord *pBlockTableRecord;
				pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);
				pBlockTableRecord->appendAcDbEntity(path);
				pBlockTableRecord->close();
				path->close();
			}
		}
		else {
			dlg.path = path->get_path();
			dlg.path.clean_jd(dlg.jd_vector);
			dlg.path.clean_qx(dlg.qx_vector);
			dlg.path.clean_lc(dlg.lc_vector);
			dlg.jd_vector = path->get_jd_vector();
			dlg.path.compute_quxian(dlg.jd_vector, dlg.qx_vector);
			dlg.path.compute_licheng(dlg.jd_vector, dlg.qx_vector, dlg.lc_vector, dlg.qdlc / 1000, dlg.qdlc % 1000);
	
			
			if (dlg.DoModal() == IDOK) {
				path->set_jd_vector(dlg.jd_vector);
				path->set_qdlc(dlg.qdlc);
				path->recompute();
				AcDbBlockTable *pBlockTable;
				acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlockTable,
					AcDb::kForRead);
				pBlockTable->close();
				AcDbBlockTableRecord *pBlockTableRecord;
				pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);
				actrTransactionManager->flushGraphics();
				acedUpdateDisplay();
				pBlockTableRecord->close();
				path->close();
			}
		}
			
			
	

		/*else {
			actrTransactionManager->flushGraphics();
			acedUpdateDisplay();
		}*/
		// Put your command code here
		
		

	}

	
	// Modal Command with pickfirst selection
	// ACED_ARXCOMMAND_ENTRY_AUTO(CDrawPathMFCApp, MyGroup, MyPickFirst, MyPickFirstLocal, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET)
	static void MyGroupMyPickFirst () {
		ads_name result ;
		int iRet =acedSSGet (ACRX_T("_I"), NULL, NULL, NULL, result) ;
		if ( iRet == RTNORM )
		{
			// There are selected entities
			// Put your command using pickfirst set code here
		}
		else
		{
			// There are no selected entities
			// Put your command code here
		}
	}

	// Application Session Command with localized name
	// ACED_ARXCOMMAND_ENTRY_AUTO(CDrawPathMFCApp, MyGroup, MySessionCmd, MySessionCmdLocal, ACRX_CMD_MODAL | ACRX_CMD_SESSION)
	static void MyGroupMySessionCmd () {
		// Put your command code here
		
	}

	// The ACED_ADSFUNCTION_ENTRY_AUTO / ACED_ADSCOMMAND_ENTRY_AUTO macros can be applied to any static member 
	// function of the CDrawPathMFCApp class.
	// The function may or may not take arguments and have to return RTNORM, RTERROR, RTCAN, RTFAIL, RTREJ to AutoCAD, but use
	// acedRetNil, acedRetT, acedRetVoid, acedRetInt, acedRetReal, acedRetStr, acedRetPoint, acedRetName, acedRetList, acedRetVal to return
	// a value to the Lisp interpreter.
	//
	// NOTE: ACED_ADSFUNCTION_ENTRY_AUTO / ACED_ADSCOMMAND_ENTRY_AUTO has overloads where you can provide resourceid.
	
	//- ACED_ADSFUNCTION_ENTRY_AUTO(classname, name, regFunc) - this example
	//- ACED_ADSSYMBOL_ENTRYBYID_AUTO(classname, name, nameId, regFunc) - only differs that it creates a localized name using a string in the resource file
	//- ACED_ADSCOMMAND_ENTRY_AUTO(classname, name, regFunc) - a Lisp command (prefix C:)
	//- ACED_ADSCOMMAND_ENTRYBYID_AUTO(classname, name, nameId, regFunc) - only differs that it creates a localized name using a string in the resource file

	// Lisp Function is similar to ARX Command but it creates a lisp 
	// callable function. Many return types are supported not just string
	// or integer.
	// ACED_ADSFUNCTION_ENTRY_AUTO(CDrawPathMFCApp, MyLispFunction, false)
	static int ads_MyLispFunction () {
		//struct resbuf *args =acedGetArgs () ;
		
		// Put your command code here

		//acutRelRb (args) ;
		
		// Return a value to the AutoCAD Lisp Interpreter
		// acedRetNil, acedRetT, acedRetVoid, acedRetInt, acedRetReal, acedRetStr, acedRetPoint, acedRetName, acedRetList, acedRetVal

		return (RTNORM) ;
	}
	
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CDrawPathMFCApp)
ACED_ARXCOMMAND_ENTRY_AUTO(CDrawPathMFCApp, MyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CDrawPathMFCApp, MyGroup, MyPickFirst, MyPickFirstLocal, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CDrawPathMFCApp, MyGroup, MySessionCmd, MySessionCmdLocal, ACRX_CMD_MODAL | ACRX_CMD_SESSION, NULL)
ACED_ADSSYMBOL_ENTRY_AUTO(CDrawPathMFCApp, MyLispFunction, false)

