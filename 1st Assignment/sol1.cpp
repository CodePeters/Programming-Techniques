#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct number{ //this struct is used id order to find the sum of all texts for an author and how many texts the author has
	int counter;
	int sum;
} value;

struct AuthorRecord{
	char *textTitle;
	int NumberOfWords;
	long Download;
	struct AuthorRecord *next;
};

typedef struct AuthorRecord *AuthorRecordType;
   
typedef struct  {
	char *firstName;
	char *lastName;
	int idNumber;
	AuthorRecordType text;	
}AuthorType;  
 
struct MemberNodeStruct{
  	AuthorType *anAuthor;
    struct MemberNodeStruct *next;
};
  
typedef struct MemberNodeStruct *MemberNodeType;

MemberNodeType fread3();//function fread3 is the function that returns the list of authors(head) 
AuthorType* fread1();//the funtion fread1 reads the information(firstname ,lastname,idnumber) of an author
AuthorRecordType fread2(); //fread2 is the function that reads the texts and returns the list of texts(head1)
void insert2(MemberNodeType *,MemberNodeType);//this is the function that places the authors into the list of authors(head) in the right order
void insert(AuthorRecordType* head,AuthorRecordType q );//function which put texts in write order
struct number fnumber(MemberNodeType head);//function that returns the number of the texts and the sum of the texts for an author
void deletenode(MemberNodeType prev,MemberNodeType *r,MemberNodeType *head,MemberNodeType* q);//function that deletes the successful authors from the list of authors(head)
void fwrite(MemberNodeType head);//function that writes all the results 

FILE *f;//the declaration of the file f is out of the main so the funtions could read the inputs from the file

int main()
{	  
   	if((f=fopen("george_petrou.txt","rt"))==NULL) return 1;//opening of the file and see if it has opened else it will return 1  
	MemberNodeType head;//head is the pointer that points at the first node of the list of authors
	head =fread3();
    fwrite(head);
    fclose(f);  	 	
    return 0;
}

MemberNodeType fread3()//function fread3 is the function that returns the list of authors(head) 
{	
	AuthorType *p;
	MemberNodeType q,head=NULL;                                                             
	int N,i;                                                                          
	AuthorRecordType head1;
	
	fscanf(f,"%d",&N);//reading the number of authors
	for(i=1;i<=N;i++){
		q=(MemberNodeType)malloc(sizeof(struct MemberNodeStruct));
		if((q==NULL)) exit(1);
		p=fread1();//the funtion fread1 reads the information(firstname ,lastname,idnumber) of an author
		q->anAuthor=p;
		q->next=NULL;
		
		insert2(&head,q);//function insert2 places in right order each node, in which pointer q is pointing, into the list of authors(head)
       
	   
		head1=fread2();	  		 
		q->anAuthor->text=head1;//we pass the list of texts into q->anAuthor->text whict is the node that we have just put in the list of authors
    }
    return head;
}

AuthorType *fread1()//the funtion fread1 reads the information(firstname ,lastname,idnumber) of an author
{   
    AuthorType *p;
    
	p=(AuthorType*)malloc(sizeof(AuthorType));
 	if (p==NULL) exit(2);
	p->firstName=(char*)malloc(30*sizeof(char));
	p->lastName=(char*)malloc(30*sizeof(char));
	p->text=NULL;
	if((p->firstName==NULL) || (p->lastName==NULL)) exit(3);
	fscanf(f,"%s %s %d\n",p->firstName,p->lastName,&(p->idNumber));
	return p;		
} 

AuthorRecordType fread2()//fread2 is the function that reads the texts and returns the list of texts(head1)
{
	   int j,n;
	   AuthorRecordType head1,k;//head1 is the list which holds the texts for each author
	   
	   fscanf(f,"%d\n",&n);//reading the number of texts for each author
		head1=NULL;
		for(j=1;j<=n;j++){
 	    	k=(AuthorRecordType)malloc(sizeof(struct AuthorRecord));
 		    if(k==NULL) exit(4);
 	    	k->textTitle=(char*)malloc(100*sizeof(char));
 	    	if(k->textTitle==NULL) exit(5);
		    k->next=NULL;   	
 	       	fscanf(f,"%[^\n] %d\n",k->textTitle,&k->Download);	
 		    insert(&head1,k);//with insert function we make the sorted list of texts for an author	  
 	    }
	    return head1;
}

void insert2(MemberNodeType *head,MemberNodeType q)//function insert2 is used to make the sorted list of authors(head)
{
  int x;
  MemberNodeType prev,curr=*head;
  prev=NULL;
  if((*head)==NULL){
  *head=q;
  }
  else{
   	while(curr!=NULL){  	
	  x=strcmp(curr->anAuthor->lastName,q->anAuthor->lastName);		   			
      if((x>0)||(x==0)&&(strcmp(curr->anAuthor->firstName,q->anAuthor->firstName)>0)){
    	if(prev!=NULL){
	       q->next=prev->next;
		   prev->next=q;		    			
		   return;
		}					
		else{	
		   q->next=*head;
		   *head=q;
	   	   return;
	    }	   		
	  }  
	  else if(((x<0) && curr->next==NULL)||((x==0) && (curr->next==NULL))){ 
	    curr->next=q;
		 return;
	  }			
   	  else{prev=curr; curr=curr->next;  } 
    }
  } 
  return;    
}

void insert(AuthorRecordType *head1,AuthorRecordType q)//function insert is used to make the sorted list of texts for an author
{
  AuthorRecordType prev,curr=*head1;
  prev=NULL;
  int x;  
  if(*head1==NULL){
     *head1=q; 
  }
  else{
    while(curr!=NULL){  		
      x=strcmp(curr->textTitle,q->textTitle);
      if(x>0){
   		if(prev!=NULL){
         q->next=prev->next;
 	     prev->next=q;		    			
		 return;			
	    }		
	    else {
		 q->next=*head1;
		 *head1=q;
		 return;
	    } 
	 }
	 else if(x<0 & curr->next==NULL) {curr->next=q; return;}
	 else if(x==0) {  curr->Download=curr->Download+q->Download; return;}  
	 else {	 prev=curr; curr=curr->next;} 	  
   }
  }   	
  return;
}
 
void fwrite(MemberNodeType head)
{
	MemberNodeType prev,head2,curr;
	AuthorRecordType textcurr;
	int SuccessfulAuthors=0;
	MemberNodeType q;
	
	curr=head;
	head2=NULL;//head2 is the list of authors with average of downloads 10000 or more(succesful authors) 	
    prev=NULL;
 	while(curr!=NULL){	
 	    textcurr=curr->anAuthor->text;
 	    while(textcurr!=NULL){
 	   	  printf("%s %s %s %d \n",curr->anAuthor->lastName,curr->anAuthor->firstName, textcurr->textTitle, textcurr->Download);
 	 	  textcurr=textcurr->next;
 	     }
 	    value=fnumber(curr);// variable vaLue which is type: struct fnumber has been declared in the line 8, where we made struct fnumber
 	    if(value.counter==0){value.counter=1;}// value.counter==1 so we will dont have a problem with the expression "value.sum/value.counter" and also it wont change the result
		printf("%s %s %d %d \n",curr->anAuthor->lastName,curr->anAuthor->firstName,value.sum,value.sum/value.counter); 		           	 	 	 	 	 	
 	 	if(value.sum>10000){
		  SuccessfulAuthors=SuccessfulAuthors+1;	                                
 	      deletenode(prev,&curr,&head,&q);  
 	      insert2(&head2,q);//the function insert2 that we used for the list head of authors,we use it to make the list head2 of succesfull authors(head2 is same type as head)
 	    }
		else{
 	     prev=curr; curr=curr->next; 
	    }
		printf("\n");	      
    }
    printf("%d \n",SuccessfulAuthors);
    curr=head2;
    while (curr!=NULL){
       printf("%s %s %d \n",curr->anAuthor->lastName,curr->anAuthor->firstName,curr->anAuthor->idNumber);
	   curr=curr->next;
    }
    printf("END");	
}

struct number fnumber(MemberNodeType curr)//function that returns the number of the texts and the sum of the texts for each author
{                                         
 struct number fnumber1={0,0};
 while(curr->anAuthor->text!=NULL){//we traverse the list of texts for the current author(curr->anAuthor)
  	fnumber1.counter=fnumber1.counter+1;	
    fnumber1.sum =fnumber1.sum+curr->anAuthor->text->Download;
 	curr->anAuthor->text=curr->anAuthor->text->next;  	
 } 
 return fnumber1;	 	
}

void deletenode(MemberNodeType prev,MemberNodeType *curr,MemberNodeType *head,MemberNodeType * q)//function that deletes the successfull authors from the authors list
{ 	 		
 if(prev!=NULL){	 		
  prev->next=(*curr)->next;
  *q=*curr;
  *curr=(*curr)->next;
 } 
 else {
  *head=(*head)->next;
  *q=*curr;
  *curr=*head;
 }
 (*q)->next=NULL;
 return;
}
