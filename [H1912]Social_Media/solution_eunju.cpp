#define MAXN 1001
#define MAXP 101010

struct post
{
	int uid;
	int timestamp;
	int like;
};

int following[MAXN][MAXN];	
post postlist[MAXP];
int postcnt;
int tc=0;

void init(int N)
{
	tc++;
	postcnt = 0;
	for (register int i = 1; i <= N; i++)
	{
		following[i][i] = tc;
	}
}

void follow(int uID1, int uID2, int timestamp)
{
	following[uID1][uID2] = tc;
}

void makePost(int uID, int pID, int timestamp)
{
	postcnt++;
	postlist[postcnt].uid = uID;
	postlist[postcnt].timestamp = timestamp;
	postlist[postcnt].like = 0;
}

void like(int pID, int timestamp)
{
	postlist[pID].like++;
}

void getFeed(int uID, int timestamp, int pIDList[])
{
	int feedcnt = 0;
	for (register int i = postcnt; i > 0; i--)
	{
		if (timestamp - postlist[i].timestamp <= 1000) // 1000초 이내인 경우
		{
			if (following[uID][postlist[i].uid]== tc)
			{
				if (feedcnt == 0)
				{
					pIDList[0] = i;
				}
				else
				{
					for (register int j = 0; j < feedcnt; j++)
					{
						if (postlist[pIDList[j]].like < postlist[i].like)
						{
							for (register int k = feedcnt-1; k >= j; k--)
							{
								if (k == 9)
								{
									continue;
								}
								
								pIDList[k + 1] = pIDList[k];
							}

							pIDList[j] = i;
							break;
						}

						if (j == feedcnt - 1 && feedcnt < 10)
						{
							pIDList[feedcnt] = i;
						}
					}

				}

				if (feedcnt < 10)
				{
					feedcnt++;
				}

			}
			else
				continue;
		}

		else // 1000초 초과 게시글
		{
			if (feedcnt < 10)
			{
				if (following[uID][postlist[i].uid]== tc)
				{
					pIDList[feedcnt] = i;
					feedcnt++;
				}
			}
			else
				break;
		}
		
	}


}