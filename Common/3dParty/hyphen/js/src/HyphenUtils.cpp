#include <iostream>
#include <cstring>

extern "C"
{
#include "../../hyphen/hyphen.c"
#include "../../hyphen/hnjalloc.h"
}

// function from hyphen.c using std::ifstream
HyphenDict * hnj_hyphen_load_stream (std::istream &in)
{
	HyphenDict *dict[2];
	HashTab *hashtab;
	char buf[MAX_CHARS];
	int nextlevel = 0;
	int i, j, k;
	HashEntry *e;
	int state_num = 0;

	/* loading one or two dictionaries (separated by NEXTLEVEL keyword) */
	for (k = 0; k < 2; k++) 
	{ 
		hashtab = hnj_hash_new();
#ifdef VERBOSE
		global[k] = hashtab;
#endif
		hnj_hash_insert (hashtab, "", 0);
		dict[k] = (HyphenDict *) hnj_malloc (sizeof(HyphenDict));
		dict[k]->num_states = 1;
		dict[k]->states = (HyphenState *) hnj_malloc (sizeof(HyphenState));
		dict[k]->states[0].match = NULL;
		dict[k]->states[0].repl = NULL;
		dict[k]->states[0].fallback_state = -1;
		dict[k]->states[0].num_trans = 0;
		dict[k]->states[0].trans = NULL;
		dict[k]->nextlevel = NULL;
		dict[k]->lhmin = 0;
		dict[k]->rhmin = 0;
		dict[k]->clhmin = 0;
		dict[k]->crhmin = 0;
		dict[k]->nohyphen = NULL;
		dict[k]->nohyphenl = 0;

	/* read in character set info */
	if (k == 0) 
	{
		for (i = 0; i < MAX_NAME; i++) 
			dict[k]->cset[i]= 0;

		if (in >> buf) 
		{
			for (i = 0; i < MAX_NAME; i++)
				if ((dict[k]->cset[i] == '\r') || (dict[k]->cset[i] == '\n'))
					dict[k]->cset[i] = 0;
		} 
		else 
		{
			dict[k]->cset[0] = 0;
		}
		dict[k]->utf8 = (strcmp(dict[k]->cset, "UTF-8") == 0);
	} 
	else 
	{
		strncpy(dict[k]->cset, dict[0]->cset, sizeof(dict[k]->cset)-1);
		dict[k]->cset[sizeof(dict[k]->cset)-1] = '\0';
		dict[k]->utf8 = dict[0]->utf8;
	}

	if (k == 0 || nextlevel) 
	{
		while (in.getline(buf, sizeof(buf), '\n')) 
		{
			if(strlen(buf) < sizeof(buf))
				strcat(buf, "\n");

			/* discard lines that don't fit in buffer */
			if (!in.eof() && strchr(buf, '\n') == NULL) 
			{
				int c;
				while ((c = in.get()) != '\n' && c != EOF);

				/* issue warning if not a comment */
				if (buf[0] != '%') 
				{
					fprintf(stderr, "Warning: skipping too long pattern (more than %lu chars)\n", sizeof(buf));
				}
				continue;
			}
			
			if (strncmp(buf, "NEXTLEVEL", 9) == 0) 
			{
				nextlevel = 1;
				break;
			} 
			else if (buf[0] != '%') 
			{
				hnj_hyphen_load_line(buf, dict[k], hashtab);
			}
		}
	} 
	else if (k == 1) 
	{
		/* default first level: hyphen and ASCII apostrophe */
		if (!dict[0]->utf8) 
			hnj_hyphen_load_line("NOHYPHEN ',-\n", dict[k], hashtab);

		else 
			hnj_hyphen_load_line("NOHYPHEN ',\xe2\x80\x93,\xe2\x80\x99,-\n", dict[k], hashtab);

		strncpy(buf, "1-1\n", MAX_CHARS - 1); /* buf rewritten by hnj_hyphen_load here */
		buf[MAX_CHARS-1] = '\0';
		hnj_hyphen_load_line(buf, dict[k], hashtab); /* remove hyphen */
		hnj_hyphen_load_line("1'1\n", dict[k], hashtab); /* ASCII apostrophe */

		if (dict[0]->utf8) 
		{
			hnj_hyphen_load_line("1\xe2\x80\x93" "1\n", dict[k], hashtab); /* endash */
			hnj_hyphen_load_line("1\xe2\x80\x99" "1\n", dict[k], hashtab); /* apostrophe */
		}
	}

  /* Could do unioning of matches here (instead of the preprocessor script).
	 If we did, the pseudocode would look something like this:

	 foreach state in the hash table
		foreach i = [1..length(state) - 1]
		   state to check is substr (state, i)
		   look it up
		   if found, and if there is a match, union the match in.

	 It's also possible to avoid the quadratic blowup by doing the
	 search in order of increasing state string sizes - then you
	 can break the loop after finding the first match.

	 This step should be optional in any case - if there is a
	 preprocessed rule table, it's always faster to use that.

*/

  /* put in the fallback states */
	for (i = 0; i < HASH_SIZE; i++)
		for (e = hashtab->entries[i]; e; e = e->next)
	  	{
			if (*(e->key)) for (j = 1; 1; j++)
	  		{          
				state_num = hnj_hash_lookup (hashtab, e->key + j);
				if (state_num >= 0)
		  		break;
	  		}
			/* KBH: FIXME state 0 fallback_state should always be -1? */
			if (e->val)
			dict[k]->states[e->val].fallback_state = state_num;
	  	}

#ifdef VERBOSE
  for (i = 0; i < HASH_SIZE; i++)
	for (e = hashtab->entries[i]; e; e = e->next)
	  {
	printf ("%d string %s state %d, fallback=%d\n", i, e->key, e->val,
		dict[k]->states[e->val].fallback_state);
	for (j = 0; j < dict[k]->states[e->val].num_trans; j++)
	  printf (" %c->%d\n", dict[k]->states[e->val].trans[j].ch,
		  dict[k]->states[e->val].trans[j].new_state);
	  }
#endif

#ifndef VERBOSE
		hnj_hash_free (hashtab);
#endif
		state_num = 0;
	}
	if (nextlevel) dict[0]->nextlevel = dict[1];
	else 
	{
		dict[1] -> nextlevel = dict[0];
		dict[1]->lhmin = dict[0]->lhmin;
		dict[1]->rhmin = dict[0]->rhmin;
		dict[1]->clhmin = (dict[0]->clhmin) ? dict[0]->clhmin : ((dict[0]->lhmin) ? dict[0]->lhmin : 3);
		dict[1]->crhmin = (dict[0]->crhmin) ? dict[0]->crhmin : ((dict[0]->rhmin) ? dict[0]->rhmin : 3);
#ifdef VERBOSE
		HashTab *r = global[0];
		global[0] = global[1];
		global[1] = r;
#endif
		return dict[1];
  	}
	return dict[0];
}
