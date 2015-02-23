

###################################
 <<< string manupulate >>>

#define DEFAULT_UBI_SERVER ("pda500.datawind-s.com")

char * get_default_server_name() 
{
	char * ret;
	/**/
		ret = OslMalloc(sizeof(DEFAULT_UBI_SERVER)+1);

		strcpy( ret,DEFAULT_UBI_SERVER);

		return ret;

}

char * get_default_server_port()
{
	char * ret;
	ret = OslMalloc(DEFAULT_UBI_SERVER_IP_PORT_CHAR_LENGTH);
	snprintf(ret,DEFAULT_UBI_SERVER_IP_PORT_CHAR_LENGTH, "%d",DEFAULT_UBI_SERVER_IP_PORT );
	return ret;

}

