#define NAME_MAXLEN 6
#define PATH_MAXLEN 1999
#define MAX_DIRECTORY 50000
#define register

typedef struct directory{

    unsigned int name;
    directory* prev;
    directory* next;
    directory* parent;
    directory* child;

}Directory;

Directory directoryPool[MAX_DIRECTORY];
int directoryPoolCnt;

Directory root;

void init(int n) {

    directoryPoolCnt = 0;
    root.next = 0;
    root.prev = 0;
    root.parent = 0;
    root.child = 0;
}


Directory* getDirectory(char path[PATH_MAXLEN + 1]){

    register unsigned int sum = 0;
    Directory* current = &root;

    for (register int i=1; path[i] ; ++i){

        if (path[i] == '/'){

            for(register Directory* direct = current->child; direct; direct=direct->next){
                if(direct->name == sum){
                    current = direct;
                }
            }
            sum = 0;
        }
        else{
            sum = (sum<<5) + path[i]-'a' + 1;
        }
    }

    return current;
}


void add_link(Directory* parent, Directory* child){

    child->prev = 0;
    child->parent = parent;
    child->next = parent->child;

    if(parent->child) parent->child->prev = child;
    parent->child = child;
}

void delete_link(Directory* current){

    if(current->prev) current->prev->next = current->next;
    else current->parent->child = current->next;
    
    if(current->next) current->next->prev = current->prev;
}


void cmd_mkdir(char path[PATH_MAXLEN + 1], char name[NAME_MAXLEN + 1]) {

    register Directory* current = getDirectory(path);

    register unsigned int sum = 0;
    for(register int i= 0; name[i]; ++i){
        sum = (sum<<5) + name[i]- 'a' + 1;
    }

    register Directory* newDirect = &directoryPool[directoryPoolCnt++];
    newDirect->name = sum;
    newDirect->child = 0;

    add_link(current, newDirect);
}

void cmd_rm(char path[PATH_MAXLEN + 1]) {

    register Directory* current = getDirectory(path);

    delete_link(current);

}

void makeDirect(Directory* src, Directory* dst)
{
    register Directory* newDirect = &directoryPool[directoryPoolCnt++];
    newDirect->name = src->name;
    newDirect->child = 0;

    add_link(dst, newDirect);

    for(register Directory* direct = src->child; direct; direct=direct->next){
        makeDirect(direct, newDirect);
    }

}

void cmd_cp(char srcPath[PATH_MAXLEN + 1], char dstPath[PATH_MAXLEN + 1]) {

    register Directory* srcDirect = getDirectory(srcPath);
    register Directory* dstDirect = getDirectory(dstPath);

    makeDirect(srcDirect, dstDirect);
}

void cmd_mv(char srcPath[PATH_MAXLEN + 1], char dstPath[PATH_MAXLEN + 1]) {

    register Directory* srcDirect = getDirectory(srcPath);
    register Directory* dstDirect = getDirectory(dstPath);

    delete_link(srcDirect);
    add_link(dstDirect, srcDirect);

}

int getChildNum(Directory* current){

    register int cnt = 0;
    for(register Directory* direct = current->child; direct; direct=direct->next){
        cnt += 1 + getChildNum(direct);
    }

    return cnt;
}

int cmd_find(char path[PATH_MAXLEN + 1]) {

    register int ret = 0;

    register Directory* current = getDirectory(path);

    ret = getChildNum(current);

	return ret;
}