/**@file tz_<1t>_arraylist.h
 * @brief arraylist of <2T>
 * @author Ting Zhao
 * @date 15-Feb-2008
 */

/* <1T> prefix <2T> type <3T> element class (object or basic) */

#ifndef _TZ_<1T>_ARRAYLIST_H_
#define _TZ_<1T>_ARRAYLIST_H_

#include "tz_cdefs.h"
<2T=Neuron_Component>
#include "tz_neuron_component.h"
</t>
<2T=Swc_Node>
#include "tz_swc_cell.h"
</t>
<2T=Stack_Tile>
#include "tz_stack_tile.h"
</t>
<2T=Stack_Tile_I>
#include "tz_stack_tile_i.h"
</t>
<2T=unipointer_t>
#include "tz_unipointer.h"
</t>

__BEGIN_DECLS

/**@brief <1T> array list
 **/
typedef struct _<1T>_Arraylist {
  int length;
  int capacity;
  <2T> *array;
} <1T>_Arraylist;

/*
 * Print_<1T>_Arraylist() prints the arraylist <a> to the screen.
 * <3T=basic><format> is the format of printing</T>
 */
void Print_<1T>_Arraylist(const <1T>_Arraylist *a<3T=basic>, const char *format</T>);

<1T>_Arraylist* New_<1T>_Arraylist();

/*
 * Note: These functions are obsolete. Make_<1T>_Arraylist() and 
 * Kill_<1T>_Arraylist() are preferred.
 */
<1T>_Arraylist* <1T>_Arraylist_New(int length, int margin);
void <1T>_Arraylist_Free(<1T>_Arraylist *a);

/*
 * Make_<1T>_Arraylist() returns a new array list with <length> elements. The
 * capacity of the array will be <length> + <margin>, where <margin> should be
 * non-negative. If <length> or <margin> is less than 0, it returns NULL.
 *
 * Kill_<1T>_Arraylist() kills the array list <a>. It frees all associated 
 * memory.
 */
<1T>_Arraylist* Make_<1T>_Arraylist(int length, int margin);
void Kill_<1T>_Arraylist(<1T>_Arraylist *a);

<1T>_Arraylist* <1T>_Arraylist_Wrap(<2T> *array, int n);

/**@brief Retrieve an element.
 */
<2T> <1T>_Arraylist_At(const <1T>_Arraylist *a, int index);

/**@brief Retrieve the last element.
 */
<2T> <1T>_Arraylist_Last(const <1T>_Arraylist *a);

/**@brief Retrieve the reference of an element.
 */
<2T>* <1T>_Arraylist_At_R(const <1T>_Arraylist *a, int index);

/**@brief Add an element.
 *
 * <1T>_Arraylist_Add() adds <item> to the end of the arraylist <a>.
 */
void <1T>_Arraylist_Add(<1T>_Arraylist *a, <2T> item);

/**@brief Insert an element.
 *
 * <1T>_Arraylist_Insert() inserts <item> to <a> at the position <index>. If the
 * <index> exceeds the current range, the array will be grown. But the new
 * elements will not be initialized except the one at <index>, which is the last
 * element.
 */
void <1T>_Arraylist_Insert(<1T>_Arraylist *a, int index, <2T> item);

/**@brief Remove an element.
 *
 * <1T>_Arraylist_Take() removes the element at <index> and returns its value.
 */
<2T> <1T>_Arraylist_Take(<1T>_Arraylist *a, int index);

/**@brief Remove an element.
 *
 * <1T>_Arraylist_Take_Last() removes the last element and returns its value.
 */
<2T> <1T>_Arraylist_Take_Last(<1T>_Arraylist *a);

/**@brief Set the length of the arraylist.
 *
 * <1T>_Arraylist_Set_Length() sets the length of <a>. It does not change the 
 * capacity if <length> does not exceed the capacity. 
 */
void <1T>_Arraylist_Set_Length(<1T>_Arraylist *a, int length);

/**@brief Set an element.
 *
 * <1T>_Aarraylist_Set() sets the element with <index> to <item>. If the array 
 * is grown, the new elements will be set to 0 if possible except the one at 
 * <index>.
 */
void <1T>_Arraylist_Set(<1T>_Arraylist *a, int index, <2T> item);

/**@brief Swap elements.
 *
 * <1T>_Arraylist_Swap() swaps two elements at the positions [idx1] and [idx2].
 * Nothing will be done when either of the positions is not valid.
 */
void <1T>_Arraylist_Swap(<1T>_Arraylist *a, int idx1, int idx2);

<3T=basic>

/* Routines for min-heap implementation 
 *
 * parent <= child
 *
 * The children of the node with index k are the nodes with indices 2k (left) 
 * and 2k+1 (right). Therefore the parent of the node is k/2. The root is the 
 * node with index 1. The node with index 0 is not used.
 */

/**@brief Depth of the heap.
 * 
 * <1T>_Heap_Depth() returns the depth of the heap <h>.
 */
int <1T>_Heap_Depth(const <1T>_Arraylist *h);

/**@brief Node query.
 *
 * <1T>_Heap_Node_At() returns a node of the heap at depth <depth>. <rank> is
 * the order of the node at its depth and it's 0-indexed. 
 */
<2T> <1T>_Heap_Node_At(const <1T>_Arraylist *h, int depth, int rank);

/**@brief Print a heap.
 *
 * Print_<1T>_Heap() prints a heap to a screen. <3T=basic>format is the format
 * to print each element.</T>
 */
void Print_<1T>_Heap(const <1T>_Arraylist *h<3T=basic>, const char *format</T>);

/**@brief Parent of a node.
 *
 * <1T>_Heap_Parent() returns the index of parent of the node with index 
 * <child_index> in the heap <h>. It returns 0 when the child node is not in 
 * <h> or it is the root.
 */
int <1T>_Heap_Parent(const <1T>_Arraylist *h, int child_index);

/**@brief Left child of a node
 *
 * <1T>_Heap_Leftchild() returns the index of left child of the node with index 
 * <parent_index> in the heap <h>. It returns 0 when the parent node is not in 
 * <h> or it is a leaf.
 */
int <1T>_Heap_Leftchild(const <1T>_Arraylist *h, int parent_index);

/**@brief Smaller child of a node
 *
 * <1T>_Heap_Small_Child() returns the index of the smaller child of the node 
 * with index  <parent_index> in the heap <h>. It returns 0 when the parent 
 * node is not in <h> or it is a leaf.
 */
int <1T>_Heap_Small_Child(const <1T>_Arraylist *h, int parent_index);

/**@brief Add a node.
 *
 * <1T>_Heap_Add() adds a node to the end of the heap. The nodes will be 
 * adjusted automatically so that <h> is still a heap.
 */
void <1T>_Heap_Add(<1T>_Arraylist *h, <2T> item);

/*@brief Remove minimal.
 *
 * <1T>_Heap_Remove() removes the root node in the heap <h> and returns its
 * value. The remained nodes will be adjusted automatically so that <h> is 
 * still a heap.
 */
<2T> <1T>_Heap_Remove(<1T>_Arraylist *h);

/**@brief Test is an arraylist is a heap
 *
 * Verify_<1T>_Heap() returns 0 if <h> is a heap. Otherwise it returns the index
 * where violation happens.
 */
int Verify_<1T>_Heap(<1T>_Arraylist *h);

<2T=int>

/* The routines with suffix _I are for associated arrays. The values in the 
 * heap are integers, but the structure of the heap is determined by a double
 * array. Briefly speaking, the comparison of the two nodes is not to compare
 * their values directly. Instead, the value of a node is used as an index
 * to access the double array. The values achieved from the double array are
 * actual values for comparison. 
 */

/**@brief Smaller child of a node.
 */
int <1T>_Heap_Small_Child_I(const <1T>_Arraylist *h, int parent_index, 
			    double *value);

/**@brief Add a node to a heap.
 *
 * <1T>_Heap_Add_I() inserts an index to a heap. The values are stored in 
 * <value>. <checked> is an array to store indices of the heap so that we can
 * obtain the position of a node in the heap quickly.
 */
void <1T>_Heap_Add_I(<1T>_Arraylist *h, <2T> item, double *value,
		     int *checked);

/**@brief Remove the root node from a heap.
 *
 * <1T>_Heap_Remove_I() removes the root from <h>. The corresponding element
 * in <checked> is set to 0.
 */
<2T> <1T>_Heap_Remove_I(<1T>_Arraylist *h, double *value, int *checked);

/**@brief Remove a specified node from a heap.
 *
 * <1T>_Heap_Remove_I_At() removes the node that has id <index>. The 
 * corresponding element in <checked> is set to 0.
 */
<2T> <1T>_Heap_Remove_I_At(<1T>_Arraylist *h, int index,
			   double *value, int *checked);

/**@brief Update a heap.
 *
 * <1T>_Heap_Update_I() updates <h> to respond to the change of node that has
 * value <item>, which means <value[item]> might be changed.
 */
void <1T>_Heap_Update_I(<1T>_Arraylist *h, <2T> item, double *value,
			int *checked);

void Print_<1T>_Heap_I(const <1T>_Arraylist *h, double *value);
int Verify_<1T>_Heap_I(const <1T>_Arraylist *h, double *value);
</t>

/*
 * <1T>_Heap_Sort() sorts an array using heap sort alogrithm.
 */
void <1T>_Heap_Sort(<1T>_Arraylist *a);

</T>

__END_DECLS

#endif
