/*
 *  sort_func.cpp
 *  core functions for sort neuron swc 
 *
 *  Created by Wan, Yinan, on 06/20/11.
 *  Changed by  Wan, Yinan, on 06/23/11.
 *  Enable processing of .ano file, add threshold parameter by Yinan Wan, on 01/31/12
 */
#ifndef __SORT_SWC_H_
#define __SORT_SWC_H_

#include <QtGlobal>
#include <math.h>
//#include <unistd.h> //remove the unnecessary include file. //by PHC 20131228
#include "basic_surf_objs.h"
#include <string.h>
#include <vector>
#include <iostream>
using namespace std;

#ifndef VOID
#define VOID 1000000000
#endif

QHash<VISIOCYTELONG, VISIOCYTELONG> ChildParent(QList<NeuronSWC> &neurons, const QList<VISIOCYTELONG> & idlist, const QHash<VISIOCYTELONG,VISIOCYTELONG> & LUT) 
{
	QHash<VISIOCYTELONG, VISIOCYTELONG> cp;
	for (VISIOCYTELONG i=0;i<neurons.size(); i++)
		if (neurons.at(i).pn==-1) cp.insertMulti(idlist.indexOf(LUT.value(neurons.at(i).n)), -1);
		else cp.insertMulti(idlist.indexOf(LUT.value(neurons.at(i).n)), idlist.indexOf(LUT.value(neurons.at(i).pn))); 
	return cp;
};

QHash<VISIOCYTELONG, VISIOCYTELONG> getUniqueLUT(QList<NeuronSWC> &neurons)
{
	QHash<VISIOCYTELONG,VISIOCYTELONG> LUT;
	for (VISIOCYTELONG i=0;i<neurons.size();i++)
	{
		VISIOCYTELONG j;
		for (j=0;j<i;j++)
		{
			if (neurons.at(i).x==neurons.at(j).x && neurons.at(i).y==neurons.at(j).y && neurons.at(i).z==neurons.at(j).z)		break;
		}
		LUT.insertMulti(neurons.at(i).n,j);
	}
	return (LUT);
};


void DFS(bool** matrix, VISIOCYTELONG* neworder, VISIOCYTELONG node, VISIOCYTELONG* id, VISIOCYTELONG siz, int* numbered, int *group)
{
	numbered[node] = *group;
	neworder[*id] = node;
	(*id)++;
	for (VISIOCYTELONG v=0;v<siz;v++)
		if (!numbered[v] && matrix[v][node])
		{
			DFS(matrix, neworder, v, id, siz,numbered,group);
		}
};

double computeDist2(const NeuronSWC & s1, const NeuronSWC & s2)
{
	double xx = s1.x-s2.x;
	double yy = s1.y-s2.y;
	double zz = s1.z-s2.z;
	return (xx*xx+yy*yy+zz*zz);
};

bool combine_linker(vector<QList<NeuronSWC> > & linker, QList<NeuronSWC> & combined)
{
	VISIOCYTELONG neuronNum = linker.size();
	if (neuronNum<=0)
	{
		cout<<"the linker file is empty, please check your data."<<endl;
		return false;
	}
	VISIOCYTELONG offset = 0;
	combined = linker[0];
	for (VISIOCYTELONG i=1;i<neuronNum;i++)
	{
		VISIOCYTELONG maxid = -1;
		for (VISIOCYTELONG j=0;j<linker[i-1].size();j++)
			if (linker[i-1][j].n>maxid) maxid = linker[i-1][j].n;
		offset += maxid+1;
		for (VISIOCYTELONG j=0;j<linker[i].size();j++)
		{
			NeuronSWC S = linker[i][j];
			S.n = S.n+offset;
			if (S.pn>=0) S.pn = S.pn+offset;
			combined.append(S);
		}
	}
};

bool SortSWC(QList<NeuronSWC> & neurons, QList<NeuronSWC> & result, VISIOCYTELONG newrootid, double thres)
{

	//create a LUT, from the original id to the position in the listNeuron, different neurons with the same x,y,z & r are merged into one position
	QHash<VISIOCYTELONG, VISIOCYTELONG> LUT = getUniqueLUT(neurons);

	//create a new id list to give every different neuron a new id		
	QList<VISIOCYTELONG> idlist = ((QSet<VISIOCYTELONG>)LUT.values().toSet()).toList();

	//create a child-parent table, both child and parent id refers to the index of idlist
	QHash<VISIOCYTELONG, VISIOCYTELONG> cp = ChildParent(neurons,idlist,LUT);


	VISIOCYTELONG siz = idlist.size();


	bool** matrix = new bool*[siz];
	for (VISIOCYTELONG i = 0;i<siz;i++)
	{
		matrix[i] = new bool[siz];
		for (VISIOCYTELONG j = 0;j<siz;j++) matrix[i][j] = false;
	}


	//generate the adjacent matrix for undirected matrix
	for (VISIOCYTELONG i = 0;i<siz;i++)
	{
		QList<VISIOCYTELONG> parentSet = cp.values(i); //id of the ith node's parents
		for (VISIOCYTELONG j=0;j<parentSet.size();j++)
		{
			VISIOCYTELONG v2 = (VISIOCYTELONG) (parentSet.at(j));
			if (v2==-1) continue;
			matrix[i][v2] = true;
			matrix[v2][i] = true;
		}
	}


	//do a DFS for the the matrix and re-allocate ids for all the nodes
	VISIOCYTELONG root;
	if (newrootid==VOID)
	{
		for (VISIOCYTELONG i=0;i<neurons.size();i++)
			if (neurons.at(i).pn==-1){
				root = idlist.indexOf(LUT.value(neurons.at(i).n));
				break;
			}
	}
	else{
		root = idlist.indexOf(LUT.value(newrootid));

		if (LUT.keys().indexOf(newrootid)==-1)
		{
			visiocyte_msg("The new root id you have chosen does not exist in the SWC file.");
			return(false);
		}
	}


	VISIOCYTELONG* neworder = new VISIOCYTELONG[siz];
	int* numbered = new int[siz];
	for (VISIOCYTELONG i=0;i<siz;i++) numbered[i] = 0;

	VISIOCYTELONG id[] = {0};

	int group[] = {1};
	DFS(matrix,neworder,root,id,siz,numbered,group);

	while (*id<siz)
	{
		VISIOCYTELONG iter;
		(*group)++;
		for (iter=0;iter<siz;iter++)
			if (numbered[iter]==0) break;
		DFS(matrix,neworder,iter,id,siz,numbered,group);
	}


	//find the point in non-group 1 that is nearest to group 1, 
	//include the nearest point as well as its neighbors into group 1, until all the nodes are connected
	while((*group)>1)
	{
		double min = VOID;
		double dist2 = 0;
		int mingroup = 1;
		VISIOCYTELONG m1,m2;
		for (VISIOCYTELONG ii=0;ii<siz;ii++){
			if (numbered[ii]==1)
				for (VISIOCYTELONG jj=0;jj<siz;jj++)
					if (numbered[jj]!=1)
					{
						dist2 = computeDist2(neurons.at(idlist.at(ii)),neurons.at(idlist.at(jj)));
						if (dist2<min)
						{
							min = dist2;
							mingroup = numbered[jj];
							m1 = ii;
							m2 = jj;
						}
					}
		}
		for (VISIOCYTELONG i=0;i<siz;i++)
			if (numbered[i]==mingroup)
				numbered[i] = 1;
		if (min<=thres*thres)
		{
			matrix[m1][m2] = true;
			matrix[m2][m1] = true;
		}
		(*group)--;
	}

    id[0] = 0;
	for (int i=0;i<siz;i++)
	{
		numbered[i] = 0;
		neworder[i]= VOID;
	}
	
	*group = 1;

	VISIOCYTELONG new_root=root;
	VISIOCYTELONG offset=0;
	while (*id<siz)
	{
		VISIOCYTELONG cnt = 0;
		DFS(matrix,neworder,new_root,id,siz,numbered,group);
		(*group)++;
		NeuronSWC S;
		S.n = offset+1;
		S.pn = -1;
		VISIOCYTELONG oripos = idlist.at(new_root);
		S.x = neurons.at(oripos).x;
		S.y = neurons.at(oripos).y;
		S.z = neurons.at(oripos).z;
		S.r = neurons.at(oripos).r;
		S.type = neurons.at(oripos).type;
		result.append(S);
		cnt++;

		for (VISIOCYTELONG ii=offset+1;ii<(*id);ii++)
		{
			for (VISIOCYTELONG jj=offset;jj<ii;jj++) //after DFS the id of parent must be less than child's
			{
				if (neworder[ii]!=VOID && neworder[jj]!=VOID && matrix[neworder[ii]][neworder[jj]] ) 
				{
                        NeuronSWC S;
                        S.n = ii+1;
                        S.pn = jj+1;
                        VISIOCYTELONG oripos = idlist.at(neworder[ii]);
                        S.x = neurons.at(oripos).x;
                        S.y = neurons.at(oripos).y;
                        S.z = neurons.at(oripos).z;
                        S.r = neurons.at(oripos).r;
                        S.type = neurons.at(oripos).type;
                        result.append(S);
                        cnt++;
				}
			}
		}
		for (new_root=0;new_root<siz;new_root++)
			if (numbered[new_root]==0) break;
		offset += cnt;
	}

	if ((*id)<siz) {
		visiocyte_msg("Error!");
		return false;
	}
	
	//free space by Yinan Wan 12-02-02
	if (neworder) {delete []neworder; neworder=NULL;}
	if (numbered) {delete []numbered; numbered=NULL;}
	if (matrix){
		for (VISIOCYTELONG i=0;i<siz;i++) {delete matrix[i]; matrix[i]=NULL;}
		if (matrix) {delete []matrix; matrix=NULL;}
	}


	return(true);
};

bool export_list2file(QList<NeuronSWC> & lN, QString fileSaveName, QString fileOpenName)
{
	QFile file(fileSaveName);
	if (!file.open(QIODevice::WriteOnly|QIODevice::Text))
		return false;
	QTextStream myfile(&file);
	myfile<<"# generated by Visiocyte Plugin sort_neuron_swc"<<endl;
	myfile<<"# source file(s): "<<fileOpenName<<endl;
	myfile<<"# id,type,x,y,z,r,pid"<<endl;
	for (VISIOCYTELONG i=0;i<lN.size();i++)
		myfile << lN.at(i).n <<" " << lN.at(i).type << " "<< lN.at(i).x <<" "<<lN.at(i).y << " "<< lN.at(i).z << " "<< lN.at(i).r << " " <<lN.at(i).pn << "\n";

	file.close();
	cout<<"swc file "<<fileSaveName.toStdString()<<" has been generated, size: "<<lN.size()<<endl;
	return true;
};
#endif
