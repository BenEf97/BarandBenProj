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
char menu();
char intAsChar(int a);
void add_person(db_mgr* mgr);
char* enterName();
void arrangeId(db_mgr* mgr);
void swapPosition(person* per1, person* per2);

void main()
{
	
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

person* init_db(db_mgr* mgr1) { //check me dont know if im right  
	db_mgr mgr2;
	if (mgr1->perCount > 0) {
		mgr2.per = (person*)malloc(mgr1->perCount * sizeof(person));
		for (int i = 0; i < mgr1->perCount; i++) {
			mgr2.per[i] = mgr1->per[i];
		}
	free(mgr1->per);
	return mgr2.per;
	}
}

char menu(){ //menu function returns the value that entered
	int tmp = 1;
	char choice;
	while (tmp){
		printf("Database System Menu:\n1. Add person\n2. Search a person\n3. Search parents\n4. Delete a person\n5. Get generation\n6. Print database\n7. Search by name\n8. Quit\n");
		getchar(choice);
		switch (choice) {
		case '1': return choice;
		case '2': return choice;
		case '3': return choice;
		case '4': return choice;
		case '5': return choice;
		case '6': return choice;
		case '7': return choice;
		case '8': return choice;
		default: printf("error! invalid input. try again.\n");
		}
	}
}

void add_person(db_mgr* mgr) { //not finished
	mgr->perCount++;
	mgr->per=init_db(mgr);
	person man;
	printf("please enter a person details:\n");
	printf("ID: ");
	scanf("%ld", &man.id);
	while (man.id < 0 || man.id>9)
	{
		printf("invalid input. try again.");
		scanf("%ld", &man.id);
	}
	printf("please enter name: ");
	man.name = enterName;
	printf("please enter last name: ");
	man.family = enterName;


	

}
char* enterName()
{ //reading and allocating name for add_person
	person p1;
	char tmp[100];
	int size;
	gets(tmp);
	fseek(stdin, 0, SEEK_END);
	size = srtlen(tmp);
	p1.name = (char*)malloc(size + 1);
	strcpy(p1.name, tmp);
	return(p1.name);
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
	for (int idx = mgr->perCount -1; 0 <= idx ; idx--)
	{
		if (mgr->per[idx].id < mgr->per[idx-1].id )

	}
}

void swapPosition(person* per1, person* per2)
{
	person temp = *per1;
	*per1 = *per2;
	*per2 = temp;
}
