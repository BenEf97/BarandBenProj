#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TRUE 1
#define FALSE 0

typedef struct{
	int year;
	char month;
	char day;
}DateOfBirth;
//I think we might need to make it as a pointer bc it might have more digits.

typedef struct{
	unsigned long id;
	char* name;
	char* family;
	DateOfBirth date;
	unsigned long partnerId;
	unsigned long MotherId;
	unsigned long FatherId;
	char NumOfChildren;
	long* childrenPtr;
}person;

//data base manager
typedef struct {
	person* per;
	int perCount;
}db_mgr;


void print_person(person* per);
char menu();
person* init_db(db_mgr* mgr1);
char intAsChar(int a);
int add_person(db_mgr* mgr);
char* enterName(char* name);
void arrangeId(db_mgr* mgr);
void swapPer(person* per1, person* per2);
long idInputCheck();
DateOfBirth inputDate();
int yearLeapChk(int year);
int dateChk(int mm, int dd, int yy);

void main()
{
	//debug:
	db_mgr manager;
	manager.perCount = 0;
	char option;
	do
	{
		option = menu();
		switch (option)
		{
		case '1':
			add_person(&manager);
			if (add_person == FALSE)
				break;
			continue;
		case '8':
			break;
		}

	} while (option != '8');
	
	for (int idx=0;idx<manager.perCount;idx++)
	free(manager.per+idx);
}

//Prints the person's info
void print_person(person* per)
{
	printf("ID: %ld\n", per->id);
	printf("First Name: %s\n", per->name);
	printf("Last Name: %s\n",per->family);
	printf("Date: %d/%d/%d \n",per->date.day,per->date.month,per->date.year);
	printf("Partner's ID: %ld\n",per->partnerId);
	printf("Mother's ID: %ld\n", per->MotherId);
	printf("Father's ID: %ld\n", per->FatherId);
	printf("Number of children: %d\n", per->NumOfChildren);
	if ((per->NumOfChildren) > 0)
	{
		for (int idx = 0; idx < per->NumOfChildren; idx++)
		{
			printf("Child No. %d:\n", idx + 1);
			printf("ID: %ld\n", per->childrenPtr[idx]);
		}
	}
}

char intAsChar(int a) {  //puts the value of int in char
	char b;
	b = (char) a;
	return b;
}

person* init_db(db_mgr* mgr1)
{ //check me dont know if im right  
	db_mgr mgr2;
	if (mgr1->perCount > 0)
	{

		mgr2.per = (person*)malloc(mgr1->perCount * sizeof(person));
		if (mgr2.per == NULL)
		{
			printf("Error! Out of memory!");
			return NULL;
		}
		if (mgr1->perCount > 2)
		{
			for (int i = 0; i < mgr1->perCount; i++) {

				swapPer(&mgr1->per[i], &mgr2.per[i]);
			}
			free(mgr1->per);
		}
		return mgr2.per;
	}
}


char menu()
{
	//menu function returns the value that entered
		int tmp = 1;
	char choice;
	printf("Database System Menu:\n1. Add person\n2. Search a person\n3. Search parents\n4. Delete a person\n5. Get generation\n6. Print database\n7. Search by name\n8. Quit\n");
	while (tmp)
	{
		fseek(stdin, 0, SEEK_END);
		scanf("%c", &choice);
		fseek(stdin, 0, SEEK_END);
		if (choice > '0'&&choice < '9') return choice;
		printf("error! invalid input. try again.\n");
	}
}

int add_person(db_mgr* mgr)
{ //not finished

	int index = mgr->perCount; //start at 0
	mgr->perCount++;
	mgr->per = init_db(mgr);
	if (mgr->per == NULL) return FALSE;
	printf("please enter a person details:\n");
	printf("ID: ");
	mgr->per[index].id = idInputCheck();
	printf("please enter name: ");
	mgr->per[index].name = enterName(mgr->per[index].name); //remove from sograim
	printf("please enter last name: ");
	mgr->per[index].date = inputDate();//row down after last name
	mgr->per[index].family = enterName(mgr->per[index].family);
	printf("Please enter your partner's ID: ");
	mgr->per[index].partnerId = idInputCheck();
	printf("Please enter your father's ID: ");
	mgr->per[index].FatherId = idInputCheck();
	printf("Please enter your mother's ID: ");
	mgr->per[index].MotherId = idInputCheck();
	printf("Please enter the number of children you have: ");
	scanf("%c", &mgr->per[index].NumOfChildren);
	if (mgr->per[index].NumOfChildren != '0')
	{
		mgr.per[index].childrenPtr[i] = (long*)malloc((mgr->per[index].NumOfChildren) * sizeof(long));
		for (int i = 0; i < mgr->per[index].NumOfChildren; i++)
		{
			printf("Please enter child's no. %d ID: ", i + 1);
			mgr->per[index].childrenPtr[i] = idInputCheck();
		}
	}

	arrangeId(mgr);
	return TRUE;
}

DateOfBirth inputDate()
{
	int day, month;
	DateOfBirth date;
	printf("Please enter date of birth in DD/MM/YY format: ");
	scanf("%d/%d/%d", &day, &month, &date.year);
	int chk = dateChk(month, day, date.year);
	while (chk=FALSE)
	{
		printf("Invalid date! try again.\n");
		scanf("%d/%d/%d", &day, &month, &date.year);
		chk = dateChk(month, day, date.year);
	}
	date.day = intAsChar(day);
	date.month =intAsChar(month);
	fseek(stdin, 0, SEEK_END);
	return date;
}


int dateChk(int mm, int dd, int yy)
{
	if (mm > 12 || dd < 0 || mm < 0) return FALSE;
	if (mm == 2 && dd <= 29)
	{
		if (dd == 29 && yearLeapChk(yy) == TRUE) return TRUE;
		return (dd <= 28) ? TRUE : FALSE;
	}
	if (mm == 1 || mm == 3 || mm == 5 || mm == 7 || mm == 8 || mm == 10 || mm == 12)
		return (dd <= 31) ? TRUE : FALSE;

	if (dd <= 30 && mm != 2) return TRUE;
	else return FALSE;
}

int yearLeapChk(int year)
{
	if (year % 400 == 0) return TRUE;
	else if (year % 100 == 0) return FALSE;
	else if (year % 4 == 0) return TRUE;
	else return FALSE;
}


long idInputCheck()
{
	long id;
	scanf("%ld", &id);
	while (id < 0 || id> 1000000000)
	{
		printf("Invalid input, please try again: ");
		scanf("%ld", &id);
	}
	fseek(stdin, 0, SEEK_END);
	return id;
}

char* enterName(char* name)
{ //reading and allocating name for add_person
	char tmp[100];
	int size;
	gets(tmp);
	size = strlen(tmp);
	fseek(stdin, 0, SEEK_END);
	name = (char*)malloc((size + 1)*sizeof(char));
	if(name==NULL) //we need to free that
	strcpy(name, tmp);
	return(name);
}

//function 1: id seeker 
void arrangeId(db_mgr* mgr)
{		

		for (int idx = mgr->perCount - 2; 0 <= idx ; idx--)
		{
			if (mgr->per[idx].id < mgr->per[idx - 1].id)
			{
				//[percount-1]     [percount-1-idx]
				swapPer(&mgr->per[mgr->perCount-1], &mgr->per[idx]);
				
			}
		}
}

void swapPer(person* per1, person* per2)
{
	person temp;
	temp.id = per1->id;
	temp.name = per1->name;
	temp.family = per1->family;
	temp.date.day = per1->date.day;
	temp.date.month = per1->date.month;
	temp.date.year = per1->date.year;
	temp.partnerId = per1->partnerId;
	temp.FatherId = per1->FatherId;
	temp.MotherId = per1->MotherId;
	temp.NumOfChildren = per1->NumOfChildren;
	/*if (per1->NumOfChildren > 0)
	{
		for (int idx = 0; idx < per1->NumOfChildren; idx++)
		{
			temp.childrenPtr[idx] = per1->childrenPtr[idx];
		}
	}
	else temp.childrenPtr = NULL;*/

	per1->id = per2->id;
	per1->name = per2->name;
	per1->family = per2->family;
	per1->date.day = per2->date.day;
	per1->date.month = per2->date.month;
	per1->date.year = per2->date.year;
	per1->partnerId = per2->partnerId;
	per1->FatherId = per2->FatherId;
	per1->MotherId = per2->MotherId;
	per1->NumOfChildren = per2->NumOfChildren;
	/*if (per2->NumOfChildren > 0)
	{
		for (int idx = 0; idx < per2->NumOfChildren; idx++)
		{
			per1->childrenPtr[idx] = per2->childrenPtr[idx];
		}
	}
	else per1->childrenPtr = NULL;*/

	per2->id = temp.id;
	per2->name = temp.name;
	per2->family = temp.family;
	per2->date.day = temp.date.day;
	per2->date.month = temp.date.month;
	per2->date.year = temp.date.year;
	per2->partnerId = temp.partnerId;
	per2->FatherId = temp.FatherId;
	per2->MotherId = temp.MotherId;
	per2->NumOfChildren = temp.NumOfChildren;
	/*if (temp.NumOfChildren > 0)
	{
		for (int idx = 0; idx < temp.NumOfChildren; idx++)
		{
			per2->childrenPtr[idx] = temp.childrenPtr[idx];
		}
	}
	else per2->childrenPtr = NULL;*/
}
