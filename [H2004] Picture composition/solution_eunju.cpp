#define MAX_PICTURE_SIZE 1000
#define MAX_PIECE_SIZE 100
#define MAXN 1500
#define HASH_MAX 600001

extern bool setPicture(int id, int x, int y);

struct pos {
	int posy;
	int posx;
};

int check[MAXN] = {0,};
int tc = 0;

pos position[MAXN]; // check 배열 있어서 초기화 필요 X
int sum[MAXN][2][MAX_PIECE_SIZE]; // 초기화 필요 X
int use[MAXN];

void mergePictures(int N, int M, int K, char pictures[MAXN][MAX_PIECE_SIZE][MAX_PIECE_SIZE]) {

	tc++;

	int cnt = 1;
	check[0] = tc;
	use[0] = 0;
	position[0] = { 0,0 };
	
	int curind = 0;

	for (register int n = 0; n < N; n++)
	{
		for (register int i = 0; i < M - 1; i++)
		{
			sum[n][0][i] = 0;
			sum[n][1][i] = 0;
			for (register int j = 0; j <= M - 1; j++)
			{
				sum[n][1][i] = ((sum[n][1][i] << 5) + pictures[n][j][i] + pictures[n][j][i+1])% HASH_MAX; 
				sum[n][0][i] = ((sum[n][0][i] << 5) + pictures[n][i][j] + pictures[n][i + 1][j]) % HASH_MAX;
				/*sum[n][1][i] += pictures[n][j][i] + pictures[n][j][i + 1];
				sum[n][0][i] += pictures[n][i][j] + pictures[n][i + 1][j];*/
			}
				
		}
	}

	while (cnt < N)
	{
		for (register int n = 1; n < N; n++)
		{
			if (check[n] == tc)
				continue;

			int flag = 0;

			for (register int i = 0; i < M - 1; i++)
			{
				if (flag == 1)
					break;

				if (sum[use[curind]][0][i] == sum[n][0][0])
				{
					for (register int j = 0; j < M; j++)
					{
						if (pictures[use[curind]][i][j] == pictures[n][0][j] && pictures[use[curind]][i+1][j] == pictures[n][1][j])
						{
							if (j == M - 1)
							{
								position[n].posy = position[use[curind]].posy + i;
								position[n].posx = position[use[curind]].posx;
								setPicture(n, position[n].posx, position[n].posy);	//
								check[n] = tc;
								use[cnt] = n;
								cnt++;
								flag = 1;
							}
						}
						else
							break;
					}
				}


				if (flag == 0 && sum[use[curind]][0][i] == sum[n][0][M-2])
				{
					for (register int j = 0; j < M; j++)
					{
						if (pictures[use[curind]][i][j] == pictures[n][M-2][j] && pictures[use[curind]][i + 1][j] == pictures[n][M-1][j])
						{
							if (j == M - 1)
							{
								position[n].posy = position[use[curind]].posy - (M - 2 - i);
								position[n].posx = position[use[curind]].posx;
								setPicture(n, position[n].posx, position[n].posy);
								check[n] = tc;
								use[cnt] = n;
								cnt++;
								flag = 1;
							}
						}
						else
							break;
					}
				}

				if (flag == 0 && sum[use[curind]][1][i] == sum[n][1][0])
				{
					for (register int j = 0; j < M; j++)
					{
						if (pictures[use[curind]][j][i] == pictures[n][j][0] && pictures[use[curind]][j][i+1] == pictures[n][j][1])
						{
							if (j == M - 1)
							{
								position[n].posy = position[use[curind]].posy;
								position[n].posx = position[use[curind]].posx+i;
								setPicture(n, position[n].posx, position[n].posy);
								check[n] = tc;
								use[cnt] = n;
								cnt++;
								flag = 1;
							}
						}
						else
							break;
					}
				}

				if (flag == 0 && sum[use[curind]][1][i] == sum[n][1][M-2])
				{
					for (register int j = 0; j < M; j++)
					{
						if (pictures[use[curind]][j][i] == pictures[n][j][M-2] && pictures[use[curind]][j][i + 1] == pictures[n][j][M-1])
						{
							if (j == M - 1)
							{
								position[n].posy = position[use[curind]].posy;
								position[n].posx = position[use[curind]].posx - (M-2-i);
								setPicture(n, position[n].posx, position[n].posy);
								check[n] = tc;
								use[cnt] = n;
								cnt++;
								flag = 1;
							}
						}
						else
							break;
					}
				}
			}
		}
		curind++;
	}
}
