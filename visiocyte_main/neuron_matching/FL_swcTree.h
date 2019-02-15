/*
 * Copyright (c)2006-2010  Hanchuan Peng (Janelia Farm, Howard Hughes Medical Institute).  
 * All rights reserved.
 */


/************
                                            ********* LICENSE NOTICE ************

This folder contains all source codes for the VISIOCYTE project, which is subject to the following conditions if you want to use it. 

You will ***have to agree*** the following terms, *before* downloading/using/running/editing/changing any portion of codes in this package.

1. This package is free for non-profit research, but needs a special license for any commercial purpose. Please contact Hanchuan Peng for details.

2. You agree to appropriately cite this work in your related studies and publications.

Peng, H., Ruan, Z., Long, F., Simpson, J.H., and Myers, E.W. (2010) “VISIOCYTE enables real-time 3D visualization and quantitative analysis of large-scale biological image data sets,” Nature Biotechnology, Vol. 28, No. 4, pp. 348-353, DOI: 10.1038/nbt.1612. ( http://penglab.janelia.org/papersall/docpdf/2010_NBT_VISIOCYTE.pdf )

Peng, H, Ruan, Z., Atasoy, D., and Sternson, S. (2010) “Automatic reconstruction of 3D neuron structures using a graph-augmented deformable model,” Bioinformatics, Vol. 26, pp. i38-i46, 2010. ( http://penglab.janelia.org/papersall/docpdf/2010_Bioinfo_GD_ISMB2010.pdf )

3. This software is provided by the copyright holders (Hanchuan Peng), Howard Hughes Medical Institute, Janelia Farm Research Campus, and contributors "as is" and any express or implied warranties, including, but not limited to, any implied warranties of merchantability, non-infringement, or fitness for a particular purpose are disclaimed. In no event shall the copyright owner, Howard Hughes Medical Institute, Janelia Farm Research Campus, or contributors be liable for any direct, indirect, incidental, special, exemplary, or consequential damages (including, but not limited to, procurement of substitute goods or services; loss of use, data, or profits; reasonable royalties; or business interruption) however caused and on any theory of liability, whether in contract, strict liability, or tort (including negligence or otherwise) arising in any way out of the use of this software, even if advised of the possibility of such damage.

4. Neither the name of the Howard Hughes Medical Institute, Janelia Farm Research Campus, nor Hanchuan Peng, may be used to endorse or promote products derived from this software without specific prior written permission.

*************/




// define swc tree class
// F. Long
// 20090108

#ifndef __SWC_TREE__
#define __SWC_TREE__

#define INVALID_VALUE -9999
#define MAX_CHILDREN_NUM 6 

class swcNode
{
public:
	VISIOCYTELONG nid; //node id
	unsigned char ntype; //node type, soma: 1; axon: 2; basal dendrite: 3; apical dendrite: 4
	float x,y,z; //x,y,z coordinate
	float radius; //radius 
	VISIOCYTELONG parentid;	// parent node id, root:-1
};

class swcTree
{
public:
	swcNode *node;
	VISIOCYTELONG treeNodeNum;
	
public:
	swcTree()
	{
		treeNodeNum = 0;
		node = 0;
	}
	
	swcTree(VISIOCYTELONG nodeNum)
	{
		treeNodeNum = nodeNum;
		node = new swcNode [nodeNum];
		
		for (int i=0; i<treeNodeNum; i++)
		{ 
			node[i].nid = 0; 
			node[i].ntype = 0;
			node[i].x = 0;
			node[i].y = 0;
			node[i].z = 0;
			node[i].radius = 0;
			node[i].parentid = 0;
		}
	}
	
	~swcTree()
	{
		if (node) {delete []node; node = 0;}
		treeNodeNum = 0;
	}

	bool readSwcFile(char *infilename, swcTree *&Tree, bool tagprint);
	bool writeSwcFile(const char *outfilename);	
	void printSwcTree();
	void genGraphvizFile(char *outfilename); //output the tree as the dot file to visualize in Graphviz

	void decompose(VISIOCYTELONG *seedNodeID, VISIOCYTELONG seedNodeNum, swcTree **&subTrees, VISIOCYTELONG &subTreeNum, VISIOCYTELONG *&branchIdx); // decompose trees into multiple sub-trees based on give seedNodeId
	void decompose(VISIOCYTELONG *seedNodeID, VISIOCYTELONG seedNodeNum, swcTree **&subTrees, VISIOCYTELONG &subTreeNum, VISIOCYTELONG *&branchIdx, VISIOCYTELONG *&subTreeSeeds); //20090911, add subTreeSeeds for matlab display code
	 
	void copyTree(swcTree *&newTree); //copy tree
	
	void getRootID(VISIOCYTELONG &rootid, VISIOCYTELONG &rootidx); // get the id and index of root of the tree
	void getIndex(VISIOCYTELONG nodeid, VISIOCYTELONG &nodeidx); // get the index of the nodeid
	void findPath(VISIOCYTELONG nodeid1, VISIOCYTELONG nodeid2, VISIOCYTELONG *&pathNodeList, VISIOCYTELONG &pathNodeNum, unsigned char &ancestorTag); // find a path between nodeid1, nodeid2
	
	void computeDepth(VISIOCYTELONG *&depth); // compute depth for each node in the tree
	void computeDepth(VISIOCYTELONG &depth, VISIOCYTELONG nodeid); //compute depth for a particular node with id value equals nodeid
	void computeDistance(VISIOCYTELONG nodeid1, VISIOCYTELONG nodeid2, float &dist); //compute physical distance between two nodes of the tree in the current coordinate system
	void computeLength(VISIOCYTELONG nodeid, unsigned char parentTag, float *&length, unsigned char &lengthNum); //compute the length between two nodes that are directly parenet and child
	void computeLength(VISIOCYTELONG nodeid1, VISIOCYTELONG nodeid2, float &length); //compute the length between any two nodes
	void computeTreeDiameter(float &treeDiameter, VISIOCYTELONG &leafNodeID1, VISIOCYTELONG &leafNodeID2); // compute the diameter of the tree
	void computeTreeCenter(float *&treeCenter, float &averageRadius); // compute the center of the tree
	
	void computeLengthWeightedDistance(VISIOCYTELONG nodeid, VISIOCYTELONG *NodeList, VISIOCYTELONG NodeListNum, float *&dist); // compute the sum of the length weighted distance from nodeid to all the nodes in Nodelist
	void computeLengthWeightedDistanceNormalized(VISIOCYTELONG nodeid, float &dist, unsigned char method); // compute the normalized length weighted distance of a node in the tree
	
	void computeBranchLengthRatio(VISIOCYTELONG nodeid, float *&lengthratio, unsigned char &lengthratioNum, unsigned char method); // compute the branch length ratio of nodeid
	void computeOrientation(VISIOCYTELONG parentNodeid, VISIOCYTELONG childNodeid, float &angle1, float &angle2); // compute the orientation of the brunch between parentNodeid and childNodeid in the current coordinate system, they need to be direct parent and child 
//	void computeBranchingAngles(VISIOCYTELONG nodeid, float *&childrenAngles, unsigned char childrenAnglesNum, float *&parentChildAngles, unsigned char parentChildAnglesNum); // compute branching angles of the node
	void computeAngle(VISIOCYTELONG nodeid, VISIOCYTELONG nodeid1, VISIOCYTELONG nodeid2, float &angle); // compute the angle between the line connecting nodeid, nodeid1, and nodeid, nodeid2
	void computeAngle(float *vec1, float *vec2, float &angle); // compute the angle between two vectors	
	void computeBranchingAngles(VISIOCYTELONG nodeid, VISIOCYTELONG *&branchRootID, VISIOCYTELONG *&branchRootIdx, VISIOCYTELONG &branchNum, float *&angles); // compute the angles between pair-wise vectors formed by direct children and parent
	void computeBranchingAngles(VISIOCYTELONG nodeid, VISIOCYTELONG &branchNum, float *&angles); // compute the angles between pair-wise vectors formed by the average vector of children path and the average vector of parent path
	void computeLeafNodeAngles(VISIOCYTELONG nodeid, float *vec, VISIOCYTELONG *leafNodeListID, VISIOCYTELONG leafNodeNum, float *&angles); // compute the angles between the vectors formed by the leaf ndoes, current tree root, and anterior tree root, this function is called by leaf node matching
	
//	void computeBranchSizeAngles(VISIOCYTELONG nodeid, VISIOCYTELONG *&branchRootID, VISIOCYTELONG *&branchRootIdx, VISIOCYTELONG &branchNum, VISIOCYTELONG *&branchNodeNum, float *&angles); // compute the angles and branch size by combining functions computeBranchingAngles and getBranchSize to remove some redunt compuation if both size and angles need to compute
	void computeBranchNodeNumAngles(VISIOCYTELONG nodeid, VISIOCYTELONG *&branchRootID, VISIOCYTELONG *&branchRootIdx, VISIOCYTELONG &branchNum, VISIOCYTELONG *&branchNodeNum, float *&angles); // compute the angles and branch size by combining functions computeBranchingAngles and getBranchSize to remove some redunt compuation if both size and angles need to compute
	void computeBranchLengthSumAngles(VISIOCYTELONG nodeid, VISIOCYTELONG *&branchRootID, VISIOCYTELONG *&branchRootIdx, VISIOCYTELONG &branchNum, float *&lengthSum, float *&angles); // compute the angles, and sum of sub-branch lengths
		
	void sortTree(VISIOCYTELONG *&nodeidx); //sort the tree in depth
	
//	void sigBranchingNodeThre(swcTree *Tree, float ratio1, float ratio2, float &lengthThre1, float &lengthThre2); //adpatively determine thresholds for keeping significant branching nodes
	
	void removeContinualNodes(swcTree *&newTree, unsigned char *&removeTag);//remove all continual nodes
	void removeContinuaLeaflNodes(swcTree *&newTree, unsigned char *&removeTag); //remove all continual nodes and leaf nodes
	void removeInsignificantNodes(float lengthThre1, float lengthThre2, swcTree *&newTree, bool keepLeaf, bool *&removeNodeTag, VISIOCYTELONG **&removeBranchTag); //remove all continual nodes,  and subtrees of insignificant branching nodes, leaf nodes of significant branching points are kept
	void removeInsignificantNodes(float *lengthRatioThre, swcTree *&newTree, bool keepLeaf, bool *&removeNodeTag, VISIOCYTELONG **&removeBranchTag); //remove all continual nodes,  and subtrees of insignificant branching nodes, leaf nodes of significant branching points are kept
	void detectCriticalBranchNodes(float *threVector, unsigned char leafMethod, unsigned char criticalNodeMethod, bool *&removeNodeTag, VISIOCYTELONG **&removeBranchTag, swcTree *&newTree, swcTree *&newTreeWithPath); //detect critical branch nodes and output new trees defined on critical branch nodes (and leaf nodes)
//	void detectCriticalBranchNodes(float *threVector, unsigned char leafMethod, unsigned char criticalNodeMethod, swcTree *Tree, bool *&removeNodeTag, VISIOCYTELONG **&removeBranchTag, swcTree *&newTree, swcTree *&newTreeWithPath); //detect critical branch nodes and output new trees defined on critical branch nodes (and leaf nodes)
	
	void getSubTree(VISIOCYTELONG nodeid, swcTree *&subtree, VISIOCYTELONG *&subtreeNodeIdx); // get the sub tree rooted at a particular node	
	void getSubTree(VISIOCYTELONG nodeid, VISIOCYTELONG depthThre, swcTree *&subtree, VISIOCYTELONG *&subtreeNodeIdx); // get the parital sub tree rooted at a particular node, nodes within the subtree have a depth no bigger then depthThre with respect to the root nodeid
	void getSubTree(VISIOCYTELONG nodeid, float subtreeRatioThre, swcTree *&subtree, VISIOCYTELONG *&subtreeNodeIdx); // get the parital sub tree rooted at a particular node, nodes within the subtree have a length ratio less that lengthRatioThre with respect to the root nodeid

	void getSubTreeNodeNum(VISIOCYTELONG *&subTreeNodeNum); // get the number of nodes of the sub tree rooted at each node
	void getSubTreeNodeNum(VISIOCYTELONG nodeid, VISIOCYTELONG &subTreeNodeNum); // get the number of nodes of the sub tree rooted at a particular node
	void sortSubTreeNodeNum(VISIOCYTELONG *subTreeNodeNum, float *&sortval, float *&sortidx); //sort nodes in a tree according to their sub tree node number 
	void getSubTreeLength(VISIOCYTELONG nodeid, float &maxlength, VISIOCYTELONG &maxLengthNodeid, float &minlength, VISIOCYTELONG &minLengthNodeid, float &totalLength); // get the maximum, minimum length between nodeid and a node, and the total length of the subtree rooted at nodeid 
	void getSubTreeLength(VISIOCYTELONG nodeid, float &maxlength, VISIOCYTELONG &maxLengthNodeid, float &secondmaxlength, VISIOCYTELONG &secondmaxLengthNodeid, float &minlength, VISIOCYTELONG &minLengthNodeid, float &totalLength); // get the maximum, second maximum, minimum length between nodeid and a node, and the total length of the subtree rooted at nodeid

//	void getSubTreeMaxLength(VISIOCYTELONG nodeid, float &maxlength, VISIOCYTELONG &maxLengthNodeid,  float *&totalLength); // get the maximum length between nodeid and a node in its subtree	
//	void getSubTreeTotalLength(VISIOCYTELONG nodeid, float &subTreeTotalLength); // get the total length of the subtree rooted at nodeid
//	void getChildrenBranchNodeNum(VISIOCYTELONG nodeid, VISIOCYTELONG *&directChildrenID, VISIOCYTELONG *&directChildrenIdx, VISIOCYTELONG &directChildrenNum, VISIOCYTELONG *&childrenBranchNodeNum); // get the number of nodes of each children branch  
//	void getBranchSize(VISIOCYTELONG nodeid, VISIOCYTELONG *&branchRootID, VISIOCYTELONG *&branchRootIdx, VISIOCYTELONG &branchNum, VISIOCYTELONG *&branchNodeNum); // get the number of nodes of each children branch and the remaining branch not belong to children 
	void getBranchNodeNum(VISIOCYTELONG nodeid, VISIOCYTELONG *&branchRootID, VISIOCYTELONG *&branchRootIdx, VISIOCYTELONG &branchNum, VISIOCYTELONG *&branchNodeNum); // get the number of nodes of branch split from nodeid, i.e., each children branch and the remaining branch not belong to children 

	void getDirectChildren(VISIOCYTELONG **&childrenList, VISIOCYTELONG *&childrenNum, unsigned char maxChildrenNum ); //get the direct children for each node in the tree
	void getDirectChildren(VISIOCYTELONG nodeid, VISIOCYTELONG *&childrenID, VISIOCYTELONG *&childrenNodeIdx, VISIOCYTELONG &childrenNum); // get the direct children of nodeid
	void getAllChildren(VISIOCYTELONG nodeid, VISIOCYTELONG *&childrenID, VISIOCYTELONG *&childrenNodeIdx, VISIOCYTELONG &childrenNum); // get all the children of nodeid
	void getAllAncestors(VISIOCYTELONG nodeid, VISIOCYTELONG *&ancestorID, VISIOCYTELONG *&ancestorIdx, VISIOCYTELONG &ancestorNum); // get all ancestors of nodeid
	void getAncestors(VISIOCYTELONG nodeid, VISIOCYTELONG *&ancestorsID, VISIOCYTELONG *&ancestorsIdx, VISIOCYTELONG &ancestorsNum); // get all the ancestors of nodeid, should combine with the above function, the same
	
	void getNodeStructuralType(unsigned char *&stype, unsigned char &maxfurcation); //get the structural type of each node in the tree, -1: root; 0: leaf; 1: continuation; 2: bifurcation; 3: trifurcation ...
	void getLeafNode(VISIOCYTELONG *&leafNodeList, VISIOCYTELONG *&leafNodeIdx, VISIOCYTELONG &leafNodeNum); // get leaf nodes from the tree
	void getLeafNode(float lengthThre, VISIOCYTELONG *&leafNodeList, VISIOCYTELONG *&leafNodeIdx, VISIOCYTELONG &leafNodeNum); // get significant leaf nodes, ignoring those on small branches determiend by lengthThre
	void getBifurcation(VISIOCYTELONG *&bifurNodeList, VISIOCYTELONG *&bifurfNodeIdx, VISIOCYTELONG &bifurNodeNum); //get birfurcation node from the tree
	void getBranchingNode(VISIOCYTELONG *&branchingNodeList, VISIOCYTELONG *&branchingNodeIdx, VISIOCYTELONG &branchingNodeNum); //get branching nodes (can be bifurcation and multi-furcation) from the tree
	void getTypedNodes(VISIOCYTELONG *&typedNodeList, VISIOCYTELONG *&typedNodeIdx, VISIOCYTELONG &typedNodeNum, unsigned char nodeType); //combine the function of getLeafNode, getBifurcation, getBranchingNode into one function
	void getSignificantBranchingNode(VISIOCYTELONG branchSizeThre, VISIOCYTELONG *&branchingNodeList, VISIOCYTELONG *&branchingNodeIdx, VISIOCYTELONG &branchingNodeNum); //get significant branching points, each branch needs to be bigger than branchSizeThre	
	
//	void deleteNode(VISIOCYTELONG nodeid, swcTree *&newTree); // delete a node in the tree to geneate a new tree, note that when nodeid is a branching point, what delected is infact a randomly selected branch
//	void deleteBranch(VISIOCYTELONG nodeid, swcTree *&newTree); //delete a branch of nodes rooted at nodeid	
	void randomDeleteNodes(VISIOCYTELONG branchSizeThre, float rate, swcTree *&newTree, VISIOCYTELONG *&deletedTag); //randomly delete some nodes in the tree according to some probability
	void randomDeleteBranch(VISIOCYTELONG branchSizeThre, float rate, swcTree *&newTree, VISIOCYTELONG *&deletedTag); //randomly delete some branches in the tree according to some probability
	void randomPerturbCoordinates(float rate, float maxdis, swcTree *&newTree, VISIOCYTELONG *&perturbedTag);// randomly select some nodes to perturb their 3d coordinates slightly 
	void deleteTreeNode(VISIOCYTELONG *nodeidlist, VISIOCYTELONG deleteNodeNum, swcTree *&newTree); //delete a list of nodes in the tree and generate a new tree
	void deleteSubTreeNode(VISIOCYTELONG *nodeIDList, VISIOCYTELONG nodeIDNum, VISIOCYTELONG &deleteNodeNum, VISIOCYTELONG *&deleteTag, swcTree *&newTree); // delete subtree rooted at each nodeid in nodeIDList, include nodeid

	void buildSuperNodeTree(VISIOCYTELONG *deleteNodeIDlist, VISIOCYTELONG *ancesterIDList, VISIOCYTELONG deleteNodeNum, swcTree *&newTree); //Build super node tree by deleting a list of nodes specified by deleteNodeIDlist, making the children of each node the children of its ancester node listed in ancesterIDList
	void buildSuperNodeTree(VISIOCYTELONG deleteNodeID, VISIOCYTELONG ancesterID, swcTree *&newTree); //Build super node tree, delete only one node 
	
	void pruneBranch(VISIOCYTELONG branchNodeNum, swcTree *&newTree, VISIOCYTELONG *&deletedTag); //prune small branches based on branch nodes
	void pruneBranch(float branchMaxLength, swcTree *&newTree, VISIOCYTELONG *&deletedTag); //prune small branches based on branch length

	void reformTree(VISIOCYTELONG rootid, swcTree *&newTree); // reform tree structure using a node (different from that of the original) as the root)
	void tree2UndirectedGraph(unsigned char *&graph);  //turn tree into an undirected graph

//	not implemented yet
	void affineTransform(float *T, swcTree *&newTree); //apply global affine transformation to each node in the tree 
	void getBranchTotalLength(VISIOCYTELONG nodeid, VISIOCYTELONG *&branchRootID, VISIOCYTELONG *&branchRootIdx, VISIOCYTELONG &branchNum, float *&totalLength); // get the total length of each branch split from nodeid;

};
	
#endif





