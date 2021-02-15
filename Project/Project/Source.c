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

typedef struct {
	person* per;
	int perCount;
	int userCount;
}db_mgr;

void print_person(person* per);
char menu();
void init_db(db_mgr* mgr);
person* db_MemoryRealloc(db_mgr* mgr1);
void init_ChildPtr(person* per);
char intAsChar(int a);
void add_person(db_mgr* mgr);
char* enterName();
void arrangeId(db_mgr* mgr);
void swapPer(person* per1, person* per2);
long idInputCheck();
DateOfBirth inputDate();
int yearLeapChk(int year);
int dateChk(int mm, int dd, int yy);
int dataBaseCheck(int perCount);
person* search_id(db_mgr* mgr, unsigned long id);
person* ptrForPerson(db_mgr* mgr);
void search_person(db_mgr* mgr);
void search_parents(db_mgr* mgr);
void delete_person(db_mgr* mgr);
void child_Deleter(person* parent, person* child);
void quit(db_mgr* mgr);
void abort_Program();
void db_Free(db_mgr* mgr);
void print_db(db_mgr* mgr);

void main()
{
	//debug:
	db_mgr manager = { NULL,0, 0};
	printf("Please enter how many people you wish to be in the data base: ");
	scanf("%d", &manager.userCount);
	fseek(stdin, 0, SEEK_END);
	init_db(&manager);
	char option;
	do
	{
		option = menu();
		switch (option)
		{
		case '1':
				add_person(&manager);
			continue;
		case '2':
			if (dataBaseCheck(manager.perCount))
				search_person(&manager);
			continue;
		case '3':
			if (dataBaseCheck(manager.perCount))
				search_parents(&manager);
			continue;
		case '4':
			if (dataBaseCheck(manager.perCount))
				delete_person(&manager);
			continue;
		case '6':
			if (dataBaseCheck(manager.perCount))
				print_db(&manager);
			continue;
		case '8':
			quit(&manager);
			break;
		}

	} while (option != '8');
	system("pause");
	}

void print_db(db_mgr* mgr) {
	printf("Printing all the information:\n");
	for(int i=0;i<mgr->perCount;i++)
	{
		print_person(&mgr->per[i]);
	}
}

//If an allocation fails, the program will display an error output and exits
void abort_Program()
{
	printf("Error! out of memory!");
	exit(1);
}

//Prints the person's info
void print_person(person* per)
{
	printf("\nID: %ld\n", per->id);
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

void init_db(db_mgr* mgr)
{
	while (mgr->userCount <= 0)
	{
		printf("Invalid input. Please try again: ");
		scanf("%d", &mgr->userCount);
		fseek(stdin, 0, SEEK_END);
	}
	mgr->per = (person*)malloc(mgr->userCount * sizeof(person));
	if (mgr->per == NULL)
		abort_Program();
	//	printf("Error! Out of memory!");
}


person* db_MemoryRealloc(db_mgr* mgr1)
{ //check me dont know if im right  
	db_mgr mgr2;
	if (mgr1->perCount > 0)
	{
		if (mgr1->perCount < mgr1->userCount)
		{
			mgr1->userCount--;
			mgr2.per = (person*)malloc(mgr1->userCount * sizeof(person));
			if (mgr2.per == NULL)
				abort_Program;
		}
		else
		{
			mgr2.per = (person*)malloc(mgr1->perCount * sizeof(person));
			if (mgr2.per == NULL)
				abort_Program();
		}
		if (mgr1->perCount > 1)
		{
			if (mgr1->per[0].id)
			{
				for (int i = 0; i < mgr1->perCount - 1; i++)
				{
					swapPer(&mgr1->per[i], &mgr2.per[i]);
				}
				free(mgr1->per);
			}
			else
			{
				for (int i = 0; i < mgr1->perCount; i++)
				{
					swapPer(&mgr1->per[i + 1], &mgr2.per[i]);
				}
				free(mgr1->per[0].childrenPtr);
				free(mgr1->per[0].name);
				free(mgr1->per[0].family);
				free(mgr1->per);
			}
			return mgr2.per;
		}
	}
}

long* realloc_ChildPtr(person* per)
{
	person newper;
	init_ChildPtr(&newper);
	//newChildPtr = (long*)malloc(per->NumOfChildren * sizeof(long));
	//if (newChildPtr == NULL)
	//{
	//	printf("Error! Out of memory!");
	//	return NULL;
	//}
	for (int idx = 0; idx < per->NumOfChildren; idx++)
	{
		newper.childrenPtr[idx] = per->childrenPtr[idx];
	}
	free(per->childrenPtr);
	return newper.childrenPtr;
}

void init_ChildPtr(person* per)
{
	per->childrenPtr = (long*)malloc(per->NumOfChildren * sizeof(long));
	if (per->childrenPtr == NULL)
	{
		abort_Program();
		//printf("Error! Out of memory!");
		//exit(1);
	}
}

char menu()
{
	//menu function returns the value that entered
		int tmp = 1;
	char choice;
	printf("\n\t\t***Database System Menu***\n1. Add person\n2. Search a person\n3. Search parents\n4. Delete a person\n5. Get generation\n6. Print database\n7. Search by name\n8. Quit\n\n");
	while (tmp)
	{
		fseek(stdin, 0, SEEK_END);
		scanf("%c", &choice);
		fseek(stdin, 0, SEEK_END);
		if (choice > '0'&&choice < '9') return choice;
		printf("Error! Invalid input. Please try again.\n");
	}
}

void add_person(db_mgr* mgr)
{
	printf("**Add Person**\n");
	int index = mgr->perCount;
	mgr->perCount++;
	if (mgr->perCount > mgr->userCount)
		mgr->per = db_MemoryRealloc(mgr);
	printf("Please enter a person details:\n");
	printf("Please enter ID: ");
	mgr->per[index].id = idInputCheck();
	printf("Please enter name: ");
	mgr->per[index].name = enterName();
	printf("Please enter last name: ");
	mgr->per[index].family = enterName();
	mgr->per[index].date = inputDate();
	printf("Please enter your partner's ID: ");
	mgr->per[index].partnerId = idInputCheck();
	printf("Please enter your father's ID: ");
	mgr->per[index].FatherId = idInputCheck();
	printf("Please enter your mother's ID: ");
	mgr->per[index].MotherId = idInputCheck();
	printf("Please enter the number of children you have: ");
	int tmp;
	scanf("%d",&tmp);
	mgr->per[index].NumOfChildren = intAsChar(tmp);
	if (mgr->per[index].NumOfChildren > 0)
	{
		//mgr->per[index].childrenPtr = (long*)malloc((mgr->per[index].NumOfChildren) * sizeof(long));//debug
		init_ChildPtr(&mgr->per[index]);
		for (int i = 0; i < mgr->per[index].NumOfChildren; i++)
		{
			printf("Please enter child's no. %d ID: ", i + 1);
			mgr->per[index].childrenPtr[i] = idInputCheck();
		}
	}
	else mgr->per[index].childrenPtr = NULL;
	arrangeId(mgr);
}

DateOfBirth inputDate()
{
	int day, month;
	DateOfBirth date;
	printf("Please enter date of birth in DD/MM/YY format: ");
	scanf("%d/%d/%d", &day, &month, &date.year);
	int chk = dateChk(month, day, date.year);
	while (!chk)
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
	if (yy > 9999 || yy < 1000) return FALSE;
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

unsigned long idInputCheck()
{
	unsigned long id;
	scanf("%ld", &id);
	while (id < 0)
	{
		printf("Invalid input, please try again: ");
		scanf("%ld", &id);
	}
	fseek(stdin, 0, SEEK_END);
	return id;
}

char* enterName()
{ //reading and allocating name for add_person
	char* name;
	char tmp[100];
	int size;
	gets(tmp);
	size = strlen(tmp);
	fseek(stdin, 0, SEEK_END);
	name = (char*)malloc((size + 1)*sizeof(char));
	if (name == NULL)
		abort_Program();
	//{
	//	printf("Out of memory!");
	//	return NULL;
	//}
	strcpy(name, tmp);
	return(name);
}

void arrangeId(db_mgr* mgr)
{
	for (int idx = mgr->perCount - 1; 0 < idx; idx--)
	{
		if (mgr->per[idx].id < mgr->per[idx - 1].id)
		{
			swapPer(&mgr->per[idx], &mgr->per[idx - 1]);
		}
	}
}

void swapPer(person* per1, person* per2)//debug with childrenPtr
{
	person temp;
	temp = *per1;
	*per1 = *per2;
	*per2 = temp;

	//temp.id = per1->id;
	//temp.name = per1->name;
	//temp.family = per1->family;
	//temp.date.day = per1->date.day;
	//temp.date.month = per1->date.month;
	//temp.date.year = per1->date.year;
	//temp.partnerId = per1->partnerId;
	//temp.FatherId = per1->FatherId;
	//temp.MotherId = per1->MotherId;
	//temp.NumOfChildren = per1->NumOfChildren;
	//temp.childrenPtr = per1->childrenPtr;

	//per1->id = per2->id;
	//per1->name = per2->name;
	//per1->family = per2->family;
	//per1->date.day = per2->date.day;
	//per1->date.month = per2->date.month;
	//per1->date.year = per2->date.year;
	//per1->partnerId = per2->partnerId;
	//per1->FatherId = per2->FatherId;
	//per1->MotherId = per2->MotherId;
	//per1->NumOfChildren = per2->NumOfChildren;
	//per1->childrenPtr = per2->childrenPtr;

	//per2->id = temp.id;
	//per2->name = temp.name;
	//per2->family = temp.family;
	//per2->date.day = temp.date.day;
	//per2->date.month = temp.date.month;
	//per2->date.year = temp.date.year;
	//per2->partnerId = temp.partnerId;
	//per2->FatherId = temp.FatherId;
	//per2->MotherId = temp.MotherId;
	//per2->NumOfChildren = temp.NumOfChildren;
	//per2->childrenPtr = temp.childrenPtr;
	
}

person* search_id(db_mgr* mgr, unsigned long id)
{
	person* ptr = mgr->per;
	int endIdx = mgr->perCount - 1;
	int idx = 0;
	if (ptr[idx].id <= id && ptr[endIdx].id >= id)
	{
		int midIdx = (endIdx / 2)+1;
		while (idx <= endIdx)
		{
			if (ptr[idx].id == id)
			{
				ptr = &mgr->per[idx];
				return ptr;
			}
			if (ptr[endIdx].id == id)
			{
				ptr = &mgr->per[endIdx];
				return ptr;
			}
			if (ptr[idx].id < id&&ptr[midIdx].id >= id)
			{
				endIdx = midIdx;
				midIdx /= 2;
				idx++;
			}
			else
			{
				idx = midIdx;
				midIdx += idx / 2;
				endIdx--;
			}
		}
		return NULL;
	}
	else return NULL;
}
person* ptrForPerson(db_mgr* mgr)
{
	long id=idInputCheck();
	person* ptr = search_id(mgr,id);
	if (ptr == NULL)
	{
		printf("The ID is not found in the data base.\n");
		return NULL;
	}
	else return ptr;
}

void search_person(db_mgr* mgr)
{
		person* ptr;
		printf("**Search Person**\nPlease enter ID: ");
		ptr = ptrForPerson(mgr);
		if (ptr != NULL)
		{
			print_person(ptr);
		}
}

void search_parents(db_mgr* mgr)
{
	printf("**Search Parents**\nPlease enter the ID the person you wish to get the info of their parents: ");
	person* ptr1 = ptrForPerson(mgr);
	if (ptr1 != NULL)
	{
		person* ptr2 = ptr1;
		if (ptr1->FatherId)
		{
			ptr1 = search_id(mgr, ptr1->FatherId);
			if (ptr1 != NULL)
				print_person(ptr1);
			else printf("The father's ID doesn't exist in the data base.\n");
		}
		else printf("The person doesn't have a father.\n");
		if (ptr2->MotherId)
		{
			ptr2 = search_id(mgr, ptr2->MotherId);
			if (ptr2 != NULL)
				print_person(ptr2);
			else printf("The mother's ID doesn't exist in the data base.\n");
		}
		else printf("The person doesn't have a mother.\n");
	}
	
}

//checking if the data base is empty or not. If empty it will return FALSE,else TRUE
int dataBaseCheck(int perCount)
{
	if (perCount > 0)
		return TRUE;
	else
	{
		printf("Error! The data base is empty!\n");
		return FALSE;
	}
}


void delete_person(db_mgr* mgr)
{
	printf("**Delete Person**\nPlease eneter the ID you wish to delete: ");
	person* ptr = ptrForPerson(mgr);
	if (ptr != NULL)
	{
		if (ptr->FatherId)
		{
			person* ptrFather = search_id(mgr, ptr->FatherId);
			if (ptrFather)
				child_Deleter(ptrFather, ptr);
		}
		if (ptr->MotherId)
		{
			person* ptrMother = search_id(mgr, ptr->MotherId);
			if (ptrMother)
				child_Deleter(ptrMother, ptr);
		}
		if (ptr->partnerId)
		{
			person* ptrPartner = search_id(mgr, ptr->partnerId);
			ptrPartner->partnerId = 0;
		}
		if (ptr->NumOfChildren)
		{
			for (int idx = 0; idx < ptr->NumOfChildren; idx++)
			{
				person* ptrChild = search_id(mgr, ptr->childrenPtr[idx]);
				if (ptrChild)
				{
					if (ptrChild->FatherId == ptr->id)
						ptrChild->FatherId = 0;
					if (ptrChild->MotherId == ptr->id)
						ptrChild->MotherId = 0;
				}
			}
		}
		ptr->id = 0;
		arrangeId(mgr);
		mgr->perCount--;
		mgr->per=db_MemoryRealloc(mgr);
	}
}

void child_Deleter(person* parent,person* child)
{
	int position=0;
	for (int idx = 0; idx < parent->NumOfChildren; idx++)
	{
		if (parent->childrenPtr[idx] == child->id)
		{
			position = idx;
			parent->childrenPtr[idx] = 0;
		}

	}
	long* temp;
	for (; position < parent->NumOfChildren - 1; position++)
	{

		temp = parent->childrenPtr[position];
		parent->childrenPtr[position] = parent->childrenPtr[position + 1];
		parent->childrenPtr[position + 1] = temp;
	}
	parent->NumOfChildren--;
	if (parent->NumOfChildren == 0)
	{
		free(parent->childrenPtr);
		parent->childrenPtr = NULL;
	}
	else
	{
		realloc_ChildPtr(parent);
	}
}

void quit(db_mgr* mgr)
{
	printf("Quitting the program!\n");
	db_Free(mgr);
}

void db_Free(db_mgr* mgr)
{
	for (int idx = 0; idx < mgr->perCount; idx++)
	{
		free(mgr->per[idx].name);
		free(mgr->per[idx].family);
		free(mgr->per[idx].childrenPtr);
	}
	free(mgr->per);
}
