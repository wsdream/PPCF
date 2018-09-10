/********************************************************
 * cP_UIPCC.h
 * Author: Jamie Zhu <jimzhu@GitHub>
 * Created: 2014/4/29
 * Last updated: 2015/02/11
********************************************************/

#include <utility>
#include <vector>
#include <map>
using namespace std; 

/* User-based CF */
void UPCC_core(double *removedData, int numUser, int numService, int topK, 
	double *predData, bool isUserSimilarity);

/* Transform a vector into a matrix */ 
double **vector2Matrix(double *vector, int row, int col);

/* Allocate memory for a 2D array */
double **createMatrix(int row, int col);

/* Free memory for a 2D array */ 
void delete2DMatrix(double **ptr);

/* Compute similarity value between two vectors */
double getUserSim(double *uA, double *uB, int numService);
double getServiceSim(double *uA, double *uB, int numUser);

/* Sort a map by value, but return a vector */
bool cmpPairbyValue(const pair<int, double>& lhs, const pair<int, double>& rhs);
vector<pair<int, double> > sortMapByValue(const map<int, double>& pccMap);





