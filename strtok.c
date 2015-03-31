#include <string.h>
#include <stdio.h>

#define CONFIG_FILE_NAME "config.txt"
#define CONFIG_FILE_LINE_MAX  80
#define CONFIG_FILE_DELIMITER "="

int main()
{
   const char str[80] = "This is - www.tutorialspoint.com - website";
   const char s[2] = "-";
   char *p_name, *p_value;


   FILE *config_file;
   char line[CONFIG_FILE_LINE_MAX];

   config_file =  fopen(CONFIG_FILE_NAME, "r+");

   //fget(line, );
   fgets(line,CONFIG_FILE_LINE_MAX, config_file );

   
   /* get the first token */
   p_name = strtok(line, CONFIG_FILE_DELIMITER);
   
   /* walk through other tokens */
   while( p_name != NULL ) 
   {
    
      p_value = strtok(NULL, CONFIG_FILE_DELIMITER);
      printf( " name == %s,  value == %s \n", p_name, p_value );
   	 fgets(line,CONFIG_FILE_LINE_MAX, config_file );
     p_name = strtok(line, CONFIG_FILE_DELIMITER);
   }
   
   return(0);
}
