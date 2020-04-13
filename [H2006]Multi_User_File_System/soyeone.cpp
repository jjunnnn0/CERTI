#define MAXNODE 10001
#define MAXUSER 22
#define MAXGROUP 12
#define MAXNAME 11
#define MAXPATH 501
#define NULL 0

typedef long long ull;

struct USER {
	char uName[MAXNAME];
	ull uNameInt;
	char gName[MAXNAME];
	ull gNameInt;
	int gIdx;
};
USER user[MAXUSER];
int userCnt;
int res;

struct GROUP {
	char gName[MAXNAME];
	ull gNameInt;
	int uIdx[MAXUSER];
	int uCnt;
};
GROUP group[MAXGROUP];
int groupCnt;

struct NODE {
	char uName[MAXNAME];
	ull uNameInt;

	char gName[MAXNAME];
	ull gNameInt;

	int type;
	char path[MAXPATH];
	char dName[MAXNAME];
	ull dNameInt;
	char fName[MAXNAME];
	ull fNameInt;
	char extName[MAXNAME];
	ull extNameInt;
	int perm;

	NODE* parent;
	NODE* child;
	NODE* next;
	NODE* prev;
	NODE* childF;
	NODE* nextF;
	NODE* prevF;
};

NODE node[MAXNODE];
int nodeCnt;
NODE* root;

char curPath[MAXPATH];
char findUserName[MAXNAME];
ull findUserNameInt;
char findGroupName[MAXNAME];
ull findGroupNameInt;

int mstrcmp(const char* a, const char* b)
{
	int i;
	for (i = 0; a[i] != '\0'; ++i) if (a[i] != b[i]) return a[i] - b[i];
	return a[i] - b[i];
}

void mstrcpy(char* dest, const char* src)
{
	int i = 0;
	while (src[i] != '\0') { dest[i] = src[i]; i++; }
	dest[i] = src[i];
}

ull hash(char* str) {
	ull pow = 1;
	ull res = 0;
	while (*str != '\0') {
		res += (*str - 'a' + 1) * pow;
		pow *= 26;
		str++;
	}
	return res;
}

void init()
{
	nodeCnt = 0;
	userCnt = 0;
	groupCnt = 0;

	for (int i = 0; i < MAXGROUP; i++) {
		group[i].uCnt = 0;
	}

	//admin
	mstrcpy(user[userCnt].uName, "admin");
	user[userCnt].uNameInt = hash(user[userCnt].uName);

	//amdin group
	mstrcpy(group[groupCnt].gName, "admin");
	group[groupCnt].gNameInt = hash(group[groupCnt].gName);
	mstrcpy(user[userCnt].gName, "admin");
	user[userCnt].gNameInt = hash(group[groupCnt].gName);

	group[groupCnt].uIdx[group[groupCnt].uCnt] = userCnt;
	group[groupCnt].uCnt++;

	userCnt++;
	groupCnt++;

	// '/'
	root = &node[nodeCnt];
	*root = {};
	mstrcpy(root->dName, "root");
	root->perm = 2;
	nodeCnt++;
}

void createUser(char userName[], char groupName[])
{
	mstrcpy(user[userCnt].uName, userName);
	user[userCnt].uNameInt = hash(userName);
	mstrcpy(user[userCnt].gName, groupName);
	user[userCnt].gNameInt = hash(groupName);

	// find group
	ull gNameInt = hash(groupName);
	int gExist = -1;
	for (int i = 0; i < groupCnt; i++) {
		if (gNameInt == group[i].gNameInt) {
			gExist = i;
			break;
		}
	}

	if (gExist > -1) {
		group[gExist].uIdx[group[gExist].uCnt] = userCnt;
		group[gExist].uCnt++;
		user[userCnt].gIdx = gExist;

		userCnt++;
	}
	else {
		mstrcpy(group[groupCnt].gName, groupName);
		group[groupCnt].gNameInt = hash(group[groupCnt].gName);

		group[groupCnt].uIdx[group[groupCnt].uCnt] = userCnt;
		group[groupCnt].uCnt++;
		user[userCnt].gIdx = groupCnt;

		userCnt++;
		groupCnt++;
	}

}

void conn3(NODE* a, NODE* b, NODE* c) {
	if (a)a->next = b;
	if (b)b->next = c;
	if (b)b->prev = a;
	if (c)c->prev = b;
}

void conn3F(NODE* a, NODE* b, NODE* c) {
	if (a)a->nextF = b;
	if (b)b->nextF = c;
	if (b)b->prevF = a;
	if (c)c->prevF = b;
}

NODE* go(NODE* cur, char* path) {
	if (cur->child == NULL || *path == '\0')
		return cur;

	char dName[MAXNAME];
	int dNameCnt = 0;

	while (*path != '/') {
		dName[dNameCnt++] = *path;
		path++;
	}
	dName[dNameCnt] = '\0';

	ull dNameInt = hash(dName);
	cur = cur->child;
	while (cur) {
		if (dNameInt == cur->dNameInt) {
			go(cur, path + 1);
			break;
		}
		cur = cur->next;
	}
}

int createDirectory(char userName[], char path[], char directoryName[], int permission)
{
	//path parsing
	char dName[MAXNAME];
	int dNameCnt = 0;
	NODE* cur = root;

	path++;
	cur = go(cur, path);

	//permission ck
	int creatFlag = 0;
	if (cur->perm == 2) {
		creatFlag = 1;
	}
	else if (cur->perm == 1) {
		ull uNameInt = hash(userName);
		for (int i = 0; i < userCnt; i++) {
			if (uNameInt == user[i].uNameInt) {
				if (user[i].gNameInt == cur->gNameInt) {
					creatFlag = 1;
					break;
				}
			}
		}
	}
	else {
		ull uNameInt = hash(userName);
		if (uNameInt == cur->uNameInt) {
			creatFlag = 1;
		}
	}

	if (creatFlag) {

		NODE* newNode = &node[nodeCnt];
		*newNode = {};
		mstrcpy(newNode->uName, userName);
		newNode->uNameInt = hash(userName);
		for (int i = 0; i < userCnt; i++) {
			if (newNode->uNameInt == user[i].uNameInt) {
				mstrcpy(newNode->gName, user[i].gName);
				newNode->gNameInt = user[i].gNameInt;
				break;
			}
		}
		mstrcpy(newNode->dName, directoryName);
		newNode->dNameInt = hash(directoryName);
		newNode->type = 0; //dir
		newNode->perm = permission;
		nodeCnt++;

		if (cur->child) {
			conn3(cur->child, newNode, cur->child->next);
		}
		else {
			cur->child = newNode;
			newNode->parent = cur;
		}
		creatFlag = 1;
	}

	return creatFlag;
}



int createFile(char userName[], char path[], char fileName[], char fileExt[])
{

	//path parsing
	char dName[MAXNAME];
	int dNameCnt = 0;
	NODE* cur = root;

	path++;
	cur = go(cur, path);

	int creatFlag = 0;
	if (cur->perm == 2) {
		creatFlag = 1;
	}
	else if (cur->perm == 1) {
		ull uNameInt = hash(userName);
		for (int i = 0; i < userCnt; i++) {
			if (uNameInt == user[i].uNameInt) {
				if (user[i].gNameInt == cur->gNameInt) {
					creatFlag = 1;
					break;
				}
			}
		}
	}
	else {
		ull uNameInt = hash(userName);
		if (uNameInt == cur->uNameInt) {
		}
	}

	if (creatFlag) {

		NODE* newNode = &node[nodeCnt];
		*newNode = {};
		mstrcpy(newNode->uName, userName);
		newNode->uNameInt = hash(userName);
		for (int i = 0; i < userCnt; i++) {
			if (newNode->uNameInt == user[i].uNameInt) {
				mstrcpy(newNode->gName, user[i].gName);
				newNode->gNameInt = user[i].gNameInt;
				break;
			}
		}
		mstrcpy(newNode->fName, fileName);
		newNode->fNameInt = hash(fileName);
		mstrcpy(newNode->extName, fileExt);
		newNode->extNameInt = hash(fileExt);
		newNode->type = 1; //file
		nodeCnt++;

		if (cur->childF) {
			conn3F(cur->childF, newNode, cur->childF->nextF);
		}
		else {
			cur->childF = newNode;
			newNode->parent = cur;
		}
		creatFlag = 1;
	}


	return creatFlag;
}

void count(NODE* cur, char* path) {

	char dName[MAXNAME];
	int dNameCnt = 0;
	int fileFlag = 0;

	char extName[MAXNAME];

	while (*path != '/') {
		if (*path == '.') {
			fileFlag = 1;
			dName[dNameCnt] = '\0';
			int extNameCnt = 0;
			path++;
			while (*path != '\0') {
				extName[extNameCnt++] = *path;
				path++;
			}
			extName[extNameCnt] = '\0';
			break;
		}
		dName[dNameCnt++] = *path;
		path++;
	}
	dName[dNameCnt] = '\0';

	if (fileFlag == 1) {
		cur = cur->childF;
		if (mstrcmp(dName, "*") == 0) {
			if (mstrcmp(extName, "*") == 0) {
				while (cur) {
					res++;
					cur = cur->nextF;
				}
				return;
			}
			else {
				ull extNameInt = hash(extName);
				while (cur) {
					if (cur->extNameInt == extNameInt)
						res++;
					cur = cur->nextF;
				}
				return;
			}
		}
		else {
			ull fNameInt = hash(dName);
			if (mstrcmp(extName, "*") == 0) {
				while (cur) {
					if (cur->fNameInt == fNameInt)
						res++;
					cur = cur->nextF;
				}
				return;
			}
			else {
				ull extNameInt = hash(extName);
				while (cur) {
					if (cur->fNameInt == fNameInt && cur->extNameInt == extNameInt) {
						res++;
						return;
					}
					cur = cur->nextF;
				}
			}
		}
	}
	else if (mstrcmp(dName, "*") != 0) {
		ull dNameInt = hash(dName);
		cur = cur->child;
		while (cur) {
			if (cur->perm == 2 && dNameInt == cur->dNameInt) {
				count(cur, path + 1);
			}
			else if (cur->perm == 1) {
				if (cur->gNameInt == findGroupNameInt && dNameInt == cur->dNameInt) {
					count(cur, path + 1);
				}
			}
			else {
				if (cur->uNameInt == findUserNameInt && dNameInt == cur->dNameInt) {
					count(cur, path + 1);
				}
			}
			cur = cur->next;
		}

	}
	else {
		cur = cur->child;
		while (cur) {
			if (cur->perm == 2) {
				count(cur, path + 1);
			}
			else if (cur->perm == 1) {
				if (cur->gNameInt == findGroupNameInt) {
					count(cur, path + 1);
				}
			}
			else {
				if (cur->uNameInt == findUserNameInt) {
					count(cur, path + 1);
				}
			}
			cur = cur->next;
		}
	}
}

int find(char userName[], char pattern[])
{
	res = 0;

	mstrcpy(findUserName, userName);
	findUserNameInt = hash(userName);

	for (int i = 0; i < userCnt; i++) {
		if (findUserNameInt == user[i].uNameInt) {
			mstrcpy(findGroupName, user[i].gName);
			findGroupNameInt = user[i].gNameInt;
			break;

		}
	}

	char dName[MAXNAME];
	int dNameCnt = 0;
	NODE* cur = root;

	pattern++;
	count(cur, pattern);

	return res;
}