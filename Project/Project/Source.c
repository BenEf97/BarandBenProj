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
	long id;
	char* name;
	char* family;
	DateOfBirth date;
	long partnerId;
	long MotherId;
	long FatherId;
	char NumOfChildren;
	long *childrenPtr;
}person;

void print_person(person* per);

void main()
{
	person test = { 123456789,"John","Cena",{"1","2",1990},2345654,1345678,'0' };
	print_person(&test);
}

//Prints the person's info
void print_person(person* per)
{
	printf("ID: %ld\n", per->id);
	printf("First Name: %s\n", per->name);
	printf("Last Name: %s\n",per->family);
	printf("Date: %s/%s/%d \n",per->date.day,per->date.month,per->date.year);//error
	printf("Partner's ID: %ld\n",per->partnerId);
	printf("Mother's ID: %ld\n", per->MotherId);
	printf("Father's ID: %ld\n", per->FatherId);
	printf("Number of children: %s\n", per->NumOfChildren); //what about 2 digits
	if ((per->NumOfChildren) != '0')
	{
		for (int idx = 0; idx < per->NumOfChildren; idx++)
		{
			printf("Child No. %d:\n", idx + 1);
			printf("ID: %ld\n", per->childrenPtr[idx]);
		}
	}
}
