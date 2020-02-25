#define MAX_P 200001
#define MAX_F 5001

int frd[1001][1001];

struct POST {
	int uid;
	int pid;
	int like;
	int timestamp;
}Post[MAX_P];
//POST P_BEAN;
int postcnt;

//struct LIST
//{
//	LIST* prev;
//	POST* p;
//	LIST* next;
//};

void init(int N)
{
	for (register int i = 0; i <= N; i++) {
		for (register int j = 0; j <= N; j++) {
			frd[i][j] = 0;
		}
		frd[i][i] = 1;
	}
	//P_BEAN.like = 0;
	postcnt = 0;
}

void follow(int uID1, int uID2, int timestamp)
{
	frd[uID1][uID2] = 1;
}

void makePost(int uID, int pID, int timestamp)
{
	POST* newpost = &Post[pID];
	//*newpost = P_BEAN;
	newpost->like = 0;
	newpost->uid = uID;
	newpost->pid = pID;
	newpost->timestamp = timestamp;
	postcnt++;
}

void like(int pID, int timestamp)
{
	Post[pID].like++;
}

void getFeed(int uID, int timestamp, int pIDList[])
{
	int cnt = 0;
	int List[10] = { 0 };

	for (register int i = postcnt; i > 0; i--) {
		if (cnt >= 10 && timestamp - 1000 > Post[i].timestamp)
			break;
		if (frd[uID][Post[i].uid] == 1) {
			if (cnt == 0) {
				List[cnt++] = Post[i].pid;
			}
			else {
				if (timestamp - 1000 <= Post[i].timestamp) {
					int ck = 0;
					for (register int j = cnt - 1; j >= 0; j--) {
						if (Post[List[j]].like >= Post[i].like) {
							if (j < 9) {
								List[j + 1] = Post[i].pid;
								if (cnt < 10) {
									cnt++;
								}
								ck = 0;
							}
							else {
								;
							}
							break;
						}
						else {
							if (j < 9) {
								List[j + 1] = List[j];
								List[j] = Post[i].pid;
								ck = 1;
							}
							else {
								List[j] = Post[i].pid;
							}
						}
					}
					if (ck == 1) {
						if (cnt < 10) {
							cnt++;
						}
					}
				}
				else {
					if (cnt < 10) {
						List[cnt++] = Post[i].pid;
					}
				}
			}
		}
	}
	for (register int i = 0; i < cnt; i++) {
		pIDList[i] = List[i];
	}
}