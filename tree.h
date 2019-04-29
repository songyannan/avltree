#ifndef __TREE_H__
#define __TREE_H__
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
typedef struct {
    int key;
}__Spec_t;
typedef struct node{
    int Id;
    int count;
    __Spec_t spec;//数据描述结构体
    struct node *pl;
    struct node *pr;
    int    height;
}__Node_t,*__Tree_t;



#define   EQ(a,b)  ((a) == (b)) 
#define   LQ(a,b)  ((a) <  (b))
#define   RQ(a,b)  ((a) >  (b))

#define   TRAVEL_PRE          0
#define   TRAVEL_MID          1
#define   TRAVEL_POS          2

#define   GET_H(node)   ((node == NULL)? 0 : (((__Node_t *)(node))->height))
#define   MAX(a,b)      ((a)>(b)?(a):(b))

#define   LIMIT_H            2

#define   OPT_SUCC           0
#define   OPT_FAIL          -1


static __Node_t * get_min_node(__Tree_t T);
static __Node_t * get_max_node(__Tree_t T);

static __Tree_t LL_T(__Tree_t T);
static __Tree_t RR_T(__Tree_t T);
static __Tree_t LR_T(__Tree_t T);
static __Tree_t RL_T(__Tree_t T);

static __Node_t * Create_node();
static int copy_data(__Node_t *ps,__Node_t *pd);

static __Tree_t insert_node(__Tree_t T,__Node_t *node);
static __Tree_t delete_node(__Tree_t T,__Node_t *node);
static __Node_t * search_node(__Tree_t T,int Id);
static __Tree_t destroy_tree(__Tree_t T);



static void middle_print(__Tree_t T);
static void left_print(__Tree_t T);
static void right_print(__Tree_t T);

static int get_node_id(__Tree_t T,int *p,int *index);
static int get_node_count(__Tree_t T,int *count);

/******************************************************
 *
 *Function :API_LibInit
 *      init lib  source
 *      
 *Return:
 *      success reutrn macro OPT_SUCC 
 *      fail return macro OPT_FAIL
 * 
 * *****************************************************/
int API_LibInit(void);
/******************************************************
 *
 *Function :API_LibInit
 *      deinit lib  source
 *Para:
 *    root tree (__Tree_t)
 *Return: 
 *      success reutrn macro OPT_SUCC 
 *      fail return macro OPT_FAIL
 * *****************************************************/
int API_LibDeInit(__Tree_t *T);
/******************************************************
 *Function :API_CreateNode
 *      create struct __Node_t node
 *Para :
 *      __Node_t **p to recvive node address
 *
 *Return : 
 *      success reutrn macro OPT_SUCC 
 *      fail return macro OPT_FAIL
 * *****************************************************/
int API_CreateNode(__Node_t **p);
/******************************************************
 *Function :API_Insert
 *      insert node 
 *Para :
 *      __Tree_t *T to recv node
 *      __Node_t *p the node 
 *Return : 
 *      success reutrn macro OPT_SUCC 
 *      fail return macro OPT_FAIL
 * *****************************************************/
int API_Insert(__Tree_t *T,__Node_t *node);
/******************************************************
 *Function :API_Delete
 *      delete node 
 *Para :
 *      __Tree_t *T to delete node
 *      __Node_t *p the node 
 *Return : 
 *      success reutrn macro OPT_SUCC 
 *      fail return macro OPT_FAIL
 * *****************************************************/
int API_Delete(__Tree_t *T,__Node_t *node);
/******************************************************
 *Function :API_Search
 *      search node 
 *Para :
 *      __Tree_t *T to delete node
 *      int Id the key of node
 *      __Node_t *p the node 
 *Return : 
 *      success reutrn macro OPT_SUCC 
 *      fail return macro OPT_FAIL
 * *****************************************************/
int API_Search(__Tree_t *T,int Id,__Node_t **p);

/******************************************************
 *Function :API_GetHeight 
 *      get tree height
 *Para :
 *      __Tree_t *T 
 *      the value of tree
 *Return : 
 *      success reutrn macro OPT_SUCC 
 *      fail return macro OPT_FAIL
 * *****************************************************/
int API_GetHeight(__Tree_t *T,int *height);

/******************************************************
 *Function :API_GetHeight 
 *      get max node
 *Para :
 *      __Tree_t *T
 *      __Node_t *p -> the max node address
 *Return : 
 *      success reutrn macro OPT_SUCC 
 *      fail return macro OPT_FAIL
 * *****************************************************/
int API_GetMAX(__Tree_t *T,__Node_t *p);

/******************************************************
 *Function :API_GetHeight 
 *      get min node
 *Para :
 *      __Tree_t *T
 *      __Node_t *p -> the min node address
 *Return : 
 *      success reutrn macro OPT_SUCC 
 *      fail return macro OPT_FAIL
 * *****************************************************/
int API_GetMIN(__Tree_t *T,__Node_t *p);

/******************************************************
 *Function :API_GetNodeCount 
 *      get node count
 *Para :
 *      __Tree_t *T
 *      int *count -> the node count
 *Return : 
 *      success reutrn macro OPT_SUCC 
 *      fail return macro OPT_FAIL
 * *****************************************************/
int API_GetNodeCount(__Tree_t *T,int *count);

/******************************************************
 *Function :API_GetALLNodeId
 *      get all node id
 *Para :
 *      __Tree_t *T
 *      the Id buffer
 *      buffer len
 *Return : 
 *      success reutrn macro OPT_SUCC 
 *      fail return macro OPT_FAIL
 * *****************************************************/
int API_GetALLNodeId(__Tree_t *T,int *pbuf,int len);


int API_NodeTravel(__Tree_t *T,int travel_mode);


#endif
