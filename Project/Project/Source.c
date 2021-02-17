#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TRUE 1
#define FALSE 0

//Date of birth Struct
typedef struct{
	int year;
	char month;
	char day;
}DateOfBirth;

//Q1: Person Struct
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
//Q3a: db_mgr Struct
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
unsigned long idInputCheck();
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
person* relative_Search(db_mgr* mgr, unsigned long id);
int search_by_name(db_mgr* mgr);
void get_gen(db_mgr* mgr);

void main()
{
	db_mgr manager = { NULL,0, 0};
	printf("\t\t***Data Base Manager STARTED***\n\n");
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
		case '5':
			if (dataBaseCheck(manager.perCount))
				get_gen(&manager);
			continue;
		case '6':
			if (dataBaseCheck(manager.perCount))
				print_db(&manager);
			continue;
		case '7':
			if (dataBaseCheck(manager.perCount))
				search_by_name(&manager);
			continue;
		case '8':
			quit(&manager);
			break;
		}
	} while (option != '8');
	system("pause");
	}

//Q6: Printing all the data base information
void print_db(db_mgr* mgr)
{
	printf("**Print Data Base**\nPrinting all the information:\n");
	printf("There are %d people in the DataBase\n", mgr->perCount);
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

//Q2: print_person: Printing the person's information
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

//Puts the value of int in char
char intAsChar(int a)
{ 
	char b;
	b = (char) a;
	return b;
}

//Q2: Init data base: allocating memory for the database of requested size from the user.
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
}
/*If there is a need to reallocate memory it will work in 2 cases:
1) User ask to add more people beyond the initial size
2) Delete person*/
person* db_MemoryRealloc(db_mgr* mgr1)
{  
	//If the user deleted all the data base and decided to add, it will allocate a new data base
	if (mgr1->per == NULL)
	{
		mgr1->per = (person*)malloc(mgr1->perCount * sizeof(person));
		return mgr1->per;
	}
	db_mgr mgr2;

	//If the user want to delete a person, and need to reallocate the memory, without passing the initial limit
	if (mgr1->perCount <= mgr1->userCount) 
	{
		if (mgr1->userCount > 0)
			mgr1->userCount--;

		//If the user deletes the last person, the data base will be free. The user may add more memory later.
		if (!mgr1->userCount)
		{
			free(mgr1->per[0].childrenPtr);
			free(mgr1->per[0].name);
			free(mgr1->per[0].family);
			free(mgr1->per);
			return NULL;
		}
		
		//Only decreasing without reaching 0
		else
		{
			mgr2.per = (person*)malloc(mgr1->userCount * sizeof(person));
			if (mgr2.per == NULL)
				abort_Program;
		}
	}

	//If the user is adding more people beyond the initial limit, or delete a person that was allocated beyond the limit.
	else
	{
		mgr2.per = (person*)malloc(mgr1->perCount * sizeof(person));
		if (mgr2.per == NULL)
			abort_Program();
	}

	//Case of allocating more memory, when adding a person.
	if (mgr1->per[0].id)
	{
		for (int i = 0; i < mgr1->perCount - 1; i++)
		{
			swapPer(&mgr1->per[i], &mgr2.per[i]);
		}
		free(mgr1->per);
	}

	//Case of deleting a person and allocating less memory, and free the deleted person.
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

//If person deleted, the parent get their pointer to their index deleted, and reallocated with less memory.
long* realloc_ChildPtr(person* per)
{
	person newPer;
	newPer.NumOfChildren = per->NumOfChildren;
	init_ChildPtr(&newPer);
	for (int idx = 0; idx < per->NumOfChildren; idx++)
	{
		newPer.childrenPtr[idx] = per->childrenPtr[idx];
	}
	free(per->childrenPtr);
	return newPer.childrenPtr;
}

//Initial memory allocation for a children pointer when adding a person
void init_ChildPtr(person* per)
{
	per->childrenPtr = (long*)malloc(per->NumOfChildren * sizeof(long));
	if (per->childrenPtr == NULL)
	{
		abort_Program();
	}
}

//Q4: Menu: Checks the input and returns the value that entered
char menu()
{
	char choice;
	printf("\n\t\t***Database System Menu***\n1. Add person\n2. Search a person\n3. Search parents\n4. Delete a person\n5. Get generation\n6. Print database\n7. Search by name\n8. Quit\n\n");
	while (TRUE)
	{
		//fseek(stdin, 0, SEEK_END);
		scanf("%c", &choice);
		fseek(stdin, 0, SEEK_END);
		if (choice > '0'&&choice < '9') return choice;
		printf("Error! Invalid input. Please try again.\n");
	}
}
//Q5: add_person: Adds a person to the database 
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
	int numofchildrenInt;
	scanf("%d",&numofchildrenInt);
	mgr->per[index].NumOfChildren = intAsChar(numofchildrenInt);
	if (mgr->per[index].NumOfChildren > 0)
	{
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
//Getting date information from the user, and checking for valid input
DateOfBirth inputDate()
{
	int day, month;
	DateOfBirth date;
	printf("Please enter date of birth in DD/MM/YY format: ");
	scanf("%d/%d/%d", &day, &month, &date.year);
	fseek(stdin, 0, SEEK_END);
	int chk = dateChk(month, day, date.year);
	while (!chk)
	{
		printf("Invalid date! try again.\n");
		scanf("%d/%d/%d", &day, &month, &date.year);
		fseek(stdin, 0, SEEK_END);
		chk = dateChk(month, day, date.year);
	}
	date.day = intAsChar(day);
	date.month =intAsChar(month);
	return date;
}

//Checking for valid input of date.
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
//Checking for right date with leap year.
int yearLeapChk(int year)
{
	if (year % 400 == 0) return TRUE;
	else if (year % 100 == 0) return FALSE;
	else if (year % 4 == 0) return TRUE;
	else return FALSE;
}

//Getting and checking user's id input
unsigned long idInputCheck()
{
	unsigned long id;
	scanf("%ld", &id);
	fseek(stdin, 0, SEEK_END);
	while (id < 0)
	{
		printf("Invalid input, please try again: ");
		scanf("%ld", &id);
		fseek(stdin, 0, SEEK_END);
	}
	return id;
}
//Getting input from the user and allocating name for add_person
char* enterName()
{
	char* name;
	char tmp[100];
	int size;
	gets(tmp);
	size = strlen(tmp);
	name = (char*)malloc((size + 1)*sizeof(char));
	if (name == NULL)
		abort_Program();
	strcpy(name, tmp);
	return(name);
}

//After changing the data base, the elements will be arranged by ID, starting from the smallest to the biggest values
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
//Swaps between 2 person spots in an array.
void swapPer(person* per1, person* per2)
{
	person temp;
	temp = *per1;
	*per1 = *per2;
	*per2 = temp;
}
//Q6: search_id: Gets id and returns a pointer for a person with the same id. If fails it will return NULL
person* search_id(db_mgr* mgr, unsigned long id)
{
	person* ptr = mgr->per;
	int endIdx = mgr->perCount - 1;

	int idx = 0;
	if (ptr[idx].id <= id && ptr[endIdx].id >= id)
	{
		int midIdx = (endIdx+idx) / 2;
		if (midIdx % 2) midIdx++;
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
			if (ptr[idx].id < id && ptr[midIdx].id >= id)
			{
				endIdx = midIdx;
				idx++;
				midIdx = (endIdx+idx)/2;
				if (midIdx % 2) midIdx++;
			}
			else
			{
				idx = midIdx;
				endIdx--;
				midIdx = (endIdx + idx) / 2;
				if (midIdx % 2) midIdx++;
			}
		}
		return NULL;
	}
	else return NULL;
}
//Automating ID input and searching for a person. Returns a pointer of the person with the wanted ID. If failed return NULL
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
//Q7: search_person: Finds a person from the database by id and prints his info.
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
//Q8: search_parents: Finds parents of a pesron in the database and prints thier info.
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

//Checking if the data base is empty or not. If empty it will return FALSE and only will be able to use add_person ,else TRUE.
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

//Q9: delete_person: Deletes a person from anywhere in the database and from relatives info, and reallocate new memory.
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
		printf("The person has been deleted succsessfully!\n");
	}
}
//Deletes persons id from the childrenPtr array in parents of the person.
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
	//Temp is used swap the positios int he children pointer array
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
		parent->childrenPtr=realloc_ChildPtr(parent);
}
//Q13: quit: Freeing the allocted memory and exit the program.
void quit(db_mgr* mgr)
{
	printf("Quitting the program!\n");
	//If the user already deleted all the info from the data base, it will skip db_free.
	if (mgr->per)
		db_Free(mgr);
}
//Freeing the allocated memory of persons info.
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

//Q12: search_by_name: Prints all the people that match the searched name.
int search_by_name(db_mgr* mgr)
{
	printf("**Search By Name**\n");
	char firstName[100];
	char lastName[100];
	printf("Please enter first name: ");
	gets(firstName);
	printf("Please enter last name: ");
	gets(lastName);
	//The function will seek for relatives by the family name to shorten the search, otherwise will search by changing indexs
	person* relative;
	for (int idx=0;idx<mgr->perCount-1;idx++)
	{
		if (strcmp(mgr->per[idx].family,lastName)==0)
		{
			if (strcmp(mgr->per[idx].name,firstName)==0)
			{
				print_person(&mgr->per[idx]);
				return 0;
			}
			relative = relative_Search(mgr, mgr->per[idx].FatherId);
			if (strcmp(relative->name,firstName)==0)
			{
				print_person(relative);
				return 0;
			}
			relative = relative_Search(mgr, mgr->per[idx].MotherId);
			if (strcmp(relative->name, firstName)==0)
			{
				print_person(relative);
				return 0;
			}
			if (mgr->per[idx].NumOfChildren)
			{
				for (int secIdx = 0; secIdx < mgr->per[idx].NumOfChildren; secIdx++)
				{
					relative = relative_Search(mgr, mgr->per[idx].childrenPtr[secIdx]);
					if (strcmp(relative->name, firstName)==0)
					{
						print_person(relative);
						return 0;
					}
				}
			}
		}
	}
	printf("The person has not been found! Please check if the name is correct.\n");
}
//Checks that the person have a relative info, and if they do the function will return a pointer for that relative
person* relative_Search(db_mgr* mgr, unsigned long id)
{
	if (id)
	{
		person* relative=search_id(mgr,id);
		if (relative)
		{
			return relative;
		}
		else return NULL;
	}
	else return NULL;
}


//Q10: get_gen: Prints the number of descendants of a chosen person.
void get_gen(db_mgr* mgr)
{
	person* perPtr;
	person* genPtr;
	person* treePtr;
	printf("**Get Gen**\nPlease enter ID: ");
	perPtr = ptrForPerson(mgr);
	if (perPtr)
	{
		int treeSize = 1;
		//The function will create a new person array that will be a family tree.
		//The final size of the array will be the number of the generation
		treePtr = (person*)malloc(treeSize * sizeof(person));
		if (!treePtr)
			abort_Program;
		treePtr[0] = *perPtr;		
		int treeIdx = 0;
		int childIdx = 0;
		while (TRUE) //Only breaks at a specific condition that occurs before the end of the loop
		{
			if (treePtr[treeIdx].childrenPtr && childIdx< treePtr[treeIdx].NumOfChildren)
			{
				genPtr = search_id(mgr, treePtr[treeIdx].childrenPtr[childIdx]);
				if (!genPtr)
				{
					childIdx++;
					for (; childIdx < treePtr[treeIdx].NumOfChildren;childIdx++)
					{
						genPtr = search_id(mgr, treePtr[treeIdx].childrenPtr[childIdx]);
						if (genPtr) break;
					}
				}
			}
			else genPtr = NULL;
			if (genPtr)
			{
				if (treeSize == treeIdx + 1)
				{
					//If there is a new generation the array will increace
					treeSize++;
					treePtr = (person*)realloc(treePtr, treeSize * sizeof(person));
					if (!treePtr)
						abort_Program();
				}
				treeIdx++;
				treePtr[treeIdx] = *genPtr;
				childIdx = 0;
			}
			//There are no children to the current generation in the data base, and moving to the next relative
			else
			{
				if (treeIdx) treeIdx--;
				else
				{
					for (childIdx = treePtr[treeIdx].NumOfChildren-1;0<=childIdx; childIdx--)
					{
						genPtr = search_id(mgr, treePtr[treeIdx].childrenPtr[childIdx]);
						if (genPtr) break;
					}
				}
				//Exits the while loop
				if (treeIdx == 0 && treePtr[0].childrenPtr[childIdx]==treePtr[1].id) break;
				for (childIdx=0; childIdx < treePtr[treeIdx].NumOfChildren; childIdx++)
				{
					if (treePtr[treeIdx].childrenPtr[childIdx] == treePtr[treeIdx + 1].id)
					{
						childIdx++;
						break;
					}
				}
			}
		}
		//If the last generation person has children that aren't in the person data base
		if (treePtr[treeSize - 1].NumOfChildren)
		{
			treeSize++;
			for (int idx = 0; idx > treeSize - 2; idx++) free(&treePtr[idx]);
		}
		else for (int idx = 0; idx > treeSize - 1; idx++) free(&treePtr[idx]);
		free(treePtr);
		printf("There are %d generations in the family\n", treeSize);
	}
}