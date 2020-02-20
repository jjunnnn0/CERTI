#define MAXL	5
#define MAXF	10
#define MAXN	10101

struct NODE {
	int id;
	NODE* next;
};

NODE addlist[MAXN];
NODE dellist[MAXN];
int score[MAXN];
int usernum;

void init(int N)
{
	for (register int i = 1; i <= N; i++)
	{
		addlist[i].next = 0;
		dellist[i].next = 0;
	}

	usernum = N;
}

void addnode(int id, NODE* dest)
{
	NODE* newnode = new NODE;
	newnode->id = id;
	newnode->next = 0;

	NODE* cur = dest;

	while (true) {
		if (cur->next == 0)
		{
			cur->next = newnode;
			break;
		}
		cur = cur->next;
	}
}

void add(int id, int F, int ids[MAXF])
{	
	for (register int i = 0; i < F; i++)
	{
		addnode(ids[i], &addlist[id]);
		addnode(id, &addlist[ids[i]]);
	}
}

void del(int id1, int id2)
{
	addnode(id2, &dellist[id1]);
	addnode(id1, &dellist[id2]);
}

int recommend(int id, int list[MAXL])	
{
	for (register int i = 1; i <= usernum; i++)
	{
		score[i] = 0;
	}

	score[id] = -2;

	NODE* cur = &addlist[id];

	while (true)
	{
		if (cur->next == 0)
		{
			break;
		}
		cur = cur->next;
		score[cur->id]--;
	}

	cur = &dellist[id];

	while (true)
	{
		if (cur->next == 0)
		{
			break;
		}
		cur = cur->next;
		score[cur->id]++;
	}

	for (register int i = 1; i <= usernum; i++)
	{
		if (score[i] == -1)
		{
			cur = &addlist[i];

			while (true)
			{
				if (cur->next == 0)
				{
					break;
				}
				cur = cur->next;
				if(cur->id!=id && score[cur->id]!=-1) score[cur->id]++;
			}
		}
	}

	for (register int i = 1; i <= usernum; i++)
	{
		if (score[i] == -1)
		{
			cur = &dellist[i];

			while (true)
			{
				if (cur->next == 0)
				{
					break;
				}
				cur = cur->next;
				if (cur->id != id && score[cur->id] != -1) score[cur->id]--;
			}
		}
	}

	int cnt=-1;
	int best5[] = { -1,-1,-1,-1,-1 };
	for (register int i = 1; i <=usernum; i++)
	{
		if (score[i] > 0)
		{
			if (cnt < 4)
			{
				cnt++;
			}

			if (score[i] > best5[cnt] && cnt <= 5)
			{
				best5[cnt] = score[i];
				list[cnt] = i;

				if (cnt > 0)
				{
					int temp = best5[cnt];
					int tempind = list[cnt];
					int j = cnt - 1;

					while ((temp > best5[j]) && (j >= 0))
					{
						best5[j + 1] = best5[j];
						list[j + 1] = list[j];
						j = j - 1;
					}
					best5[j + 1] = temp;
					list[j + 1] = tempind;
				}

			}
		}
		
	}

	return cnt+1;
}