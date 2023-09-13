#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEXEMA 256

#define COMP 256
#define ID 257
#define IF 258
#define ELSE 259
#define NUM 260

struct Handler
{
	FILE *file;
	char lexem[MAXLEXEMA];
};

int lex(struct Handler *h)
{
	size_t i = 0;
	h->lexem[i] = fgetc(h->file);
	if (h->lexem[i] == '<')
	{
		h->lexem[++i] = fgetc(h->file);
		if (h->lexem[i] == '=')
		{
			h->lexem[++i] = '\0';
			return COMP;
		} // else
		ungetc(h->lexem[i], h->file);
		h->lexem[i] = '\0';
		return COMP;
	} // else
	if (h->lexem[i] == '>')
	{
		h->lexem[++i] = fgetc(h->file);
		if (h->lexem[i] == '=')
		{
			h->lexem[++i] = '\0';
			return COMP;
		} // else
		ungetc(h->lexem[i], h->file);
		h->lexem[i] = '\0';
		return COMP;
	} // else
	if ((h->lexem[i] >= 'a' && h->lexem[i] <= 'z') ||
	    (h->lexem[i] >= 'A' && h->lexem[i] <= 'Z') || h->lexem[i] == '_')
	{
		do
		{
			h->lexem[++i] = fgetc(h->file);
		} while ((h->lexem[i] >= 'a' && h->lexem[i] <= 'z') ||
			   (h->lexem[i] >= 'A' && h->lexem[i] <= 'Z') ||
			   (h->lexem[i] >= '0' && h->lexem[i] <= '9') || h->lexem[i] == '_');
		h->lexem[i] = '\0';
		ungetc(h->lexem[i], h->file);
		if (strcmp(h->lexem, "if") == 0)
		{
			return IF;
		}
		else if (strcmp(h->lexem, "else") == 0)
		{
			return ELSE;
		}
		return ID;
	}
	if(h->lexem[i] >= '0' && h->lexem[i] <= '9'){
		do
		{
			h->lexem[++i] = fgetc(h->file);
		} while(h->lexem[i] >= '0' && h->lexem[i] <= '9');
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
		// printf("Usage: %s <sourcefile>\n", argv[0]);
		// return -1;
	}
	struct Handler *handler = (struct Handler *)malloc(sizeof(struct Handler));
	// handler->file = fopen(argv[1], "r");
	handler->file = fopen("test.c", "r");

	int tok;
	while ((tok = lex(handler)) != EOF)
	{
		printf("Lexem (%d): %s\n", tok, handler->lexem);
	}
	fclose(handler->file);
	free(handler);
	return 0;
}