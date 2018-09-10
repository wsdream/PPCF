/********************************************************
 * cP_UIPCC.cpp: C++ implements on P_UIPCC
 * Author: Jamie Zhu <jimzhu@GitHub>
 * Created: 2014/4/29
 * Last updated: 2015/02/11
********************************************************/

#include <iostream>
#include <cstring>
#include <map>
#include <vector>
#include <utility>
#include <algorithm> 
#include <cmath>
#include "cP_UIPCC.h"
using namespace std;


void UPCC_core(double *removedData, int numUser, int numService, int topK, 
	double *predData, bool isUserSimilarity)
{	
    double **predMatrix = vector2Matrix(predData, numUser, numService);
    double **removedMatrix = vector2Matrix(removedData, numUser, numService);

    int i, j;
    double **pccMatrix = createMatrix(numUser, numUser);

    // compute mean values
    double *uMean = new double[numUser];
    for (i = 0; i < numUser; i++) {
    	int cnt = 0;
    	double sum = 0;
    	for (j = 0; j < numService; j++) {
    		if (removedMatrix[i][j] != 0) {
    			sum += removedMatrix[i][j];
    			cnt++;
    		}
    	}
    	uMean[i] = sum / cnt;
    }

  	for (i = 0; i < numUser; i++) {
  		// compute similarity
  		map<int, double> pccMap;
  		for (j = 0; j < numUser; j++) {
  			if(uMean[i] == 0 || uMean[j] == 0) continue;
  			double pccValue = 0;
  			if (j > i) {
  				if (isUserSimilarity == true) {	
					pccValue = getUserSim(removedMatrix[i], removedMatrix[j], numService);
				}
				else {
					pccValue = getServiceSim(removedMatrix[i], removedMatrix[j], numService);
				}
				pccMatrix[i][j] = pccValue;
				pccMatrix[j][i] = pccValue;
  			}
  			else if (j < i) {
  				pccValue = pccMatrix[i][j];
  			}
			// find similar users
			if (pccValue > 0) pccMap[j] = pccValue;
  		}

  		vector<pair<int, double> > sortedPccMap = sortMapByValue(pccMap);

  		// predict the value for each entry 
		for (j = 0; j < numService; j++) {
			if(removedMatrix[i][j] != 0) continue; // Skip the remained entries
			
			int k = 0;
			double pccSum = 0; 
			double predValue = 0;
			vector<pair<int, double> >::iterator it = sortedPccMap.begin();
			while (k < topK && it != sortedPccMap.end()) {
				int userID = it->first;
				double userPCCValue = it->second;
				it++;
				// if the similar user does not use this item previously, can not be used. 
				if (removedMatrix[userID][j] == 0) continue;				
				pccSum += userPCCValue;
				k++;
				predValue += userPCCValue * removedMatrix[userID][j];
			}

			// no similar users, use umean. 
			if (pccSum == 0) {
				predValue = 0;
			} 
			else {
				predValue = predValue / pccSum;
			}

			predMatrix[i][j] = predValue;
		}
  	}

  	delete2DMatrix(pccMatrix);
    delete ((char*) predMatrix);
    delete ((char*) removedMatrix);
}


double **vector2Matrix(double *vector, int row, int col)  
{
	double **matrix = new double *[row];
	if (!matrix) {
		cout << "Memory allocation failed in vector2Matrix." << endl;
		return NULL;
	}

	int i;
	for (i = 0; i < row; i++) {
		matrix[i] = vector + i * col;  
	}
	return matrix;
}


double **createMatrix(int row, int col) {
    double **matrix = new double *[row];
    matrix[0] = new double[row * col];
    memset(matrix[0], 0, row * col * sizeof(double)); // Initialization
    int i;
    for (i = 1; i < row; i++) {
    	matrix[i] = matrix[i - 1] + col;
    }
    return matrix;
}


void delete2DMatrix(double **ptr) {
	delete ptr[0];
	delete ptr;
}


double getUserSim(double *uA, double *uB, int numService) {
	vector<int> commonIndex, uAIndex, uBIndex;
	int i;
	for (i = 0; i < numService; i++) {
		if(uA[i] != 0 && uB[i] != 0) {
			commonIndex.push_back(i);
		}
		if(uA[i] != 0 ) {
			uAIndex.push_back(i);
		}
		if(uB[i] != 0) {
			uBIndex.push_back(i);
		}
	}

	// no common rate items. 
	if(commonIndex.size() < 2) return 0;

	double upperAll = 0;
	for (i = 0; i < commonIndex.size(); i++) {
		int key = commonIndex[i];
		double valueA = uA[key];
		double valueB = uB[key];
		upperAll += valueA * valueB;
	}
	double pcc = upperAll / sqrt(uAIndex.size() * uBIndex.size());

	return pcc;
}


double getServiceSim(double *uA, double *uB, int numUser) {
	vector<int> commonIndex, uAIndex, uBIndex;
	int i;
	for (i = 0; i < numUser; i++) {
		if(uA[i] != 0 && uB[i] != 0) {
			commonIndex.push_back(i);
		}
	}

	// no common rate items. 
	if(commonIndex.size() < 2) return 0;

	double upperAll = 0;
	double downAllA = 0;
	double downAllB = 0;
	for (i = 0; i < commonIndex.size(); i++) {
		int key = commonIndex[i];
		double valueA = uA[key];
		double valueB = uB[key];
		upperAll += valueA * valueB;
		downAllA += valueA * valueA;
		downAllB += valueB * valueB;
	}
	double downValue = sqrt(downAllA * downAllB);
	if(downValue == 0) return 0;

	double pcc = upperAll / downValue;

	return pcc;
}


bool cmpPairbyValue(const pair<int, double>& lhs, const pair<int, double>& rhs) {  
  return lhs.second > rhs.second;  
}  
  

vector<pair<int, double> > sortMapByValue(const map<int, double>& pccMap) {
	vector<pair<int, double> > vec(pccMap.begin(), pccMap.end());  
    sort(vec.begin(), vec.end(), cmpPairbyValue); 
	return vec;
}

