#define MAX_USER 1000
#define MAX_POST 100000
#define register  

typedef struct _user{
    int uid;
    int f_cnt;
    int follow[MAX_USER];
}User;

User user[MAX_USER+1];

typedef struct _post{
    int pid;
    int like; 
    int timestamp;
    struct _post* next;
}Post;

Post post[MAX_POST+1];
Post userPost[MAX_USER+1];

Post* heap[MAX_POST];
int heapSize;

void init(int N)
{
    for(register int i =1; i<= MAX_USER; i++)
    {
        user[i].uid = i;
        user[i].f_cnt = 0;
        userPost[i].next = 0;
    }
    heapSize = 0;
}

int compare (Post* p1, Post* p2){

    if (p1->like > p2->like){
        return 1; 
    }else if(p1->like == p2->like && p1->timestamp > p2->timestamp){
        return 1; 
    }
    return 0;
}

int heapPush(Post* value)
{
	heap[heapSize] = value;

	int current = heapSize;
	while (current > 0 && compare(heap[current], heap[(current - 1) / 2]))
	{
		Post* temp = heap[(current - 1) / 2];
		heap[(current - 1) / 2] = heap[current];
		heap[current] = temp;
		current = (current - 1) / 2;
	}

	heapSize = heapSize + 1;
	return 1;
}

int heapPop(Post** value)
{
	*value = heap[0];
	heapSize = heapSize - 1;

	heap[0] = heap[heapSize];

	int current = 0;
	while (current * 2 + 1 < heapSize)
	{
		int child;
		if (current * 2 + 2 == heapSize)
		{
			child = current * 2 + 1;
		}
		else
		{
			child = compare(heap[current * 2 + 1], heap[current * 2 + 2]) ? current * 2 + 1 : current * 2 + 2;
		}

		if (compare(heap[current], heap[child]))
		{
			break;
		}

		Post* temp = heap[current];
		heap[current] = heap[child];
		heap[child] = temp;

		current = child;
	}
	return 1;
}

void follow(int uID1, int uID2, int timestamp)
{
    user[uID1].follow[user[uID1].f_cnt++] = uID2;
}

void makePost(int uID, int pID, int timestamp)
{
    post[pID].pid = pID ;
    post[pID].like = 0;
    post[pID].timestamp = timestamp; 

    post[pID].next = userPost[uID].next;
    userPost[uID].next = &post[pID];
}

void like(int pID, int timestamp)
{
    post[pID].like++;
}

void getFeed(int uID, int timestamp, int pIDList[])
{
    for(register Post* cur = userPost[uID].next; cur ;cur= cur->next)
    {
        if(timestamp-cur->timestamp > 1000) cur->like = 0;
        heapPush(cur);
    }

    int id;

    for (register int i = 0 ; i < user[uID].f_cnt ; i++){

        id = user[uID].follow[i];

        for(register Post* cur = userPost[id].next; cur ; cur = cur->next){
            if(timestamp-cur->timestamp > 1000) cur->like = 0;
            heapPush(cur);
        }
    }

    Post* value;
    int cnt = 0;

    while(heapSize > 0 && cnt<10){
        heapPop(&value);
        pIDList[cnt++] = value->pid;
    }

    heapSize = 0;
}