//consensus_skeleton.cpp
//generate consensus tree from multiple neurons
//by Yinan Wan
//(1) cluster nearby nodes over all trees
//(2) build a confidence graph from clustering results, nodes->cluster centers & edges->w(e)=conf=1/link(cluter1,cluster2)
//(3) final result is a minimum spanning tree  from the c-graph

//2011-10-14: create the project
//2012-01-24: change the definition of confidence map
//2012-01-26: try mean shift clustering
//2012-02-14: use dijstra's algorithm to construct mst

#define INF 100000000

#include "consensus_skeleton.h"
#include "kcentroid_cluster.h"
#include "mst_dij.h"
#include "mst_prim.h"
#include <QtGlobal>
#include <math.h>

bool consensus_skeleton(vector<NeuronTree> & nt_list, QList<NeuronSWC> & merge_result, VISIOCYTELONG n_sampling, int method_code)
{
	int neuronNum = nt_list.size();

	printf("(1). clustering over all the nodes.\n");
	if (n_sampling==0)
//default case: initial cluster number is the average size of all input trees
	{
		for (int i=0;i<neuronNum;i++)
				n_sampling += nt_list[i].listNeuron.size();
		n_sampling /= neuronNum;
	}
	vector<vector<double> > feature;
	QHash<VISIOCYTELONG, pair<VISIOCYTELONG,VISIOCYTELONG> > feaMap;//map the feature space to original nodes
//	double bandwidth = 0; //bw equals to average link length
//	VISIOCYTELONG linkNum = 0;
	for (int i=0;i<neuronNum;i++)
		for (VISIOCYTELONG j=0;j<nt_list[i].listNeuron.size();j++)
		{
			NeuronSWC s = nt_list[i].listNeuron.at(j);
			vector<double> cur(3,0);
			cur[0] = s.x;
			cur[1] = s.y;
			cur[2] = s.z;
			feature.push_back(cur);
			pair<VISIOCYTELONG,VISIOCYTELONG> idx(i,j);
			feaMap.insert(feature.size()-1, idx);
//			if (s.pn<0) continue;
//			NeuronSWC s_par = nt_list[i].listNeuron.at(s.pn-1);
//			bandwidth += sqrt((s.x-s_par.x)*(s.x-s_par.x)+(s.y-s_par.y)*(s.y-s_par.y)+(s.z-s_par.z)*(s.z-s_par.z));
//			linkNum++;
		}

	VISIOCYTELONG pntNum = feature.size();
	vector<VISIOCYTELONG> clusterTag, clusterCen;
//	vector<VISIOCYTELONG> clusterTag;
//	vector<vector<double> > clusterCen;
	if (!kcentroid_cluster(feature, n_sampling, clusterTag, clusterCen))
	{
		fprintf(stderr,"error in kcentroid_clustering.\n");
		return false;
	}
//
//	//compute bandwidth for mean shift
//	bandwidth = bandwidth/(linkNum*2);
//	if (!mean_shift(feature, clusterTag, clusterCen,bandwidth))
//	{
//		fprintf(stderr, "error in mean shift clustering.\n");
//		return false;
//	}
//
//	for (VISIOCYTELONG i=0;i<pntNum;i++)
//		printf("%d\t", clusterTag[i]);
//	printf("\n\n");
//	for (VISIOCYTELONG i=0;i<clusterCen.size();i++)
//	{
//		for (int j=0;j<3;j++)
//		printf("%.3f\t", clusterCen[i][j]);
//		printf("\n");
//	}

	printf("(2). generating confidence graph.\n");
	double * adjMatrix;
	VISIOCYTELONG * plist;
//	n_sampling = clusterCen.size();
	try{
		adjMatrix = new double[n_sampling*n_sampling];
		plist = new VISIOCYTELONG[n_sampling];
		for (VISIOCYTELONG i=0;i<n_sampling*n_sampling;i++) adjMatrix[i] = 0;
	}
	catch (...)
	{
		fprintf(stderr,"fail to allocate memory.\n");
		if (adjMatrix) {delete[] adjMatrix; adjMatrix=0;}
		if (plist) {delete[] plist; plist=0;}
		return false;
	}

	vector<VISIOCYTELONG> clusterSize(n_sampling, 0);
	for (int i=0;i<neuronNum;i++)
	{
		for (VISIOCYTELONG j=0;j<nt_list[i].listNeuron.size();j++)
		{
			NeuronSWC cur = nt_list[i].listNeuron[j];
			if (cur.pn<0) continue;
			VISIOCYTELONG col,row;
			col = clusterTag[feaMap.key(pair<VISIOCYTELONG,VISIOCYTELONG>(i,j))];
			VISIOCYTELONG pid=nt_list[i].hashNeuron.value(cur.pn);
			row = clusterTag[feaMap.key(pair<VISIOCYTELONG,VISIOCYTELONG>(i,pid))];
			adjMatrix[col*n_sampling+row]++;
			adjMatrix[row*n_sampling+col]++;
			clusterSize[col]++;
		}
	}

//	for (VISIOCYTELONG i=0;i<n_sampling;i++)
//		for (VISIOCYTELONG j=i+1;j<n_sampling;j++)
//		{
//			if (adjMatrix[i*n_sampling+j]>neuronNum)
//			{
//				adjMatrix[i*n_sampling+j] = 1;
//				adjMatrix[j*n_sampling+i] = 1;
//			}
//			else {
//				adjMatrix[i*n_sampling+j] = adjMatrix[i*n_sampling+j]/neuronNum;
//				adjMatrix[j*n_sampling+i] = adjMatrix[j*n_sampling+i]/neuronNum;
//			}
//		}
	
	
	printf("(3). computing minimum-spanning tree.\n");
	if (method_code==0)
	{
		if (!mst_dij(adjMatrix, n_sampling, plist, 0))
		{
			fprintf(stderr,"Error in minimum spanning tree!\n");
			return false;
		}
	}
	else if (method_code==1)
		mst_prim(adjMatrix, n_sampling,plist,0);

	merge_result.clear();
	for (VISIOCYTELONG i=0;i<n_sampling;i++)
	{
		NeuronSWC tmp;
		int neuronIdx = feaMap.value(clusterCen[i]).first;
		VISIOCYTELONG nodeIdx = feaMap.value(clusterCen[i]).second;
		tmp= nt_list[neuronIdx].listNeuron[nodeIdx];
		//		tmp.x = clusterCen[i][0];
		//		tmp.y = clusterCen[i][1];
		//		tmp.z = clusterCen[i][2];
		tmp.type = 2;
		//		tmp.r = 0.5;
		tmp.n = i+1;
		tmp.pn = plist[i]+1;
		if (tmp.pn<=0) tmp.pn = -1;
		merge_result.append(tmp);
	}

	if (adjMatrix) {delete[] adjMatrix; adjMatrix=0;}
	if (plist) {delete[] plist; plist=0;}
	return true;

}
bool export_listNeuron_2swc(QList<NeuronSWC> & list, const char* filename)
{
	FILE * fp;
	fp = fopen(filename,"w");
	if (fp==NULL)
	{
		fprintf(stderr,"ERROR: %s: failed to open file to write!\n",filename);
		return false;
	}
	fprintf(fp,"##n,type,x,y,z,radius,parent\n");
	for (int i=0;i<list.size();i++)
	{
		NeuronSWC curr = list.at(i);
		fprintf(fp,"%d %d %.2f %.2f %.2f %.3f %d\n",curr.n,curr.type,curr.x,curr.y,curr.z,curr.r,curr.pn);
	}
	fclose(fp);
	return true;
}

