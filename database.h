#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <ctype.h>

#define MAX_CONTACTS 256
#define MAX_FIELD_LENGTH 128
typedef struct contact
{
	int id;
	int set;
	char firstName[MAX_FIELD_LENGTH];
	char lastName[MAX_FIELD_LENGTH];
	char phone[MAX_FIELD_LENGTH];
	char email[MAX_FIELD_LENGTH];
	char dateAdded[MAX_FIELD_LENGTH];
} contact;

typedef struct database
{
	contact contactsBook[MAX_CONTACTS];
} database;

typedef struct connection
{
	FILE* file;
	database *db;
} connection;

//database functions

//opens a connection to the database
connection* db_open(const char* filename, char mode);

//ecrypts database file
void encryption(connection* conn);

//decrypts database file
void decryption(connection* conn);

//encryption helper functions
void randomizer(contact* cont);
void solver(contact* cont);

//closes a connection to the database
void db_close(connection* conn);

//creates/initializes a new database
void db_create(connection* conn);

//loads the database into memory
void db_load(connection* conn);

//saves the database to file
void db_save(connection* conn);

//adds a contact to the database
void db_add(connection* conn, contact* cont);

//deletes a contact from the database
void db_del(connection* conn, const char* firstName);

//gets a contact from the database
void db_get(connection* conn, const char* firstName);

//lists the entire database
void db_list(connection* conn);

//contact functions
//prints a contact to screen
void print_contact(contact* cont);

//error reporting function
void err_exit(char* message);
