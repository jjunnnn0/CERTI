// only 6 test cases out of 10 passed
#define USER 1001
#define GROUP 101
#define EVENTS_PER_GROUP 201
#define EVENTS_PER_USER 101
#define SAME_EVENTS 5
#define NONE -1

struct Node {
	int uid;
	char ename[15];
	int gid;
};

Node node[50005];
int NN = 0;

int eventUid[GROUP][EVENTS_PER_GROUP][SAME_EVENTS]; // 해당 그룹의 같은 ename을 가진 uid 저장용, 실제 uid 저장
int eventName[GROUP][EVENTS_PER_GROUP]; // group의 ename 저장용, node의 index 저장
int userEvent[USER][EVENTS_PER_USER]; // ename 및 gid 저장용, node의 index 저장
int countEvent[USER];


int mstrcmp(const char str1[], const char str2[]) {
	int c = 0;
	while (str1[c] != 0 && str1[c] == str2[c]) ++c;
	return str1[c] - str2[c];
}

void mstrcpy(char dst[], const char src[]) {
	int c = 0;
	while ((dst[c] = src[c]) != 0) ++c;
}

void init()
{
	NN = 0;
	for (int i = 0; i < 50000; i++) {
		node[i].ename[0] = '\n';
		node[i].uid = NONE;
		node[i].gid = NONE;
	}
	for (int i = 0; i < GROUP; i++)
	{
		for (int j = 0; j < EVENTS_PER_GROUP; j++)
		{
			eventName[i][j] = NONE;
			for (int k = 0; k < SAME_EVENTS; k++)
			{
				eventUid[i][j][k] = NONE;
			}
		}
	}
	for (int i = 0; i < USER; i++)
	{
		countEvent[i] = 0;
		for (int j = 0; j < EVENTS_PER_USER; j++)
		{
			userEvent[i][j] = NONE;
		}
	}
}

void addEvent(int uid, char ename[], int groupid)
{
	// node에 먼저 저장	
	//if (NN == 13281) printf("NN==13281 %d %s %d\n", uid, ename, groupid);
	mstrcpy(node[NN].ename, ename);
	node[NN].gid = groupid;
	node[NN].uid = uid;
	// countEvent[uid]에 현재 user의 event개수가 저장되어있다.
	int eventNum = countEvent[uid]++;	
	for (int i = 0; i < EVENTS_PER_GROUP; i++)
	{
		int curr = eventName[groupid][i];
		// 차례대로 저장하기 때문에 현재 저장된 ename이 없다면 중복 없다는 뜻
		if (eventName[groupid][i] == NONE)
		{			
			break;
		}
		// 일치하면 중복으로 추가. userEvent, eventName, eventUid 업뎃
		else if (mstrcmp(node[curr].ename, ename) == 0)
		{
			userEvent[uid][eventNum] = NN;
			if (eventName[groupid][i] == NONE) eventName[groupid][i] = NN;
			//if (NN == 13281) printf("found ename uid:%d eventNum:%d i:%d\n", uid, eventNum,i);
			for (int j = 0; j < SAME_EVENTS; j++)
			{
				if (eventUid[groupid][i][j] == NONE)
				{
					eventUid[groupid][i][j] = uid;
					break;
				}
			}
			NN++;
			return;
		}
	}
	// 일치하는게 없어 새로 추가한다면 모두 다 업뎃
	for (int i = 0; i < EVENTS_PER_GROUP; i++)
	{
		if (eventName[groupid][i] == NONE && eventUid[groupid][i][0] == NONE) {
			eventName[groupid][i] = NN;
			eventUid[groupid][i][0] = uid;
			break;
		}
	}
	
	userEvent[uid][eventNum] = NN;
	/*if (uid == 64) {
		printf("eventNum: %d NN: %d\n", eventNum, NN);
		printf("userEvent uid %d,NN: %d\n", 26, userEvent[26][6]);
		printf("userEvent uid %d,NN: %d\n", 64, userEvent[64][14]);
		printf("userEvent[uid][eventNum] %d \n", NN);
	}*/	
	NN++;
	return;
}

// 삭제하고 난 뒤 앞으로 땡기기.
int deleteEvent(int uid, char ename[])
{
	int cnt = 0;

	for (int i = 0; i < EVENTS_PER_USER; i++)
	{
		int curr = userEvent[uid][i];
		if (curr != NONE)
		{
			if (mstrcmp(node[curr].ename, ename) == 0) //찾았다...
			{
				int gid = node[curr].gid;
				
				
				for (int j = 0; j < EVENTS_PER_GROUP; j++)
				{
					if (mstrcmp(node[eventName[gid][j]].ename, node[curr].ename) == 0) // event 순서 찾음
					{
						if (uid == 768)
						{
							////printf("found ename %s, gid is %d curr NN: %d\n", ename, gid, curr);
							//PrintResult_group(gid);
							////PrintResult_uid(0, gid, j);
						}
						
						// master이면 전체 일정 삭제 및 eventName 앞으로 당기기
						// eventUid 첫번째에 있는게 master
						if (eventUid[gid][j][0] == uid)
						{
							////if (uid == 768) printf("isMaster\n");
							//printf("isMaster\n");
							// 전체 삭제 진행

							for (int k = 0; k < SAME_EVENTS; k++)
							{
								int curr_uid = eventUid[gid][j][k];
								//printf("curr_uid: %d\n", curr_uid);
								if (curr_uid == NONE)
								{
									////if (k == 0) printf("master but NONE\n");
									break;
								}
								eventUid[gid][j][k] = NONE;

								// 지운거 세는거는 여기서만
								countEvent[curr_uid]--;
								cnt++;
							}
							// 해당 eventName 지우고 뒤에 있는것들 땡겨오기
							// eventUid도 삭제하고 나면 뒤에 있는게 앞으로 와야된다
							for (int k = j; k < EVENTS_PER_GROUP; k++)
							{
								if (k == (EVENTS_PER_GROUP - 1))
								{
									eventName[gid][k] = NONE;
									for (int h = 0; h < SAME_EVENTS; h++)
									{
										eventUid[gid][k][h] = NONE;
										if (eventUid[gid][k][h + 1] == NONE) break;// 다음거 없으면 break
									}
								}
								else
								{
									eventName[gid][k] = eventName[gid][k + 1];
									for (int h = 0; h < SAME_EVENTS; h++)
									{
										if (eventUid[gid][k + 1][h] == NONE) break; // 뒤에 있는게 없으면 그만
										eventUid[gid][k][h] = eventUid[gid][k + 1][h];
									}
									if (eventName[gid][k + 1] == NONE) break; // 다음거 없으면 그만.
								}

							}
							// userEvent 지우고 뒤에 있는것들 땡겨오기
							for (int k = 0; k < EVENTS_PER_USER; k++)
							{
								int curr_idx = userEvent[uid][k];
								if (mstrcmp(node[curr_idx].ename, ename) == 0)
								{
									for (int l = k; l < EVENTS_PER_USER; l++) {
										if (l == (EVENTS_PER_USER - 1))
										{
											userEvent[uid][l] = NONE;
										}
										else
										{
											userEvent[uid][l] = userEvent[uid][l + 1];
											if (userEvent[uid][l + 1] == NONE) break;
										}

									}
									break;
								}
							}
							return cnt;
						}
						// master 아니면 해당 event만 삭제, userEvent 앞으로 당기기
						else
						{
							//if (uid == 216) printf("isNot master\n");
							//printf("isNotMaster eventUid[%d][%d][0] %d\n",gid, j, eventUid[gid][j][0]);
							for (int k = 1; k < SAME_EVENTS; k++)
							{
								//printf("eventUid[%d][%d][%d] uid: %d, eventUid: %d\n", gid, j, k, uid, eventUid[gid][j][k]);
								if (eventUid[gid][j][k] == uid)
								{
									// 찾았으면 앞으로 떙기기
									for (int l = k; l < SAME_EVENTS; l++)
									{
										if (l == (SAME_EVENTS - 1))
										{
											eventUid[gid][j][l] = NONE;
										}
										else
										{
											eventUid[gid][j][l] = eventUid[gid][j][l + 1];
											if (l + 1 == NONE) break;
										}
									}
									countEvent[uid]--;
									cnt++;
								}
								else if (eventUid[gid][j][k] == NONE)
								{
									//printf("\nCouldn't find uid\n\n");
									break;
								}
							}
							//printf("deleteEvent cnt: %d\n", cnt);
							return cnt;
						}
					}
					
				}
				//if (uid == 216) printf("couldnt find\n");
			}
		}
	}
	//printf("deleteEvent failed\n");
	return -1; // delete failed
}

int changeEvent(int uid, char ename[], char cname[])
{
	int cnt = 0;
	// 일단 찾는다
	for (int i = 0; i < EVENTS_PER_USER; i++) {
		int curr = userEvent[uid][i];
		if (curr != NONE)
		{
			if (mstrcmp(node[curr].ename, ename) == 0) // 찾았다
			{
				int gid = node[curr].gid;
				// 이제 group내에서 몇번째 event인지 알아내야한다.
				for (int j = 0; j < EVENTS_PER_GROUP; j++)
				{
					int curr_idx = eventName[gid][j];
					// 일치하면 몇 번째인지 찾음
					if (mstrcmp(node[curr_idx].ename, ename) == 0)
					{
						// eventUid의 첫번째 값이 master이므로
						// master인 경우 그 자리에서 이름만 바꾸면 된다. node->ename
						if (eventUid[gid][j][0] == uid)
						{
							if (uid == 538)
							{
								////printf("uid %d is Master\n", uid);
								////PrintResult_uid(0,gid,j);
							}
							mstrcpy(node[curr_idx].ename, cname);
							// 그리고 나서 바뀌는게 몇개인지 count
							for (int k = 0; k < SAME_EVENTS; k++)
							{
								if (eventUid[gid][j][k] != NONE) {
									cnt++;
								}
							}
							return cnt;
						}
						// master가 아닌 경우 그 자리에서 빼고 추가해주기. delete와 add를 해주면 될듯.
						// eventName, eventUid, userEvent 모두 다 업데이트
						else
						{
							// ename delete를 먼저 해주고
							deleteEvent(uid, ename);
							// cname add를 해주자
							addEvent(uid, ename, gid);
							// 바뀐건 하나
							return 1;
						}
					}
				}
			}
		}
	}
	// failed to change event
	return -1;
}

int getCount(int uid)
{
	return countEvent[uid];
}


//void PrintResult(int uid, int groupid) {
//	printf("---eventName---\n");
//	for (int i = 0; i < EVENTS_PER_GROUP; i++) {
//		int curr = eventName[groupid][i];
//		if (eventName[groupid][i] == NONE)
//		{
//			printf("---------------\n");
//			break;
//		}
//		printf("%s\n", node[curr].ename);
//		printf("eventUid: ");
//		for (int j = 0; j < SAME_EVENTS; j++)
//		{
//			if (eventUid[groupid][i][j] == NONE)
//			{
//				printf("\n");
//				break;
//			}
//			printf("%d ", eventUid[groupid][i][j]);
//		}
//	}
//	printf("---userEvent---\n");
//	for (int i = 0; i < EVENTS_PER_USER; i++)
//	{
//		int curr = userEvent[uid][i];
//		if (curr == NONE)
//		{
//			printf("---------------\n");
//			break;
//		}
//		printf("%s gid:%d\n", node[curr].ename, node[curr].gid);
//	}
//}
//
//void PrintResult_user(int loopnum, int uid)
//{
//	printf("%d---userEvent uid %d---\n", loopnum, uid);
//	for (int i = 0; i < EVENTS_PER_USER; i++)
//	{
//		int curr = userEvent[uid][i];
//		if (curr == NONE)
//		{
//			printf("---------------\n");
//			break;
//		}
//		printf("%d %s gid:%d NN: %d\n", i, node[curr].ename, node[curr].gid, curr);
//	}
//}
//
//void PrintResult_group(int gid)
//{
//	printf("--eventName gid %d--\n", gid);
//	for (int i = 0; i < EVENTS_PER_GROUP; i++)
//	{
//		int curr = eventName[gid][i];
//		if (curr == NONE)
//		{
//			printf("---------------\n");
//			break;
//		}
//		printf("%d %s NN: %d\n", i, node[curr].ename, curr);
//	}
//}
//
//void PrintResult_uid(int loopnum, int gid, int eid)
//{
//	printf("--%d eventUid[%d][%d]--\n", loopnum, gid, eid);
//	for (int i = 0; i < SAME_EVENTS; i++)
//	{
//		int curr = eventUid[gid][eid][i];
//		if (curr == NONE)
//		{
//			printf("\n---------------\n");
//			break;
//		}
//		printf("%d ", eventUid[gid][eid][i]);
//	}
//	//printf("\n");
//}