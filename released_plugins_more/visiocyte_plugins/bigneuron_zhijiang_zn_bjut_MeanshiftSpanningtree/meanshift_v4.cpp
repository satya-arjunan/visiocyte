/* meanshift_func.cpp
* This file contains the functions used in plugin domenu and dufunc, you can use it as a demo.
* 2015-03-18 : by Zhijiang Wan
*/
#include <visiocyte_interface.h>
#include "visiocyte_message.h"
#include "stackutil.h"
#include "meanshift_func.h"
#include "D:\Visiocyte\visiocyte_main\neurontracing_vn21\app2/heap.h"
#include "D:\Visiocyte\visiocyte_main\neuron_editing\v_neuronswc.h"
#include "D:\Visiocyte/sort_neuron_swc/sort_swc.h"
#include <vector>
#include <iostream>

using namespace std;

/* function used in DOMENU typically takes  inputs:
* "callback" - provide information from the plugin interface, and 
* "parent"   - the parent widget of the Visiocyte main window
*/
QMap<int,QList<Node*>> Map_finalnode_list;//��ֵ�����ӵ�����QList�Ƕ�Ӧ���ӵ����Ľڵ���
QMap<int,Node*> Map_rootnode;//int�ǽڵ�ı�ţ�Ҳ�����ı��
QMultiMap<int,Node*> Map_allnodes;//��ͼ���е�ÿһ�����ص㶼����mark

void printSwcByMap(QMap<int,Node*> nodeMap,char* path)
{
	VISIOCYTELONG number=0;
	FILE * fp = fopen(path, "wt");
	if (!fp) return;

	fprintf(fp, "#name\n");
	fprintf(fp, "#comment\n");
	fprintf(fp, "##n,type,x,y,z,radius,parent\n");


	for(QMap<int,Node*>::iterator iter = nodeMap.begin(); iter != nodeMap.end(); iter++)
	{
		Node* temp_node=iter.value();;
		fprintf(fp, "%ld %d %ld %ld %ld %5.3f %ld\n",
			number, 1,  temp_node->x,  temp_node->y,  temp_node->z, temp_node->r, -1);
		number++;
	}


	fclose(fp);

	return;
}


void enlarge_radiusof_single_node_xy(unsigned char * &img1d,Node * &node,VISIOCYTELONG sz_x, VISIOCYTELONG sz_y, VISIOCYTELONG sz_z)
{
	//����xyƽ��4����İ뾶����

	int allnodes=0;
	int back_nodes=0;
	int max=0;//�����뾶
	double threshold=30;
	QQueue<Node*> *queue=new QQueue<Node*>();
	queue->clear();
	queue->enqueue(node);
	while(!(queue->isEmpty()))//���ڸ��ڵ���1Ϊ������һ��һ�����������ѷ�Χ�ڵĵ㶼�ӵ���������
	{

		Node* head=queue->head();
		Node* up=new Node(head->x,head->y-1,head->z);
		Node* down=new Node(head->x,head->y+1,head->z);
		Node* left=new Node(head->x-1,head->y,head->z);
		Node* right=new Node(head->x+1,head->y,head->z);

		//if(!queue->contains(up))//��������û�а����ýڵ�
		if(!contain(queue,up->x,up->y,up->z))
		{

			allnodes=allnodes+1;
			//printf("%lf\n",(double)img1d[GET_IND(up->x,up->y,up->z)]);
			//printf("%ld   %ld   %ld\n",up->x,up->y,up->z);
			if(img1d[GET_IND(up->x,up->y,up->z)]<=30)
			{
				back_nodes=back_nodes+1;
				//printf("back:::%d\n",back_nodes);
			}

			queue->enqueue(up);

		}

		//if(!queue->contains(down))
		if(!contain(queue,down->x,down->y,down->z))
		{
			//printf("no down\n");
			allnodes=allnodes+1;
			//printf("%lf\n",(double)img1d[GET_IND(down->x,down->y,down->z)]);
			//printf("%ld   %ld   %ld\n",down->x,down->y,down->z);
			if(img1d[GET_IND(down->x,down->y,down->z)]<=30)
			{
				back_nodes=back_nodes+1;
				//printf("back:::%d\n",back_nodes);
			}

			queue->enqueue(down);

		}

		//if(!queue->contains(left))
		if(!contain(queue,left->x,left->y,left->z))
		{
			//printf("no left\n");
			allnodes=allnodes+1;
			//printf("%lf\n",(double)img1d[GET_IND(left->x,left->y,left->z)]);
			//printf("%ld   %ld   %ld\n",left->x,left->y,left->z);
			if(img1d[GET_IND(left->x,left->y,left->z)]<=30)
			{
				back_nodes=back_nodes+1;
				//printf("back:::%d\n",back_nodes);
			}

			queue->enqueue(left);

		}

		//if(!queue->contains(right))
		if(!contain(queue,right->x,right->y,right->z))
		{
			//printf("no right\n");
			allnodes=allnodes+1;
			//printf("%lf\n",(double)img1d[GET_IND(right->x,right->y,right->z)]);
			//printf("%ld   %ld   %ld\n",right->x,right->y,right->z);
			if(img1d[GET_IND(right->x,right->y,right->z)]<=30)
			{
				back_nodes=back_nodes+1;
				//printf("back:::%d\n",back_nodes);
			}

			queue->enqueue(right);

		}
		//printf("size:::%d   \n",queue.size());
		//printf("%d   %d\n",allnodes,back_nodes);

		double up_r=(double)sqrt((double)(node->x-up->x)*(node->x-up->x)+(double)(node->y-up->y)*(node->y-up->y)+(double)(node->z-up->z)*(node->z-up->z));
		double down_r=(double)sqrt((double)(node->x-down->x)*(node->x-down->x)+(double)(node->y-down->y)*(node->y-down->y)+(double)(node->z-down->z)*(node->z-down->z));
		double left_r=(double)sqrt((double)(node->x-left->x)*(node->x-left->x)+(double)(node->y-left->y)*(node->y-left->y)+(double)(node->z-left->z)*(node->z-left->z));
		double right_r=(double)sqrt((double)(node->x-right->x)*(node->x-right->x)+(double)(node->y-right->y)*(node->y-right->y)+(double)(node->z-right->z)*(node->z-right->z));

		max=up_r>down_r?up_r:down_r;
		max=max>left_r?max:left_r;
		max=max>right_r?max:right_r;



		float per=(float)back_nodes/(float)allnodes;

		if(per>0.1)
		{
			//root->r=(up_r+down_r+left_r+right_r)/4;
			//node->r=max;
			node->r=(up_r+down_r+left_r+right_r)/4;
			//printf("per:::%f   r:::%lf\n",per,node->r);
			break;

		}

		//����
		queue->dequeue();

	}
	delete queue;


}


void writeSWC_file(char* path,QList<NeuronSWC> listNeuron)
{
	FILE * fp = fopen(path, "a");
	if (!fp) return;

	fprintf(fp, "#name\n");
	fprintf(fp, "#comment\n");
	fprintf(fp, "##n,type,x,y,z,radius,parent\n");
	for(int i=0;i<listNeuron.size();i++)
	{
		NeuronSWC temp=listNeuron.at(i);
		fprintf(fp, "%ld %d %f %f %f %f %ld\n",
			temp.n, temp.type,  temp.x,  temp.y,  temp.z, temp.r, temp.parent);

	}
	fclose(fp);
	return;

}
void meanshift_plugin_vn4(VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
	visiocytehandle curwin = callback.currentImageWindow();
	if(!curwin)
	{
		QMessageBox::information(0, title, QObject::tr("No image is open."));
		return -1;
	}
	Image4DSimple *p4d = callback.getImage(curwin);
	VISIOCYTELONG sz_x = p4d->getXDim();
	VISIOCYTELONG sz_y = p4d->getYDim();
	VISIOCYTELONG sz_z = p4d->getZDim();
	unsigned char* img1d = p4d->getRawDataAtChannel(1);
	flag=new int [sz_x*sz_y*sz_z];

	QList<Node*> temp1;//������ʱ�������ص��ҵ���·������ɺ󶼷ŵ�MapList���棻���ﲻ�ܶ���ָ�룬����ָ��Ļ��������Ϊָ��ָ����ǵ�ַ����ַ�ı��ˣ�����Ҳ��䣻
	VISIOCYTELONG r=10;//meanshift�õ��İ뾶
	VISIOCYTELONG count=0;
	int times=100;
	int num_mark=1;//�����ڳ�ʼ����ʱ������ӵ���з�����
	//QList<Node*> list;
	printf("### Initializing...  ###\n");//20150315��һ��ʼ�Ͷ�����ͼ����г�ʼ��������Ѱ�Ҳ�������û�б�Ҫ�ģ�����һ��Ѱ����ֹ�㣬һ�߸������Ľڵ���б�ǣ��Ժ������޸�
	

	for(VISIOCYTELONG ii=0;ii<sz_x;ii++)
	{
		for(VISIOCYTELONG jj=0;jj<sz_y;jj++)
		{
			for(VISIOCYTELONG kk=0;kk<sz_z;kk++)
			{
				VISIOCYTELONG ind=GET_IND(ii,jj,kk);//����ά����ת���ɶ�ά���꣬ͨ����ά����������ǿ��
				//flag[ind]=0;
				if(img1d[ind]==0)//����ǿ��Ϊ0������㲻�迼�ǣ�ֻ�Ծ�������ǿ�ȵĵ����meanshift
					continue;
				/*	if(img1d[ind]<30)//����ǿ�ȳ���ĳһ��ֵ������㲻�迼�ǣ�ֻ�Ծ�������ǿ�ȵĵ����meanshift
				continue;*/
				if(!found_final(img1d,ii,jj,kk,sz_x, sz_y, sz_z, r))//û�ж�
				{
					Node* final=new Node(ii,jj,kk);
					//QList<Node*> *list=new QList<Node*>();
					final->class_mark=num_mark;
					final->parent=-1;//���ڵ���Ϊ-1
					final->number=0;//ÿ�����ڵ����ű��Ϊ0
					
					Map_finalnode.insert(ind,num_mark);//�����ڵ���������,ind�Ƕ�ά����,num_mark�Ǳ��
					enlarge_radiusof_single_node_xy(img1d,final,sz_x, sz_y, sz_z);//������ڵ�İ뾶
					Map_rootnode.insert(num_mark,final);//�����ڵ��������ţ�num_mark�����final��Ӧ��Ӧ�Ľڵ�
					num_mark++;
					

				}

			}
		}
	}

	//printSwcByList( final_nodeList1,"D:\\meanshift_vn3_finalnode.swc");
	printSwcByMap(Map_rootnode,"D:\\meanshift_vn3_finalnode.swc");
	printf("###   initializing finished   ###\n");
	printf("###  start meanshift   ###\n");
	for(VISIOCYTELONG i=0;i<sz_x;i++)
	{
		for(VISIOCYTELONG j=0;j<sz_y;j++)
		{
			for(VISIOCYTELONG k=0;k<sz_z;k++)
			{
				VISIOCYTELONG ind=GET_IND(i,j,k);//����ά����ת���ɶ�ά���꣬ͨ����ά����������ǿ��
				if(img1d[ind]==0)//����ǿ��Ϊ0������㲻�迼�ǣ�ֻ�Ծ�������ǿ�ȵĵ����meanshift
					continue;
			//20150318,�������ӵ�Ȼ������������û�б�Ҫ�ģ����е���ڼ���meanshift�����飬����һ��shiftһ�����յ�
				meanshift_vn5(img1d,i,j,k,sz_x,sz_y,sz_z,r,times);//ͨ��������������еĽڵ㶼�����ű���ķŵ���Map_finalnode_list<int,QList<Node*>>��
			}

		}

	}
	//��һ���Ľ���ǽ����е㰴�ո��ڵ�������ű���ķŵ���Map_finalnode_list����
	printSwcByMap(Map_rootnode,"D:\\result\\meanshift_vn3_finalnode.swc");
	printSwcByMap(Map_allnodes,"D:\\result\\meanshift_allnode.swc");
	printSWCByMap_List(Map_finalnode_list,"D:\\result\\meanshift_allnode_List.swc");

	merge_rootnode(Map_rootnode,img1d,sz_x,sz_y,sz_z);//�����������������һ���µ�Map_rootnode,��֮ǰ�ĸ��ڵ����٣�����������ڵ�֮�䲻���غ�
	//20150312,ǰ��Ĳ�����ʵ���˶��������ص�ķ��࣬��һ���Ƕ�ÿһ�������С����������,finalclass_node���������յ�����ص�֮��Ķ�Ӧ��ϵ
	QList<QList <NeuronSWC>> MST_sorted;
	NeuronTree nt_result;
	//��һ���Ƕ��ںϺ�����ӵ���spanning tree������Ԫ�ع�
	//20150317,�����ӵ�����ع�
	//construct_tree_vn2(finalclass_node);//�����ҵ������ӵ�

	//construct_tree(finalclass_node, sz_x, sz_y, sz_z);//�õ������е���������
	//nt_result=merge_spanning_tree(result_tree_part);
	//merge_spanning_tree(result_tree_part);//����������ڽ����ɵ�����spanning tree���ӳ�һ����
	//printSWCByQList_Neuron(result_list,"D:\\result\\meanshift_result.swc");
	//writeSWC_file("D:\\result\\meanshift_result_tree_part.swc",result_final_tree);
	//printSWCByQList_QList( result_tree_part,"D:\\result\\meanshift_result_tree_part.swc");
	

	delete []flag;
}

void meanshift_vn5(unsigned char * &img1d,VISIOCYTELONG x,VISIOCYTELONG y,VISIOCYTELONG z,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z,VISIOCYTELONG r,int iteration)
{
	int iter=0;
	int same_times=0;
	int situation=0;//���ڹ۲��ҵ������յ�������������

	QList<Node*> nodeList;
	nodeList.clear();//ÿ�ζ�clear
	
	Node *cur_center=new Node(x,y,z);//��ǰԲ�����ĵ㣬��¼Բ���ĵ����ά����
	double intensity=img1d[GET_IND(x,y,z)];//ͨ��Բ���ĵ���ά������������ź�ǿ��
	//cur_center->intensity=intensity;

	while(1)
	{
		if(iter==iteration)
		{
			break;

		}else
		{
			double sum1_z=0,sum2_z=0,sum1_y=0,sum2_y=0,sum1_x=0,sum2_x=0,w=0;
			//���巶Χ���԰뾶Ϊr��������������ƶ�
			VISIOCYTELONG xe=cur_center->x-r;if(xe<0) xe=0;
			VISIOCYTELONG xb=cur_center->x+r+1;if(xb>sz_x) xb=sz_x;
			VISIOCYTELONG ye=cur_center->y-r;if(ye<0) ye=0;
			VISIOCYTELONG yb=cur_center->y+r+1;if(yb>sz_y) yb=sz_y;
			VISIOCYTELONG ze=cur_center->z-r;if(ze<0) ze=0;
			VISIOCYTELONG zb=cur_center->z+r+1;if(zb>sz_z) zb=sz_z;
			for(VISIOCYTELONG i=xe;i<xb;i++) 
			{
				for(VISIOCYTELONG j=ye;j<yb;j++)
				{
					for(VISIOCYTELONG k=ze;k<zb;k++)
					{//Բ�ĵ���Ҫ����������е�ÿ���㶼����Ȩֵ

						if(GET_IND(i,j,k)==GET_IND(cur_center->x,cur_center->y,cur_center->z)) continue;//Բ�ĵ㲻����
						if(img1d[GET_IND(i,j,k)]==0) continue;//����Ϊ0�ĵ㲻����
						double cur_intensity=img1d[GET_IND(i,j,k)];
						w=cal_weight(i,j,k,cur_center->x,cur_center->y,cur_center->z,cur_intensity,intensity,r);//����������ÿ�����Բ�ĵ�Ȩ�أ���Ҫ�ǻ�������ǿ�Ⱥ;���

						double core_z=cal_core(k,cur_center->z,r);//����˺�����z����
						sum1_z+=core_z*w*k;//����Z�����꣬�ۼӺ���ΪMeanShift��Ҫ���㹫ʽ�ķ���
						sum2_z+=core_z*w;//�ۼӺ���ΪMeanShift��Ҫ���㹫ʽ�ķ�ĸ
						//printf("%ld  %ld  %lf \n",k,cur_center->z,core_z);

						double core_y=cal_core(j,cur_center->y,r);//y��
						sum1_y+=core_y*w*j;
						sum2_y+=core_y*w;

						double core_x=cal_core(i,cur_center->x,r);//x��
						sum1_x+=core_x*w*i;
						sum2_x+=core_x*w;
						//printf("%ld   %ld  %ld  %lf  %lf  %lf  %lf  %lf  %lf  %lf  %lf  %lf\n",i,j,k,w,core_x,core_y,core_z,sum1_x,sum2_x,sum1_y,sum2_y,sum1_z,sum2_z);

					}
				}
			}
			VISIOCYTELONG temp_x=cur_center->x;//3��temp����ǰһ��Բ�ĵ�x\y\z����
			VISIOCYTELONG temp_y=cur_center->y;
			VISIOCYTELONG temp_z=cur_center->z;
			flag[GET_IND(temp_x,temp_y,temp_z)]=1;//����һ��Բ�ĵ���Ϊ1����ʾ�õ��Ѿ��������

			VISIOCYTELONG next_x=(sum1_x/sum2_x)+0.5;
			VISIOCYTELONG next_y=(sum1_y/sum2_y)+0.5;
			VISIOCYTELONG next_z=(sum1_z/sum2_z)+0.5;
			cur_center->x=next_x;//���µ���һ��Բ�ĵ㸳����Ϊ��ǰԲ��
			cur_center->y=next_y;
			cur_center->z=next_z;
			intensity=img1d[GET_IND(cur_center->x,cur_center->y,cur_center->z)];//���µ�ǰԲ�ĵ�����ǿ��


			//��ʽ2
			Node *pre_center=new Node(temp_x,temp_y,temp_z);
			if(!nodeList.contains(pre_center))//�ж���һ��Բ���Ƿ���List���棬����ڵĻ�˵����ǰ�ڵ��ǰһ�ڵ���ͬ��ǰһ�ڵ㲢δ�ƶ�
			{	//�����İ뾶
				enlarge_radiusof_single_node_xy(img1d,pre_center,sz_x,sz_y,sz_z);//����ÿһ����İ뾶�����ܻ�Ը��ڵ�����ظ�����
				//printf("%lf\n",pre_center->r);
				nodeList.append(pre_center);
			}	

			if(flag[GET_IND(cur_center->x,cur_center->y,cur_center->z)]==1)
			{//��������ĵ�ǰ���ĵ��Ѿ��������������whileѭ�������ٽ��м���
				//�������˵����ǰ�ͱ������һ�Σ��ټ���Ļ����ǻᱻ�ƶ����յ㣬����������
				break;
			}

			iter++;//���մ�ʩ������100�κ󣬲��ٵ���

		}
	}

	//bool f=found_final(img1d,cur_center->x,cur_center->y,cur_center->z,sz_x,sz_y,sz_z,r);//�����жϸõ��Ƿ����ƶ���Ҳ��ʾ������Ǹ��ڵ㻹��·���ϵĵ�
	VISIOCYTELONG ind2=GET_IND(cur_center->x,cur_center->y,cur_center->z);

	double temp_dis=1000000;

	//if((f==false))//˵��û���ƶ����Ǹ��ڵ㣬��Ҫ������·���ϵĵ㶼������ڵ�ı��

	{//�ҵ����µĸ����ܶ����ĵ�
		//situation=1;//���ڱ���ҵ������յ������������
		int mark=0;
		if(Map_finalnode.contains(ind2))//20150311,�����Map_finalnode���棬˵�����ܱ��ƶ��ˣ����յ�
		{//���Map_finalnode�������������ڵ㣬�����List�ŵ���Ӧ��Map_finalnode_List��
			for(int iii=0;iii<nodeList.size();iii++)
			{
				VISIOCYTELONG node_x=nodeList.at(iii)->x;
				VISIOCYTELONG node_y=nodeList.at(iii)->y;
				VISIOCYTELONG node_z=nodeList.at(iii)->z;
				Map_nodes[GET_IND(node_x,node_y,node_z)]=Map_finalnode[ind2];//��nodeList����Ľڵ㸳����Ӧ���ڵ�ı��
				Map_allnodes.insert(Map_finalnode[ind2],nodeList.at(iii));
				Map_finalnode_list[Map_finalnode[ind2]].append(nodeList.at(iii));
				

			}
			
		}else if(Map_nodes.contains(GET_IND(cur_center->x,cur_center->y,cur_center->z)))//�յ����ƶ������ұ������
		{
			//�����������꣬�ҵ�������mark,��nodeList��ֵ
			situation=2;

			int mark3=Map_nodes[GET_IND(cur_center->x,cur_center->y,cur_center->z)];

			for(int iii=0;iii<nodeList.size();iii++)
			{
				VISIOCYTELONG node_x2=nodeList.at(iii)->x;
				VISIOCYTELONG node_y2=nodeList.at(iii)->y;
				VISIOCYTELONG node_z2=nodeList.at(iii)->z;
				Map_nodes[GET_IND(node_x2,node_y2,node_z2)]=mark3;//��nodeList����Ľڵ㸳����Ӧ���ڵ�ı��
				Map_allnodes.insert(mark3,nodeList.at(iii));
				Map_finalnode_list[mark3].append(nodeList.at(iii));
				//Map_allnode.insert(GET_IND(node_x2,node_y2,node_z2),mark4);//��ֵ��ÿ����Ķ�ά���꣬mark4���յ�Ķ�ά���ꣻ
				//Map_finalnode_list[mark3].append(nodeList.at(iii));

			}
			//mark_nodeList(mark3,nodeList,situation);
			printf("1111111111111111111111111111111111111111\n");

		}
		else
		{
			//printf("not found node in the Map_finalnode");
			//���Ļ����û�г�����Map_finalnode�����û�У��򽫸õ��ΪMap_finalnode�к�����������ĸ��ڵ��class_mark
			//20150306,��ǰ�ķ��������⣬���Խ���Map_finalnode�в������ĵ����¸�����ţ���nodeList����ĵ㶼�鲢������������
			printf("22222222222222222222222222222222222222\n");
			int new_mark=Map_rootnode.size()+1;
			cur_center->class_mark=new_mark;

			enlarge_radiusof_single_node_xy(img1d,cur_center,sz_x,sz_y,sz_z);//������ڵ�İ뾶
			//if(!Map_rootnode.contains(cur_center))
			{
				Map_rootnode.insert(new_mark,cur_center);

			}


			for(int iii=0;iii<nodeList.size();iii++)
			{
				VISIOCYTELONG node_x1=nodeList.at(iii)->x;
				VISIOCYTELONG node_y1=nodeList.at(iii)->y;
				VISIOCYTELONG node_z1=nodeList.at(iii)->z;

				Map_nodes[GET_IND(node_x1,node_y1,node_z1)]=new_mark;//��nodeList����Ľڵ㸳����Ӧ���ڵ�ı��
				Map_allnodes.insert(new_mark,nodeList.at(iii));
				Map_finalnode_list[new_mark].append(nodeList.at(iii));
				//Map_allnode.insert(GET_IND(node_x1,node_y1,node_z1),ind2);//��ֵ��ÿ����Ķ�ά���꣬ind2���յ�Ķ�ά���ꣻ
				//Map_finalnode_list[Map_finalnode[ind2]].append(nodeList.at(iii));

			}
			//mark_nodeList(mark,nodeList,situation);
		}

	}
	nodeList.free();
	delete cur_center;
	
	

}









