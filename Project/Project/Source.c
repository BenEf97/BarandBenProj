#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
//char menu();
person* init_db(db_mgr* mgr1);
//void init_db(db_mgr* mgr);
char intAsChar(int a);
void add_person(db_mgr* mgr);
char* enterName(char* name);
void arrangeId(db_mgr* mgr);
void swapPosition(person* per1, person* per2);

void main()
{
	//debug:
	db_mgr manager = { NULL,0 };
	add_person(&manager);
	add_person(&manager);
	add_person(&manager);
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
		for (int i = 0; i < mgr1->perCount; i++) {
			//mgr2.per[i].id = mgr1->per[i].id; //error
			swapPosition(&mgr2.per[i], &mgr1->per[i]);
		}
	free(mgr1->per);
	}
	return mgr2.per;
}



//the function didn't work so rn just wanna check without it
char menu()
{
	//menu function returns the value that entered
		int tmp = 1;
	char choice;
	while (tmp)
	{
		printf("Database System Menu:\n1. Add person\n2. Search a person\n3. Search parents\n4. Delete a person\n5. Get generation\n6. Print database\n7. Search by name\n8. Quit\n");
		scanf("%c", &choice);
		if (choice > '0'&&choice < '9') return choice;
		printf("error! invalid input. try again.\n");
	}
}

void add_person(db_mgr* mgr)
{ //not finished

	int index = mgr->perCount; //start at 0
	mgr->perCount++;
	mgr->per = init_db(mgr);
//	person man;
	printf("please enter a person details:\n");
	printf("ID: ");
//	scanf("%ld", &man.id);
	//while (man.id < 0 || man.id>9)
	//{
	//	printf("invalid input. try again.");
	//	scanf("%ld", &man.id);
	//}
	scanf("%ld", &mgr->per[index].id);
	fseek(stdin, 0, SEEK_END);
	while (mgr->per[index].id < 0 && mgr->per[index].id > 1000000000 )
	{
		printf("invalid input. try again.");
		scanf("%ld", &mgr->per[index].id);
	}
	printf("please enter name: ");
	//man.name = enterName;
	
	//enterName(&(mgr->per[index].name));
	mgr->per[index].name = enterName(&mgr->per[index].name);
	printf("please enter last name: ");
	//man.family = enterName;
	mgr->per[index].family = enterName(&(mgr->per[index].family));
	arrangeId(mgr);
}

//I think we should use it differently. I mean that we won't have to use the p1, and just allocate memory and copy the new string.
//I didn't manage to do that, we should take a look at it.
char* enterName(char* name)
{ //reading and allocating name for add_person
//	person p1;
	char tmp[100];
	int size;
	gets(tmp);
	size = strlen(tmp);
	fseek(stdin, 0, SEEK_END);
	name = (char*)malloc((size + 1)*sizeof(char)); //we need to free that
	strcpy(name, tmp);
	return(name);
}

//int arrangeId(db_mgr* mgr,int num) {
//	int i;
//	for (i = 0; i < mgr->perCount; i++) {
//		if (i == mgr->perCount - 1) return i;
//		if (mgr->per[i].id < num) i++;
//		if (mgr->per[i].id > num) {
//			for()
//		}
//
//	}
//}

//function 1: id seeker
void arrangeId(db_mgr* mgr)
{
		for (int idx = mgr->perCount - 1; 0 < idx ; idx--)
		{
			if (mgr->per[idx].id < mgr->per[idx - 1].id)
			{
				//swapPosition(&mgr->per[idx], &mgr->per[idx - 1]);
				person temp = mgr->per[idx];
				mgr->per[idx] = mgr->per[idx - 1];
				mgr->per[idx - 1] = temp;
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
	if (per1->NumOfChildren > 0)
	{
		for (int idx = 0; idx < per1->NumOfChildren; idx++)
		{
			temp.childrenPtr[idx] = per1->childrenPtr[idx];
		}
	}
	else temp.childrenPtr = NULL;

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
	if (per2->NumOfChildren > 0)
	{
		for (int idx = 0; idx < per2->NumOfChildren; idx++)
		{
			per1->childrenPtr[idx] = per2->childrenPtr[idx];
		}
	}
	else per1->childrenPtr = NULL;

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
	if (temp.NumOfChildren > 0)
	{
		for (int idx = 0; idx < temp.NumOfChildren; idx++)
		{
			per2->childrenPtr[idx] = temp.childrenPtr[idx];
		}
	}
	else per2->childrenPtr = NULL;

}
