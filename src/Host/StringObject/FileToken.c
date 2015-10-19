#include "FileToken.h"

/* Create a new file token
 * input :
 *			name is the file name
 * return :
 *			identifier token
 */
File *fileNew(char *name)
{
	File *file = malloc(sizeof(Identifier)); //*file = file pointer
	file->type = FILE_TOKEN;
	file->name = name;
	
	return file;
}

/* Delete file token
 * input :
 *			Operator pointer pointing operator token structure
 * output :
 *			clear operator token value and free the memory allocated
 */
void fileDel(File *file)
{
	if(file)
		free(file);
}