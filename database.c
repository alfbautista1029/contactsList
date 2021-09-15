#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>

void err_exit(char* message)
{
	if(errno)
		perror(message);
	else
		printf("\nError: %s\n", message);
	
	exit(EXIT_FAILURE);
}

//database functions
//opens a connection to the database
connection* db_open(const char* filename, char mode)
{	
	connection* conn = (connection*)malloc(sizeof(connection));
	
	if(mode == 'c')
		conn->file = fopen(filename, "w");
	else
		conn->file = fopen(filename, "r+");
	
	conn->db = (database*)malloc(sizeof(database));
	
	return conn;
}


//ecrypts database file
void encryption(connection* conn)
{
	srand(time(NULL));
	for(int index = 0; index < MAX_CONTACTS; index++)
	{
		randomizer(&conn->db->contactsBook[index]);
	}
}

//decrypts database file
void decryption(connection* conn)
{
	for(int index = 0; index < MAX_CONTACTS; index++)
		{
			solver(&conn->db->contactsBook[index]);
		}
}

//encryption helper functions
void randomizer(contact* cont)
{
	char cipher = (rand() % (126 - 33) + 33);
	char* letters[5];
	
	letters[0] = cont->firstName;
	letters[1] = cont->lastName;
	letters[2] = cont->email;
	letters[3] = cont->phone;
	letters[4] = cont->dateAdded;
	
	while(*(letters[0]) != '\0' || *(letters[1]) != '\0' || *(letters[2]) != '\0' || *(letters[3]) != '\0' || *(letters[4]) != '\0')
	{
		if(*(letters[0]) != '\0')
		{
			*(letters[0]) += cipher;
			letters[0]++;
		}
		
		if(*(letters[1]) != '\0')
		{
			*(letters[1]) += cipher;
			letters[1]++;
		}
		
		if(*(letters[2]) != '\0')
		{
			*(letters[2]) += cipher;
			letters[2]++;
		}
		
		if(*(letters[3]) != '\0')
		{
			*(letters[3]) += cipher;
			letters[3]++;
		}
		
		if(*(letters[4]) != '\0')
		{
			*(letters[4]) += cipher;
			letters[4]++;
		}
	}
	
	strncat(cont->phone, &cipher, 1);
}

void solver(contact* cont)
{
	char* letters[5];
	letters[0] = cont->firstName;
	letters[1] = cont->lastName;
	letters[2] = cont->email;
	letters[3] = cont->phone;
	letters[4] = cont->dateAdded;
	
	char* temp_ptr = letters[3];
	while(*(temp_ptr) != '\0')
		temp_ptr++;
	
	char cipher = *(temp_ptr - 1);
	
	*temp_ptr = '\0';
	
	while(*(letters[0]) != '\0' || *(letters[1]) != '\0' || *(letters[2]) != '\0' || *(letters[3]) != '\0' || *(letters[4]) != '\0' )
		{
			if(*(letters[0]) != '\0')
				{
					*(letters[0]) -= cipher;
					letters[0]++;
				}
			
			if(*(letters[1]) != '\0')
				{
					*(letters[1]) -= cipher;
					letters[1]++;
				}
			
			if(*(letters[2]) != '\0')
				{
					*(letters[2]) -= cipher;
					letters[2]++;
				}
			
			if(*(letters[3]) != '\0')
				{
					*(letters[3]) -= cipher;
					letters[3]++;
				}
			
			if(*(letters[4]) != '\0')
				{
					*(letters[4]) -= cipher;
					letters[4]++;
				}
		}

}

//closes a connection to the database
void db_close(connection* conn)
{
	if(conn)
	{
		if(conn->file)
			fclose(conn->file);
		if(conn->db)
			free(conn->db);
	}
}

//creates a new database
void db_create(connection* conn)
{
	contact temp = {.id= 0, .set = 0, .firstName = "none", .lastName = "none", .email = "none", .phone = "none", .dateAdded = "none"};
	for(int index = 0; index < MAX_CONTACTS; index++)
	{
		conn->db->contactsBook[index] = temp;
		conn->db->contactsBook[index].id = index;
	}
}

//loads the database into memory
void db_load(connection* conn)
{	
	db_create(conn);
	int index = 0;
	for(index = 0; index < MAX_CONTACTS; index++)
		{
			fscanf(conn->file, "%d %d %s %s %s %s %s\n", &conn->db->contactsBook[index].id, &conn->db->contactsBook[index].set, conn->db->contactsBook[index].firstName,conn->db->contactsBook[index].lastName, conn->db->contactsBook[index].email, conn->db->contactsBook[index].phone, conn->db->contactsBook[index].dateAdded);
		}
	
	if(index != MAX_CONTACTS)
		err_exit("There was a problem reading your contacts from file.");
	
	decryption(conn);
}

//saves the database to file
void db_save(connection* conn)
{
	rewind(conn->file);
	encryption(conn);
	
	int index = 0;
	for(index = 0; index < MAX_CONTACTS; index++)
		{
			fprintf(conn->file, "%d %d %s %s %s %s %s\n", conn->db->contactsBook[index].id, conn->db->contactsBook[index].set, conn->db->contactsBook[index].firstName,conn->db->contactsBook[index].lastName, conn->db->contactsBook[index].email, conn->db->contactsBook[index].phone, conn->db->contactsBook[index].dateAdded);
		}
	
	if(index != MAX_CONTACTS)
		err_exit("There was a problem writing your contacts to file.");
}

//adds a contact to the database
void db_add(connection* conn, contact* cont)
{
	time_t currentDateTime = time(NULL);
	
	if(currentDateTime == (time_t)-1)
		err_exit("There was a problem recording the date added for this operation.");
		
	char* currentDateTimeStr; 
	currentDateTimeStr = ctime(&currentDateTime);
	int dateStrSize = MAX_FIELD_LENGTH; 
	
	int filled = 1;
	int index = 0;

	while(filled && index < MAX_CONTACTS)
	{
		if(conn->db->contactsBook[index].set == 0)
			filled = 0;
		else
			index++;
	}
	
	if((index == MAX_CONTACTS) && (filled == 1))
		err_exit("The contacts list is full. Delete some contacts before adding in new ones.");
	else
	{
		cont->id = index;
		cont->set = 1;
		conn->db->contactsBook[index] = *cont;
		strncpy(conn->db->contactsBook[index].dateAdded, currentDateTimeStr, dateStrSize);
	}
}

//deletes a contact from the database
void db_del(connection* conn, const char* firstName)
{	
	int match = 0;
	int index = 0;
	
	while(!match && index < MAX_CONTACTS)
	{
		if(strcmp(conn->db->contactsBook[index].firstName, firstName) == 0)
			match = 1;
		else
			index++;
	}
	
	if(index == MAX_CONTACTS - 1 && match == 0)
		err_exit("The contact you are trying to delete does not exist in this contacts list.");
	else
	{
		contact temp = {.id = index, .set = 0, .firstName = "none", .lastName = "none", .email = "none", .phone = "none"};
		conn->db->contactsBook[index] = temp;
	}
}

//gets a contact from the database
void db_get(connection* conn, const char* firstName)
{
	
	contact* temp;
	int match = 0;
	int index = 0;
	
	while(!match && index < MAX_CONTACTS)
	{
		if(strcmp(conn->db->contactsBook[index].firstName, firstName) == 0)
		{
			match = 1;
			temp = &conn->db->contactsBook[index];
		}
		else
			index++;
	}
	
	if((index == MAX_CONTACTS) && (match == 0))
		err_exit("The contact you are trying to delete does not exist in this contacts list.");
	
	print_contact(temp);
}

//prints a contact to the screen
void print_contact(contact* cont)
{
	printf("%d. %s %s, Email: %s, Phone: %s Date Added: %s\n", (cont->id + 1), cont->firstName, cont->lastName, cont->email, cont->phone, cont->dateAdded);
}

//lists the entire database
void db_list(connection* conn)
{
	int index = 0;
	for(index = 0; index < MAX_CONTACTS; index++)
	{
		if(conn->db->contactsBook[index].set == 1)
			print_contact(&conn->db->contactsBook[index]);
	}
}
	