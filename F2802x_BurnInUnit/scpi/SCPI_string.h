/**
 * @file SCPI_string.h
 *
 * This file provides functions used by various parts
 * of the parser for string manipulation.
 *
 * The standard C file string.h is not used as only
 * a subset of its functions are required and some
 * have been altered to suit the requirements of the
 * parser
 *
 * This file should not need to be edited by the user
 *
 */

#ifndef SCPI_STRING_H_
#define SCPI_STRING_H_

#ifdef __cplusplus
	extern "C" {
#endif

#define isnan(x) ((x) != (x))				/**< Check if x is NaN. */
#define isinf(x) (!isnan(x) && isnan(x - x))/**< Check if x is Infinite. */

/** Converts n to a string, places the result in s and returns s.
 * @param[in]	n	Value to be converted.
 * @param[out]	s	String that converted value is to be placed in.
 * @return			Equal to s.
 */
extern char * dtoa(double n, char *s);

/** Converts value to a number in base radix, places the result in str and returns str.
 * Thus with a radix of 10, this converts an integer to its ASCII representation.
 * @param[in]	value	Value to be converted.
 * @param[out]	str		String that converted value is to be placed in.
 * @param[in]	radix	Radix or base of value.
 * @return				Equal to str.
 */
extern char * itoa (int value, char* str, int radix);

/** Concatenates or appends source to destination and returns destination.
 * All characters from source are copied including the terminating null character.
 * @param[out]	destination	String that is to be concatenated to.
 * @param[in]	source		String that is to be added.
 * @return					Equal to destination.
 */
extern char * strcat (char * destination, const char * source);

/** Copies characters from src to dest up to and including the terminating null
 * character then returns dest.
 * @param[out]	dest	Location to be copied to.
 * @param[in]	src		String to be copied.
 * @return				Equal to dest.
 */
extern char * strcpy (char *restrict dest, const char *restrict  src);

/** Searches string for the first occurrence of c and returns a pointer to it.
 * The null character terminating string is included in the search.
 * @param[in]	s	String to search.
 * @param[in]	c	Character to search for.
 * @return			Location of c in s or null if c isn't found.
 */
extern char * strchr (const char *s, Uint16 c);

/** Calculates and returns the length of the initial segment of s1 which consists
 * entirely of characters in s2.
 * @sa strcspn
 * @param[in]	s1	String to search.
 * @param[in]	s2	String of characters to match in s1.
 * @return			Number of characters in the initial segment of s1 which
 * 					consist only of characters from str2.
 */
extern Uint16 strspn (const char *s1, const char *s2);

/** Calculates and returns the length of the initial segment of s1 which consists
 *  entirely characters NOT in s2.
 * @sa strspn()
 * @param[in]	s1	String to search.
 * @param[in]	s2	String of characters to match in s1.
 * @return			Number of characters in the initial segment of s1 which are NOT in s2.
 */
extern Uint16 strcspn(const char *s1, const char *s2);

/** A reentrant version of strtok().
 * Breaks s1 into a series of tokens using delimiters, using context pointer lasts to
 * determine where to start on each iteration, saving any progress to lasts on
 * completion and returning a pointer to the last token found, or NULL if no further
 * tokens are left.
 * @param[in]		s			The string to be broken into tokens.
 * @param[in]		delimiters	A string of delimiter characters.
 * @param[in,out]	lasts		A context pointer that indicates the previous. On the first use
 * 								of this function for a specific string this should be NULL, and
 * 								will be then set by the function. Subsequent calls should then
 * 								pass the value as previously set by the function.
 * @return						Location of the start of the found token.
 */
extern char * strtok_r (char *s, const char *delimiters, char **lasts);

/** Breaks s1 into a series of tokens using delimiters, returns a pointer to the last
 * token found, or NULL if no further tokens are left. This function is designed to be
 * non-reentrant. For a reentrant version use strtok_r().
 * @param[in]	s1			The string to be broken into tokens.
 * @param[in]	delimiters	A string of delimiter characters.
 * @return					Location of the start of the found token.
 */
extern char * strtok (char *restrict s1, const char *restrict delimiters);

/** Compares str1 and str2.
 * @param[in]	s1	The first string to be compared.
 * @param[in]	s2	The second string to be compared.
 * @return			< 0 if str1 is less than str2,
 * 					> 0 if str2 is less than str1,
 * 					= 0 if str1 is equal to str2.
 */
extern Uint16 strcmp(const char *s1, const char *s2);

/** Uses strtok() to split searchStr at each occurrence of delimiter, then saves
 * each token to a results array.
 * @param[in]	searchStr	The string to search.
 * @param[out]	results		The location at which to place the results.
 * @param[in]	delimiter	A string of delimiter characters.
 */
extern void breakString(char * searchStr, char ** results, const char * delimiter);

#ifdef __cplusplus
	}
#endif /* extern "C" */

#endif /* SCPI_STRING_H_ */
