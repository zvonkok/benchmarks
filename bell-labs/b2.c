/*      Bell Labs benchmark B2
TEST bell-labs2
DESC bell labs benchmark #2
CAT integer
END
*/
#include <bench.h>
#define	NIL	(struct tnode *)0

struct tnode
{
	char *word;
	int count;
	struct tnode *left;
	struct tnode *right;
};

char *wdarray[500] =
{
	"updating", "the", "test", "tables", "if", "the",
	"test", "is", "to", "be", "run", "for",
	"floating", "point", "as", "well", "as", "integral",
	"compilers", "both", "and", "must", "be", "updated",
	"The", "test", "file", "system", "organization",
	"consists", "of", "a", "directory", "containing",
	"the", "shell", "scripts", "tables", "is", "needed",
	"the", "byte", "size", "may", "be", "altered",
	"To", "alter", "the", "byte", "size", "used"
};

struct tnode tnarray[500];
int t;

char chrs[5000];
int c;

void bell_labs2(void)
{
	int i, n;
	struct tnode *root, *tree ();
	
	for (i = 1; i <= WORKLOAD; i++)
	{
		t = 0;
		c = 0;
		root = NIL;
		for (n = 0; n < 500; n++)
			root = tree (root, wdarray[n]);
		treeprint (root);
	}
}

struct tnode *
tree (p, w)			/* Add word to tree                                     */

struct tnode *p;
char *w;
{
	struct tnode *talloc ();
	char *strsave ();
	int cond;
	
	if (p == NIL)
	{
		p = talloc ();
		p->word = strsave (w);
		p->count = 1;
		p->left =
		p->right = NIL;
	}
	else if ((cond = stcmp (w, p->word)) == 0)
		p->count++;
	else if (cond < 0)
		p->left = tree (p->left, w);
	else
		p->right = tree (p->right, w);
	
	return p;
}

struct tnode *
talloc ()			/* Allocate a tnode                                     */
{
	return &tnarray[t++];
}

char *
strsave (w)			/* Make a copy of the string            */

char *w;
{
	char *cp, *cps;
	
	cps =
	cp = &chrs[c];
	
	while (*cp++ = *w++)
		c++;
	c++;
	return cps;
}

treeprint (p)			/* print tree                                           */

struct tnode *p;
{
	if (p != NIL)
	{
		treeprint (p->left);
		treeprint (p->right);
	}
}

stcmp (s, u)

register char *s, *u;
{
	if (!s || !u)
		return 0;
	for (; *s == *u; s++, u++)
		if (*s == '\0')
		return 0;
	
	return (*s - *u);
}

SINGLE_BENCHMARK(bell_labs2())
