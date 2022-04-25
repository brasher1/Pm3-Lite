//2Dedge.h
//
#ifndef __2DEDGE_H__
#define __2DEDGE_H__

#include "photog_math.h"

class Point_2D
{
private:
	char *m_szImageName;
	VECTOR m_Point;
public:
	Point_2D() { m_szImageName=NULL; m_Point=NULL;}
	~Point_2D() 
	{ 
		if( m_szImageName ) 
			free(m_szImageName);
		if( m_Point ) 
			free_vector(m_Point, 1,3);
	}
	void setImageName(char *name)
	{
		if( m_szImageName )
			free(m_szImageName);
		if( name )
			m_szImageName = strdup(name);
		else
			m_szImageName = NULL;
	}
	const char *getImageName() { return m_szImageName; }
	void setPoint( VECTOR point )
	{
		if( !m_Point )
			m_Point = vector(1,3);
		m_Point[1] = point[1];
		m_Point[2] = point[2];
		m_Point[3] = point[3];
	}
	VECTOR getPoint()
	{
		return m_Point;
	}
};

class Edge_2D 
{
public:
	Edge_2D()
	{
		m_pointList = NULL;
		m_szImageName = NULL;
//		m_dirPoint = NULL;
		m_iNumPoints = 0;
	}
	~Edge_2D()
	{
		if( m_pointList )
			free_matrix( m_pointList,1,m_iNumPoints,1,3);
		if( m_szImageName )
			free(m_szImageName);
//		if( m_dirPoint )
//			free_vector(m_dirPoint,1,3);
	}

//private:
	PNTLST m_pointList;
	int m_iNumPoints;
	char *m_szImageName;
	Point_2D m_dirPoint;
};

#endif //__2DEDGE_H__
