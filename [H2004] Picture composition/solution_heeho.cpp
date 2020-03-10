#define MAX_PICTURE_SIZE 1000
#define MAX_PIECE_SIZE 100
#define MAXN 1500
#define MAX_TABLE 100007

extern bool setPicture(int id, int x, int y);
 
int g_N, g_M, g_K;
 
typedef struct node {
    struct node* next;
    int i;
}Node;

Node nodePool[MAXN * 4];
int nodePoolCnt;

Node* ht[MAX_TABLE];

// k = 0이면 pic[i][0~5]인 가로 방향으로 해시 값 구하기
// k = 1이면 pic[0~5][i]인 세로 방향으로 해시 값 구하기

int myhash(char pic[MAX_PIECE_SIZE][MAX_PIECE_SIZE], int i, int k)
{
    register unsigned long hash = 5381 + k * 7;

    if (k == 0) {
        // 앞 숫자 5개만을 사용하여 Hash 만들기
        for (register int j = 0; j < g_M && j < 5; j++) {
            hash = (((hash << 5) + hash) + pic[i][j]) % MAX_TABLE;
        }
    }
    else {
        // 앞 숫자 5개만을 사용하여 Hash 만들기
        for (register int j = 0; j < g_M && j < 5; j++) {
            hash = (((hash << 5) + hash) + pic[j][i]) % MAX_TABLE;
        }
    }
    return hash % MAX_TABLE;
}

int heehash(char pic[MAX_PIECE_SIZE][MAX_PIECE_SIZE], int i, int k)
{
    register unsigned long hash = 5381 + k * 7;

    if (k == 0) {
        // 앞 숫자 5개만을 사용하여 Hash 만들기
        for (register int j = 0; j < g_M && j < 5; j++) {
            hash = (((hash << 5) + hash) + pic[i][j]) % MAX_TABLE;
        }
    }
    else {
        // 앞 숫자 5개만을 사용하여 Hash 만들기
        for (register int j = 0; j < g_M && j < 5; j++) {
            hash = (((hash << 5) + hash) + pic[j][i]) % MAX_TABLE;
        }
    }

    return hash % MAX_TABLE;
}
 
 
void add(int i, char pic[MAX_PIECE_SIZE][MAX_PIECE_SIZE]) {

    register int k;
    register Node*p;

    // 첫번째 행(가로 방향)
    k = myhash(pic, 0, 0);
    p= &nodePool[nodePoolCnt++];
    p->i = i;
    p->next = ht[k];
    ht[k] = p;
 
    // 첫번째 열(세로 방향)
    k = myhash(pic, 0, 1);
    p = &nodePool[nodePoolCnt++];
    p->i = i;
    p->next = ht[k];
    ht[k] = p;
 
    // 마지막 행(가로 방향)
    k = myhash(pic, g_M-1, 0);
    p = &nodePool[nodePoolCnt++];
    p->i = i;
    p->next = ht[k];
    ht[k] = p;
 
    // 마지막 열(세로 방향)
    k = myhash(pic, g_M-1, 1);
    p = &nodePool[nodePoolCnt++];
    p->i = i;
    p->next = ht[k];
    ht[k] = p;
}

inline void dfs(int r, int c, int n, char pictures[MAXN][MAX_PIECE_SIZE][MAX_PIECE_SIZE]) {
 
    register int k, i, cnt  = 0;
    register Node* p;

    // 현재 기준이 되는 색종이는 두번째 행/열 부터 마지막에서 두번째 행/열 까지 

    for (i = 1; i < g_M -1; i++) {
        
        // 기준이 되는 색종이의 행에 해당하는 해시값 구함 
        k = myhash(pictures[n], i, 0); 

        // 위에서 구한 해시값에 해당하는 행을 가지고 있는 색종이들을 검사할 것
        for (p = ht[k]; p ; p = p->next) {

            // 색종이 첫번째 행과 기준이 되는 색종이 검사
            if (setPicture(p->i, r , c + i) ) {
                // 만약 기준이되는 색종이에 붙을 수 있는 색종이를 찾았다면
                // 기준이 되는 색종이가 바뀜 좌표 (r, c+i), 색종이 번호 p->i
                dfs(r, c + i, p->i, pictures);
                cnt++;
            }

            // 색종이 마지막 번째 행과 기준이 되는 색종이 검사
            if(setPicture(p->i, r, c + i - g_M + 1)){
                // 만약 기준이되는 색종이에 붙을 수 있는 색종이를 찾았다면
                // 기준이 되는 색종이가 바뀜 좌표 (r, c+i-g_M+1), 색종이 번호 p->i
                dfs(r, c + i - g_M + 1, p->i, pictures);
                cnt++;
            }
        }

        // 기준이 되는 색종이의 열에 해당하는 해시값 구함 
        k = myhash(pictures[n], i, 1);

        for (p = ht[k]; p ; p = p->next) {
            if (setPicture(p->i, r + i, c)) {
                dfs(r + i, c, p->i, pictures);
                cnt++;
            }
            if (setPicture(p->i, r + i - g_M + 1, c)) {
                dfs(r + i - g_M + 1, c, p->i, pictures);
                cnt++;
            }
        }

        // 동서 남북 4 모서리에 대해서 모두 색종이를 붙였다면 더이상 검사할 필요 없음
        if (flag == 4) break;
    }
}
 
void mergePictures(int N, int M, int K, char pictures[MAXN][MAX_PIECE_SIZE][MAX_PIECE_SIZE]) {
    
    nodePoolCnt = 0;
    for (register int i = 0; i < MAX_TABLE; i++) ht[i] = 0;

    g_N = N, g_M = M, g_K = K;
 
    for (register int i = 0; i < N; i++) {
        add(i, pictures[i]);
    }
 
    dfs(0, 0, 0, pictures);  
    // 왼쪽 젤 상위 좌표가 (0,0)이고 번호가 0인 색종이를 시작으로
}