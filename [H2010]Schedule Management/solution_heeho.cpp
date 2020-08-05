#define MAX_USER 1000
#define MAX_GROUP 100
#define MAX_EVENT_NAME 200
#define MAX_TABLE (MAX_EVENT_NAME*2 + 7)
#define MAX_EVENT 50000
#define MAX_NODE (MAX_EVENT*2)

#define ull unsigned long long

typedef struct event {
    int u_id;
    int g_id;
    ull e_name;
}Event;

typedef struct node {

    struct event* e;
    struct node* next;
    struct node* child;

}Node;

Event eventPool[MAX_EVENT];
int eventPoolCnt;

Node nodePool[MAX_NODE];
int nodePoolCnt;

Node eventOfUser[MAX_USER];
int cntEventOfUser[MAX_USER];
Node groupEvent[MAX_GROUP][MAX_TABLE];

inline ull str_to_ull(char* str) {

    ull ret = 0;

    for (register int i = 0; str[i]; ++i) {
        ret += (ret << 5) + str[i] - 'a' + 1;
    }

    return ret;
}

void init()
{
    eventPoolCnt = 0;
    nodePoolCnt = 0;

    for (register int i = 0; i < MAX_USER; ++i) {
        eventOfUser[i].next = 0;
        cntEventOfUser[i] = 0;
    }
    for (register int i = 0; i < MAX_GROUP; ++i) {
        for (register int j = 0; j < MAX_TABLE; ++j) {
            groupEvent[i][j].next = 0;
        }
    }

}

void addEvent(int uid, char ename[], int groupid)
{
    ull enameUll = str_to_ull(ename);

    Event* newEvent = &eventPool[eventPoolCnt++];
    newEvent->u_id = uid;
    newEvent->e_name = enameUll;
    newEvent->g_id = groupid;

    Node* newNode = &nodePool[nodePoolCnt++];
    newNode->e = newEvent;
    newNode->child = 0;
    newNode->next = eventOfUser[uid].next;

    eventOfUser[uid].next = newNode;
    cntEventOfUser[uid]++;

    newNode = &nodePool[nodePoolCnt++];
    newNode->e = newEvent;
    newNode->child = 0;
    newNode->next = 0;

    register Node* curr;
    int hash = enameUll % MAX_TABLE;

    for (curr = groupEvent[groupid][hash].next; curr; curr = curr->next) {

        if (curr->e->e_name == enameUll) {
            newNode->child = curr->child;
            curr->child = newNode;
            break;
        }

    }

    if (!curr) {
        newNode->next = groupEvent[groupid][hash].next;
        groupEvent[groupid][hash].next = newNode;
    }

}

int deleteEvent(int uid, char ename[])
{
    int ret = 0;

    ull enameUll = str_to_ull(ename);
    int g_id;

    register Node* curr;
    register Node* prev;

    prev = &eventOfUser[uid];
    for (curr = eventOfUser[uid].next; curr; curr = curr->next) {
        if (curr->e->e_name == enameUll) {
            g_id = curr->e->g_id;
            prev->next = curr->next;
            break;
        }
        prev = curr;
    }

    int hash = enameUll % MAX_TABLE;

    prev = &groupEvent[g_id][hash];
    for (curr = groupEvent[g_id][hash].next; curr; curr = curr->next) {

        if (curr->e->e_name == enameUll) {

            if (curr->e->u_id == uid) {

                prev->next = curr->next;

                for (register Node* node = curr; node; node = node->child) {
                    cntEventOfUser[node->e->u_id]--;
                    ret++;
                }
                break;
            }

            else {

                prev = curr;
                for (curr = prev->child; curr; curr = curr->child) {

                    if (curr->e->u_id == uid) {
                        prev->child = curr->child;
                        ret = 1;
                        cntEventOfUser[uid]--;
                        break;
                    }

                    prev = curr;
                }

            }

            break;
        }

        prev = curr;
    }

    return ret;
}

int changeEvent(int uid, char ename[], char cname[])
{

    ull e_name = str_to_ull(ename);
    ull c_name = str_to_ull(cname);

    int g_id;

    register Node* curr;
    register Node* prev;


    for (curr = eventOfUser[uid].next; curr; curr = curr->next) {
        if (curr->e->e_name == e_name) {
            g_id = curr->e->g_id;
            break;
        }
    }

    int hash = e_name % MAX_TABLE;
    int hash_c = c_name % MAX_TABLE;

    int ret = 0;

    prev = &groupEvent[g_id][hash];
    for (curr = groupEvent[g_id][hash].next; curr; curr = curr->next) {
        if (curr->e->e_name == e_name) {
            if (curr->e->u_id == uid) {

                prev->next = curr->next;

                for (register Node* node = curr; node; node = node->child) {
                    node->e->e_name = c_name;
                    ret++;
                }

                curr->next = groupEvent[g_id][hash_c].next;
                groupEvent[g_id][hash_c].next = curr;

                break;
            }

            else {
                prev = curr;
                for (curr = prev->child; curr; curr = curr->child) {
                    if (curr->e->u_id == uid) {

                        prev->child = curr->child;

                        curr->e->e_name = c_name;
                        ret = 1;

                        curr->next = groupEvent[g_id][hash_c].next;
                        groupEvent[g_id][hash_c].next = curr;

                        break;
                    }
                    prev = curr;
                }
            }

            break;
        }
        prev = curr;
    }

    return ret;
}

int getCount(int uid)
{
    return cntEventOfUser[uid];
}


