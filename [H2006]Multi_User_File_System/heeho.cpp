
#define MAX_INFO 10000
#define FILE 0
#define DIRECT 1

#define MAX_USER 20
#define MAX_TABLE (MAX_USER*2+7)

#define ull unsigned long long
#define register

typedef struct info{

    int type;
    ull name;
    ull user;
    ull ext;
    int permission;

    struct info *parent, *child;
    struct info *next;

}Info;

Info infoPool[MAX_INFO];
int infoPoolCnt;

Info root;

typedef struct group{

    ull user;
    ull group;
    struct group* next;

}Group;

Group HT[MAX_TABLE];

Group groupPool[MAX_USER];
int groupPoolCnt;

ull string_to_ull(char str[]){

    register ull ret = 0LL;

    for(register int i=0; str[i]; ++i){
        ret = (ret << 5) + str[i]-'a'+1;
    }

    return ret;
}

void init()
{
    infoPoolCnt = 0;
    groupPoolCnt = 0;
    
    for(register int i=0 ;i<MAX_TABLE; ++i){
        HT[i].next = 0;
    }

    char admin[] = "admin";

    Group* newGroup = &groupPool[groupPoolCnt++];
    newGroup->user = string_to_ull(admin);
    newGroup->group = string_to_ull(admin);
    newGroup->next = 0;

    newGroup->next = HT[newGroup->user%MAX_TABLE].next;
    HT[newGroup->user%MAX_TABLE].next = newGroup;

    // root direcotry 셋팅
    root.type = DIRECT;
    root.name = string_to_ull(admin);
    root.user = string_to_ull(admin);
    root.permission = 2;

    root.parent = 0;
    root.child = 0;
    root.next = 0;

}

ull getGroup(ull user){

    for(register Group* g = HT[user%MAX_TABLE].next; g; g= g->next){

        if(g->user == user){
            return g->group;
        }
    }
}

void createUser(char userName[], char groupName[])
{
    register Group* newGroup = &groupPool[groupPoolCnt++];
    newGroup->user = string_to_ull(userName);
    newGroup->group = string_to_ull(groupName);
    newGroup->next = 0;

    newGroup->next = HT[newGroup->user%MAX_TABLE].next;
    HT[newGroup->user%MAX_TABLE].next = newGroup;

}



void add_link(Info* parent, Info* child){

    child->parent = parent;
    child->next = parent->child;
    parent->child = child;
}


int createDirectory(char userName[], char path[], char directoryName[], int permission)
{
    ull user = string_to_ull(userName);
    ull directName = string_to_ull(directoryName);

    Info* info = &root;
    ull name = 0;

    for (register int i =1 ; path[i]; ++i){

        if(path[i] != '/'){
            name = (name << 5) + path[i] - 'a' + 1;
        }
        else{

            for (register Info* curr = info->child ; curr; curr = curr->next){

                if(curr->type == DIRECT && curr->name == name){

                    info = curr;

                    break;
                }
            }
            name = 0 ;
        }
    }

    if(info->permission == 0 && info->user != user) return 0;
    else if (info->permission == 1 && getGroup(info->user)!=getGroup(user)) return 0;

    Info* newDirect = &infoPool[infoPoolCnt++];
    newDirect->type = DIRECT;
    newDirect->name = directName;
    newDirect->user = user;
    newDirect->permission = permission;

    newDirect->child = 0;
    newDirect->parent = 0;
    newDirect->next = 0;

    add_link(info, newDirect);

    return 1;
}

int createFile(char userName[], char path[], char fileName[], char fileExt[])
{
    ull user = string_to_ull(userName);
    ull fName = string_to_ull(fileName);
    ull fExt = string_to_ull(fileExt);

    Info* info = &root;
    ull name = 0;


    int ret = 1;
    for (register int i =1 ; path[i]; ++i){

        if(path[i] != '/'){
            name = (name << 5) + path[i] - 'a' + 1;
        }
        else{

            for (register Info* curr = info->child ; curr; curr = curr->next){

                if(curr->type == DIRECT && curr->name == name){

                    info = curr;
                    break;
                }
            }
            name = 0 ;
        }
    }

    if(info->permission == 0 && info->user != user) return 0;
    else if (info->permission == 1 && getGroup(info->user)!=getGroup(user)) return 0;


    Info* newFile = &infoPool[infoPoolCnt++];
    newFile->type = FILE;
    newFile->name = fName;
    newFile->ext = fExt;
    newFile->user = user;

    newFile->child = 0;
    newFile->parent = 0;
    newFile->next = 0;

    add_link(info, newFile);

    return 1;
}


int dsf(Info* info, int cur_depth, ull user, ull value[], int depth){
    
    int ret =0 ;

    if(info->type == FILE){
        if(cur_depth + 2 == depth) {
            if(value[cur_depth] == 0 || info->name == value[cur_depth]){
                if(value[cur_depth+1] == 0 || info->ext == value[cur_depth+1]){
                    return 1;
                }
            }
        }

        return 0;

    }else{

        if(info->permission == 0 && info->user != user)  return 0;
        else if (info->permission == 1 && getGroup(info->user)!=getGroup(user) ) return 0;

        if(value[cur_depth] != 0 and info->name != value[cur_depth]) return 0;
    }

    for (register Info* curr = info->child ; curr; curr = curr->next){
        ret += dsf(curr, cur_depth+1, user, value, depth);   
    }

    return ret;
}



int find(char userName[], char pattern[])
{
    int ret = 0;
    ull user = string_to_ull(userName);

    ull name = 0;
    int cnt = 0; 

    ull value[50];

    for (register int i =1 ; pattern[i] ; ++i){

        if(pattern[i] != '/' and pattern[i] != '.'){
            if(pattern[i] != '*') name = (name << 5) + pattern[i] - 'a' + 1;
        }
        else{
            value[cnt++] = name;
            name = 0 ;
        }
    }


    value[cnt++] = name;

    for (register Info* curr = root.child ; curr ; curr = curr->next){
        ret += dsf(curr, 0, user, value, cnt);
    }

    return ret;
}