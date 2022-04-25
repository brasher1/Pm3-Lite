// elem_help.cpp
//
#include "stdafx.h"
#include "fotobrow.h"
#include "fotobdoc.h"
#include "mainfrm.h"
#include "fotobvw.h"

#include "2Dedge.h"
#include "elem_help.h"

int getSelectedEdge(Edge_2D *m_currentEdge, int flag /*=FALSE*/)
{
	CDocument *pDoc;
	CObList *pList = new CObList;
	((CFotobrowApp *)AfxGetApp())->GetDocumentList(pList);
	POSITION pos = pList->GetHeadPosition();
	while( pos )
	{
		pDoc = (CDocument *)(pList->GetNext(pos));
		if( pDoc->IsKindOf( RUNTIME_CLASS( CFotobrowDoc) ) )
		{
			POSITION pos2 = pDoc->GetFirstViewPosition();
			while(pos2)
			{
				CFotobrowView *view = 
					((CFotobrowView *)(pDoc->GetNextView(pos2)));
				ASSERT(view);
				if( view->m_pointList.GetCount() )
				{//CList
					m_currentEdge->m_szImageName = _strdup(((CFotobrowDoc *)pDoc)->szName);
					m_currentEdge->m_pointList = 
						matrix(1,view->m_pointList.GetCount(),1,3);
					POSITION pos3 = view->m_pointList.GetHeadPosition();
					m_currentEdge->m_iNumPoints = view->m_pointList.GetCount();
					int i=1;
					while(pos3)
					{
						CPoint *p = view->m_pointList.GetNext(pos3);
						(*(m_currentEdge->m_pointList))[i][1] = (double)(p->x);
						(*(m_currentEdge->m_pointList))[i][2] = (double)(p->y);
						(*(m_currentEdge->m_pointList))[i][3] = 0.0;
						i++;
					}
					if ( flag )
						view->cleanMultiSelect();
					while( pList->GetCount() )
						pList->RemoveTail();
					delete pList;
					return TRUE;
				}
			}
		}
	}
	while( pList->GetCount() )
		pList->RemoveTail();
	delete pList;
	return FALSE;
}

int getSelectedPoint(Point_2D &point, char *pszImageName /*=NULL*/)
{
	CDocument *pDoc;
	CObList *pList = new CObList;
	((CFotobrowApp *)AfxGetApp())->GetDocumentList(pList);
	POSITION pos = pList->GetHeadPosition();
	while( pos )
	{
		pDoc = (CDocument *)(pList->GetNext(pos));
		if( pDoc->IsKindOf( RUNTIME_CLASS( CFotobrowDoc) ) )
		{
			POSITION pos2 = pDoc->GetFirstViewPosition();
			while(pos2)
			{
				CFotobrowView *view = 
					((CFotobrowView *)(pDoc->GetNextView(pos2)));
				ASSERT(view);
				if( view->m_selected )
				{
					if( pszImageName && strcmp(pszImageName, ((CFotobrowDoc *)pDoc)->szName)!=0 )
					{
					}
					else
					{
						point.setImageName((char *)(const char *)((CFotobrowDoc *)pDoc)->szName);
						VECTOR v = vector(1,3);
						v[1] = (double)(view->m_sel.x);
						v[2] = (double)(view->m_sel.y);
						v[3] = 0.0;
						point.setPoint(v);
						free_vector(v,1,3);
						while( pList->GetCount() )
							pList->RemoveTail();
						delete pList;
						return TRUE;
					}
				}
			}
		}
	}
	while( pList->GetCount() )
		pList->RemoveTail();
	delete pList;
	return FALSE;
}