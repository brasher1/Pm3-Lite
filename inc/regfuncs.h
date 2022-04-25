#ifndef __REGFUNCS_H__
#define __REGFUNCS_H__

int getSurveyModelTransform(double *dest, const char *projectID);
int saveSurveyModelTransform(double *source, const char *projectID);
int getMeasurementTransform(double *dest, const char *projectID);
int saveMeasurementTransform(double *source, const char *projectID);
int getTransform(const char *keyname, double *dest, const char *projectID);
int saveTransform(const char *keyname, double *source, const char *projectID);
int getUnits();
int saveUnits(int units);

#endif//__REGFUNCS_H__