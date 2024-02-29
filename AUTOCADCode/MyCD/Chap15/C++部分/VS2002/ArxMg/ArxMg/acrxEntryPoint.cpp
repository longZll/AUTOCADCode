// (C) Copyright 2002-2005 by Autodesk, Inc. 
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
//----- acrxEntryPoint.h
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"

extern "C" __declspec(dllexport) int InsertDwg(LPCTSTR fileName, 
											   LPCTSTR blockName,
											   AcGePoint3d insertPt, 
											   AcDbObjectId &entId)
{
	AcDbDatabase pExternalDb(Adesk::kFalse);    
	TCHAR str[300];
	strcpy(str, fileName); 

	Acad::ErrorStatus es = pExternalDb.readDwgFile(str);
	if (es != Acad::eOk)
	{
		return es;
	}

	AcDbDatabase *pTempDb;             
	es = pExternalDb.wblock(pTempDb);
	if (es != Acad::eOk)
	{
		return es;
	}

	AcDbObjectId blkId;
	es = acdbHostApplicationServices()->workingDatabase()->insert
		(blkId, blockName, pTempDb);
	if (es != Acad::eOk)
	{
		return es;
	}

	AcDbBlockReference *pBlkRef = new AcDbBlockReference ; 
	pBlkRef->setBlockTableRecord (blkId) ; 
	pBlkRef->setScaleFactors(AcGeScale3d(1.0, 1.0, 1.0));
	pBlkRef->setPosition(insertPt); 
	pBlkRef->setRotation (0.0); 

	AcDbBlockTable *pBt ; 
	acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBt, 
		AcDb::kForRead); 
	AcDbBlockTableRecord *pBtr; 
	pBt->getAt (ACDB_MODEL_SPACE, pBtr, AcDb::kForWrite) ; 
	pBt->close () ; 
	pBtr->appendAcDbEntity(entId, pBlkRef);
	pBtr->close();
	pBlkRef->close(); 

	delete pTempDb;
	return es;
}

//-----------------------------------------------------------------------------
#define szRDS _RXST("ahlzl")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CArxMgApp : public AcRxArxApp {

public:
	CArxMgApp () : AcRxArxApp () {}

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

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CArxMgApp)

