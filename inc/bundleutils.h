#ifndef ___bundleutils_h___

void GetCalcPnt( DB_ADDR dbaBundle, DB_ADDR dbaPointsRec, VECTOR & vCalcPnt, 
	DB_ADDR & dbaCalcPnt );
void GetSurvPnt( DB_ADDR dbaPointsRec, VECTOR & vSurvPnt );
int IsValidTMatrix( struct tmatrix TMatrixRec );

#endif	// #ifdef ___bundleutils_h___
