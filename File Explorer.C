#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<unistd.h>
#include<time.h>

FILE *fp1=NULL;
char fname[50],ch;
//result==0,if a particular file is present
int result;


struct node
{ 
  char data[50];
  struct node *ltree;
  struct node *rtree;
} *root=NULL,*current,*previous;


void addnode()
{
  struct node *temp;
  temp=(struct node*)malloc(sizeof(struct node));
  strcpy(temp->data,fname);
  temp->ltree=NULL;
  temp->rtree=NULL;
  current=root;
  if(root==NULL)
  {
    root=temp;
    return;
  }
  else
  {
    while(current!=NULL)
    {
      result=strcmp(current->data,fname);
      previous=current;
      if(result>0)
        current=current->ltree;
      else
        current=current->rtree;
    }
    if(result>0)
      previous->ltree=temp;
    else
      previous->rtree=temp;
  }
  current=temp;
}

//input_f2BST copies the file names of pre existing files into the linked list
void input_f2BST()
{
  fp1=fopen("filenames.txt","r");
  if(fp1==NULL)
    printf("\nError retrieving pre existing files\n");
  else
  {
    while(!feof(fp1))
    {
      fscanf(fp1,"%s",fname);
      addnode();
    }
  }
  fclose(fp1);
}

void search()
{
  current=root;
  previous=NULL;
  result=2;
  while(result!=0&&current!=NULL)
  {
    result=strcmp(current->data,fname);
    if(result>0)
    {
      previous=current;
      current=current->ltree;
    }
    if(result<0)
    {
      previous=current;
      current=current->rtree;
    }
  }
  if(result==0)
    printf("\nFile Found\n");
  else
    printf("\nFile doesn't exist\n");
}

void Delete()
{
  struct node *successor,*Psuccessor,*q;
  if(current->ltree!=NULL&&current->rtree!=NULL)
  {
    successor=current->rtree;
    while(successor->ltree!=NULL)
    {
      Psuccessor=successor;
      successor=successor->ltree;
    }
    Psuccessor->ltree=successor->rtree;
    successor->ltree=current->ltree;
    successor->rtree=current->rtree;
    if(previous!=NULL)
    {
      if(previous->ltree==current)
        previous->ltree=successor;
      else
        previous->rtree=successor;
    }
    else
      root=successor;
    free(current);
    remove(fname);
  }
  else
  {
    if(current->rtree==NULL)
      q=current->ltree;
    else
      q=current->rtree;
    if(previous!=NULL)
    {
      if(previous->rtree==current)
        previous->rtree=q;
      else
        previous->ltree=q;
    }
    else
      root=q;
    free(current);
    remove(fname);
  }
  printf("\nFile successfully deleted\n");
  return;
}

void fileview ()
{
	fp1=fopen(fname,"r");
  if (fp1==NULL)
	{
		fclose(fp1);
		printf("\nError in opening the file\n");
		return;
	}
  while(!feof(fp1))
	{
		fscanf(fp1,"%c",&ch);
		printf("%c",ch);
	}
	printf("\n------------------END OF FILE------------------\n");
  fclose(fp1);
	
}

void fileappend ()
{
	fp1=fopen(fname,"a");
	if (fp1==NULL)
	{
		fclose(fp1);
		printf("\nError in opening the file\n");
		return;
	}
	else
	{
		printf("\nWrite the text to be written, Press tab once done\n");
		while(ch!='\t')
		{
			scanf("%c",&ch);
			fprintf(fp1,"%c",ch);
		}
		printf("\nSuccessfully written on file\n");
	}
	fclose(fp1);
	return;
}


void filecreate ()
{
	fp1=fopen(fname,"w");
	printf("\nFile successfully created\n");
	fclose(fp1);
	addnode();
	return;
}

void attributes()
{
  struct stat properties;
  if (stat(fname, &properties) == -1)
  {
    perror("stat");
    return;
  }
  
  printf("\nI-node number:            %ld\n",(long) properties.st_ino);
  printf("Mode:                     %lo (octal)\n",(unsigned long) properties.st_mode);
  printf("Link count:               %ld\n", (long) properties.st_nlink);
  printf("Ownership:                UID=%ld   GID=%ld\n",(long) properties.st_uid, (long) properties.st_gid);
  printf("File size:                %lld bytes\n",(long long) properties.st_size);
  printf("Last status change:       %s\n", ctime(&properties.st_ctime));
  printf("Last access time:         %s\n", ctime(&properties.st_atime));
  printf("Last modification time:   %s\n", ctime(&properties.st_mtime));

}

//Inorder traversal
void display(struct node *R)
{
  if(R!=NULL)
	{
	  display(R->ltree);
	  printf("%s\n", R->data);
	  display(R->rtree);
	}
}

//output_BST2f updates the filenames.txt file from linked list data
void output_BST2f(struct node *R)
{
  if(R!=NULL)
	{
	  output_BST2f(R->ltree);
	  fprintf(fp1,"\n%s",R->data);
	  output_BST2f(R->rtree);
	}
}


int main()
{
  int choice;
  fflush(stdin);
  input_f2BST();
  while(1)
  {
    MARK01:printf("\n**********MAIN MENU**********\n");
    printf("\n1. List all the files\n");
    printf("2. Enter the file name to carry out various operations\n");
    printf("3. Exit\n\n");
    printf("OPTION:");
    scanf("%d",&choice);
    printf("\n\n");
    switch(choice)
    {
      case 1:
        display(root);
        goto MARK01;
        break;
        
      case 2:
        printf("Enter the file name\n");
        scanf("%s",fname);
        search();
        if(result==0) //When file is present
        {
          MARK02:while(1)
          {
            printf("\n\n1)Delete\n2)View\n3)Append\n4)File Properties\n5)Go Back\n6)Exit\n\nOPTION:");
            scanf("%d",&choice);
            printf("\n\n");
            switch(choice)
            {
              case 1:
                Delete();
                goto MARK01;
                break;
          
              case 2:
                fileview();
                break;
              
              case 3:
                fileappend();
                break;
                
              case 4:
                attributes();
                break;
                
                
              case 5:
              	goto MARK01;
               
               
              case 6:
                fp1=fopen("filenames.txt","w");
                output_BST2f(root);
                fclose(fp1);
                exit(0);
                
                
              default:
                printf("\nInvalid Input\n");
                break;
            }
          }
        }
        else
        {
          while(1)
          {
            printf("Do you want to create it? (0/1)\n");
            scanf("%d",&choice);
            switch(choice)
            {
              case 0:
                goto MARK01;
                break;
                
              case 1:
                filecreate();
                goto MARK02;
                break;
                
              default:
                printf("\nInvalid Input\n");
                break;
            }
         }
        }
        
      case 3:
        fp1=fopen("filenames.txt","w");
        output_BST2f(root);
        fclose(fp1);
        exit(0);
        
      default:
          printf("\nInvalid Input\n");
          break;
    }
  }
  return 0;
  
}
