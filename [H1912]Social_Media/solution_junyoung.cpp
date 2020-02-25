#define MAX_USER 1001
#define MAX_POST 100001

typedef struct _post {
	int uID;
	int time;
	int like;
}Post;

Post postArr[MAX_POST];
int followArr[MAX_USER][MAX_USER] = { 0, };
int numPost = 0;
int cnt = 0;

void init(int N)
{
	numPost = 0;
	cnt++;
	for (int i = 0; i < MAX_POST; i++) {
		postArr[i].uID = 0;
		postArr[i].time = 0;
		postArr[i].like = 0;
	}
}

void follow(int uID1, int uID2, int timestamp)
{
	followArr[uID1][uID1] = cnt;
	followArr[uID1][uID2] = cnt;
}

void makePost(int uID, int pID, int timestamp)
{
	postArr[pID].uID = uID;
	postArr[pID].time = timestamp;
	numPost++;
}

void like(int pID, int timestamp)
{
	postArr[pID].like++;
}

void getFeed(int uID, int timestamp, int pIDList[])
{
	int numP = 0;
	int priorList[MAX_POST] = { 0, };
	for (int i = numPost; i > 0; i--) {
		if (timestamp - 1000 > postArr[i].time && numP >= 10) break;
		if (followArr[uID][postArr[i].uID] != cnt) continue;

		priorList[numP]=i;

		if (timestamp - 1000 > postArr[i].time) {
			numP++;
			continue;
		}
		for (int j = numP - 1; j >= 0; j--) {
			if (postArr[i].like > postArr[priorList[j]].like) {
				priorList[j + 1] = priorList[j];
				priorList[j] = i;
				continue;
			}
			else break;
		}
		numP++;
	}
	for (int i = 0; i < 10; i++) {
		pIDList[i] = priorList[i];
	}
}