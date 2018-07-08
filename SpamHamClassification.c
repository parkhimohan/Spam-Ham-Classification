#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
typedef struct bstree{
	char word[30];
	int weight, freq;
	struct bstree *left, *right;
}BSTREE;
typedef struct trie {
    int isLeaf;     
    struct trie *children[26];
} TRIE;
int C2I(char ch) {
	return (int)ch - (int)'a';
}
TRIE *getNode(void) {
    TRIE *cnode = NULL;
    cnode = (TRIE *)malloc(sizeof(TRIE));
    if (cnode) {
        int i;
        cnode->isLeaf = false; 
        for (i = 0; i < 26; i++)
            cnode->children[i] = NULL;
    }
    return cnode;
}
void inserttrie(TRIE *root,char key[]) {
    int level, index, length;
    length = strlen(key);
    TRIE *cnode = root; 
    for (level = 0; level < length; level++) {
        index = C2I(key[level]);
        if (!cnode->children[index])
            cnode->children[index] = getNode();
        cnode = cnode->children[index];
    }
    cnode->isLeaf = 1;
}
int y=0;
int search(TRIE *root, const char *key) {
    int level, length, index;
    length = strlen(key);
    TRIE *trie = root;
    for (level = 0; level < length; level++) {
        index = C2I(key[level]);
 
        if (!trie->children[index])
        	break;
        trie = trie->children[index];
    }
    if(level==length)
	    y++;
    return y;
}
TRIE* readFile(FILE *fp,TRIE *root)
{
	root = getNode(); 
    int i,count=0;
    int l;
    char str[50];
	fp =fopen("words.txt", "r");
	while((fscanf(fp,"%s",str))>0)
	{
		l=strlen(str);
		for(i=0;i<l;i++)
		{
			if(str[i]>=65 && str[i]<=90)
			str[i]=str[i]+32;
		}
        		inserttrie(root,str);
	}
	return root;
}
BSTREE *createNode(){
	BSTREE *p=(BSTREE*)malloc(sizeof(BSTREE));
	p->left=NULL;
	p->right=NULL;
	p->freq=0;
	return p;	
}
BSTREE *insert(char str[],BSTREE *root, int weight){
	BSTREE *p=createNode();
	strcpy(p->word,str);
	p->weight = weight;
	if(root==NULL) root=p;
	else{
		BSTREE *q=root,*prev;
		while(q!=NULL){
			if(strcmp(q->word,str)>=0){
				prev=q;
				q=q->right;
			}
			else{
				prev=q;
				q=q->left;
			}
		}
		if(strcmp(prev->word,str)>=0) prev->right=p;
		else prev->left=p;
	}
	return root;
}
BSTREE *constructBinary(char filename[],BSTREE *root){
	FILE *fp=fopen(filename,"r");
	char str[30],ch;
	int i=0, weight=0;
	while(fscanf(fp,"%c",&ch)>=1){
		if((ch>='a' && ch<='z') || ch==' ') str[i++]=ch;
		if(ch>='0' && ch<='9') weight = weight*10 + ch - 48;
		if(ch=='\n' || ch==EOF){
			str[i-1]='\0';
			i=0;
			root=insert(str,root, weight);
			weight = 0;
		}
	}
	fclose(fp);
	return root;
}
void printBstree(BSTREE *node){
	if(node!=NULL){
		printBstree(node->left);
		printf("%s weight=%d\n",node->word,node->weight);
		printBstree(node->right);
	}
}
BSTREE *searchFile(char mail[],BSTREE *node){
	FILE *fp=fopen(mail,"r");
	char c,store[20];
	int index=0,length=strlen(node->word);
	while(fscanf(fp,"%c",&c)>=1){
		if(c>='A' && c<='Z') c=c+32;
		if((c>=97 && c<=122) || c==' ' ||c=='.' ||c=='?' ||c=='!' ||c==',' || c=='(' || c==')' || c=='-' || c=='/'){
			if(c==node->word[index]) index++;
			else if(index==length){
				node->freq++;
				index=0;
			}	
			else if(c!=node->word[index]) index=0;
		}
	}
	fclose(fp);
	return node;	
}
void searchWord(char mail[],BSTREE *node){
	if(node!=NULL){
		node=searchFile(mail,node);		
		searchWord(mail,node->left);		
		searchWord(mail,node->right);
	}
}
char opcodeMenu(void){
	char a;	
	int opcode,q;
	printf("------------------------------------------------------------------------------\n");
	printf("Enter the code of Enquiry from  the menu below :\n\n");
	printf("1 : View the blacklisted words\n");
	printf("2 : Search for a blacklisted word \n");
	printf("3 : Check for the spam mail\n");
	printf("4 : Edit the weight of a blacklisted word\n");
	printf("5 : Add a word\n");
	printf("6 : Delete a word\n");
	printf("7 : Exit the portal\n");
	printf("------------------------------------------------------------------------------\n");
	printf("Enter your response :\n");
	scanf("%d",&opcode);
	while(opcode<1 || opcode>7){
		printf("Please enter a valid number!\n");
		scanf("%d", &opcode);
	}
	return opcode;
}
void printMenu(char filename[],char extension[]){
	printf("\nEnter the type of mail from the below menu:\n\n");
	printf("-> Commerce\n");
	printf("-> Employment\n");
	printf("-> Financial\n");
	printf("-> General\n");
	printf("-> Marketing\n");
	printf("-> Medical\n");
	printf("\nYour response:\n");
	scanf("%s",filename);
	getchar();
	if(filename[0]>='A' && filename[0]<='Z') filename[0]=filename[0]+32;
	int i,len=strlen(filename);
	filename[len++]='.';
	filename[len++]='t';
	filename[len++]='x';
	filename[len++]='t';
	filename[len]='\0';
}
bool searchForGiven(BSTREE *node,char add[]){
	while(node!=NULL){
		
		if(strcmp(node->word,add)>0) node=node->right;
		else if(strcmp(node->word,add)<0) node=node->left;
		else return 1;	
	}
	return 0;
}
void rewrite(FILE *fp,BSTREE *node,char delete[]){
	if(node!=NULL){
		rewrite(fp,node->left, delete);
		if(strcmp(node->word,delete)!=0) fprintf(fp,"%s %d\n",node->word, node->weight);
		rewrite(fp,node->right, delete);
	}
}

int generalCount(char fname[],TRIE *trie)
{
	TRIE *root=trie;
	FILE *fp1=fopen(fname,"r");
       	int i=0;
       	char c,str[20];
       	while(fscanf(fp1,"%c",&c)>=1){
		if(c==' ' ||c=='.' ||c=='?' ||c=='!' ||c==',' || c=='(' || c==')' || c=='-' || c=='/' || c=='\n'){
			str[i]='\0';
			y=search(root,str);
			i=0;
		}
		else{
			if(c>='A' && c<='Z')
				c=c+32;
			str[i++]=c;
		}
		}
		fclose(fp1);
		return y;
}

int wordCount(char fname[],TRIE *trie){
	FILE *fp=fopen(fname,"r");
	int count=0;
	char c;
	while(fscanf(fp,"%c",&c)>=1){
		if(c==' ' ||c=='.' ||c=='?' ||c=='!' ||c==',' || c=='(' || c==')' || c=='-' || c=='/'){
			count++;
			fseek(fp,1,SEEK_CUR);
			fscanf(fp,"%c",&c);
			if(c==' ' ||c=='.' ||c=='?' ||c=='!' ||c==',' || c=='(' || c==')' || c=='-' || c=='/') count--;
			fseek(fp,-1,SEEK_CUR);
		}		
	}
	int k=generalCount(fname,trie);
	return count-k;
}

int densityOfSpam(BSTREE *root, int num){
	if(root!=NULL){
		num = densityOfSpam(root->left, num);
		num = num + (root->freq)*(root->weight);
		num = densityOfSpam(root->right, num);
	}
	return num;
}
void modify(BSTREE *node,char *A,int w){

	if(node!=NULL){
		if(strcmp(node->word,A)==0){
		node->weight=w;
		}
		modify(node->left,A,w);
		modify(node->right,A,w);
	}
	
}
void rewriteModified(FILE *fp,BSTREE *node){
	if(node!=NULL){
		rewriteModified(fp,node->left);
		fprintf(fp,"%s %d\n",node->word, node->weight);
		rewriteModified(fp,node->right);
	}
}
void func1(void){
	char filename[100],extension[10],mail[20],add[20],delete[20],opcode;
	BSTREE *root=NULL;
	FILE *fp;
	printMenu(filename,extension);
	root=constructBinary(filename,root);
	printBstree(root);
	
	return;
}
void func2(void){
	char filename[100],extension[10],mail[20],word[20],opcode;
	int k;
	BSTREE *root=NULL;
	FILE *fp;
	printMenu(filename,extension);
	root=constructBinary(filename,root);
	printf("Enter the blacklisted word to be searched :\n");
	scanf("%[^\n]s",word);
	k=searchForGiven(root,word);
	if(k==1) printf("The given word is in the blacklisted words...\n");
	else printf("The given word is NOT in the blacklisted words...\n");
	return;
}
void func3(TRIE *trie){
	char filename[100],extension[10],mail[20],add[20],delete[20],opcode;
	BSTREE *root=NULL;
	FILE *fp;
	printMenu(filename,extension);
	printf("Enter the name of mail :\n");
	scanf("%s",mail);
	root=constructBinary(filename,root);
	searchWord(mail,root);
	int totalWordsDensity = wordCount(mail,trie)*100;
	int spamDensity = densityOfSpam(root, 0);
	float percentage = ((float)spamDensity/(float)totalWordsDensity)*100;
	
	if(percentage>5)
	{
		printf("******** THE GIVEN MAIL IS A SPAM ********\n");
	}
	else
	{
		printf("******** THE GIVEN MAIL IS A HAM ********\n");
	}
	
	return;
}
void func4(void){

	char filename[100],extension[10],mail[20],add[20],delete[20],opcode,option;
	int p,weight;
	BSTREE *root=NULL;
	FILE *fp;
	printMenu(filename,extension);
	root=constructBinary(filename,root);
	printBstree(root);
	printf("Enter the word for which the weight has to be edited :\n");
	
	scanf("%[^\n]s", add);
	getchar();
	p=searchForGiven(root,add);
	if(p==0){
		printf("The word you entered is not in the Blacklist\nDo you want to add it to the Blacklist?y/n\n");
		getchar();
		scanf("%c",&option);
		
		if(option=='y' || option=='Y'){
			printf("Enter the weight of the word :\n");
			scanf("%d", &weight);
			root=insert(add,root, weight);
			fp=fopen(filename,"a");
			fprintf(fp,"%s %d\n",add, weight);
			printf("%s has been added to the Blacklist with weight %d\n",add, weight);
			fclose(fp);
		}
		else if(option=='n' || option =='N') return;
	}
	else{
		printf("Enter the modified weight :\n");
		scanf("%d",&p);
		modify(root,add,p);
		printBstree(root);
		fp=fopen(filename,"w");
		rewriteModified(fp,root);
		fclose(fp);
	}
	return;
}
void func5(void){
	char filename[100],extension[10],mail[20],add[20],delete[20];
	int p,opcode;
	BSTREE *root=NULL;
	FILE *fp;
	printMenu(filename,extension);
	root=constructBinary(filename,root);
	printBstree(root);
	printf("Enter the word to be added in the Blacklist :\n");
	scanf("%[^\n]s", add);
	getchar();
	p=searchForGiven(root,add);
	if(p==1) printf("The word you entered is already in the Blacklist\n");
	else{
		int weight;
		printf("Please enter the weight of the word :\n");
		scanf("%d", &weight);
		root=insert(add,root, weight);
		fp=fopen(filename,"a");
		fprintf(fp,"%s %d\n",add, weight);
		printf("%s has been added to the Blacklist with weight %d\n",add, weight);
		fclose(fp);
	}

	return;
}
void func6(void){
	char filename[100],extension[10],mail[20],add[20],delete[20];
	int p,opcode;
	BSTREE *root=NULL;
	FILE *fp;
	printMenu(filename,extension);
	root=constructBinary(filename,root);
	printf("Enter the word to be deleted from the Blacklist :\n");
	scanf("%[^\n]s", delete);
	getchar();
	p=searchForGiven(root, delete);
	if(p==0) printf("No such word exists in the Blacklist\n");
	else{
		fp=fopen(filename,"w");
		rewrite(fp,root, delete);
		fclose(fp);
		printf("%s has been deleted from the Blacklist\n", delete);
	}
	return;
}
void func7(void){
	exit(1);return;
}
void selection(int opcode,TRIE *trie){
	int op;	
	switch(opcode){
		case 1:
			func1();
			break;
		case 2:
			func2();	
			break;
		case 3:
			func3(trie);
			break;
		case 4:
			func4();
			break;
		case 5:
			func5();
			break;
		case 6:
			func6();
			break;
		case 7:
			func7();
			break;
	}
	y=0;
	op=opcodeMenu();
	selection(op,trie);
	return;
}
int main(){
	int opcode,i;
	FILE *fp,*fp1;
	TRIE *root=NULL;
	fp =fopen("words.txt", "r");
	char str[20],c;
	root=readFile(fp,root);
	opcode=opcodeMenu();
	selection(opcode,root);
	return 0;
}
