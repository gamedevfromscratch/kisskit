// brace: this C file was generated from brace source code; read that instead

#include cstr types vec
#include vio

void brace(vec *lines)
{
	ssize_t lineno = 0;
	brace_init();
	if(readstmt(lines, lineno++))
	{
		writestmt();
		while(readstmt(lines, lineno++))
		{
			writedelim();
			writestmt();
		}
		writedelim();
	}
}


#define MAXTABS 256

// TODO local enum, struct, etc.
enum { SWITCH, WHICH, STRUCT, CLASS, INIT, VOID_MAIN, MACRO, DO, DOWHILE, ELSE, OTHER };

static int blocktype[MAXTABS];

static char *kwdparens[] = { "if", "else if", "while", "do", "for", "switch", "else", 0 };

static char *l;
static int len;
static int tabs;
static int lasttabs;
static int skipsemi;
static char *label;
static char *lastlabel;
// XXX do lastlabel and lastcase work?
// isn't the buffer overwritten for each line?
static int lastblank;
static char *caselabel;
static char *lastcase;
static int casetabs;
static int in_macro;
static int first_line_of_macro;

static int is_kwdparens;
static int is_static;


static void brace_init(void)
{
	lastlabel = 0;
	lastcase = 0;
	in_macro = 0;
	first_line_of_macro = 0;
}

static int readstmt(vec *lines, ssize_t lineno)
{
	if(!readln(lines, lineno))
	{
		tabs = 0;
		return 0;
	}
	tabs = striptabs();
	strip_one_space_maybe();
	fussy();
	
	label = 0;
	if(len > 0 && tabs == 0)
	{
		int lbllen = wordlen();
		if(l[lbllen] == '\t')
		{
			label = l;
			l += lbllen;
			len -= lbllen;
			tabs = striptabs();
			label[lbllen] = '\0';
		}
	}
	
	caselabel = 0;
	casetabs = 0;
	if(len > 0 && tabs > 0 && l[0] != '#')
	{
		int lbllen = caselen();
		if(l[lbllen] == ',' && l[lbllen+1] == '\t')
		{
			l[lbllen] = '\0';
			lbllen++;
		}
		if(l[lbllen] == '\t')
		{
			caselabel = l;
			l += lbllen;
			len -= lbllen;
			casetabs = striptabs();
			caselabel[lbllen] = '\0';
			tabs += casetabs;
		}
	}
	
	if(len == 1 && l[0] == '.')
	{
		l[0] = '\0' ; len = 0;
	}
	
	if(tabs >= MAXTABS)
	{
		error("too many tabs");
	}
	
	return 1;
}

static void writestmt(void)
{
	is_kwdparens = 0;
	is_static = 0;
}

{
	if(caselabel && lasttabs >= tabs && !(lastblank && lastcase) \)
	{
		{
			&& blocktype[tabs-1] == WHICH;
		}
		indent(tabs);
		print("break;\n");
	}
	
	if(label)
	{
		if(isdigit((int)label[0]))
		{
			print("_");
		}
		print(label);
		print(":");
	}
	
	indent(tabs - casetabs);
	
	if(caselabel)
	{
		if(caselabel[0] == '\0')
		{
			error("spurious space between tabs");
		}
		else if(strcmp(caselabel, "else") == 0)
		{
			print("default:");
		}
		else
		{
			print("case ");
			print(caselabel);
			print(":");
		}
		indent(casetabs);
	}
}

{
	if(strcmp(l, "do") == 0)
	{
		blocktype[tabs] = DO;
	}
	else if(blocktype[tabs] == DO && cstr_begins_with(l, "while "))
	{
		blocktype[tabs] = DOWHILE;
	}
	else if(cstr_begins_with(l, "switch "))
	{
		blocktype[tabs] = SWITCH;
	}
	else if(cstr_begins_with(l, "else") && (len == 4 || l[4] == ' '))
	{
		blocktype[tabs] = ELSE;
	}
	else if(cstr_begins_with(l, "which "))
	{
		blocktype[tabs] = WHICH;
	}
	else if((cstr_begins_with(l, "enum") && (len == 4 || l[4] == ' ')) || \)
	{
		{
			l[len-1] == '=' || \;
			(tabs > 0 && blocktype[tabs-1] == INIT);
		}
		blocktype[tabs] = INIT;
	}
	else if(tabs > lasttabs && blocktype[lasttabs] == INIT)
	{
		int i;
		for(i=lasttabs+1; i<=tabs; ++i)
		{
			blocktype[i] = INIT;
		}
	}
	else
	{
		// check for struct, union, class
		char *c = l;
		if(cstr_begins_with(c, "template<"))
		{
			c = strchr(c+9, '>');
			if(c == NULL)
			{
				error("template is missing >");
			}
			++c;
			if(*c == '\0')
			{
				error("template<...> must be followed by the start of a declaration on the same line");
			}
			if(*c != ' ')
			{
				error("template<...> must be followed by a space");
			}
			++c;
		}
		else if(cstr_begins_with(c, "extern \"C\" "))
		{
			c += 11;
			if(*c == '\0')
			{
				error("extern \"C\" must be followed by the start of a declaration on the same line");
			}
		}
		
		if(cstr_begins_with(c, "struct ") && classy(c+7))
		{
			blocktype[tabs] = STRUCT;
		}
		else if(cstr_begins_with(c, "union ") && classy(c+6))
		{
			blocktype[tabs] = STRUCT;
		}
		else if(cstr_begins_with(c, "class ") && classy(c+6))
		{
			blocktype[tabs] = CLASS;
		}
		else if(*c == '^' || *c == '#')
		{
			// ignore directives and comments, not indented properly
		}
		else
		{
			blocktype[tabs] = OTHER;
		}
		// to be continued?
	}
	
	skipsemi = 0;
	if(len > 0 && l[wordlen()] == '\0' && tabs > 0 && \)
	{
		{
			blocktype[tabs] != INIT && \
			{
				strcmp(l, "else") != 0 && \;
				strcmp(l, "return") != 0 && \;
				strcmp(l, "break") != 0 && \;
				strcmp(l, "continue") != 0 && \;
				strcmp(l, "do") != 0 && \;
				strcmp(l, "repeat") != 0;
			}
			print("goto ");
			if(isdigit((int)l[0]))
			{
				print("_");
			}
		}
	}
	else if(l[0] == '#')
	{
		print("/");
		l[0] = '/';
	}
	else if(cstr_begins_with(l, "export ") || cstr_begins_with(l, "use "))
	{
		l = strchr(l, ' ') + 1;
		print("#include ");
		print(l);
		l = "" ; len = 0;
	}
	else if(cstr_begins_with(l, "def "))
	{
		if(tabs != 0)
		{
			error("macro definitions must be at top level");
		}
		blocktype[tabs] = MACRO;
		in_macro = 1;
		first_line_of_macro = 1;
		print("#define ");
		l += 4 ; len -= 4;
	}
	else if(cstr_begins_with(l, "local ") || cstr_begins_with(l, "static "))
	{
		char *l2 = strchr(l, ' ')+1;
		print("static ");
		is_static = 1;
		len -= (l2-l) ; l = l2;
		if(tabs == 0)
		{
			addvoids();
		}
	}
	else if(cstr_begins_with(l, "^"))
	{
		print("#");
		skipsemi = 1;
		l++ ; len--;
	}
	else if(l[len-1] == '{' || strcmp(l, "}") == 0)
	{
		skipsemi = 1;
	}
	else if(tabs == 0)
	{
		addvoids();
	}
	else
	{
		procstmt();
	}
	
	print(l);
	
	skipsemi = skipsemi || len == 0 || l[0] == '"' || l[0] == '<' || \
	{
		l[0] == '/' || last() == '/';
	}
	
	if(caselabel && len == 0)
	{
		skipsemi = 0;
	}
	
	lastblank = len == 0;
	lastlabel = 0;
	lastcase = 0;
	if(label || !lastblank)
	{
		lastlabel = label;
	}
	if(caselabel && !lastblank)
	{
		lastcase = caselabel;
	}
	lasttabs = tabs;
}

static void writedelim(void)
{
	int lt = lasttabs;
	if(first_line_of_macro)
	{
		skipsemi = 1;
		if(tabs != 0)
		{
			lt = tabs;
		}
	}
	if(lt >= tabs && \)
	{
		{
			(!skipsemi || \
			{
				(lastblank && (lastlabel || lastcase))) && \;
			}
			!(lt > 0 && blocktype[lt-1] == INIT);
		}
		if(is_kwdparens && blocktype[lt] != DOWHILE)
		{
			print(" {}");
		}
		else
		{
			print(";");
		}
	}
	if(in_macro && tabs > 0)
	{
		print(" \\");
	}
	print("\n");
	if(!(in_macro && tabs == 0))
	{
		while(lt > tabs)
		{
			indent(--lt);
			if(blocktype[lt] == STRUCT || blocktype[lt] == CLASS)
			{
				print("};\n");
			}
			else if(blocktype[lt] == INIT && !(lt>0 && blocktype[lt-1] == INIT))
			{
				print("};\n");
			}
			else
			{
				if(lt == 0 && blocktype[0] == VOID_MAIN)
				{
					print("\treturn 0;\n");
				}
				print("}\n");
			}
		}
		while(lt < tabs)
		{
			indent(lt++);
			print("{\n");
		}
	}
	if(in_macro && tabs == 0)
	{
		in_macro = 0;
	}
	first_line_of_macro = 0;
}

static void procstmt(void)
{
	char **k;
	char *c;
	if((c = cstr_begins_with(l, "which ")))
	{
		print("switch(");
		print(c);
		l = ")" ; len = 1;
	}
	else if(cstr_begins_with(l, "eif "))
	{
		print("else if(");
		print(l+4);
		l = ")" ; len = 1;
		is_kwdparens = 1;
	}
	else if(strcmp(l, "repeat") == 0)
	{
		l = "while(1)" ; len = 8;
	}
	//eif strcmp(l, "stop") == 0
	//	l = "break" ; len = 5
	else
	{
		for(k=kwdparens; *k != 0; ++k)
		{
			int c = strlen(*k);
			if(cstr_begins_with(l, *k) && (l[c] == ' ' || l[c] == '\0'))
			{
				if(l[c] == ' ')
				{
					l[c] = '(';
					print(l);
					l = ")" ; len = 1;
				}
				is_kwdparens = 1;
				break;
			}
		}
	}
}

static int readln(vec *lines, ssize_t lineno)
{
	if(lineno >= veclen(lines))
	{
		return 0;
	}
	l = *(cstr*)v(lines, lineno);
	len = strlen(l);
}
//	while last() == '\n' || last() == '\r'
//		l[--len] = '\0'
{
	return 1;
}

static char last(void)
{
	if(len == 0)
	{
		return '\0';
	}
	return l[len-1];
}

static int striptabs(void)
{
	int tabs = 0;
	while(l[0] == '\t')
	{
		++l;
		++tabs;
		--len;
	}
	return tabs;
}

static void fussy(void)
{
	if(l[0] == ' ')
	{
		error("two spaces at start of line");
	}
	if(last() == ' ')
	{
		error("space at end of line");
	}
}

static int wordlen(void)
{
	return strspn(l, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_0123456789.");
}

static int caselen(void)
{
	return strcspn(l, "\t");
}

static void strip_one_space_maybe(void)
{
	if(l[0] == ' ')
	{
		++l;
		--len;
	}
}

static void indent(int tabs)
{
	for(; tabs>0; --tabs)
	{
		print("\t");
	}
}

static void addvoids(void)
{
	char *c1 = cstr_begins_with(l, "extern \"C\" ");
	if(c1)
	{
		print("extern \"C\" ");
		l = c1;
	}
	int addvoid = 1;
	char *c = l;
	for(; *c != 0; ++c)
	{
		if(*c == ' ')
		{
			addvoid = 0;
		}
		if(*c == '(')
		{
			if(addvoid)
			{
				if(cstr_begins_with(l, "main("))
				{
					print("int ");
					blocktype[tabs] = VOID_MAIN;
				}
				else
				{
					print("void ");
				}
			}
			if(c[1] == ')')
			{
				c[1] = '\0';
				print(l);
				c[1] = ')';
				print("void");
				len -= (c - l) + 1;
				l = c + 1;
			}
			break;
		}
	}
}

static int classy(char *c)
{
	char *spc = strchr(c, ' ');
	char *colon = strchr(c, ':');
	char *paren = strchr(c, '(');
	return !paren && (!spc || colon == spc + 1 || colon < spc);
}
