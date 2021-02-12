#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
	int year;
	char month;
	char day;
}DateOfBirth;

typedef struct{
	long id;
	char* name;
	char* family;
	DateOfBirth date;
	long partnerId;
	long MotherId;
	long FatherId;
	char NumOfChildren;
	long *childrenPtr
}person;

void main()
{
	
}

void print_person(person* per)
{
	printf("ID: %ld\n", per->id);
	printf("First Name: %s\n", per->name);
	printf("Last Name: %s\n",per->family);
	printf("Date: %s/%s/%d \n",per->date.day,per->date.month,per->date.year);
	printf("Partner's ID: %ld\n",per->partnerId);
	printf("Mother's ID: %ld\n", per->MotherId);
	printf("Father's ID: %ld\n", per->FatherId);

}
