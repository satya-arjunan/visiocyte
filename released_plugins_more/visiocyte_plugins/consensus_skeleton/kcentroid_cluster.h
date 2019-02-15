#ifndef __KCENTROID_CLUSTER_H_
#define __KCENTROID_CLUSTER_H_

#include <vector>
#include <stdio.h>
using namespace std;

#include <climits>

//K-centroid clustering
//by Y. Wan
//11-10-14

double computeDist2(vector<double> & v1, vector<double> & v2)
{
	if (v1.size()!=v2.size())
	{
		printf("error in computeDist2: vector size must agree.\n");
		return -1;
	}
	double res = 0;
	for (VISIOCYTELONG i=0;i<v1.size();i++)
		res += (v1[i]-v2[i])*(v1[i]-v2[i]);
	return res;
};

bool kcentroid_cluster(vector<vector<double> > & feature, VISIOCYTELONG clusterNum, vector<VISIOCYTELONG> & clusterTag, vector<VISIOCYTELONG> & clusterCen)
{
	//check parameters
	VISIOCYTELONG pntNum = feature.size();
	if (pntNum <= 0)
	{
		fprintf(stderr,"error in feature space: no point.\n");
		return false;
	}
	VISIOCYTELONG pntDim = feature[0].size();
	for (VISIOCYTELONG i=0;i<pntNum;i++)
	{
		if (feature[i].size()!=pntDim)
		{
			fprintf(stderr,"error in feature space: feature dim must agree.\n");
			return false;
		}
	}
	if (clusterNum<=0 || clusterNum>pntNum)
	{
		fprintf(stderr,"illegal cluster number: it must be between 1 to pntNum.\n");
		return false;
	}
	clusterTag = vector<VISIOCYTELONG>(pntNum,-1);
	clusterCen = vector<VISIOCYTELONG>(clusterNum, 0);

	//initialize: min-max method
//	if (clusterNum==1) clusterCen[0] = 0;
//	
//	else {
//		double maxDist2 = 0;
//		VISIOCYTELONG first,second;
//		for (VISIOCYTELONG i=0;i<pntNum;i++)
//			for (VISIOCYTELONG j=0;j<i;j++)
//				if (computeDist2(feature[i],feature[j])>maxDist2)
//				{
//					maxDist2 = computeDist2(feature[i],feature[j]);
//					first = i;
//					second = j;
//				}
//		clusterCen[0] = first;
//		clusterCen[1] = second;
//		VISIOCYTELONG clusterSet = 2;
//		while (clusterSet<=clusterNum)
//		{
//			maxDist2 = 0;
//			VISIOCYTELONG newMember;
//			for (VISIOCYTELONG j=0;j<pntNum;j++)
//			{
//				double minCenDist2 = INF;
//				for (VISIOCYTELONG i=0;i<clusterSet;i++)
//					if (computeDist2(feature[clusterCen[i]],feature[j])<minCenDist2) minCenDist2 = computeDist2(feature[clusterCen[i]],feature[j]);
//				if (minCenDist2>maxDist2)
//				{
//					maxDist2 = minCenDist2;
//					newMember = j;
//				}
//			}
//			clusterCen[clusterSet] = newMember;
//			clusterSet++;
//		}
//	}

	for (VISIOCYTELONG i=0;i<clusterNum;i++)
		clusterCen[i] = i;

	//begin loop
	double clusterChange = 1;
	VISIOCYTELONG iter = 0;
	bool validClusters;
	while (clusterChange>1e-6 && iter<INF)
	{
		iter++;
		validClusters = true;
		//printf("iter=%d\n",(int)iter);
		clusterChange = 0;
		vector<VISIOCYTELONG> newCen(clusterNum, -1);
		vector<vector<VISIOCYTELONG> > clusters(clusterNum,vector<VISIOCYTELONG>());
		for (VISIOCYTELONG i=0;i<pntNum;i++)
		{
			double minDist2 = INF;
			for (VISIOCYTELONG j=0;j<clusterNum;j++)
			{
				double dist2 = computeDist2(feature[i],feature[clusterCen[j]]);
				if (dist2<minDist2)
				{
					minDist2 = dist2;
					clusterTag[i] = j;
				}
			}
			clusters[clusterTag[i]].push_back(i);
		}

		for (VISIOCYTELONG i=0;i<clusterNum;i++)
		{
			//printf("cluster:%d\t",(int)i);
			double minDev = LONG_MAX;
			for (VISIOCYTELONG j=0;j<clusters[i].size();j++)
			{
				//printf("%d\t",(int)clusters[i][j]);
				double sum = 0;
				for (VISIOCYTELONG k=0;k<clusters[i].size();k++)
					sum += computeDist2(feature[clusters[i][j]],feature[clusters[i][k]]);

				if (sum<=minDev)
				{
					minDev = sum;
					newCen[i] = clusters[i][j];
				}
			}
			//printf("center:%d\n",(int)newCen[i]);
		}
		
		//if empty clusters are observed, replace the centroid with a point that is furthest to its current centroid
		vector<bool> replaceTag(clusterNum, false);
		for (VISIOCYTELONG i=0;i<clusterNum;i++)
		{
			if (clusters[i].size()==0)
			{
				validClusters = false;
				double maxDist = 0;
				VISIOCYTELONG replaceEmpty = -1;
				VISIOCYTELONG oldCluster = -1;
				for (VISIOCYTELONG j=0;j<clusterNum;j++)
				{
					if (replaceTag[j]) continue;
					for (VISIOCYTELONG k=0;k<clusters[j].size();k++)
					{
						double singleLength = computeDist2(feature[newCen[j]], feature[clusters[j][k]]);
						if (maxDist<=singleLength)
						{
							maxDist = singleLength;
							replaceEmpty = clusters[j][k];
							oldCluster = j;
						}
					}
				}
				
				replaceTag[oldCluster] = true;
				newCen[i] = replaceEmpty;
			}
		}

		for (VISIOCYTELONG i=0;i<clusterNum;i++)
			clusterChange += computeDist2(feature[newCen[i]],feature[clusterCen[i]]);

		clusterCen = newCen;

		if (!validClusters) //there are empty clusters in the result, do another round
			clusterChange = 1;
	}
	return true;
};

#endif
