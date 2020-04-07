#define NAME_MAXLEN 6
#define PATH_MAXLEN 1999
#define MAX_TABLE 200003

struct node {
	char name[NAME_MAXLEN + 1];
	// unsigned long hash;
	node* child;
	node* parent;
	node* sibling;
	node* prev;
	int descendants;
};

struct tree
{
	node* root;
};

tree mytree;
//int ans;

//unsigned long hash(const char *str)
//{
//	unsigned long hash = 5381;
//	int c;
//
//	while (c = *str++)
//	{
//		hash = (((hash << 5) + hash) + c) % MAX_TABLE;
//	}
//
//	return hash % MAX_TABLE;
//}

int mstrcmp(const char *a, const char *b)
{
	int i;
	for (i = 0; a[i] != '\0'; i++)
	{
		if (a[i] != b[i])
			return a[i] - b[i];
	}
	return a[i] - b[i];
}

int mstrncmp(const char *a, const char *b, int len)
{
	for (int i = 0; i < len; i++)
	{
		if (a[i] != b[i])
			return a[i] - b[i];
	}
	return 0;
}

int mstrlen(const char *a)
{
	int len = 0;

	while (a[len] != '\0')
		len++;

	return len;
}

void mstrcpy(char *dest, const char *src)
{
	int i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = src[i];
}

void mstrncpy(char *dest, const char *src, int len)
{
	for (int i = 0; i < len; i++)
	{
		dest[i] = src[i];
	}
	dest[len] = '\0';
}

node* findpath(char path[PATH_MAXLEN + 1], node* cur)
{
	char tmp[NAME_MAXLEN + 1];

	if (path[1] != '\0')
	{
		cur = cur->child;
		int ind1 = 1;	//path index
		int ind2 = 0;	//tmp index
		while (true)
		{
			if (path[ind1] == '/')
			{
				tmp[ind2] = '\0';
				while (true)
				{
					/*unsigned int val = hash(tmp);
					if (val == cur->hash)
					{
						if (mstrcmp(tmp, cur->name) == 0)
						{
							ind2 = 0;
							break;
						}
					}*/
					if (mstrcmp(tmp, cur->name) == 0)
					{
						ind2 = 0;
						break;
					}

					else
					{
						cur = cur->sibling;
					}
				}

				ind1++;

				if (path[ind1] != '\0')
				{
					cur = cur->child;
				}

				if (path[ind1] == '\0')
				{
					return cur;
				}


			}
			else
			{
				tmp[ind2] = path[ind1];

				ind1++;
				ind2++;
			}
		}
	}
	else
	{
		return cur;
	}
}

void insertbelow(node* newnode, node* cur)
{

	if (cur->child == 0)
	{
		cur->child = newnode;
	}

	else
	{
		cur = cur->child;
		while (true)
		{
			if (cur->sibling == 0)
			{
				cur->sibling = newnode;
				break;
			}
			cur = cur->sibling;
		}
	}

	newnode->prev = cur;
}


void copynode(node* src, node* dst)
{
	node* newnode = new node;
	mstrcpy(newnode->name, src->name);
	// newnode->hash = hash(newnode->name);
	newnode->sibling = 0;
	newnode->child = 0;
	newnode->parent = dst;
	newnode->descendants = src->descendants;
	insertbelow(newnode, dst);

	if (src->child != 0)
	{
		copynode(src->child, newnode);
	}

	if (src->sibling != 0)
	{
		copynode(src->sibling, dst);
	}

	if (src->child == 0 && src->sibling == 0)
	{
		return;
	}

}

void deletenode(node* cur)
{
	node* tmp = cur->parent;
	while (tmp != 0)
	{
		tmp->descendants -= cur->descendants + 1;
		tmp = tmp->parent;
	}

	if (cur->sibling != 0)
		cur->sibling->prev = cur->prev;

	if (cur->prev->child == cur)
	{
		cur->prev->child = cur->sibling;
	}
	else
	{
		cur->prev->sibling = cur->sibling;
	}
}

//void countnode(node* cur)	//
//{
//	if (cur->child != 0)
//	{
//		ans++;
//		countnode(cur->child);
//	}
//
//	if (cur->sibling != 0) 
//	{
//		ans++;
//		countnode(cur->sibling);
//	}
//
//	if (cur->child == 0 && cur->sibling == 0)
//	{
//		return;
//	}
//
//}

void init(int n) {

	node* newnode = new node;
	newnode->child = 0;
	newnode->parent = 0;
	newnode->sibling = 0;
	newnode->prev = 0;
	newnode->descendants = 0;
	//newnode->name=
	mytree.root = newnode;

}

void cmd_mkdir(char path[PATH_MAXLEN + 1], char name[NAME_MAXLEN + 1]) {

	node* cur = mytree.root;

	cur = findpath(path, cur);

	node* newnode = new node;
	mstrcpy(newnode->name, name);
	// newnode->hash = hash(newnode->name);
	newnode->child = 0;
	newnode->sibling = 0;
	newnode->parent = cur;
	newnode->descendants = 0;
	node* tmp = cur;
	while (tmp != 0)
	{
		tmp->descendants += newnode->descendants + 1;
		tmp = tmp->parent;
	}
	insertbelow(newnode, cur);

}

void cmd_rm(char path[PATH_MAXLEN + 1]) {

	node* cur = mytree.root;

	cur = findpath(path, cur);
	deletenode(cur);
}

void cmd_cp(char srcPath[PATH_MAXLEN + 1], char dstPath[PATH_MAXLEN + 1]) {
	node* src = mytree.root;
	node* dst = mytree.root;

	src = findpath(srcPath, src);
	dst = findpath(dstPath, dst);


	node* newnode = new node;
	mstrcpy(newnode->name, src->name);
	// newnode->hash = hash(newnode->name);
	newnode->sibling = 0;
	newnode->child = 0;
	newnode->parent = dst;
	newnode->descendants = src->descendants;

	node* tmp = dst;
	while (tmp != 0)
	{
		tmp->descendants += newnode->descendants + 1;
		tmp = tmp->parent;
	}
	insertbelow(newnode, dst);

	if (src->child != 0)
	{
		copynode(src->child, newnode);
	}

}

void cmd_mv(char srcPath[PATH_MAXLEN + 1], char dstPath[PATH_MAXLEN + 1]) {

	node* src = mytree.root;
	node* dst = mytree.root;

	src = findpath(srcPath, src);
	dst = findpath(dstPath, dst);

	node* newnode = new node;
	mstrcpy(newnode->name, src->name);
	// newnode->hash = hash(newnode->name);
	newnode->sibling = 0; //
	newnode->child = 0;
	newnode->parent = dst;
	newnode->descendants = src->descendants;

	node* tmp = dst;
	while (tmp != 0)
	{
		tmp->descendants += newnode->descendants + 1;
		tmp = tmp->parent;
	}
	insertbelow(newnode, dst);

	if (src->child != 0)
	{
		copynode(src->child, newnode);
	}

	deletenode(src);
}

int cmd_find(char path[PATH_MAXLEN + 1]) {

	node* cur = mytree.root;

	cur = findpath(path, cur);
	/*
	ans=0;

	if (cur->child != 0)
	{
		ans++;
		countnode(cur->child);
	}*/


	return cur->descendants;
}