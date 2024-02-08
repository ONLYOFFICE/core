#ifndef _LOG_FILE_H
#define _LOG_FILE_H

#include <stdio.h>
#define LOG_BUFFER_SIZE 1000

namespace Logging
{
void logBytes(char* name, unsigned char* str, int len)
{
	char buffer[LOG_BUFFER_SIZE];
	char* name_cur = name;
	char* buf_cur = buffer;

	while (*name_cur != 0)
	{
		*buf_cur++ = *name_cur++;
	}

	*buf_cur++ = ':';
	*buf_cur++ = ' ';
	*buf_cur++ = '[';

	for (int i = 0; i < len; ++i)
	{
		unsigned char c = str[i];

		unsigned char n1 = (unsigned char)(c / 100);
		c -= (n1 * 100);

		unsigned char n2 = (unsigned char)(c / 10);
		c -= (n2 * 10);

		if (buf_cur - buffer + 4 >= LOG_BUFFER_SIZE)
		{
			*buf_cur++ = '\0';
			printf("%s\n", buffer);

			buf_cur = buffer;
		}

		*buf_cur++ = (char)('0' + n1);
		*buf_cur++ = (char)('0' + n2);
		*buf_cur++ = (char)('0' + c);
		*buf_cur++ = ',';
	}

	buf_cur--;
	*buf_cur++ = ']';
	*buf_cur++ = '\0';

	printf("%s\n", buffer);
}
}

#endif // _LOG_FILE_H
