// fastmarching_dt.h       2012-02-23 by Hang Xiao

#ifndef __FASTMARCHING_DT_H__
#define __FASTMARCHING_DT_H__

#include <iostream>
#include <sstream>
#include <map>
#include "heap.h"
#include "my_surf_objs.h"
#include "fastmarching_macro.h"

using namespace std;

#ifndef ABS
#define ABS(x) ((x) > 0 ? (x) : (-(x)))
#endif

/******************************************************************************
 * Fast marching based distance transformation
 *
 * Input : inimg1d     original input grayscale image
 *         cnn_type    the connection type
 *         bkg_thresh  the background threshold, less or equal then bkg_thresh will considered as background
 *
 * Output : phi       distance tranformed output image
 *
 * Notice :
 * 1. the input pixel number should not be larger than 2G if sizeof(long) == 4
 * 2. The background point is of intensity 0
 * *****************************************************************************/

template<class T> bool fastmarching_dt(T * inimg1d, float * &phi, int sz0, int sz1, int sz2, int cnn_type = 3, int bkg_thresh = 0)
{
	enum{ALIVE = -1, TRIAL = 0, FAR = 1};

	long tol_sz = sz0 * sz1 * sz2;
	long sz01 = sz0 * sz1;
	//int cnn_type = 3;  // ?

	if(phi == 0) phi = new float[tol_sz];
	char * state = new char[tol_sz];
	int bkg_count = 0;                          // for process counting
	int bdr_count = 0;                          // for process counting
	for(long i = 0; i < tol_sz; i++)
	{
		if(inimg1d[i] <= bkg_thresh)
		{
			phi[i] = inimg1d[i];
			state[i] = ALIVE;
			//cout<<"+";cout.flush();
			bkg_count++;
		}
		else
		{
			phi[i] = INF;
			state[i] = FAR;
		}
	}
	cout<<endl;

	BasicHeap<HeapElem> heap;
	map<long, HeapElem*> elems;

	// init heap
	{
		long i = -1, j = -1, k = -1;
		for(long ind = 0; ind < tol_sz; ind++)
		{
			i++; if(i%sz0 == 0){i=0; j++; if(j%sz1==0){j=0; k++;}}
			if(state[ind] == ALIVE)
			{
				for(int kk = -1; kk <= 1; kk++)
				{
					long k2 = k+kk;
					if(k2 < 0 || k2 >= sz2) continue;
					for(int jj = -1; jj <= 1; jj++)
					{
						long j2 = j+jj;
						if(j2 < 0 || j2 >= sz1) continue;
						for(int ii = -1; ii <=1; ii++)
						{
							long i2 = i+ii;
							if(i2 < 0 || i2 >= sz0) continue;
							int offset = ABS(ii) + ABS(jj) + ABS(kk);
							if(offset == 0 || offset > cnn_type) continue;
							long ind2 = k2 * sz01 + j2 * sz0 + i2;
							if(state[ind2] == FAR)
							{
								long min_ind = ind;
								// get minimum Alive point around ind2
								if(phi[min_ind] > 0.0)
								{
									for(int kkk = -1; kkk <= 1; kkk++)
									{
										long k3 = k2 + kkk;
										if(k3 < 0 || k3 >= sz2) continue;
										for(int jjj = -1; jjj <= 1; jjj++)
										{
											long j3 = j2 + jjj;
											if(j3 < 0 || j3 >= sz1) continue;
											for(int iii = -1; iii <= 1; iii++)
											{
												long i3 = i2 + iii;
												if(i3 < 0 || i3 >= sz0) continue;
												int offset2 = ABS(iii) + ABS(jjj) + ABS(kkk);
												if(offset2 == 0 || offset2 > cnn_type) continue;
												long ind3 = k3 * sz01 + j3 * sz0 + i3;
												if(state[ind3] == ALIVE && phi[ind3] < phi[min_ind]) min_ind = ind3;
											}
										}
									}
								}
								// over
								phi[ind2] = phi[min_ind] + inimg1d[ind2];
								state[ind2] = TRIAL;
								HeapElem * elem = new HeapElem(ind2, phi[ind2]);
								heap.insert(elem);
								elems[ind2] = elem;
								bdr_count++;
							}
						}
					}
				}
			}
		}
	}

	cout<<"bkg_count = "<<bkg_count<<" ("<<bkg_count/(double)tol_sz<<")"<<endl;
	cout<<"bdr_count = "<<bdr_count<<" ("<<bdr_count/(double)tol_sz<<")"<<endl;
	cout<<"elems.size() = "<<elems.size()<<endl;
	// loop
	int time_counter = bkg_count;
	double process1 = 0;
	while(!heap.empty())
	{
		double process2 = (time_counter++)*100000.0/tol_sz;
		if(process2 - process1 >= 100) {cout<<"\r"<<((int)process2)/1000.0<<"%";cout.flush(); process1 = process2;
		}

		HeapElem* min_elem = heap.delete_min();
		elems.erase(min_elem->img_ind);

		long min_ind = min_elem->img_ind;
		delete min_elem;

		state[min_ind] = ALIVE;
		int i = min_ind % sz0;
		int j = (min_ind/sz0) % sz1;
		int k = (min_ind/sz01) % sz2;

		int w, h, d;
		for(int kk = -1; kk <= 1; kk++)
		{
			d = k+kk;
			if(d < 0 || d >= sz2) continue;
			for(int jj = -1; jj <= 1; jj++)
			{
				h = j+jj;
				if(h < 0 || h >= sz1) continue;
				for(int ii = -1; ii <= 1; ii++)
				{
					w = i+ii;
					if(w < 0 || w >= sz0) continue;
					int offset = ABS(ii) + ABS(jj) + ABS(kk);
					if(offset == 0 || offset > cnn_type) continue;
					long index = d*sz01 + h*sz0 + w;

					if(state[index] != ALIVE)
					{
						float new_dist = phi[min_ind] + inimg1d[index] * sqrt(double(offset));

						if(state[index] == FAR)
						{
							phi[index] = new_dist;
							HeapElem * elem = new HeapElem(index, phi[index]);
							heap.insert(elem);
							elems[index] = elem;
							state[index] = TRIAL;
						}
						else if(state[index] == TRIAL)
						{
							if(phi[index] > new_dist)
							{
								phi[index] = new_dist;
								HeapElem * elem = elems[index];
								heap.adjust(elem->heap_id, phi[index]);
							}
						}
					}
				}
			}
		}
	}
	//END_CLOCK;
	assert(elems.empty());
	if(state) {delete [] state; state = 0;}
	return true;
}


/******************************************************************************
 * Fast marching based distance transformation
 *
 * Input : inimg1d     original input grayscale image
 *         cnn_type    the connection type
 *         bkg_thresh  the background threshold, less or equal then bkg_thresh will considered as background
 *
 * Output : phi       distance tranformed output image
 *
 * Notice :
 * 1. the input pixel number should not be larger than 2G if sizeof(long) == 4
 * 2. The background point is of intensity 0
 * *****************************************************************************/

template<class T> bool fastmarching_dt1(T * inimg1d, float * &phi, int sz0, int sz1, int sz2, float md, int cnn_type = 3, int bkg_thresh = 0)
{
	enum{ALIVE = -1, TRIAL = 0, FAR = 1};

	long tol_sz = sz0 * sz1 * sz2;
	long sz01 = sz0 * sz1;
	//int cnn_type = 3;  // ?

	if(phi == 0) phi = new float[tol_sz];
	char * state = new char[tol_sz];
	int bkg_count = 0;                          // for process counting
	int bdr_count = 0;                          // for process counting
	for(long i = 0; i < tol_sz; i++)
	{
		if(inimg1d[i] <= bkg_thresh)
		{
			phi[i] = inimg1d[i];
			state[i] = ALIVE;
			//cout<<"+";cout.flush();
			bkg_count++;
		}
		else
		{
			phi[i] = INF;
			state[i] = FAR;
		}
	}
	cout<<endl;

	BasicHeap<HeapElem> heap;
	map<long, HeapElem*> elems;

	// init heap
	{
		long i = -1, j = -1, k = -1;
		for(long ind = 0; ind < tol_sz; ind++)
		{
			i++; if(i%sz0 == 0){i=0; j++; if(j%sz1==0){j=0; k++;}}
			if(state[ind] == ALIVE)
			{
				for(int kk = -1; kk <= 1; kk++)
				{
					long k2 = k+kk;
					if(k2 < 0 || k2 >= sz2) continue;
					for(int jj = -1; jj <= 1; jj++)
					{
						long j2 = j+jj;
						if(j2 < 0 || j2 >= sz1) continue;
						for(int ii = -1; ii <=1; ii++)
						{
							long i2 = i+ii;
							if(i2 < 0 || i2 >= sz0) continue;
							int offset = ABS(ii) + ABS(jj) + ABS(kk);
							if(offset == 0 || offset > cnn_type) continue;
							long ind2 = k2 * sz01 + j2 * sz0 + i2;
							if(state[ind2] == FAR)
							{
								long min_ind = ind;
								// get minimum Alive point around ind2
								if(phi[min_ind] > 0.0)
								{
									for(int kkk = -1; kkk <= 1; kkk++)
									{
										long k3 = k2 + kkk;
										if(k3 < 0 || k3 >= sz2) continue;
										for(int jjj = -1; jjj <= 1; jjj++)
										{
											long j3 = j2 + jjj;
											if(j3 < 0 || j3 >= sz1) continue;
											for(int iii = -1; iii <= 1; iii++)
											{
												long i3 = i2 + iii;
												if(i3 < 0 || i3 >= sz0) continue;
												int offset2 = ABS(iii) + ABS(jjj) + ABS(kkk);
												if(offset2 == 0 || offset2 > cnn_type) continue;
												long ind3 = k3 * sz01 + j3 * sz0 + i3;
												if(state[ind3] == ALIVE && phi[ind3] < phi[min_ind]) min_ind = ind3;
											}
										}
									}
								}
								// over
								phi[ind2] = phi[min_ind] + inimg1d[ind2];
								state[ind2] = TRIAL;
								HeapElem * elem = new HeapElem(ind2, phi[ind2]);
								heap.insert(elem);
								elems[ind2] = elem;
								bdr_count++;
							}
						}
					}
				}
			}
		}
	}

	cout<<"bkg_count = "<<bkg_count<<" ("<<bkg_count/(double)tol_sz<<")"<<endl;
	cout<<"bdr_count = "<<bdr_count<<" ("<<bdr_count/(double)tol_sz<<")"<<endl;
	cout<<"elems.size() = "<<elems.size()<<endl;
	// loop
	int time_counter = bkg_count;
	double process1 = 0;
	while(!heap.empty())
	{
		double process2 = (time_counter++)*100000.0/tol_sz;
		if(process2 - process1 >= 100) {cout<<"\r"<<((int)process2)/1000.0<<"%";cout.flush(); process1 = process2;
		}

		HeapElem* min_elem = heap.delete_min();
		elems.erase(min_elem->img_ind);

		long min_ind = min_elem->img_ind;
		delete min_elem;

		state[min_ind] = ALIVE;
		int i = min_ind % sz0;
		int j = (min_ind/sz0) % sz1;
		int k = (min_ind/sz01) % sz2;

		int w, h, d;
		for(int kk = -1; kk <= 1; kk++)
		{
			d = k+kk;
			if(d < 0 || d >= sz2) continue;
			for(int jj = -1; jj <= 1; jj++)
			{
				h = j+jj;
				if(h < 0 || h >= sz1) continue;
				for(int ii = -1; ii <= 1; ii++)
				{
					w = i+ii;
					if(w < 0 || w >= sz0) continue;
					int offset = ABS(ii) + ABS(jj) + ABS(kk);
					if(offset == 0 || offset > cnn_type) continue;
					long index = d*sz01 + h*sz0 + w;

					if(state[index] != ALIVE)
					{
						float new_dist = phi[min_ind] + inimg1d[index] * sqrt(double(offset));

						if(new_dist > md)
						{

							for(int ip = 0; ip < tol_sz; ip ++)
							{
								if(phi[i] > md)
									phi[i] = md;

							}

							//elems.empty();

							if(state) {delete [] state; state = 0;}

							                            // clear the heap
                            while(!heap.empty())
                            {

                                HeapElem* min_elem1 = heap.delete_min();
                                elems.erase(min_elem1->img_ind);

                                delete min_elem1;

                            }

							assert(elems.empty());


							return true;


						}


						if(state[index] == FAR)
						{
							phi[index] = new_dist;
							HeapElem * elem = new HeapElem(index, phi[index]);
							heap.insert(elem);
							elems[index] = elem;
							state[index] = TRIAL;
						}
						else if(state[index] == TRIAL)
						{
							if(phi[index] > new_dist)
							{
								phi[index] = new_dist;
								HeapElem * elem = elems[index];
								heap.adjust(elem->heap_id, phi[index]);
							}
						}
					}
				}
			}
		}
	}
	//END_CLOCK;
	assert(elems.empty());
	if(state) {delete [] state; state = 0;}
	return true;
}

/******************************************************************************
 * Fast marching based distance transformation in greedly connecting the components
 *
 * Input : inimg1d     original input grayscale image
 *         cnn_type    the connection type
 *         bkg_thresh  the background threshold, less or equal then bkg_thresh will considered as background
 *
 * Output : phi       distance tranformed output image
 *
 * Notice :
 * 1. the input pixel number should not be larger than 2G if sizeof(long) == 4
 * 2. The background point is of intensity 0
 * *****************************************************************************/

template<class T> float fastmarching_cc(T * inimg1d, uchar * seedimg1d, float * &phi, int sz0, int sz1, int sz2, int *tip_seed, uchar &target, float md, int cnn_type = 3, int bkg_thresh = 0)
{
	enum{ALIVE = -1, TRIAL = 0, FAR = 1};

	long tol_sz = sz0 * sz1 * sz2;
	long sz01 = sz0 * sz1;
	//int cnn_type = 3;  // ?


    phi = new float[tol_sz];
	char * state = new char[tol_sz];

	long i1 = 0;

//	cout << (int)inimg1d[i1] << endl;

		//cout << "..............................................................." << endl;

//	cout << "The tol_sz is " << tol_sz << endl;



	int bkg_count = 0;                          // for process counting
	int bdr_count = 0;                          // for process counting
	for(long i = 0; i < tol_sz; i++)
	{

      //  cout << i << endl;

       // cout << (int)inimg1d[i] << endl;

      //  cout << bkg_thresh << endl;

		if(inimg1d[i] <= bkg_thresh)
		{
           // cout << phi[i] <<

			phi[i] = inimg1d[i];
			state[i] = ALIVE;
	//		cout<<"+";cout.flush();
			bkg_count++;

		//	cout << bkg_count << endl;
		}
		else
		{
			phi[i] = INF;
			state[i] = FAR;

			//cout << i << endl;
		}
	}
	cout<<endl;

	BasicHeap<HeapElem> heap;
	map<long, HeapElem*> elems;





	// init heap
	{
		long i = -1, j = -1, k = -1;
		for(long ind = 0; ind < tol_sz; ind++)
		{
			i++; if(i%sz0 == 0){i=0; j++; if(j%sz1==0){j=0; k++;}}
			if(state[ind] == ALIVE)
			{
				for(int kk = -1; kk <= 1; kk++)
				{
					long k2 = k+kk;
					if(k2 < 0 || k2 >= sz2) continue;
					for(int jj = -1; jj <= 1; jj++)
					{
						long j2 = j+jj;
						if(j2 < 0 || j2 >= sz1) continue;
						for(int ii = -1; ii <=1; ii++)
						{
							long i2 = i+ii;
							if(i2 < 0 || i2 >= sz0) continue;
							int offset = ABS(ii) + ABS(jj) + ABS(kk);
							if(offset == 0 || offset > cnn_type) continue;
							long ind2 = k2 * sz01 + j2 * sz0 + i2;
							if(state[ind2] == FAR)
							{
								long min_ind = ind;
								// get minimum Alive point around ind2
								if(phi[min_ind] > 0.0)
								{
									for(int kkk = -1; kkk <= 1; kkk++)
									{
										long k3 = k2 + kkk;
										if(k3 < 0 || k3 >= sz2) continue;
										for(int jjj = -1; jjj <= 1; jjj++)
										{
											long j3 = j2 + jjj;
											if(j3 < 0 || j3 >= sz1) continue;
											for(int iii = -1; iii <= 1; iii++)
											{
												long i3 = i2 + iii;
												if(i3 < 0 || i3 >= sz0) continue;
												int offset2 = ABS(iii) + ABS(jjj) + ABS(kkk);
												if(offset2 == 0 || offset2 > cnn_type) continue;
												long ind3 = k3 * sz01 + j3 * sz0 + i3;
												if(state[ind3] == ALIVE && phi[ind3] < phi[min_ind]) min_ind = ind3;
											}
										}
									}
								}
								// over
								phi[ind2] = phi[min_ind] + inimg1d[ind2];
								state[ind2] = TRIAL;
								HeapElem * elem = new HeapElem(ind2, phi[ind2]);
								heap.insert(elem);
								elems[ind2] = elem;
								bdr_count++;
							}
						}
					}
				}
			}
		}
	}

	cout<<"bkg_count = "<<bkg_count<<" ("<<bkg_count/(double)tol_sz<<")"<<endl;
	cout<<"bdr_count = "<<bdr_count<<" ("<<bdr_count/(double)tol_sz<<")"<<endl;
	cout<<"elems.size() = "<<elems.size()<<endl;
	// loop
	int time_counter = bkg_count;
	double process1 = 0;
	while(!heap.empty())
	{
		double process2 = (time_counter++)*100000.0/tol_sz;
		if(process2 - process1 >= 100) {cout<<"\r"<<((int)process2)/1000.0<<"%";cout.flush(); process1 = process2;
		}

		HeapElem* min_elem = heap.delete_min();
		elems.erase(min_elem->img_ind);

		long min_ind = min_elem->img_ind;
		delete min_elem;

		state[min_ind] = ALIVE;
		int i = min_ind % sz0;
		int j = (min_ind/sz0) % sz1;
		int k = (min_ind/sz01) % sz2;

		int w, h, d;
		for(int kk = -1; kk <= 1; kk++)
		{
			d = k+kk;
			if(d < 0 || d >= sz2) continue;
			for(int jj = -1; jj <= 1; jj++)
			{
				h = j+jj;
				if(h < 0 || h >= sz1) continue;
				for(int ii = -1; ii <= 1; ii++)
				{
					w = i+ii;
					if(w < 0 || w >= sz0) continue;
					int offset = ABS(ii) + ABS(jj) + ABS(kk);
					if(offset == 0 || offset > cnn_type) continue;
					int index = d*sz01 + h*sz0 + w;

					if(state[index] != ALIVE)
					{
						float new_dist = phi[min_ind] + inimg1d[index] * sqrt(double(offset));

						//int seed = (int)seedimg1d[index];

						if((new_dist > md) || (seedimg1d[index] > 0))
						{


                            tip_seed[0] = w;

                            tip_seed[1] = h;

                            tip_seed[2] = d;

                            target = seedimg1d[index];

                            cout << "The current distance is " << new_dist << endl;

                            phi[index] = new_dist;

							for(int ip = 0; ip < tol_sz; ip ++)
							{
								if(phi[ip] > md)
									phi[ip] = md + 10;

							}

                            if(state)
                            {
                                delete [] state;
                                state = 0;
                            }

                            // clear the heap
                            while(!heap.empty())
                            {

                                HeapElem* min_elem1 = heap.delete_min();
                                elems.erase(min_elem1->img_ind);

                                delete min_elem1;

                            }

							assert(elems.empty());

							return new_dist;

							//return true;


						}

						if(state[index] == FAR)
						{
							phi[index] = new_dist;
							HeapElem * elem = new HeapElem(index, phi[index]);
							heap.insert(elem);
							elems[index] = elem;
							state[index] = TRIAL;
						}
						else if(state[index] == TRIAL)
						{
							if(phi[index] > new_dist)
							{
								phi[index] = new_dist;
								HeapElem * elem = elems[index];
								heap.adjust(elem->heap_id, phi[index]);
							}
						}
					}
				}
			}
		}
	}
	//END_CLOCK;
	assert(elems.empty());
	if(state) {delete [] state; state = 0;}

	md = md ;
	return md;
}


 //   vector<int> px;

 //   vector<int> py;

 //   vector<int> pz;

 //   vector<double> prop_b;




template<class T> float fastmarching_cc3(uchar * inimg1d, uchar * seedimg1d, float * &phi, int sz0, int sz1, int sz2, vector<double> pl, vector<int> &px,
vector<int> &py, vector<int> &pz, vector<double> &target_prop, T md, int cnn_type = 3, int bkg_thresh = 0)
{
	enum{ALIVE = -1, TRIAL = 0, FAR = 1};

	long tol_sz = sz0 * sz1 * sz2;
	long sz01 = sz0 * sz1;
	//int cnn_type = 3;  // ?


    phi = new float[tol_sz];
	char * state = new char[tol_sz];

	long i1 = 0;

//	cout << (int)inimg1d[i1] << endl;

		//cout << "..............................................................." << endl;

//	cout << "The tol_sz is " << tol_sz << endl;



	int bkg_count = 0;                          // for process counting
	int bdr_count = 0;                          // for process counting
	for(long i = 0; i < tol_sz; i++)
	{

      //  cout << i << endl;

       // cout << (int)inimg1d[i] << endl;

      //  cout << bkg_thresh << endl;

		if((float)inimg1d[i] <= bkg_thresh)
		{
           // cout << phi[i] <<

			phi[i] = (float)inimg1d[i];
			state[i] = ALIVE;
	//		cout<<"+";cout.flush();
			bkg_count++;

		//	cout << bkg_count << endl;
		}
		else
		{
			phi[i] = INF;
			state[i] = FAR;

			//cout << i << endl;
		}
	}
	cout<<endl;

	BasicHeap<HeapElem> heap;
	map<long, HeapElem*> elems;







	// init heap
	{
		long i = -1, j = -1, k = -1;
		for(long ind = 0; ind < tol_sz; ind++)
		{
			i++; if(i%sz0 == 0){i=0; j++; if(j%sz1==0){j=0; k++;}}
			if(state[ind] == ALIVE)
			{
				for(int kk = -1; kk <= 1; kk++)
				{
					long k2 = k+kk;
					if(k2 < 0 || k2 >= sz2) continue;
					for(int jj = -1; jj <= 1; jj++)
					{
						long j2 = j+jj;
						if(j2 < 0 || j2 >= sz1) continue;
						for(int ii = -1; ii <=1; ii++)
						{
							long i2 = i+ii;
							if(i2 < 0 || i2 >= sz0) continue;
							int offset = ABS(ii) + ABS(jj) + ABS(kk);
							if(offset == 0 || offset > cnn_type) continue;
							long ind2 = k2 * sz01 + j2 * sz0 + i2;
							if(state[ind2] == FAR)
							{
								long min_ind = ind;
								// get minimum Alive point around ind2
								if(phi[min_ind] > 0.0)
								{
									for(int kkk = -1; kkk <= 1; kkk++)
									{
										long k3 = k2 + kkk;
										if(k3 < 0 || k3 >= sz2) continue;
										for(int jjj = -1; jjj <= 1; jjj++)
										{
											long j3 = j2 + jjj;
											if(j3 < 0 || j3 >= sz1) continue;
											for(int iii = -1; iii <= 1; iii++)
											{
												long i3 = i2 + iii;
												if(i3 < 0 || i3 >= sz0) continue;
												int offset2 = ABS(iii) + ABS(jjj) + ABS(kkk);
												if(offset2 == 0 || offset2 > cnn_type) continue;
												long ind3 = k3 * sz01 + j3 * sz0 + i3;
												if(state[ind3] == ALIVE && phi[ind3] < phi[min_ind]) min_ind = ind3;
											}
										}
									}
								}
								// over
								phi[ind2] = phi[min_ind] + (float)inimg1d[ind2];
								state[ind2] = TRIAL;
								HeapElem * elem = new HeapElem(ind2, phi[ind2]);
								heap.insert(elem);
								elems[ind2] = elem;
								bdr_count++;
							}
						}
					}
				}
			}
		}
	}

	cout<<"bkg_count = "<<bkg_count<<" ("<<bkg_count/(double)tol_sz<<")"<<endl;
	cout<<"bdr_count = "<<bdr_count<<" ("<<bdr_count/(double)tol_sz<<")"<<endl;
	cout<<"elems.size() = "<<elems.size()<<endl;
	// loop
	int time_counter = bkg_count;
	double process1 = 0;

	int n_base = pl.size();


	double *c_pl =  new double[n_base];

	for(int i = 0; i < n_base; i++)
        c_pl[i] = 0;

     int max_b = distance(pl.begin(),max_element(pl.begin(),pl.end()));

     c_pl[max_b] = pl[max_b];



    double prop_base_merged = 0;

    float min_dist_found = 0;

    uchar tmp_label = 0;


    int * b_label_tick = new int[n_base];

    for(int i = 0; i < n_base; i++)
        b_label_tick[i] = 0;


	while(!heap.empty())
	{
		double process2 = (time_counter++)*100000.0/tol_sz;
		if(process2 - process1 >= 100) {cout<<"\r"<<((int)process2)/1000.0<<"%";cout.flush(); process1 = process2;
		}

		HeapElem* min_elem = heap.delete_min();
		elems.erase(min_elem->img_ind);

		long min_ind = min_elem->img_ind;
		delete min_elem;

		state[min_ind] = ALIVE;
		int i = min_ind % sz0;
		int j = (min_ind/sz0) % sz1;
		int k = (min_ind/sz01) % sz2;

		int w, h, d;
		for(int kk = -1; kk <= 1; kk++)
		{
			d = k+kk;
			if(d < 0 || d >= sz2) continue;
			for(int jj = -1; jj <= 1; jj++)
			{
				h = j+jj;
				if(h < 0 || h >= sz1) continue;
				for(int ii = -1; ii <= 1; ii++)
				{
					w = i+ii;
					if(w < 0 || w >= sz0) continue;
					int offset = ABS(ii) + ABS(jj) + ABS(kk);
					if(offset == 0 || offset > cnn_type) continue;
					int index = d*sz01 + h*sz0 + w;
                    if(state[index] != ALIVE)
                    {
                        float new_dist = phi[min_ind] + (float)inimg1d[index] * sqrt(double(offset));

                        //int seed = (int)seedimg1d[index];

                        if((seedimg1d[index] > 0))
                        {



                            int label = (int)seedimg1d[index];

                            //cout << "tmp_label is " << (int)tmp_label << endl;

                            //cout << "seedimg is " << label << endl;

                            //cin.get();


                            if(b_label_tick[label - 1] == 0)
                            {


                                //cout << "tmp_label is " << (int)tmp_label << endl;

                                //cout << "seedimg is " << label << endl;

                                //cout << "previous proportion is " << pl[(int)tmp_label - 1] << endl;

                                //cout << "current proportion is " << pl[label - 1] << endl;


                               // cin.get();


                                px.push_back(w);

                                py.push_back(h);

                                pz.push_back(d);

                                target_prop.push_back(pl[label - 1]);

                                b_label_tick[label - 1] = 1;

                                //cin.get();

                            }

                            tmp_label = seedimg1d[index];

                            c_pl[label - 1] = pl[label - 1];

                            prop_base_merged = 0;

                            for(int i = 0; i < n_base; i++)
                                prop_base_merged = prop_base_merged + c_pl[i];

                         //   cout << "The merged base is " <<   prop_base_merged << endl;

                         //   cin.get();

                            if(prop_base_merged > 0.98)
                            {

                                phi[index] = new_dist;

                                //for(int ip = 0; ip < tol_sz; ip ++)
                                //{
                                //  if(phi[ip] > md)
                                //    phi[ip] = md + 10;

                                //}


                                if(state)
                                {
                                    delete [] state;
                                    state = 0;
                                }

                                delete [] b_label_tick;

                                // clear the heap
                                while(!heap.empty())
                                {

                                    HeapElem* min_elem1 = heap.delete_min();
                                    elems.erase(min_elem1->img_ind);

                                    delete min_elem1;

                                }

                                assert(elems.empty());



                                return new_dist;



                            }


                        }


                        // tip_seed[0] = w;

                        // tip_seed[1] = h;

                        // tip_seed[2] = d;

                        // target = seedimg1d[index];

                        //cout << "The current distance is " << new_dist << endl;



                        //return true;




						if(state[index] == FAR)
						{
							phi[index] = new_dist;
							HeapElem * elem = new HeapElem(index, phi[index]);
							heap.insert(elem);
							elems[index] = elem;
							state[index] = TRIAL;
						}
						else if(state[index] == TRIAL)
						{
							if(phi[index] > new_dist)
							{
								phi[index] = new_dist;
								HeapElem * elem = elems[index];
								heap.adjust(elem->heap_id, phi[index]);
							}
						}
					}
				}
			}
		}
	}
	//END_CLOCK;
	assert(elems.empty());
	if(state) {delete [] state; state = 0;}

	delete [] b_label_tick;

	//md = md ;
	return md;
}

template<class T> float fastmarching_cc2(uchar * inimg1d, uchar * seedimg1d, float * &phi, int sz0, int sz1, int sz2, int *tip_seed, uchar &target, T md, int cnn_type = 3, int bkg_thresh = 0)
{
	enum{ALIVE = -1, TRIAL = 0, FAR = 1};

	long tol_sz = sz0 * sz1 * sz2;
	long sz01 = sz0 * sz1;
	//int cnn_type = 3;  // ?


    phi = new float[tol_sz];
	char * state = new char[tol_sz];

	long i1 = 0;

//	cout << (int)inimg1d[i1] << endl;

		//cout << "..............................................................." << endl;

//	cout << "The tol_sz is " << tol_sz << endl;



	int bkg_count = 0;                          // for process counting
	int bdr_count = 0;                          // for process counting
	for(long i = 0; i < tol_sz; i++)
	{

      //  cout << i << endl;

       // cout << (int)inimg1d[i] << endl;

      //  cout << bkg_thresh << endl;

		if((float)inimg1d[i] <= bkg_thresh)
		{
           // cout << phi[i] <<

			phi[i] = (float)inimg1d[i];
			state[i] = ALIVE;
	//		cout<<"+";cout.flush();
			bkg_count++;

		//	cout << bkg_count << endl;
		}
		else
		{
			phi[i] = INF;
			state[i] = FAR;

			//cout << i << endl;
		}
	}
	cout<<endl;

	BasicHeap<HeapElem> heap;
	map<long, HeapElem*> elems;





	// init heap
	{
		long i = -1, j = -1, k = -1;
		for(long ind = 0; ind < tol_sz; ind++)
		{
			i++; if(i%sz0 == 0){i=0; j++; if(j%sz1==0){j=0; k++;}}
			if(state[ind] == ALIVE)
			{
				for(int kk = -1; kk <= 1; kk++)
				{
					long k2 = k+kk;
					if(k2 < 0 || k2 >= sz2) continue;
					for(int jj = -1; jj <= 1; jj++)
					{
						long j2 = j+jj;
						if(j2 < 0 || j2 >= sz1) continue;
						for(int ii = -1; ii <=1; ii++)
						{
							long i2 = i+ii;
							if(i2 < 0 || i2 >= sz0) continue;
							int offset = ABS(ii) + ABS(jj) + ABS(kk);
							if(offset == 0 || offset > cnn_type) continue;
							long ind2 = k2 * sz01 + j2 * sz0 + i2;
							if(state[ind2] == FAR)
							{
								long min_ind = ind;
								// get minimum Alive point around ind2
								if(phi[min_ind] > 0.0)
								{
									for(int kkk = -1; kkk <= 1; kkk++)
									{
										long k3 = k2 + kkk;
										if(k3 < 0 || k3 >= sz2) continue;
										for(int jjj = -1; jjj <= 1; jjj++)
										{
											long j3 = j2 + jjj;
											if(j3 < 0 || j3 >= sz1) continue;
											for(int iii = -1; iii <= 1; iii++)
											{
												long i3 = i2 + iii;
												if(i3 < 0 || i3 >= sz0) continue;
												int offset2 = ABS(iii) + ABS(jjj) + ABS(kkk);
												if(offset2 == 0 || offset2 > cnn_type) continue;
												long ind3 = k3 * sz01 + j3 * sz0 + i3;
												if(state[ind3] == ALIVE && phi[ind3] < phi[min_ind]) min_ind = ind3;
											}
										}
									}
								}
								// over
								phi[ind2] = phi[min_ind] + (float)inimg1d[ind2];
								state[ind2] = TRIAL;
								HeapElem * elem = new HeapElem(ind2, phi[ind2]);
								heap.insert(elem);
								elems[ind2] = elem;
								bdr_count++;
							}
						}
					}
				}
			}
		}
	}

	cout<<"bkg_count = "<<bkg_count<<" ("<<bkg_count/(double)tol_sz<<")"<<endl;
	cout<<"bdr_count = "<<bdr_count<<" ("<<bdr_count/(double)tol_sz<<")"<<endl;
	cout<<"elems.size() = "<<elems.size()<<endl;
	// loop
	int time_counter = bkg_count;
	double process1 = 0;
	while(!heap.empty())
	{
		double process2 = (time_counter++)*100000.0/tol_sz;
		if(process2 - process1 >= 100) {cout<<"\r"<<((int)process2)/1000.0<<"%";cout.flush(); process1 = process2;
		}

		HeapElem* min_elem = heap.delete_min();
		elems.erase(min_elem->img_ind);

		long min_ind = min_elem->img_ind;
		delete min_elem;

		state[min_ind] = ALIVE;
		int i = min_ind % sz0;
		int j = (min_ind/sz0) % sz1;
		int k = (min_ind/sz01) % sz2;

		int w, h, d;
		for(int kk = -1; kk <= 1; kk++)
		{
			d = k+kk;
			if(d < 0 || d >= sz2) continue;
			for(int jj = -1; jj <= 1; jj++)
			{
				h = j+jj;
				if(h < 0 || h >= sz1) continue;
				for(int ii = -1; ii <= 1; ii++)
				{
					w = i+ii;
					if(w < 0 || w >= sz0) continue;
					int offset = ABS(ii) + ABS(jj) + ABS(kk);
					if(offset == 0 || offset > cnn_type) continue;
					int index = d*sz01 + h*sz0 + w;

					if(state[index] != ALIVE)
					{
						float new_dist = phi[min_ind] + (float)inimg1d[index] * sqrt(double(offset));

						//int seed = (int)seedimg1d[index];

						if((new_dist > md) || (seedimg1d[index] > 0))
						{


                            tip_seed[0] = w;

                            tip_seed[1] = h;

                            tip_seed[2] = d;

                            target = seedimg1d[index];

                            cout << "The current distance is " << new_dist << endl;

                            phi[index] = new_dist;

							for(int ip = 0; ip < tol_sz; ip ++)
							{
								if(phi[ip] > md)
									phi[ip] = md + 10;

							}

							if(state) {delete [] state; state = 0;}

                            // clear the heap
                            while(!heap.empty())
                            {

                                HeapElem* min_elem1 = heap.delete_min();
                                elems.erase(min_elem1->img_ind);

                                delete min_elem1;

                            }

							assert(elems.empty());



							return new_dist;

							//return true;


						}

						if(state[index] == FAR)
						{
							phi[index] = new_dist;
							HeapElem * elem = new HeapElem(index, phi[index]);
							heap.insert(elem);
							elems[index] = elem;
							state[index] = TRIAL;
						}
						else if(state[index] == TRIAL)
						{
							if(phi[index] > new_dist)
							{
								phi[index] = new_dist;
								HeapElem * elem = elems[index];
								heap.adjust(elem->heap_id, phi[index]);
							}
						}
					}
				}
			}
		}
	}
	//END_CLOCK;
	assert(elems.empty());
	if(state) {delete [] state; state = 0;}

	md = md ;
	return md;
}

template<class T> float fastmarching_cc1(T * inimg1d, uchar * seedimg1d, float * &phi,float * dist_img, int sz0, int sz1, int sz2, int *tip_seed, uchar &target, float md, int cnn_type = 3, int bkg_thresh = 0)
{
	enum{ALIVE = -1, TRIAL = 0, FAR = 1};

	long tol_sz = sz0 * sz1 * sz2;
	long sz01 = sz0 * sz1;
	//int cnn_type = 3;  // ?


    phi = new float[tol_sz];
	char * state = new char[tol_sz];

	long i1 = 0;

//	cout << (int)inimg1d[i1] << endl;

		//cout << "..............................................................." << endl;

//	cout << "The tol_sz is " << tol_sz << endl;



	int bkg_count = 0;                          // for process counting
	int bdr_count = 0;                          // for process counting
	for(long i = 0; i < tol_sz; i++)
	{

      //  cout << i << endl;

       // cout << (int)inimg1d[i] << endl;

      //  cout << bkg_thresh << endl;

		if(inimg1d[i] <= bkg_thresh)
		{
           // cout << phi[i] <<

			phi[i] = inimg1d[i];
			state[i] = ALIVE;
	//		cout<<"+";cout.flush();
			bkg_count++;

		//	cout << bkg_count << endl;
		}
		else
		{
			phi[i] = INF;
			state[i] = FAR;

			//cout << i << endl;
		}
	}
	cout<<endl;

	BasicHeap<HeapElem> heap;
	map<long, HeapElem*> elems;





	// init heap
	{
		long i = -1, j = -1, k = -1;
		for(long ind = 0; ind < tol_sz; ind++)
		{
			i++; if(i%sz0 == 0){i=0; j++; if(j%sz1==0){j=0; k++;}}
			if(state[ind] == ALIVE)
			{
				for(int kk = -1; kk <= 1; kk++)
				{
					long k2 = k+kk;
					if(k2 < 0 || k2 >= sz2) continue;
					for(int jj = -1; jj <= 1; jj++)
					{
						long j2 = j+jj;
						if(j2 < 0 || j2 >= sz1) continue;
						for(int ii = -1; ii <=1; ii++)
						{
							long i2 = i+ii;
							if(i2 < 0 || i2 >= sz0) continue;
							int offset = ABS(ii) + ABS(jj) + ABS(kk);
							if(offset == 0 || offset > cnn_type) continue;
							long ind2 = k2 * sz01 + j2 * sz0 + i2;
							if(state[ind2] == FAR)
							{
								long min_ind = ind;
								// get minimum Alive point around ind2
								if(phi[min_ind] > 0.0)
								{
									for(int kkk = -1; kkk <= 1; kkk++)
									{
										long k3 = k2 + kkk;
										if(k3 < 0 || k3 >= sz2) continue;
										for(int jjj = -1; jjj <= 1; jjj++)
										{
											long j3 = j2 + jjj;
											if(j3 < 0 || j3 >= sz1) continue;
											for(int iii = -1; iii <= 1; iii++)
											{
												long i3 = i2 + iii;
												if(i3 < 0 || i3 >= sz0) continue;
												int offset2 = ABS(iii) + ABS(jjj) + ABS(kkk);
												if(offset2 == 0 || offset2 > cnn_type) continue;
												long ind3 = k3 * sz01 + j3 * sz0 + i3;
												if(state[ind3] == ALIVE && phi[ind3] < phi[min_ind]) min_ind = ind3;
											}
										}
									}
								}
								// over
								phi[ind2] = phi[min_ind] + inimg1d[ind2];
								state[ind2] = TRIAL;
								HeapElem * elem = new HeapElem(ind2, phi[ind2]);
								heap.insert(elem);
								elems[ind2] = elem;
								bdr_count++;
							}
						}
					}
				}
			}
		}
	}

	cout<<"bkg_count = "<<bkg_count<<" ("<<bkg_count/(double)tol_sz<<")"<<endl;
	cout<<"bdr_count = "<<bdr_count<<" ("<<bdr_count/(double)tol_sz<<")"<<endl;
	cout<<"elems.size() = "<<elems.size()<<endl;
	// loop
	int time_counter = bkg_count;
	double process1 = 0;
	while(!heap.empty())
	{
		double process2 = (time_counter++)*100000.0/tol_sz;
		if(process2 - process1 >= 100) {cout<<"\r"<<((int)process2)/1000.0<<"%";cout.flush(); process1 = process2;
		}

		HeapElem* min_elem = heap.delete_min();
		elems.erase(min_elem->img_ind);

		long min_ind = min_elem->img_ind;
		delete min_elem;

		state[min_ind] = ALIVE;
		int i = min_ind % sz0;
		int j = (min_ind/sz0) % sz1;
		int k = (min_ind/sz01) % sz2;

		int w, h, d;
		for(int kk = -1; kk <= 1; kk++)
		{
			d = k+kk;
			if(d < 0 || d >= sz2) continue;
			for(int jj = -1; jj <= 1; jj++)
			{
				h = j+jj;
				if(h < 0 || h >= sz1) continue;
				for(int ii = -1; ii <= 1; ii++)
				{
					w = i+ii;
					if(w < 0 || w >= sz0) continue;
					int offset = ABS(ii) + ABS(jj) + ABS(kk);
					if(offset == 0 || offset > cnn_type) continue;
					int index = d*sz01 + h*sz0 + w;

					if(state[index] != ALIVE)
					{
						float new_dist = phi[min_ind] + inimg1d[index] * sqrt(double(offset));

						int seed = (int)seedimg1d[index];

						cout << "The current distance is " << (double)dist_img[index] << endl;

						if((new_dist > md) || (seedimg1d[index] > 0))
						{


                            tip_seed[0] = w;

                            tip_seed[1] = h;

                            tip_seed[2] = d;

                            target = seedimg1d[index];

                            cout << "The current distance is " << new_dist << endl;

                            phi[index] = new_dist;

							for(int ip = 0; ip < tol_sz; ip ++)
							{
								if(phi[ip] > md)
									phi[ip] = md + 10;

							}


							if(state) {delete [] state; state = 0;}
							                            // clear the heap
                            while(!heap.empty())
                            {

                                HeapElem* min_elem1 = heap.delete_min();
                                elems.erase(min_elem1->img_ind);

                                delete min_elem1;

                            }

							assert(elems.empty());


							return new_dist;

							//return true;


						}

						if(state[index] == FAR)
						{
							phi[index] = new_dist;
							HeapElem * elem = new HeapElem(index, phi[index]);
							heap.insert(elem);
							elems[index] = elem;
							state[index] = TRIAL;
						}
						else if(state[index] == TRIAL)
						{
							if(phi[index] > new_dist)
							{
								phi[index] = new_dist;
								HeapElem * elem = elems[index];
								heap.adjust(elem->heap_id, phi[index]);
							}
						}
					}
				}
			}
		}
	}
	//END_CLOCK;
	assert(elems.empty());
	if(state) {delete [] state; state = 0;}

	md = md ;
	return md;
}




template<class T> bool fastmarching_dt_XY(T * inimg1d, float * &phi, int sz0, int sz1, int sz2, int cnn_type = 3, int bkg_thresh = 0)
{
	enum{ALIVE = -1, TRIAL = 0, FAR = 1};

	long tol_sz = sz0 * sz1 * sz2;
	long sz01 = sz0 * sz1;
	//int cnn_type = 3;  // ?

	if(phi == 0) phi = new float[tol_sz];
	char * state = new char[tol_sz];
	int bkg_count = 0;                          // for process counting
	int bdr_count = 0;                          // for process counting
	for(long i = 0; i < tol_sz; i++)
	{
		if(inimg1d[i] <= bkg_thresh)
		{
			phi[i] = inimg1d[i];
			state[i] = ALIVE;
			//cout<<"+";cout.flush();
			bkg_count++;
		}
		else
		{
			phi[i] = INF;
			state[i] = FAR;
		}
	}
	cout<<endl;

	BasicHeap<HeapElem> heap;
	map<long, HeapElem*> elems;

	// init heap
	{
		long i = -1, j = -1, k = -1;
		for(long ind = 0; ind < tol_sz; ind++)
		{
			i++; if(i%sz0 == 0){i=0; j++; if(j%sz1==0){j=0; k++;}}
			if(state[ind] == ALIVE)
			{
				for(int kk = 0; kk <= 0; kk++)
				{
					long k2 = k+kk;
					if(k2 < 0 || k2 >= sz2) continue;
					for(int jj = -1; jj <= 1; jj++)
					{
						long j2 = j+jj;
						if(j2 < 0 || j2 >= sz1) continue;
						for(int ii = -1; ii <=1; ii++)
						{
							long i2 = i+ii;
							if(i2 < 0 || i2 >= sz0) continue;
							int offset = ABS(ii) + ABS(jj) + ABS(kk);
							if(offset == 0 || offset > cnn_type) continue;
							long ind2 = k2 * sz01 + j2 * sz0 + i2;
							if(state[ind2] == FAR)
							{
								long min_ind = ind;
								// get minimum Alive point around ind2
								if(phi[min_ind] > 0.0)
								{
									for(int kkk = 0; kkk <= 0; kkk++)
									{
										long k3 = k2 + kkk;
										if(k3 < 0 || k3 >= sz2) continue;
										for(int jjj = -1; jjj <= 1; jjj++)
										{
											long j3 = j2 + jjj;
											if(j3 < 0 || j3 >= sz1) continue;
											for(int iii = -1; iii <= 1; iii++)
											{
												long i3 = i2 + iii;
												if(i3 < 0 || i3 >= sz0) continue;
												int offset2 = ABS(iii) + ABS(jjj) + ABS(kkk);
												if(offset2 == 0 || offset2 > cnn_type) continue;
												long ind3 = k3 * sz01 + j3 * sz0 + i3;
												if(state[ind3] == ALIVE && phi[ind3] < phi[min_ind]) min_ind = ind3;
											}
										}
									}
								}
								// over
								phi[ind2] = phi[min_ind] + inimg1d[ind2];
								state[ind2] = TRIAL;
								HeapElem * elem = new HeapElem(ind2, phi[ind2]);
								heap.insert(elem);
								elems[ind2] = elem;
								bdr_count++;
							}
						}
					}
				}
			}
		}
	}

	cout<<"bkg_count = "<<bkg_count<<" ("<<bkg_count/(double)tol_sz<<")"<<endl;
	cout<<"bdr_count = "<<bdr_count<<" ("<<bdr_count/(double)tol_sz<<")"<<endl;
	cout<<"elems.size() = "<<elems.size()<<endl;
	// loop
	int time_counter = bkg_count;
	double process1 = 0;
	while(!heap.empty())
	{
		double process2 = (time_counter++)*100000.0/tol_sz;
		if(process2 - process1 >= 10) {cout<<"\r"<<((int)process2)/1000.0<<"%";cout.flush(); process1 = process2;
			//SAVE_PHI_IMAGE(phi, sz0, sz1, sz2, string("phi") + num2str((int)process1) + ".tif");
		}

		HeapElem* min_elem = heap.delete_min();
		elems.erase(min_elem->img_ind);

		long min_ind = min_elem->img_ind;
		delete min_elem;

		state[min_ind] = ALIVE;
		int i = min_ind % sz0;
		int j = (min_ind/sz0) % sz1;
		int k = (min_ind/sz01) % sz2;

		int w, h, d;
		for(int kk = 0; kk <= 0; kk++)
		{
			d = k+kk;
			if(d < 0 || d >= sz2) continue;
			for(int jj = -1; jj <= 1; jj++)
			{
				h = j+jj;
				if(h < 0 || h >= sz1) continue;
				for(int ii = -1; ii <= 1; ii++)
				{
					w = i+ii;
					if(w < 0 || w >= sz0) continue;
					int offset = ABS(ii) + ABS(jj) + ABS(kk);
					if(offset == 0 || offset > cnn_type) continue;
					long index = d*sz01 + h*sz0 + w;

					if(state[index] != ALIVE)
					{
						float new_dist = phi[min_ind] + inimg1d[index] * sqrt(double(offset));

						if(state[index] == FAR)
						{
							phi[index] = new_dist;
							HeapElem * elem = new HeapElem(index, phi[index]);
							heap.insert(elem);
							elems[index] = elem;
							state[index] = TRIAL;
						}
						else if(state[index] == TRIAL)
						{
							if(phi[index] > new_dist)
							{
								phi[index] = new_dist;
								HeapElem * elem = elems[index];
								heap.adjust(elem->heap_id, phi[index]);
							}
						}
					}
				}
			}
		}
	}

	assert(elems.empty());
	if(state) {delete [] state; state = 0;}
	return true;
}

/******************************************************************************
 * Fast marching based distance transformation tree
 *
 * Input : inimg1d     original input grayscale image
 *         cnn_type    the connection type
 *         bkg_thresh  the background threshold, less or equal then bkg_thresh will considered as background
 *
 * Output : phi       distance tranformed output image
 *
 * Notice :
 * 1. the input pixel number should not be larger than 2G if sizeof(long) == 4
 * 2. The background point is of intensity 0
 * *****************************************************************************/

template<class T> bool fastmarching_dt_tree(T * inimg1d, vector<MyMarker*> &outtree, int sz0, int sz1, int sz2, int cnn_type = 3, int bkg_thresh = 0)
{
	enum{ALIVE = -1, TRIAL = 0, FAR = 1};

	long tol_sz = sz0 * sz1 * sz2;
	long sz01 = sz0 * sz1;
	//int cnn_type = 3;  // ?

	float * phi = new float[tol_sz];
	long * parent = new long[tol_sz]; for(long ind = 0; ind < tol_sz; ind++) parent[ind] = ind;
	char * state = new char[tol_sz];
	int bkg_count = 0;                          // for process counting
	int bdr_count = 0;                          // for process counting
	for(long i = 0; i < tol_sz; i++)
	{
		if(inimg1d[i] <= bkg_thresh)
		{
			phi[i] = inimg1d[i];
			state[i] = ALIVE;
			//cout<<"+";cout.flush();
			bkg_count++;
		}
		else
		{
			phi[i] = INF;
			state[i] = FAR;
		}
	}
	cout<<endl;

	BasicHeap<HeapElem> heap;
	map<long, HeapElem*> elems;

	// init heap
	{
		long i = -1, j = -1, k = -1;
		for(long ind = 0; ind < tol_sz; ind++)
		{
			i++; if(i%sz0 == 0){i=0; j++; if(j%sz1==0){j=0; k++;}}
			if(state[ind] == ALIVE)
			{
				for(int kk = -1; kk <= 1; kk++)
				{
					long k2 = k+kk;
					if(k2 < 0 || k2 >= sz2) continue;
					for(int jj = -1; jj <= 1; jj++)
					{
						long j2 = j+jj;
						if(j2 < 0 || j2 >= sz1) continue;
						for(int ii = -1; ii <=1; ii++)
						{
							long i2 = i+ii;
							if(i2 < 0 || i2 >= sz0) continue;
							int offset = ABS(ii) + ABS(jj) + ABS(kk);
							if(offset == 0 || offset > cnn_type) continue;
							long ind2 = k2 * sz01 + j2 * sz0 + i2;
							if(state[ind2] == FAR)
							{
								long min_ind = ind;
								// get minimum Alive point around ind2
								if(phi[min_ind] > 0.0)
								{
									for(int kkk = -1; kkk <= 1; kkk++)
									{
										long k3 = k2 + kkk;
										if(k3 < 0 || k3 >= sz2) continue;
										for(int jjj = -1; jjj <= 1; jjj++)
										{
											long j3 = j2 + jjj;
											if(j3 < 0 || j3 >= sz1) continue;
											for(int iii = -1; iii <= 1; iii++)
											{
												long i3 = i2 + iii;
												if(i3 < 0 || i3 >= sz0) continue;
												int offset2 = ABS(iii) + ABS(jjj) + ABS(kkk);
												if(offset2 == 0 || offset2 > cnn_type) continue;
												long ind3 = k3 * sz01 + j3 * sz0 + i3;
												if(state[ind3] == ALIVE && phi[ind3] < phi[min_ind]) min_ind = ind3;
											}
										}
									}
								}
								// over
								phi[ind2] = phi[min_ind] + inimg1d[ind2];
								state[ind2] = TRIAL;
								parent[ind2] = min_ind;
								HeapElem * elem = new HeapElem(ind2, phi[ind2]);
								heap.insert(elem);
								elems[ind2] = elem;
								bdr_count++;
							}
						}
					}
				}
			}
		}
	}

	cout<<"bkg_count = "<<bkg_count<<" ("<<bkg_count/(double)tol_sz<<")"<<endl;
	cout<<"bdr_count = "<<bdr_count<<" ("<<bdr_count/(double)tol_sz<<")"<<endl;
	cout<<"elems.size() = "<<elems.size()<<endl;
	// loop
	int time_counter = bkg_count;
	double process1 = 0;
	while(!heap.empty())
	{
		double process2 = (time_counter++)*100000.0/tol_sz;
		if(process2 - process1 >= 10) {cout<<"\r"<<((int)process2)/1000.0<<"%";cout.flush(); process1 = process2;
			//SAVE_PHI_IMAGE(phi, sz0, sz1, sz2, string("phi") + num2str((int)process1) + ".tif");
		}

		HeapElem* min_elem = heap.delete_min();
		elems.erase(min_elem->img_ind);

		long min_ind = min_elem->img_ind;
		delete min_elem;

		state[min_ind] = ALIVE;
		int i = min_ind % sz0;
		int j = (min_ind/sz0) % sz1;
		int k = (min_ind/sz01) % sz2;

		int w, h, d;
		for(int kk = -1; kk <= 1; kk++)
		{
			d = k+kk;
			if(d < 0 || d >= sz2) continue;
			for(int jj = -1; jj <= 1; jj++)
			{
				h = j+jj;
				if(h < 0 || h >= sz1) continue;
				for(int ii = -1; ii <= 1; ii++)
				{
					w = i+ii;
					if(w < 0 || w >= sz0) continue;
					int offset = ABS(ii) + ABS(jj) + ABS(kk);
					if(offset == 0 || offset > cnn_type) continue;
					long index = d*sz01 + h*sz0 + w;

					if(state[index] != ALIVE)
					{
						float new_dist = phi[min_ind] + inimg1d[index] * sqrt(double(offset));

						if(state[index] == FAR)
						{
							phi[index] = new_dist;
							HeapElem * elem = new HeapElem(index, phi[index]);
							heap.insert(elem);
							elems[index] = elem;
							state[index] = TRIAL;
							parent[index] = min_ind;
						}
						else if(state[index] == TRIAL)
						{
							if(phi[index] > new_dist)
							{
								phi[index] = new_dist;
								HeapElem * elem = elems[index];
								heap.adjust(elem->heap_id, phi[index]);
								parent[index] = min_ind;
							}
						}
					}
				}
			}
		}
	}

	map<long, MyMarker*> marker_map;
	for(long ind = 0; ind < tol_sz; ind++)
	{
		if(inimg1d[ind] < bkg_thresh || parent[ind] == ind) continue;
		int i = ind % sz0;
		int j = ind / sz0 % sz1;
		int k = ind / sz01;
		MyMarker * marker = new MyMarker(i, j, k);
		outtree.push_back(marker);
		marker_map[ind] = marker;
	}
	for(int m = 0; m < outtree.size(); m++)
	{
		MyMarker * child_marker = outtree[m];
		MyMarker * parent_marker = marker_map[parent[child_marker->ind(sz0, sz01)]];
		child_marker->parent = parent_marker;
	}

	assert(elems.empty());
	if(phi){delete [] phi; phi = 0;}
	if(parent){delete [] parent; parent = 0;}
	if(state) {delete [] state; state = 0;}
	return true;
}

template<class T> bool fastmarching_dt_tree_XY(T * inimg1d, vector<MyMarker*> & outtree, int sz0, int sz1, int sz2, int cnn_type = 3, int bkg_thresh = 0)
{
	enum{ALIVE = -1, TRIAL = 0, FAR = 1};

	long tol_sz = sz0 * sz1 * sz2;
	long sz01 = sz0 * sz1;
	//int cnn_type = 3;  // ?

	float * phi = new float[tol_sz];
	long * parent = new long[tol_sz]; for(long ind = 0; ind < tol_sz; ind++) parent[ind] = ind;
	char * state = new char[tol_sz];
	int bkg_count = 0;                          // for process counting
	int bdr_count = 0;                          // for process counting
	for(long i = 0; i < tol_sz; i++)
	{
		if(inimg1d[i] <= bkg_thresh)
		{
			phi[i] = inimg1d[i];
			state[i] = ALIVE;
			//cout<<"+";cout.flush();
			bkg_count++;
		}
		else
		{
			phi[i] = INF;
			state[i] = FAR;
		}
	}
	cout<<endl;

	BasicHeap<HeapElem> heap;
	map<long, HeapElem*> elems;

	// init heap
	{
		long i = -1, j = -1, k = -1;
		for(long ind = 0; ind < tol_sz; ind++)
		{
			i++; if(i%sz0 == 0){i=0; j++; if(j%sz1==0){j=0; k++;}}
			if(state[ind] == ALIVE)
			{
				for(int kk = 0; kk <= 0; kk++)
				{
					long k2 = k+kk;
					if(k2 < 0 || k2 >= sz2) continue;
					for(int jj = -1; jj <= 1; jj++)
					{
						long j2 = j+jj;
						if(j2 < 0 || j2 >= sz1) continue;
						for(int ii = -1; ii <=1; ii++)
						{
							long i2 = i+ii;
							if(i2 < 0 || i2 >= sz0) continue;
							int offset = ABS(ii) + ABS(jj) + ABS(kk);
							if(offset == 0 || offset > cnn_type) continue;
							long ind2 = k2 * sz01 + j2 * sz0 + i2;
							if(state[ind2] == FAR)
							{
								long min_ind = ind;
								// get minimum Alive point around ind2
								if(phi[min_ind] > 0.0)
								{
									for(int kkk = 0; kkk <= 0; kkk++)
									{
										long k3 = k2 + kkk;
										if(k3 < 0 || k3 >= sz2) continue;
										for(int jjj = -1; jjj <= 1; jjj++)
										{
											long j3 = j2 + jjj;
											if(j3 < 0 || j3 >= sz1) continue;
											for(int iii = -1; iii <= 1; iii++)
											{
												long i3 = i2 + iii;
												if(i3 < 0 || i3 >= sz0) continue;
												int offset2 = ABS(iii) + ABS(jjj) + ABS(kkk);
												if(offset2 == 0 || offset2 > cnn_type) continue;
												long ind3 = k3 * sz01 + j3 * sz0 + i3;
												if(state[ind3] == ALIVE && phi[ind3] < phi[min_ind]) min_ind = ind3;
											}
										}
									}
								}
								// over
								phi[ind2] = phi[min_ind] + inimg1d[ind2];
								state[ind2] = TRIAL;
								parent[ind2] = min_ind;
								HeapElem * elem = new HeapElem(ind2, phi[ind2]);
								heap.insert(elem);
								elems[ind2] = elem;
								bdr_count++;
							}
						}
					}
				}
			}
		}
	}

	cout<<"bkg_count = "<<bkg_count<<" ("<<bkg_count/(double)tol_sz<<")"<<endl;
	cout<<"bdr_count = "<<bdr_count<<" ("<<bdr_count/(double)tol_sz<<")"<<endl;
	cout<<"elems.size() = "<<elems.size()<<endl;
	// loop
	int time_counter = bkg_count;
	double process1 = 0;
	while(!heap.empty())
	{
		double process2 = (time_counter++)*100000.0/tol_sz;
		if(process2 - process1 >= 10) {cout<<"\r"<<((int)process2)/1000.0<<"%";cout.flush(); process1 = process2;
			//SAVE_PHI_IMAGE(phi, sz0, sz1, sz2, string("phi") + num2str((int)process1) + ".tif");
		}

		HeapElem* min_elem = heap.delete_min();
		elems.erase(min_elem->img_ind);

		long min_ind = min_elem->img_ind;
		delete min_elem;

		state[min_ind] = ALIVE;
		int i = min_ind % sz0;
		int j = (min_ind/sz0) % sz1;
		int k = (min_ind/sz01) % sz2;

		int w, h, d;
		for(int kk = 0; kk <= 0; kk++)
		{
			d = k+kk;
			if(d < 0 || d >= sz2) continue;
			for(int jj = -1; jj <= 1; jj++)
			{
				h = j+jj;
				if(h < 0 || h >= sz1) continue;
				for(int ii = -1; ii <= 1; ii++)
				{
					w = i+ii;
					if(w < 0 || w >= sz0) continue;
					int offset = ABS(ii) + ABS(jj) + ABS(kk);
					if(offset == 0 || offset > cnn_type) continue;
					long index = d*sz01 + h*sz0 + w;

					if(state[index] != ALIVE)
					{
						float new_dist = phi[min_ind] + inimg1d[index] * sqrt(double(offset));

						if(state[index] == FAR)
						{
							phi[index] = new_dist;
							HeapElem * elem = new HeapElem(index, phi[index]);
							heap.insert(elem);
							elems[index] = elem;
							state[index] = TRIAL;
							parent[index] = min_ind;
						}
						else if(state[index] == TRIAL)
						{
							if(phi[index] > new_dist)
							{
								phi[index] = new_dist;
								HeapElem * elem = elems[index];
								heap.adjust(elem->heap_id, phi[index]);
								parent[index] = min_ind;
							}
						}
					}
				}
			}
		}
	}

	map<long, MyMarker*> marker_map;
	for(long ind = 0; ind < tol_sz; ind++)
	{
		if(inimg1d[ind] < bkg_thresh || parent[ind] == ind) continue;
		int i = ind % sz0;
		int j = ind / sz0 % sz1;
		int k = ind / sz01;
		MyMarker * marker = new MyMarker(i, j, k);
		outtree.push_back(marker);
		marker_map[ind] = marker;
	}
	for(int m = 0; m < outtree.size(); m++)
	{
		MyMarker * child_marker = outtree[m];
		MyMarker * parent_marker = marker_map[parent[child_marker->ind(sz0, sz01)]];
		child_marker->parent = parent_marker;
	}

	assert(elems.empty());
	if(phi){delete [] phi; phi = 0;}
	if(parent){delete [] parent; parent = 0;}
	if(state) {delete [] state; state = 0;}
	return true;
}
#endif
