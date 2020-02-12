#define MAXL 5
#define MAXF 10

typedef struct _friend {
	int id;
	struct _friend *next;
}rec;

rec a[1000000];
int pIdx;
int n;

rec REC[10001];

void init(int N) {
	n = N;
	pIdx = 0;
	for (int i = 1; i <= N; i++) {
		REC[i].next = nullptr;
	}
}

void add(int id, int F, int ids[MAXF]) {
	for (int i = 0; i < F; i++) {
		// 나한테 먼저 넣고
		rec* temp = &a[pIdx++]; // 메모리 할당
		temp->id = ids[i];
		temp->next = REC[id].next;
		REC[id].next = temp;

		// 친구에도 나를 넣고
		rec* temp2 = &a[pIdx++];
		temp2->id = id;
		temp2->next = REC[ids[i]].next;
		REC[ids[i]].next = temp2;
	}
}

void del(int id1, int id2) {
	rec* Next = REC[id1].next; // 내 다음 노드
	rec* Now = &REC[id1]; // 나
	while (Next) {
		if (Next->id == id2) {
			Now->next = Next->next;

			Next = REC[id2].next;
			Now = &REC[id2];
			while (Next) {
				if (Next->id == id1) {
					Now->next = Next->next;
					break;
				}
				Now = Next;
				Next = Next->next;
			}
			break;
		}
		Now = Next;
		Next = Next->next;
	}
}

int recommend(int id, int list[MAXL])
{
	int friend_map[10001]; // 함께아는 친구 count용도
	for (int i = 1; i <= n; i++) // 초기화
		friend_map[i] = 0;

	rec* me = REC[id].next; // 나의 첫번째 친구부터 탐색
	rec* myfriend;
	while (me) {
		myfriend = REC[me->id].next;
		while (myfriend) {
			friend_map[myfriend->id]++;
			myfriend = myfriend->next;
		}
		me = me->next;
	}

	// 추천친구 리스트에서 제외
	friend_map[id] = 0; // 나는 
	me = REC[id].next; // 나의 첫번째 친구
	while (me) { // 친구들도 제외
		friend_map[me->id] = 0;
		me = me->next;
	}

	int max = 0;
	int index;
	int ans = 0;
	// 제일 많이 함께 아는 친구부터 리스트에 저장.
	for (int j = 0; j < 5; j++) {
		max = 0;
		index = 1;
		for (int i = 1; i <= n; i++) {
			if (friend_map[i] > max) {
				max = friend_map[i];
				index = i;
			}
		}
		if (max == 0) break;
		else {
			ans++;
			friend_map[index] = 0;
			list[j] = index;
		}
	}
	return ans;
}
