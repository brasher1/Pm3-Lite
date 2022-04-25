// img_help.cpp
//
#include "stdafx.h"

#include "fotobrow.h"
#include "common.h"
#include "photog.h"
#include "fotobdoc.h"

BOOL OpenImageByName( char *name )
{
	char temp[128];
	struct images image;

	CFotobrowApp *app = (CFotobrowApp *)AfxGetApp();

	rdmRecLock(IMAGES, "r", TASKER);
	int i = dt_keyfind(IMAGES_SZNAME,name, TASKER);
	if( i != S_OKAY )
	{
		AfxMessageBox("ERROR: Image Record Not Found");
		return FALSE;
	}
	dt_recread( &image, TASKER);
	rdmRecFree(IMAGES,TASKER);

	sprintf(temp,"%s%s", (const char *)(app->m_ImagePath), image.szFileName );
//	AfxMessageBox(temp);
	CDocument *pDoc = AfxGetApp()->OpenDocumentFile(temp);
	ASSERT(pDoc);
	if( pDoc == NULL )
	{
//		AfxMessageBox("CDocument not created");
		return FALSE;
	}
  	((CFotobrowDoc *)pDoc)->setImageName(name);
	return TRUE;

#ifdef THIS_IS_GOOD_STUFF_JUST_NOT_FOR_HERE
	CMDIChildWnd *pNewFrame;
//	CreateOrActivateFrame(((CFotobrowApp *)AfxGetApp())->m_pGraphicTemplate,
//		RUNTIME_CLASS(CFotoView));
	CDocTemplate *pTemplate = ((CFotobrowApp *)AfxGetApp())->m_pDocTemplate;
	
	TRACE("The Document exists and making a new frame\n");
	pNewFrame = (CMDIChildWnd*)(pTemplate->CreateNewFrame(pDoc, NULL));
	ASSERT(pNewFrame);
	TRACE("Calling Initial Update\n");
	pTemplate->InitialUpdateFrame(pNewFrame, pDoc, TRUE);
	return TRUE;
#endif
}

void ConvertPoint(_Point3d *in_ptg, _Point3d *out_ptg)
{
	_Point3d temp;

	double *TRANS = ((double *)((CFotobrowApp *)(AfxGetApp()))->GetTransform());
	
	temp.x = (TRANS[0]*in_ptg->x + TRANS[1]*in_ptg->y + TRANS[2]*in_ptg->z + TRANS[9] );
	temp.y = (TRANS[3]*in_ptg->x + TRANS[4]*in_ptg->y + TRANS[5]*in_ptg->z + TRANS[10] );
	temp.z = (TRANS[6]*in_ptg->x + TRANS[7]*in_ptg->y + TRANS[8]*in_ptg->z + TRANS[11] );
//	memcpy(out_ptg,&temp,sizeof(temp));
	out_ptg->x = temp.x;
	out_ptg->y = temp.y;
	out_ptg->z = temp.z;
}
// img_help.cpp
