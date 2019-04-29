#include "tree.h"

static sem_t g_sem;
static __Node_t * get_min_node(__Tree_t T){
    __Node_t *p = T;
    if(NULL == T){
        return NULL;
    }
    while(NULL != p->pl){
        p = p->pl;
    }
    return p; 
}
static __Node_t * get_max_node(__Tree_t T){
    __Node_t *p = T;
    if(NULL == T){
        return NULL;
    }
    while(NULL != p->pr){
        p = p->pr;
    }
    return p;
}

static __Tree_t LL_T(__Tree_t T){
    __Node_t *p = T->pl;                                                                                                                      
    T->pl = p->pr;
    p->pr = T;

    T->height = MAX(GET_H(T->pl),GET_H(T->pr)) + 1;
    p->height = MAX(GET_H(p->pl),GET_H(T)) + 1;
    return p;

}
static __Tree_t RR_T(__Tree_t T){
    __Node_t *p = T->pr;
    T->pr = p->pl;
    p->pl = T;

    T->height = MAX(GET_H(T->pl),GET_H(T->pr)) + 1;
    p->height = MAX(GET_H(p->pl),GET_H(T)) + 1;
    return p;

}

static __Tree_t LR_T(__Tree_t T){ 
    T->pl = RR_T(T->pl);
    return LL_T(T);
}
static __Tree_t RL_T(__Tree_t T){                                                                                                             
    T->pr = LL_T(T->pr);
    return RR_T(T);
}

static __Node_t * Create_node(){
    __Node_t *p = (__Node_t *)malloc(sizeof(__Node_t));
    if(NULL == p){
        return NULL;
    }
    memset(p,0,sizeof(__Node_t)); 
    return p;
}
static int copy_data(__Node_t *pd,__Node_t *ps){
    if((NULL == ps) || (NULL == pd)){
        return OPT_FAIL;
    }
    pd->Id   = ps->Id;
    pd->spec = ps->spec;
    return OPT_SUCC;
}
static __Tree_t insert_node(__Tree_t T,__Node_t *node){
    if(NULL == node){
        return NULL;
    }
    if(NULL == T){
        T = node;
    }else if(EQ(node->Id,T->Id)){
        //update data spec 
        copy_data(T,node);
        free(node);
    }else if(LQ(node->Id,T->Id)){
        T->pl = insert_node(T->pl,node);
        if(LIMIT_H == (GET_H(T->pl) - GET_H(T->pr))){
            if(LQ(node->Id,T->pl->Id)){
                T = LL_T(T);
            }else{
                T = LR_T(T);
            }
        }
    }else{
        T->pr = insert_node(T->pr,node);
        if(LIMIT_H == (GET_H(T->pr) - GET_H(T->pl))){
            if(RQ(node->Id,T->pr->Id)){
                T = RR_T(T);
            }else{
                T = RL_T(T);
            }
        }
    }
    T->height = MAX(GET_H(T->pl),GET_H(T->pr)) + 1;
    return T;
}

static __Tree_t delete_node(__Tree_t T,__Node_t *node){
    if((NULL == node) || (NULL == T)){
        return T;
    }
    if(EQ(node->Id,T->Id)){
        if((NULL != T->pl)&&(NULL != T->pr)){
            __Node_t *p = NULL;
            if(RQ(GET_H(T->pl),GET_H(T->pr))){
                p = get_max_node(T->pl);
                copy_data(node,p);
                node->pl = delete_node(T->pl,p);
            }else{
                p = get_min_node(T->pr);
                copy_data(node,p);
                node->pr = delete_node(T->pr,p);
            }
        }else{
            __Node_t *pt = T;
            T = T->pl?T->pl:T->pr;
            free(pt);
        }
    }else if(LQ(node->Id,T->Id)){
        T->pl = delete_node(T->pl,node);
        if(LIMIT_H == (GET_H(T->pr) - GET_H(T->pl))){
            __Node_t *r = T->pr;
            if(RQ(GET_H(r->pl),GET_H(r->pr))){
                T = RL_T(T);
            }else{
                T = RR_T(T);
            }
        }
    }else{
        T->pr = delete_node(T->pr,node);
        if(LIMIT_H == (GET_H(T->pl) - GET_H(T->pr))){
            __Node_t *l = T->pl;
            if(RQ(GET_H(l->pr),GET_H(l->pl))){
                T = LR_T(T);
            }else{
                T = LL_T(T);
            }
        }
    }
    if(NULL != T){
        T->height = MAX(GET_H(T->pl),GET_H(T->pr)) + 1;
    }
    return T;
}

static __Node_t * search_node(__Tree_t T,int Id){
    __Node_t *p = T;
    if((NULL == p) || (EQ(Id,p->Id))){
        return p;
    }else if(LQ(Id,p->Id)){
         p = search_node(p->pl,Id);
    }else{
         p = search_node(p->pr,Id);
    }
    return p;
}
static __Tree_t destroy_tree(__Tree_t T){
    if(NULL == T){
        return NULL;
    }
    if(NULL != T->pl){
        destroy_tree(T->pl);
    }
    if(NULL != T->pl){
        destroy_tree(T->pr);
    }
    
    free(T);
    return T;
}
static void middle_print(__Tree_t T){
    if(NULL == T){
        return;
    }
    middle_print(T->pl);
    printf("Cur data is : %d\n",T->Id); 
    middle_print(T->pr);
}

static void left_print(__Tree_t T){
    if(NULL == T){
        return;
    }
    printf("Cur data is : %d\n",T->Id); 
    left_print(T->pl);
    left_print(T->pr);
}

static void right_print(__Tree_t T){
    if(NULL == T){
        return;
    }
    right_print(T->pl);
    right_print(T->pr);
    printf("Cur data is : %d\n",T->Id); 
}
static int get_node_id(__Tree_t T,int *pbuf,int *index){
    if(NULL == T){
        return 0;
    }
    get_node_id(T->pl,pbuf,index);
    pbuf[*index] = T->Id;
    (*index)++;
    get_node_id(T->pr,pbuf,index);
    return *index;
}

static int get_node_count(__Tree_t T,int *count){
    if(NULL == T){
        return 0;
    }
    (*count)++;
    get_node_count(T->pl,count);
    get_node_count(T->pr,count);
    return *count;
}
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
int API_LibInit(){
    if(sem_init(&g_sem, 0, 1)){
        perror("sem init fail");
        return OPT_FAIL;
    } 
    return OPT_SUCC;
}
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
int API_LibDeInit(__Tree_t *T){
    sem_wait(&g_sem);
    destroy_tree(*T);
    sem_post(&g_sem);
    if(sem_destroy(&g_sem)){
        perror("sem destroy fail");
        return OPT_FAIL;
    }
    return OPT_SUCC;
}
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
int API_CreateNode(__Node_t **p){
    sem_wait(&g_sem);
    *p = Create_node();
    sem_post(&g_sem);
    if(NULL == p){
        return OPT_FAIL;
    }
    return OPT_SUCC;
}
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
int API_Insert(__Tree_t *T,__Node_t *p){
    if(NULL == p){
        return OPT_FAIL;
    }
    sem_wait(&g_sem);
    *T = insert_node(*T,p);
    sem_post(&g_sem);
    if(NULL == *T){
        return OPT_FAIL;
    }
    API_GetNodeCount(T,&((*T)->count));
    return OPT_SUCC;
}
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
int API_Delete(__Tree_t *T,__Node_t *p){
    if(NULL == p){
        return OPT_FAIL;
    }
    sem_wait(&g_sem);
    *T = delete_node(*T,p);
    sem_post(&g_sem);
    API_GetNodeCount(T,&((*T)->count));
    return OPT_SUCC;
}
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
int API_Search(__Tree_t *T,int Id,__Node_t **p){
    sem_wait(&g_sem);
    *p = search_node(*T,Id);
    sem_post(&g_sem);
    if(NULL == *p){
        printf("Can't find the Id = %d\n",Id);
        return OPT_FAIL;
    }
    return OPT_SUCC;
}
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
int API_GetHeight(__Tree_t *T,int *height){
    if(NULL == *T){
        return OPT_FAIL;
    }
    *height = GET_H(*T);
    return OPT_FAIL;
}
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
int API_GetMAX(__Tree_t *T,__Node_t *p){
    sem_wait(&g_sem);
    p = get_min_node(*T);
    sem_post(&g_sem);
    if(NULL == p){
        return OPT_FAIL;
    }  
    return OPT_SUCC;
}
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
int API_GetMIN(__Tree_t *T,__Node_t *p){
    sem_wait(&g_sem);
    p = get_min_node(*T);
    sem_post(&g_sem);
    if(NULL == p){
        return OPT_FAIL;
    }  
    return OPT_SUCC;
}
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
int API_GetNodeCount(__Tree_t *T,int *count){
    if(NULL == T){
        printf("The tree is null");
        return OPT_FAIL;
    }
    *count = 0;
    sem_wait(&g_sem);
    get_node_count(*T,count);
    sem_post(&g_sem);
    return OPT_SUCC;
}
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
int API_GetALLNodeId(__Tree_t *T,int *pbuf,int len){
    int index = 0;
    if(NULL == pbuf){
        printf("The para buffer is null \n");
        return OPT_FAIL;
    }
    if(len < (*T)->count){
        printf("It's too small too copy data to buffer\n");
        return OPT_FAIL;
    }
    sem_wait(&g_sem);
    index = get_node_id(*T,pbuf,&index);
    sem_post(&g_sem);
    return OPT_SUCC;
}
/******************************************************
 *Function :API_NodeTravel
 *      print all node
 *Para :
 *      __Tree_t *T
 *      int trave_mode trave mode 
 *Return : 
 *      success reutrn macro OPT_SUCC 
 *      fail return macro OPT_FAIL
 * *****************************************************/
int API_NodeTravel(__Tree_t *T,int trave_mode){
    switch (trave_mode){
        case TRAVEL_PRE:
            left_print(*T);
            break;
        case TRAVEL_MID:
            middle_print(*T);
            break;
        case TRAVEL_POS:
            right_print(*T);
            break;
        default:
            return OPT_FAIL;
            break;
    }
    return OPT_SUCC;
}
