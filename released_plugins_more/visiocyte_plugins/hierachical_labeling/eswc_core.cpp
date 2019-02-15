/* eswc_core.cpp
   core functions in eswc_convert plugin
   12-02-16 by Yinan Wan
 */
#include "eswc_core.h"
bool swc2eswc(NeuronTree input, vector<VISIOCYTELONG> & segment_id, vector<VISIOCYTELONG> & segment_layer)
{
	VISIOCYTELONG pntNum = input.listNeuron.size();
	if (pntNum<=0)
	{
		fprintf(stderr,"Illegal input neuronTree.\n");
		return false;
	}
	segment_id = vector<VISIOCYTELONG>(pntNum, -1);
	segment_layer = vector<VISIOCYTELONG>(pntNum, -1);
	vector<VISIOCYTELONG> nchild(pntNum,0);
	
	for (VISIOCYTELONG i=0;i<pntNum;i++)
	{
		NeuronSWC S = input.listNeuron[i];
		if (S.pn<0) continue;
		VISIOCYTELONG pid = input.hashNeuron.value(S.pn);
		nchild[pid] ++;
	}

	VISIOCYTELONG segNum = 0;
	VISIOCYTELONG layerNum = 0;
	vector<VISIOCYTELONG> to_prune;
	do
	{
		layerNum++;
		to_prune.clear();
		for (VISIOCYTELONG i=0;i<pntNum;i++)
		{
			if (nchild[i]==0 && segment_id[i]==-1) //current point is an unvisited tip
			{
				segNum++;
				VISIOCYTELONG cur = i;
				VISIOCYTELONG pid;
				do 
				{
					segment_id[cur] = segNum; 
					segment_layer[cur] = layerNum;
					pid = input.listNeuron[cur].pn;
					if (pid<0) break;
					pid = input.hashNeuron.value(pid);
					cur = pid;
				} while (nchild[pid]==1);
				if (pid>=0) to_prune.push_back(pid);
			}
		}
		for (VISIOCYTELONG i=0;i<to_prune.size();i++)
			nchild[to_prune[i]]--;
	} while (!to_prune.empty());
	return true;
}

int check_eswc(NeuronTree input, vector<VISIOCYTELONG> & segment_id, vector<VISIOCYTELONG> & segment_layer)
//return value:
//            1: check passed
//           -1: check not passed (not correct number/format)
//            0: contain default value
{
	vector<VISIOCYTELONG> std_seg_id, std_seg_layer;
	if (!swc2eswc(input, std_seg_id, std_seg_layer))
	{
		fprintf(stderr, "Cannot convert to eswc.\n");
		return -1;
	}
	VISIOCYTELONG pntNum = input.listNeuron.size();
	if (segment_id.size()!= pntNum || segment_layer.size()!=pntNum)
		return -1;
	//segment layer should ba exactly the same
	for (VISIOCYTELONG i=0;i<pntNum;i++)
	{
		if (segment_layer[i]<0) return 0;
		if (segment_layer[i]!=std_seg_layer[i])
			return -1;
	}

	//segment id should be 1-1 mapped
	//check from curr_seg_id -> std_seg_id
	map<VISIOCYTELONG,VISIOCYTELONG> id_map;
	id_map.clear();
	for (VISIOCYTELONG i=0;i<pntNum;i++)
	{
		if (segment_id[i]<0) return 0;
		if (!id_map.insert(pair<VISIOCYTELONG,VISIOCYTELONG>(segment_id[i], std_seg_id[i])).second);
			if (id_map[segment_id[i]]!=std_seg_id[i]) return -1;
	}
	id_map.clear();
	//check from std_seg_id -> curr_seg_id
	for (VISIOCYTELONG i=0;i<pntNum;i++)
	{
		if (!id_map.insert(pair<VISIOCYTELONG,VISIOCYTELONG>(segment_id[i], std_seg_id[i])).second)
			if (id_map[std_seg_id[i]]!=segment_id[i]) return -1;
	}

	return 1;
}
