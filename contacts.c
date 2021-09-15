#include "database.h"
#include <stdlib.h>

int main(int argc, char* argv[])
{	
	if((argc < 3 && argv[1][0] != 'h'))
		err_exit("USAGE: contacts <contacts list> <action> [action params]\nFor help, use \"contacts h\" for help");
	else if(argc <= 2 && argv[1][0] == 'h')
	{
		puts("Help on valid actions and syntax\n--------------------------------\n");
		puts("help doc:                           contacts h");
		puts("to create a contact list:           contacts <dbfile> c ");
		puts("to add a contact:                   contacts <dbfile> a <first name> <last name> <email> <phone>");
		puts("to delete a contact:                contacts <dbfile> d <first name>");
		puts("to list all all contacts:           contacts <dbfile> l");
		puts("to show one one contact:            contacts <dbfile> g <first name>");
		exit(1);
	}	
	
	char* filename = argv[1];
	char action = argv[2][0];
	contact temp;
	
	connection *conn = db_open(filename, action);
	switch(action)
	{
		case 'c':
			if(argc != 3)
				err_exit("Invalid number of arguments. Please try again or use \"contacts h\" for help");
			
			db_create(conn);
			db_save(conn);
			break;
		
		case 'a':
			if(argc != 7)
				err_exit("Invalid number of arguments. Please try again or use \"contacts h\" for help");
			
			temp.id = 0;
			temp.set = 0;
			strcpy(temp.firstName, argv[3]);
			strcpy(temp.lastName, argv[4]);
			strcpy(temp.email, argv[5]);
			strcpy(temp.phone, argv[6]);
			
			db_load(conn);
			db_add(conn, &temp);
			db_save(conn);
			break;
		
		case 'g':
			if(argc != 4)
				err_exit("Invalid number of arguments. Please try again or use \"contacts h\" for help");
			
			db_load(conn);
			db_get(conn, argv[3]);
			break;
		
		case 'd':
			if(argc != 4)
				err_exit("Invalid number of arguments. Please try again or use \"contacts h\" for help");
			
			db_load(conn);
			db_del(conn, argv[3]);
			db_save(conn);
			break;
		
		case 'l':
			if(argc != 3)
				err_exit("Invalid number of arguments. Please try again or use \"contacts h\" for help");

			db_load(conn);
			db_list(conn);
			break;
		
		default:
			err_exit("Invalid action. Use \"contacts h\" for help on valid actions and syntax.");
	}
	
	db_close(conn);
	return EXIT_SUCCESS;
}
