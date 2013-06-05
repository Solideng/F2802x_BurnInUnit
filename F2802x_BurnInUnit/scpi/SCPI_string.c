#include "scpi.h"
#include "math.h"

static double PRECISION = 0.00000000000001;

char * dtoa(double n, char *s) {
    if (isnan(n)) {			/* Handle special cases. */
        strcpy(s, "nan");	/* NaN. */
    } else if (isinf(n)) {
        strcpy(s, "inf");	/* Infinite. */
    } else if (n == 0.0) {
        strcpy(s, "0");		/* 0 */
    } else {
        int digit, m, m1;
        char *c = s;
        int neg = (n < 0);
        if (neg)
            n = -n;
        m = log10(n); 		/* Calculate magnitude. */
        int useExp = (m >= 14 || (neg && m >= 9) || m <= -9);
        if (neg)
            *(c++) = '-';
        if (useExp) {		/* Set up for scientific notation. */
            if (m < 0)
               m -= 1.0;
            n = n / pow(10.0, m);
            m1 = m;
            m = 0;
        }
        if (m < 1.0) {
            m = 0;
        }
        while (n > PRECISION || m >= 0) {	/* Convert the number. */
            double weight = pow(10.0, m);
            if (weight > 0 && !isinf(weight)) {
                digit = floor(n / weight);
                n -= (digit * weight);
                *(c++) = '0' + digit;
            }
            if (m == 0 && n > 0)
                *(c++) = '.';
            m--;
        }
        if (useExp) {		/* Convert the exponent. */
            int i, j;
            *(c++) = 'e';
            if (m1 > 0) {
                *(c++) = '+';
            } else {
                *(c++) = '-';
                m1 = -m1;
            }
            m = 0;
            while (m1 > 0) {
                *(c++) = '0' + m1 % 10;
                m1 /= 10;
                m++;
            }
            c -= m;
            for (i = 0, j = m-1; i<j; i++, j--) {	/* Swap without temporary. */
                c[i] ^= c[j];
                c[j] ^= c[i];
                c[i] ^= c[j];
            }
            c += m;
        }
        *(c) = '\0';
    }
    return s;
}

char * itoa (int value, char* str, int radix) {
	/* Converts value to a number in base radix, places the result in str and returns str.
	 * Thus with a radix of 10, this converts an integer to its ASCII representation.
	 */
	static char dig[] = "0123456789" "abcdefghijklmnopqrstuvwxyz";
	int n = 0, neg = 0;
	Uint16 v;
	char *p, *q;
	char c;

	if (radix == 10 && value < 0) {			/* Check if value is negative. */
		value = -value;						/* Convert to positive. */
		neg = 1;							/* Set negative value flag. */
	}
	v = value;								/*  Convert to unsigned type. */
	do {
		str[n++] = dig[v%radix];			/* Get first numeral character. */
		v /= radix;							/* Re-scale number. */
	} while (v);							/* Repeat if number is still > 0. */
	
	if (neg)								/* Check negative value flag. */
		str[n++] = '-';						/* Add negative symbol. */
	str[n] = '\0';							/* Add a string terminator. */
											/* Reverse the string. */
	for (p = str, q = p + (n-1); p < q; ++p, --q)
		c = *p, *p = *q, *q = c;
	return str;
}

char * strcat (char * destination, const char * source) {
	/* Concatenates or appends source to destination and returns destination.
	 * All characters from source are copied including the terminating null character.
	 */
	char *d = destination;
	while (*d) ++d;							/* Move to the end of the current string. */
	while ((*d++ = *source++) != '\0') ;	/* Copy in the new string. */
	return destination;
}

char * strcpy (char *restrict dest, const char *restrict src) {
	/* Copies characters from src to dest up to and including the terminating null
	 * character then returns dest.
	 */
	char * ptr = dest;
	while (*dest++ = *src++);	/* Copy each character from src to dest. */
	return ptr;
}

char * strchr (const char *s, Uint16 c) {
	/* Searches string for the first occurrence of c and returns a pointer to it.
	 * The null character terminating string is included in the search.
	 */
	while (*s != '\0' && *s != (char)c)
		s++;					/* Scan s until c is found or the end is reached. */
	return ( (*s == c) ? (char *) s : NULL );
}

Uint16 strspn (const char *s1, const char *s2) {
	/* Calculates and returns the length of the initial segment of s1 which consists
	 * entirely of characters in s2.
	 */
	const char *sc1;
	for (sc1 = s1; *sc1 != '\0'; sc1++)		/* Scan s1. */
		if (strchr(s2, *sc1) == NULL)		/* Check s1 against s2 characters. */
			return (sc1 - s1);				/* Return position of occurrence of non-s2
											 * character relative to s1 start.
											 */
	return sc1 - s1;
}

Uint16 strcspn (const char *s1, const char *s2) {
	/* Calculates and returns the length of the initial segment of s1 which consists
	 * entirely characters NOT in s2.
	 */
	const char *sc1;
	for (sc1 = s1; *sc1 != '\0'; sc1++)		/* Scan s1. */
		if (strchr(s2, *sc1) != NULL)		/* Check s1 against s2 characters. */
			return (sc1 - s1);				/* Return position of occurrence of s2
			 	 	 	 	 	 	 	 	 * character relative to s1 start.
			 	 	 	 	 	 	 	 	 */
	return sc1 - s1;
}

char * strtok_r (char *s, const char *delimiters, char **lasts) {
	/* Reentrant version of strtok().
	 * Breaks s1 into a series of tokens using delimiters, using context pointer lasts to
	 * determine where to start on each iteration, saving any progress to lasts on
	 * completion and returning a pointer to the last token found, or NULL if no further
	 * tokens are left.
	 */
	char *sbegin, *send;
	sbegin = s ? s : *lasts;/* If s isn't null begin at the start of s, else start at lasts. */
							/* Find length of next token. */
	sbegin += strspn(sbegin, delimiters);
	if (*sbegin == '\0') {	/* Check if the end of s was reached. */
		*lasts = "";		/* Reset context pointer. */
		return NULL;		/* Indicate that no more tokens are in s. */
	}
							/* Find end of current token. */
	send = sbegin + strcspn(sbegin, delimiters);
	if (*send != '\0')		/* Check that the end of the token is not the end of s. */
		*send++ = '\0';		/* Replace delimiter with string terminator */
	*lasts = send;			/* Save start of next token to context pointer ready for
							 * next iteration.
							 */
	return sbegin;			/* Return pointer to start of token. */
}

char * strtok (char *restrict s1, const char *restrict delimiters) {
	/* Breaks s1 into a series of tokens using delimiters, returns a pointer to the
	 * last token found, or NULL if no further tokens are left.
	 * This function is designed to be non-reentrant.
	 * For a reentrant version use strtok_r().
	 */
	static char *ssave = "";				/* Save the context pointer. */
	return strtok_r(s1, delimiters, &ssave);/* Use strtok_r with ssave. */
}


Uint16 strcmp(const char *s1, const char *s2) {
	/* Compares str1 and str2 and returns:
	 * < 0 if str1 is less than str2,
	 * > 0 if str2 is less than str1,
	 * = 0 if str1 is equal to str2.
	 */
	unsigned char uc1, uc2; 
	while (*s1 != '\0' && *s1 == *s2) {
		s1++;	/* Scan the strings, stop when they are not equal or the end is reached. */
		s2++;
	}
				/* Check if the pointer magnitude are the same. */
	uc1 = (*(unsigned char *) s1);
	uc2 = (*(unsigned char *) s2);
	return ((uc1 < uc2) ? -1 : (uc1 > uc2));
}

void breakString(char * searchStr, char ** results, const char * delimiter) {
	/* Uses strtok() to split searchStr at each occurrence of delimiter, then saves
	 * each token to the 2-D results array.
	 */
    *results = strtok(searchStr, delimiter);/* Search for first token. */
    if (*results == NULL)					/* Check if there was no valid first token. */
		*results = searchStr;				/* Save the full string as the result. */
    while (*results != NULL) {				/* Check if there was a valid token */
		results++;							/* Get next token and save to next array position. */
		*results = strtok(NULL, delimiter);
    }
}
