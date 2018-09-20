#include "common.h"

bool match_strings_letters_only(const char *s1, const char *s2)
{
	for(;;)
	{
		if(*s1 != '\x00' && !is_letter(*s1))
		{
			s1++;
		}
		else if(*s2 != '\x00' && !is_letter(*s2))
		{
			s2++;
		}
		else if(*s1 == '\x00' || *s2 == '\x00')
		{
			return *s1 == *s2;
		}
		else if(char_to_lower_case(*s1) != char_to_lower_case(*s2))
		{
			return false;
		}
		else
		{
			s1++;
			s2++;
		}
	}
}

char char_to_lower_case(char c)
{
	if(c >= 'A' && c <= 'Z')
	{
		return c - 'A' + 'a';
	}
	else
	{
		return c;
	}
}

bool is_letter(char c)
{
	if(c >= 'A' && c <= 'Z')
	{
		return true;
	}
	if(c >= 'a' && c <= 'z')
	{
		return true;
	}
	else
	{
		return false;
	}
}

