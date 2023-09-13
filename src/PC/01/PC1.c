#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEXEMA 256

// #define COMP 256
// #define ID 257
// #define IF 258
// #define ELSE 259
#define NUM 256
#define REAL 257

struct Handler
{
	FILE *file;
	char lexem[MAXLEXEMA];
};

int lex(struct Handler *h)
{
	size_t i = 0;
	h->lexem[i] = fgetc(h->file);

	if (h->lexem[i] >= '0' && h->lexem[i] <= '9')
	{
		do
		{
			h->lexem[++i] = fgetc(h->file);
		} while (h->lexem[i] >= '0' && h->lexem[i] <= '9');
		if (h->lexem[i] == '.')
		{
			do
			{
				h->lexem[++i] = fgetc(h->file);
			} while (h->lexem[i] >= '0' && h->lexem[i] <= '9');
			if (h->lexem[i] == 'e' || h->lexem[i] == 'E')
			{
				do
				{
					h->lexem[++i] = fgetc(h->file);
				} while ((h->lexem[i] >= '0' && h->lexem[i] <= '9') || (h->lexem[i] == '+' || h->lexem[i] == '-'));
				h->lexem[i] = '\0';
				ungetc(h->lexem[i], h->file);
				return REAL;
			}
			h->lexem[i] = '\0';
			ungetc(h->lexem[i], h->file);
			return REAL;
		}
		if (h->lexem[i] == 'e' || h->lexem[i] == 'E')
		{
			do
			{
				h->lexem[++i] = fgetc(h->file);
			} while ((h->lexem[i] >= '0' && h->lexem[i] <= '9') || (h->lexem[i] == '+' || h->lexem[i] == '-'));
			h->lexem[i] = '\0';
			ungetc(h->lexem[i], h->file);
			return REAL;
		}

		h->lexem[i] = '\0';
		ungetc(h->lexem[i], h->file);
		return NUM;
	}

	return h->lexem[i] != EOF ? lex(h) : EOF;
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("Usage: %s <sourcefile>\n", argv[0]);
		return -1;
	}
	struct Handler *handler = (struct Handler *)malloc(sizeof(struct Handler));
	handler->file = fopen(argv[1], "r");

	int tok;
	while ((tok = lex(handler)) != EOF)
	{
		printf("Lexem (%d): %s\n", tok, handler->lexem);
	}
	fclose(handler->file);
	free(handler);
	return 0;
}