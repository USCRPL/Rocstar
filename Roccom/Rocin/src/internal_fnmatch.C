/*
 * Rocstar bundled fnmatch function.
 *
 * Copied from a standard implementation available online.
 */
 
#include <internal_fnmatch.h>
 

static int rangematch(const char *, wchar_t, int, char **, mbstate_t *);
static int fnmatch1(const char *, const char *, const char *, int, mbstate_t,
		mbstate_t);
 
int
fnmatch(const char * pattern, const char * string, int flags)
{
	static mbstate_t initial;

	return (fnmatch1(pattern, string, string, flags, initial, initial));
}

static int
fnmatch1(const char * pattern, const char * string, const char * stringstart, int flags, mbstate_t patmbs, mbstate_t strmbs)
{
	char *newp;
	char c;
	wchar_t pc, sc;
	size_t pclen, sclen;

	for (;;) {
		pclen = mbrtowc(&pc, pattern, MB_LEN_MAX, &patmbs);
		if (pclen == (size_t)-1 || pclen == (size_t)-2)
			return (FNM_NOMATCH);
		pattern += pclen;
		sclen = mbrtowc(&sc, string, MB_LEN_MAX, &strmbs);
		if (sclen == (size_t)-1 || sclen == (size_t)-2) {
			sc = (unsigned char)*string;
			sclen = 1;
			memset(&strmbs, 0, sizeof(strmbs));
		}
		switch (pc) {
		case EOS:
			if ((flags & FNM_LEADING_DIR) && sc == '/')
				return (0);
			return (sc == EOS ? 0 : FNM_NOMATCH);
		case '?':
			if (sc == EOS)
				return (FNM_NOMATCH);
			if (sc == '/' && (flags & FNM_PATHNAME))
				return (FNM_NOMATCH);
			if (sc == '.' && (flags & FNM_PERIOD) &&
			    (string == stringstart ||
			    ((flags & FNM_PATHNAME) && *(string - 1) == '/')))
				return (FNM_NOMATCH);
			string += sclen;
			break;
		case '*':
			c = *pattern;
			/* Collapse multiple stars. */
			while (c == '*')
				c = *++pattern;

			if (sc == '.' && (flags & FNM_PERIOD) &&
			    (string == stringstart ||
			    ((flags & FNM_PATHNAME) && *(string - 1) == '/')))
				return (FNM_NOMATCH);

			/* Optimize for pattern with * at end or before /. */
			if (c == EOS)
				if (flags & FNM_PATHNAME)
					return ((flags & FNM_LEADING_DIR) ||
					    strchr(string, '/') == NULL ?
					    0 : FNM_NOMATCH);
				else
					return (0);
			else if (c == '/' && flags & FNM_PATHNAME) {
				if ((string = strchr(string, '/')) == NULL)
					return (FNM_NOMATCH);
				break;
			}

			/* General case, use recursion. */
			while (sc != EOS) {
				if (!fnmatch1(pattern, string, stringstart,
				    flags, patmbs, strmbs))
					return (0);
				sclen = mbrtowc(&sc, string, MB_LEN_MAX,
				    &strmbs);
				if (sclen == (size_t)-1 ||
				    sclen == (size_t)-2) {
					sc = (unsigned char)*string;
					sclen = 1;
					memset(&strmbs, 0, sizeof(strmbs));
				}
				if (sc == '/' && flags & FNM_PATHNAME)
					break;
				string += sclen;
			}
			return (FNM_NOMATCH);
		case '[':
			if (sc == EOS)
				return (FNM_NOMATCH);
			if (sc == '/' && (flags & FNM_PATHNAME))
				return (FNM_NOMATCH);
			if (sc == '.' && (flags & FNM_PERIOD) &&
			    (string == stringstart ||
			    ((flags & FNM_PATHNAME) && *(string - 1) == '/')))
				return (FNM_NOMATCH);

			switch (rangematch(pattern, sc, flags, &newp,
			    &patmbs)) {
			case RANGE_ERROR:
				goto norm;
			case RANGE_MATCH:
				pattern = newp;
				break;
			case RANGE_NOMATCH:
				return (FNM_NOMATCH);
			}
			string += sclen;
			break;
		case '\\':
			if (!(flags & FNM_NOESCAPE)) {
				pclen = mbrtowc(&pc, pattern, MB_LEN_MAX,
				    &patmbs);
				if (pclen == (size_t)-1 || pclen == (size_t)-2)
					return (FNM_NOMATCH);
				pattern += pclen;
			}
			/* FALLTHROUGH */
		default:
		norm:
			if (pc == sc)
				;
			else if ((flags & FNM_CASEFOLD) &&
				 (towlower(pc) == towlower(sc)))
				;
			else
				return (FNM_NOMATCH);
			string += sclen;
			break;
		}
	}
	/* NOTREACHED */
}

static int rangematch(const char * pattern, wchar_t test,int flags ,char ** newp,mbstate_t * patmbs)
{
	int negate, ok;
	wchar_t c, c2;
	size_t pclen;
	const char *origpat;
	
	/*
	 * A bracket expression starting with an unquoted circumflex
	 * character produces unspecified results (IEEE 1003.2-1992,
	 * 3.13.2).  This implementation treats it like '!', for
	 * consistency with the regular expression syntax.
	 * J.T. Conklin (conklin@ngai.kaleida.com)
	 */
	if ( (negate = (*pattern == '!' || *pattern == '^')) )
		++pattern;

	if (flags & FNM_CASEFOLD)
		test = towlower(test);

	/*
	 * A right bracket shall lose its special meaning and represent
	 * itself in a bracket expression if it occurs first in the list.
	 * -- POSIX.2 2.8.3.2
	 */
	ok = 0;
	origpat = pattern;
	for (;;) {
		if (*pattern == ']' && pattern > origpat) {
			pattern++;
			break;
		} else if (*pattern == '\0') {
			return (RANGE_ERROR);
		} else if (*pattern == '/' && (flags & FNM_PATHNAME)) {
			return (RANGE_NOMATCH);
		} else if (*pattern == '\\' && !(flags & FNM_NOESCAPE))
			pattern++;
		pclen = mbrtowc(&c, pattern, MB_LEN_MAX, patmbs);
		if (pclen == (size_t)-1 || pclen == (size_t)-2)
			return (RANGE_NOMATCH);
		pattern += pclen;

		if (flags & FNM_CASEFOLD)
			c = towlower(c);

		if (*pattern == '-' && *(pattern + 1) != EOS &&
		    *(pattern + 1) != ']') {
			if (*++pattern == '\\' && !(flags & FNM_NOESCAPE))
				if (*pattern != EOS)
					pattern++;
			pclen = mbrtowc(&c2, pattern, MB_LEN_MAX, patmbs);
			if (pclen == (size_t)-1 || pclen == (size_t)-2)
				return (RANGE_NOMATCH);
			pattern += pclen;
			if (c2 == EOS)
				return (RANGE_ERROR);

			if (flags & FNM_CASEFOLD)
				c2 = towlower(c2);

			if (c <= test && test <= c2)
				ok = 1;
		} else if (c == test)
			ok = 1;
	}

	*newp = (char *)pattern;
	return (ok == negate ? RANGE_NOMATCH : RANGE_MATCH);
}
