#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

struct node{//this is the main struct where we put the word
	char* Word;
	struct node* left;
	struct node* middle;
	struct node2* right;//right is the pointer at the head of  the list of files 
};
typedef struct node* nodeptr; 
struct node2{//this is the struct that we use to hold the name of the file and also has the list(struct node *left)  of the coordinates(line and column) of the searching word in this file
	char *Word;
	struct node2 *right;
	struct node3 *left;//this is the pointer that points to the list of the coordinates(line,column) of the word in the file with the name word
};
struct node3{//this struct is used to hold the column and line of the word that we search, into the variants column,line
	int column;//column is the column of the word in the file 
	int line;//line is the line of the word in the file
	struct node3 *next;
};
typedef struct node2* nodeptr2;
typedef struct node3* nodeptr3;

struct files{//this struct is used to make a list of the file names,that will help us chek if a name of a file has been given twice or more
	char *NameOfFile;
	struct files* next;
}; 
typedef struct files* filesptr;

int number(int Bpower);//this function calculates the the arithetic term: (256^(n-1-i))mod1031
void input(nodeptr* hashTableVar,char *n,int column,int line,char *FileReader);// this is the function that places the word into the correct field of hashtable
void searchnode(nodeptr2* FileVar,char* FileReader,int column,int line);//this is the function that finds where should the word be placed in the binary search tree
void node3cmp(nodeptr3 * a,nodeptr3 b);//this is the function that places the struct with the column,line coordinates in the right place in the list
void nodemaker(nodeptr *hashTableVar,char* n,int column,int line,char* FileReader);//function nodemaker is called if the filed of hashtable is null which means that we haven't put any words
                                                                           //so it creates the appropriate structs and stores the appropriate information such as the name of the word

void searchword(nodeptr hashTableVar,char* n);//this function finds if the word exists in the binary search tree and if yes then it calls searchword2 to find  which file contains the word  
void searchword2(nodeptr2 b);//this function calls searchword3 to print the name of the file and the coordinates(line,column) of the word in this file for every file that word exists
void searchword3(char * word,nodeptr3 c);//thid function prints the name of the file and the line and column of the word
void finalsearch(nodeptr HashTable[1031]); //this is the function that reads the word that we want to search and finds the right hashnumber for this word
int number2(char* n,int i);//number2 calculates where should we add the word,in which  hashtable field
void mainreader(char* FileReader,nodeptr hashTable[1031]); //this is the main function that reads the letters from each file and calls the function(input) that places the word into hashtable 
void hashTablenull(nodeptr hashTable[1031]);//hashtablenull give all the fields of array hastable the null value

void inputfiles(char* FileReader,filesptr *head);//void inputfiles has as parameters the variant FileReader and the head of the file list and and adds a new node to the list head
                                                //that contains the name of the file 

int checkfile(char* FileReader,filesptr head);//function checkfile traverse the list of the files that have read and opened and 
                                             //   checks if the file that is going to open already has been read,in that case checkfile returns 1 

int main()
{
  char *FileReader;	//FileReader is the variant that we read the name of the files
  filesptr head=NULL;//head is the head of the list of files that we create in order to see if a file is given more than one time
  nodeptr hashTable[1031];
  hashTablenull(hashTable);//hashtablenull give all the filds of array hastable the null value
  FILE *g;//g is the pointer to the the main file from which we read the names of the files
  if ((g=fopen("input.txt","rt"))==NULL) exit(1);
  if ((FileReader=(char*)malloc(21*sizeof(char)))==NULL) exit(1);
  while(fscanf(g,"%s",FileReader)!=EOF)
  {
    if(checkfile(FileReader,head)==1) continue;//function checkfile traverse the list and checks if the name of the file has been given twice or more and in that case it continues with the next file
  	inputfiles(FileReader,&head);//function inputfiles is called after we make sure that the file is not given twice, and it puts the name of the file into the list head 
  	mainreader( FileReader,hashTable);//main reader is the basic function that opens the file,read the words and use other function to input the word in hashtable
    if ((FileReader=(char*)malloc(21*sizeof(char)))==NULL) exit(1);//we create agin space for the new name of file because in the previous we have placed a pointer that keeps
  }                                                                //the name of the file of each word in hashTable so we can't use agian
  fclose(g);
  finalsearch(hashTable);//function finalsearch is used to search the word and the coordinates of the word and the name of the file that contains the word 
  return 0;	
}




 void searchword(nodeptr hashTableVar,char* n)//this function finds if the word exists in the binary search tree and if yes then it calls searchword2 to find  which file contains the word  
 {
 	if(hashTableVar==NULL){
 		printf("not found");
 	}
 	else 
	{
		int i=strcmp(n,hashTableVar->Word);
		if(i==0) {searchword2(hashTableVar->right); }
        else if(i>0) searchword(hashTableVar->middle,n);  
 	    else searchword(hashTableVar->left,n);  
	 }	
 }

 void searchword2(nodeptr2 b)//this function calls searchword3 to print the name of the file and the line and column of the word in this file for every file in which the word exists
 {
 	if(b!=NULL){
 		searchword3(b->Word,b->left);
 		searchword2(b->right);
 	} 	
 }

void searchword3(char * word,nodeptr3 c)//thid function prints the name of the file and the line and column of the word
{
	if(c!=NULL){
		printf("%s(%d,%d)\n",word,c->line,c->column);
		searchword3(word,c->next);
	}
}



int number(int Bpower)//this function calculates the the arithetic term: (256^(n-1-i))mod1031
{
	if(Bpower==0) return 1;
	else return  (256*number(Bpower-1))%1031;
}


void input(nodeptr *hashTableVar,char *n,int column,int line,char *FileReader)// this is the function that places the word into the correct field of hashtable
{   
	 if((*hashTableVar)!=NULL){
	  int i;
	  i=strcmp(n,(*hashTableVar)->Word);	
	  if(i==0)
	  {
	  	
	    searchnode(&((*hashTableVar)->right),FileReader,column,line);	
	  	
	  }
	  else if(i<0)
	  {
	  	input(&((*hashTableVar)->left),n,column,line,FileReader);
	  }
	  else
	  {
	  	input(&((*hashTableVar)->middle),n,column,line,FileReader);
	  }
	}
	else if((*hashTableVar)==NULL)
    {
    
   	nodemaker(hashTableVar,n,column,line,FileReader);
    }
}

void searchnode(nodeptr2 *FileVar,char* FileReader,int column,int line)//this is the function that finds where should the word be placed in the binary search tree
{
	if ((*FileVar)==NULL){
        nodeptr2 n=(nodeptr2)malloc(sizeof(struct node2));
        nodeptr3 w=(nodeptr3)malloc(sizeof(struct node3));
		n->Word=(char*)malloc(21*sizeof(char));
		n->Word=FileReader;	
		n->right=NULL;
		w->next=NULL;
		n->left=w;
		((n->left)->column)=column;
		(n->left)->line=line;
		(n->left)->next=NULL;
		(*FileVar)=n;
	}
	else 
	{   
	    int i=strcmp((*FileVar)->Word,FileReader);
	    if(i==0){
	    	nodeptr3 m=(nodeptr3)malloc(sizeof(struct node3));
	    	m->column=column;
	    	m->line=line;
	    	m->next=NULL;
	        node3cmp(&	((*FileVar)->left),m ); 
	    }
	    else{
	    searchnode(&((*FileVar)->right),FileReader,column,line );	
	    }
	}    
	
}

void node3cmp(nodeptr3 * a,nodeptr3 b)//this is the function that places the struct with the column,line coordinates in the right place in the list
{
	if((*a)==NULL){
		(*a)=b;
	}
	else{
		node3cmp(&((*a)->next),b);
	}
}

void nodemaker(nodeptr *hashTableVar,char* n,int column,int line,char* FileReader)//function nodemaker is called if the filed of hashtable is null which means that we haven't put any words
{                                                                         //so it creates the appropriate structs and stores the appropriate information such as the name of the word 
	nodeptr a=(nodeptr)malloc(sizeof(struct node));
	nodeptr2 b=(nodeptr2)malloc(sizeof(struct node2));
	nodeptr3 c=(nodeptr3)malloc(sizeof(struct node3));
	a->Word=n;
	a->left=a->middle=NULL;
	(b->Word)=FileReader;
	b->right=NULL;
	c->column=column;
	c->line=line;
	c->next=NULL;
	b->left=c;
	a->right=b;
	(*hashTableVar)=a;
}
	
	
	
void finalsearch(nodeptr  hashTable[1031])//this is the function that reads the word that we want to search and finds the right hashnumber for this word
{
 
  char *n=(char*)malloc(60*sizeof(char));  
  scanf("%s",n);
  int l=strlen(n);
 
   int k=number2(n,l);
  searchword(hashTable[k],n);	
}	
	
int number2(char* n,int i)//number2 calculates where should we add the word,in which  hashtable field
{
  int j,k=0;	 
  for(j=0;j<i;j++){  
	k=k+((number(i-1-j)*(n[j]%1031))%1031);
  }
   k=k%1031;
   return k;
}	
	
void mainreader(char* FileReader,nodeptr hashTable[1031]) //this is the main function that reads the letters from each file and calls the function(input) that places the word into hashtable 
{   
    int i,column,line,k;//column,line are the column,line of the word at the file
    char *n;//n is the pointer that we use to read the letters of the word,from the file	
	FILE *f;//file f is a pointer to the file that we are going to open 
    if((f=fopen(FileReader,"rt"))==NULL) exit(1); //FileReader has been declared in main and contais the name of the file that each time we want to open
    if ((n=(char*)malloc(60*sizeof(char)))==NULL) exit(1);    
    line=1;//we start from the first line so line=1
    i=column=0;
    while ((n[i]=fgetc(f))!=EOF)
    {
      if(n[i]==' '){ 	 //if we find ' ' it means that the word has finished so we put the word into hashtable
   	     column++;
   	     k=number2(n,i);			     
		 n[i]='\0';//n[i] is space char
		 if(i>0 ){input(&hashTable[k],n,column,line,FileReader);}//we want i>0 for the case that we have more than one ' '(space char)          												 													                 		  
		 column=column+strlen(n);	
		 if(i==0) free(n); //we only free(n) when i==0 because we don't need the char" " but,we don't free(n) if i>0 because the pointer from the struct that containes the word at hashtable also has a pointer to the content of n 
		 if( (n=(char*)malloc(21*sizeof(char)))==NULL) exit(1);
		 i=0;//we start again counting
      }
      else if(n[i]=='\n'){  
	     column++;	    
		 k=number2(n,i);//number2 calculates where should we add the word,in which  hashtable field,when we read space char" " for a lot of times k would have the value 0 because 		 
	                    //n will contain only space char but it will not enter hashtable because of if(i>0) condition(when we read more than one space char i remains with the value 0)	
		 n[i]='\0';//we put at the n[i] where we had read the space char the end of the word
		 if(i>0) input(&hashTable[k],n,column,line,FileReader);
		 if(i==0) free(n);
		 i=0; 
		 column=0;
		 line++;
		 if((n=(char*)malloc(21*sizeof(char)))==NULL) exit(1);    	
      }
      else{
   	     i++;
      }
    }
    n[i]='\0';//we do the same thing as above for the last word that was not been put to the hashtable because the while loop had finished cause we found the EOF char 
    column++;//and the last word was not entered the hashTable But in the occasion that we have "\n" before the EOF  the final word would have entered the hashtable and i==0 so
            //because of the if(i>0) it will not enter twice	     
	if(i>0){ 
  	 k=number2(n,i); 
	 input(&hashTable[k],n,column,line,FileReader);
	 }
    fclose(f);
  }
		
	
void hashTablenull(nodeptr hashTable[1031])//hashtablenull give all the fields of array hastable the null value
{
	int i;
	for(i=0;i<1031; i++){
 	  hashTable[i]=NULL;
    }	
}

void inputfiles(char* FileReader,filesptr *head)//void inputfiles has as parameters the variant FileReader and the head of the file list and and adds a new node to the list 
{                                               //that contains the name of the file    
  filesptr newptr=(filesptr)malloc(sizeof(struct files));
  newptr->next=NULL;
  newptr->NameOfFile=(char*)malloc(21*sizeof(char));
  strcpy(newptr->NameOfFile,FileReader);
  if(*head==NULL){
   	*head=newptr;
  }
  else{
    filesptr curr=*head;
    while(curr!=NULL){
  	  if((curr)->next==NULL) {(curr)->next=newptr; break;}
 	  else (curr)=(curr)->next;
    } 	
  }
}

int checkfile(char* FileReader,filesptr head)//function checkfile traverse the list of the files that have read and opened and 
{                                           //   checks if the file that is going to open already has been read,in that case checkfile returns 1 
	if(head==NULL) return 0;
	else{
	 while(head!=NULL){
		int i=strcmp(head->NameOfFile,FileReader);
		if(i==0) return 1;
		head=head->next;
	 }  
	 return 0;
    }
}
