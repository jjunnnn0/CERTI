#define MAXL	5
#define MAXF	10
#define MAX_USER 10000

#include <stdio.h>


int g_user;

typedef struct _user{
    int id;
    struct _user* next;
}USER;


USER head[MAX_USER+1];
USER userPool[1000001];
int userPoolCnt;

int friendCnt[MAX_USER+1];


void init(int N)
{
    g_user = N;

    for (int i = 0 ; i<= g_user ; i++){
        head[i].next = 0;
    }
    userPoolCnt = 0;
}

inline void addfriend(int id1, int id2){

    USER* newUser = &userPool[userPoolCnt++];
    newUser->id = id2;

    newUser->next = head[id1].next;
    head[id1].next = newUser;
}


void add(int id, int F, int ids[MAXF])
{
    for(int i =0 ; i<F; i++){
        addfriend(id, ids[i]);
        addfriend(ids[i], id);
    }

}

inline void delfriend(int id1, int id2){

    for(USER* cur = &head[id1]; cur; cur = cur->next){

        if(cur->next && cur->next->id == id2){
            cur->next = cur->next->next;
            break;
        }
    }
}

void del(int id1, int id2)
{   
    delfriend(id1, id2);
    delfriend(id2, id1);
}

int recommend(int id, int list[MAXL])
{
    for (int i = 1; i<=g_user; i++){
        friendCnt[i] = 0;
    }

    int f_id;
    for (USER* cur = head[id].next; cur; cur=  cur->next){
        f_id = cur->id;

        for(USER* cur2 = head[f_id].next ; cur2; cur2 = cur2->next){
            friendCnt[cur2->id]++;
        }
    }

    for (USER* cur = head[id].next; cur; cur=  cur->next){
        friendCnt[cur->id] = 0;
    }
    friendCnt[id] = 0;
    
    int cnt = 0 ;

    for(;cnt<5; cnt++){

        int max_value = 0 ;
        int max_value_id = 0;

        for(int i= 1; i<=g_user; i++){

            if(max_value < friendCnt[i]){
                max_value = friendCnt[i];
                max_value_id = i;
            }
        }

        if(max_value == 0) break;;

        list[cnt] = max_value_id;
        friendCnt[max_value_id] = 0;
    }

    return cnt;
}


    // int res = 0;
    // int p = 0;

    // list[0] = list[1] = list[2] = list[3] = list[4] = 0;

    // for ( int i = 1; i <= g_user; ++i)
    //     if (friendCnt[list[p]] < friendCnt[i]) {
    //         list[p] = i;
    //         res++;
    //         while (p > 0 && friendCnt[list[p - 1]] < friendCnt[list[p]]) {
    //             int tmp = list[p - 1];
    //             list[p - 1] = list[p];
    //             list[p--] = tmp;
    //         }
    //         p = res > 4 ? 4 : res;
     
    //     }

    // return res > 4 ? 5 : res;


    // heapSize = 0;
    // for (int i = 1; i<=g_user; i++){

    //     if(friendCnt[i] != 0){

    //         NODE* newNode = &nodePool[nodePoolCnt++];
    //         newNode->id = i;
    //         newNode->co_friend = friendCnt[i];
    //         heapPush(newNode);
    //     }
    // }

    // int cnt = 0;

    // if(heapSize == 0){
    //     return 0;
    // }
    // else {
    //     while(heapSize > 0 && cnt<5){

    //         NODE value;
    //         NODE* pvalue = &value;
    //         heapPop(&pvalue);
    //         list[cnt] = pvalue->id;
    //         cnt++;

    //     }

    //     return cnt;
  
    // }
	// return 0;


// #define NULL 0
// #define MOD 31
// struct NODE {
//     int n;
//     NODE *next;
// };
     
// int cnt[10001], nn, n;
// NODE head[10001][MOD+1], node[1000001];
     
// void init(int N)
// {
//     n = N;
//     nn = 0;
//     for ( int i = 0; i <= n; ++i)
//         for ( int j = 0; j < MOD+1; ++j)
//             head[i][j].next = NULL;
// }
     
// inline void insert(int a, int b) {
//     node[nn].n = b;
//     node[nn].next = head[a][b&MOD].next;
//     head[a][b&MOD].next = &node[nn++];
// }
     
// inline void _del(int a, int b) {
//      NODE *cur = &head[a][b&MOD];
//     while (cur->next != NULL) {
//         if (cur->next->n == b) {
//             cur->next = cur->next->next;
//             return;
//         }
//         cur = cur->next;
//     }
// }
     
// void add(int id, int F, int ids[10])
// {
//     for ( int i = 0; i < F; ++i) {
//         insert(id, ids[i]);
//         insert(ids[i], id);
//     }
// }
     
// void del(int id1, int id2)
// {
//     _del(id1, id2), _del(id2, id1);
// }
     
// int recommend(int id, int list[5])
// {
//      int res = 0;
//     int p = 0;
//     for ( int i = 1; i <= n; ++i) cnt[i] = 0;
//     cnt[id] = -1111111;
     
//     for ( int i = 0; i < MOD+1; ++i) {
//          NODE *cur = head[id][i].next;
//         while (cur != NULL) {
//             for ( int j = 0; j < MOD+1; ++j) {
//                  NODE *cur2 = head[cur->n][j].next;
//                 cnt[cur->n] = -1111111;
//                 while (cur2 != NULL) {
//                     cnt[cur2->n]++;
//                     cur2 = cur2->next;
//                 }
//             }
//             cur = cur->next;
//         }
//     }
//     list[0] = list[1] = list[2] = list[3] = list[4] = 0;
//     for ( int i = 1; i <= n; ++i)
//         if (cnt[list[p]] < cnt[i]) {
//             list[p] = i;
//             res++;
//             while (p > 0 && cnt[list[p - 1]] < cnt[list[p]]) {
//                 int tmp = list[p - 1];
//                 list[p - 1] = list[p];
//                 list[p--] = tmp;
//             }
//             p = res > 4 ? 4 : res;
     
//         }
//     return res > 4 ? 5 : res;
// }