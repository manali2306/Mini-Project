#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
void input();
void writefile();
void search();
void output();
void new();
void additem();
typedef struct date
{
	   int month;
	   int day;
	   int year;
}date;
struct account 
{
	int number;
	char name[100];
	int acct_no;
	float mobile_no;
	char street[100];
	char city[100];
	char acct_type;
	float oldbalance;
	float newbalance;
	float payment;
	struct date lastpayment;
}customer;
typedef struct product
{
	char p_name[100];
	int p_id;
	float price;
}product;
int tl,sl,ts;
int main()
{
	int i,n;
	char ch;
	printf("   CUSTOMER BILLING SYSTEM:\n\n");
	printf("===============================\n");
	printf("\n1:    ADD NEW ACCOUNT\n");
	printf("\n2:    SEARCH EXISTING ACCOUNT\n");
        printf("\n3:    NEW PURCHASE\n ");
	printf("\n4:    EXIT\n");
	printf("\n================================\n");
	do
	{
		printf("\nselect what do you want to do?");
	       	scanf("%c", &ch);
	}while(ch<='0' || ch>'3');
	switch(ch)
	{
		case '1':
			printf("\nhow many customer accounts?");
			scanf("%d",&n);
			for(i=0;i<n;i++)
			{
				input();
				if(customer.payment>0)
					customer.acct_type=(customer.payment<0.1*customer.oldbalance)? 'O': 'D';
				else
						customer.acct_type=(customer.oldbalance>0)?'D' : 'C';
				customer.newbalance=customer.oldbalance - customer.payment;
				writefile();
			}
			main();
		case '2':
			printf("search by what?\n");
			printf("\n1 --- search by customer number\n");
			printf("2 --- search by customer name\n");
			search();
			scanf("%c", &ch);
			main();
		case '3':
			new();
		case '4':
			exit(1);
	  }
}
void input()
{
	FILE *fp=fopen("bidur.dat","w+");
	fseek (fp,0,SEEK_END);
	tl=ftell(fp);
	sl=sizeof(customer);
	ts=tl/sl;
	fseek(fp,(ts-1)*sl,SEEK_SET);
	fread(&customer,sizeof(customer),1,fp);
	printf("\ncustomer no:%d\n",++customer.number);
	fclose(fp);
	printf("         Account number:");
	scanf("%d",&customer.acct_no);
	printf("\n       Name:");
	scanf("%s",customer.name);
	printf("\n       mobile no:");
	scanf("%f",&customer.mobile_no);
	printf("         Street:");
	scanf("%s",customer.street);
	printf("         City:");
	scanf("%s",customer.city);
	return;
}
void writefile()
{
	FILE *fp;
	fp=fopen("bidur.dat","w+");
	fwrite(&customer,sizeof(customer),1,fp);
	fclose(fp);
	return;
}
void search()
{
	char ch;
	char nam[100];
	int n,i,m=1;
	FILE *fp;
	fp=fopen("bidur.dat","rb");
	do
	{
		printf("\nenter your choice:");
		scanf("%c", &ch);
	 }while(ch!='1' && ch!='2');
	 switch(ch)
	{
	      	case '1':
		    	fseek(fp,0,SEEK_END);
		    	tl=ftell(fp);
		    	sl=sizeof(customer);
		    	ts=tl/sl;
		    	do
			{
				printf("\nchoose customer number:");
				scanf("%d",&n);
				if(n<=0 || n>ts)
					printf("\nenter correct\n");
				else
				{
			    		fseek(fp,(n-1)*sl,SEEK_SET);
			    		fread(&customer,sl,1,fp);
			    		output();
				}
				printf("\n\nMAIN MENU AGAIN?(y/n)");
				scanf("%c", &ch);
		    	}while(ch=='y');
		    	fclose(fp);
		    	break;
	      	case '2':
		    	fseek(fp,0,SEEK_END);
		    	tl=ftell(fp);
		    	sl=sizeof(customer);
		    	ts=tl/sl;
		    	fseek(fp,(ts-1)*sl,SEEK_SET);
		    	fread(&customer,sizeof(customer),1,fp);
		   	n=customer.number;
			do
			{
				printf("\nenter the name:");
				scanf("%s",nam);
				fseek(fp,0,SEEK_SET);
				for(i=1;i<=n;i++)
				{
			     		fread(&customer,sizeof(customer),1,fp);
			     		if(strcmp(customer.name,nam)==0)
			     		{
						output();
						m=0;
						break;
			     		}
				}
				if(m!=0)
					printf("\n\ndoesn't exist\n");
				printf("\nanother?(y/n)");
				scanf("%c",&ch);
		    	}while(ch=='y');
		    	fclose(fp);
	}
	return;
}
void output()
{
	printf("\n\n    Customer no    :%d\n",customer.number);
	printf("    Name 	   :%s\n",customer.name);
	printf("    Mobile no      :%.f\n",customer.mobile_no);
	printf("    Account number :%d\n",customer.acct_no);
	printf("    Street         :%s\n",customer.street);
	printf("    City           :%s\n",customer.city);
	printf("    Account status :");
	switch(customer.acct_type)
	{
	      	case 'C':
		 	printf("CURRENT\n\n");
		 	break;
	      	case 'O':
			printf("OVERDUE\n\n");
		 	break;
	      	case 'D':
		 	printf("DELINQUENT\n\n");
		 	break;
	      	default:
		 	printf("ERROR\\n\n");
	}
	return;
}
void new()
{
	int i, n, sum;
	float bill;
	float disc;
	product pd[500];
        printf("Enter number of items:\n");
        scanf("%d", &n);
        printf("Enter Item Name, Id and Price\n");
        for(i = 0; i < n; i++)
        	scanf("%s %d %f", pd[i].p_name, &pd[i].p_id, &pd[i].price );
        for(i = 0; i < n; i++)
        {
 		sum = sum + pd[i].price;
 	}
 	if(sum > 1000)
 		disc = sum * 0.1;
 	else if(sum > 500)
 		disc = sum * 0.05;
 	else
 		disc = 0;
 	bill = sum - disc;
 	printf("\nYour Bill is Rs.%9.2f", bill);
}
			












