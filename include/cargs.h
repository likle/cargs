#pragma once

/**
 * This is a simple alternative cross-platform implementation of getopt, which
 * is used to parse argument strings submitted to the executable (argc and argv
 * which are received in the main function).
 */

#ifndef CAG_LIBRARY_H
#define CAG_LIBRARY_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

/**
 * The following defines CAG_EXPORT and CAG_IMPORT which are required to export
 * shared library functions.
 */
#if defined(_WIN32) || defined(__CYGWIN__)
#define CAG_EXPORT __declspec(dllexport)
#define CAG_IMPORT __declspec(dllimport)
#elif __GNUC__ >= 4
#define CAG_EXPORT __attribute__((visibility("default")))
#define CAG_IMPORT __attribute__((visibility("default")))
#else
#define CAG_EXPORT
#define CAG_IMPORT
#endif

/**
 * This block defines CAG_PUBLIC, which only uses CAG_EXPORT and CAG_IMPORT if
 * the cargs is compiled as a shared library.
 */
#if defined(CAG_SHARED)
#if defined(CAG_EXPORTS)
#define CAG_PUBLIC CAG_EXPORT
#else
#define CAG_PUBLIC CAG_IMPORT
#endif
#else
#define CAG_PUBLIC
#endif

/**
 * This block defines CAG_DEPRECATED which can be used to deprecate library
 * functions including a comment on the deprecation.
 */
#if (!__cplusplus && __STDC_VERSION__ >= 202311L) || (__cplusplus >= 201402L)
#define CAG_DEPRECATED(comment) [[deprecated(comment)]]
#else
#define CAG_DEPRECATED(comment)
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * An option is used to describe a flag/argument option submitted when the
 * program is run.
 */
typedef struct cag_option
{
  const char identifier;
  const char *access_letters;
  const char *access_name;
  const char *value_name;
  const char *description;
} cag_option;

/**
 * A context is used to iterate over all options provided. It stores the parsing
 * state.
 */
typedef struct cag_option_context
{
  const struct cag_option *options;
  size_t option_count;
  int argc;
  char **argv;
  int index;
  int inner_index;
  int error_index;
  char error_letter;
  bool forced_end;
  char identifier;
  char *value;
} cag_option_context;

/**
 * Prototype for printer used in cag_option_printer. For example fprintf have
 * same prototype
 */
typedef int (*cag_printer)(void *ctx, const char *fmt, ...);

/**
 * This is just a small macro which calculates the size of an array.
 */
#define CAG_ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

/**
 * @brief Prepare argument options context for parsing.
 *
 * This function prepares the context for iteration and initializes the context
 * with the supplied options and arguments. After the context has been prepared,
 * it can be used to fetch arguments from it.
 *
 * @param context The context which will be initialized.
 * @param options The registered options which are available for the program.
 * @param option_count The amount of options which are available for the
 * program.
 * @param argc The amount of arguments the user supplied in the main function.
 * @param argv A pointer to the arguments of the main function.
 */
CAG_PUBLIC void cag_option_init(cag_option_context *context,
  const cag_option *options, size_t option_count, int argc, char **argv);

/**
 * @brief Fetches an option from the argument list.
 *
 * This function fetches a single option from the argument list. The context
 * will be moved to that item. Information can be extracted from the context
 * after the item has been fetched.
 * The arguments will be re-ordered, which means that non-option arguments will
 * be moved to the end of the argument list. After all options have been
 * fetched, all non-option arguments will be positioned after the index of
 * the context.
 *
 * @param context The context from which we will fetch the option.
 * @return Returns true if there was another option or false if the end is
 * reached.
 */
CAG_PUBLIC bool cag_option_fetch(cag_option_context *context);

/**
 * @brief Gets the identifier of the option.
 *
 * This function gets the identifier of the option, which should be unique to
 * this option and can be used to determine what kind of option this is.
 *
 * @param context The context from which the option was fetched.
 * @return Returns the identifier of the option.
 */
CAG_PUBLIC char cag_option_get_identifier(const cag_option_context *context);

/**
 * @brief Gets the value from the option.
 *
 * This function gets the value from the option, if any. If the option does not
 * contain a value, this function will return NULL.
 *
 * @param context The context from which the option was fetched.
 * @return Returns a pointer to the value or NULL if there is no value.
 */
CAG_PUBLIC const char *cag_option_get_value(const cag_option_context *context);

/**
 * @brief Gets the current index of the context.
 *
 * This function gets the index within the argv arguments of the context. The
 * context always points to the next item which it will inspect. This is
 * particularly useful to inspect the original argument array, or to get
 * non-option arguments after option fetching has finished.
 *
 * @param context The context from which the option was fetched.
 * @return Returns the current index of the context.
 */
CAG_PUBLIC int cag_option_get_index(const cag_option_context *context);

/**
 * @brief Retrieves the index of an invalid option.
 *
 * This function retrieves the index of an invalid option if the provided option
 * does not match any of the options specified in the `cag_option` list. This is
 * particularly useful when detailed information about an invalid option is
 * required.
 *
 * @param context Pointer to the context from which the option was fetched.
 * @return Returns the index of the invalid option, or -1 if it is not invalid.
 */
CAG_PUBLIC int cag_option_get_error_index(const cag_option_context *context);

/**
 * @brief Retrieves the letter character of the invalid option.
 *
 * This function retrieves the character of the invalid option character
 * if the provided option does not match any of the options specified in the
 * `cag_option` list.
 *
 * @param context Pointer to the context from which the option was fetched.
 * @return Returns the letter that was unknown, or 0 otherwise.
 */
CAG_PUBLIC char cag_option_get_error_letter(const cag_option_context *context);

/**
 * @brief Prints the error associated with the invalid option to the specified
 * destination.
 *
 * This function prints information about the error associated with the invalid
 * option to the specified destination (such as a file stream). It helps in
 * displaying the error of the current context.
 *
 * @param context Pointer to the context from which the option was fetched.
 * @param destination Pointer to the file stream where the error information
 * will be printed.
 */
#ifndef CAG_NO_FILE
CAG_PUBLIC void cag_option_print_error(const cag_option_context *context,
  FILE *destination);
#endif

/**
 * @brief Prints the error associated with the invalid option using user
 * callback.
 *
 * This function prints information about the error associated with the invalid
 * option using user callback. Callback prototype is same with fprintf. It helps
 * in displaying the error of the current context.
 *
 * @param context Pointer to the context from which the option was fetched.
 * @param printer The printer callback function. For example fprintf.
 * @param printer_ctx The parameter for printer callback. For example fprintf
 * could use parameter stderr.
 */
CAG_PUBLIC void cag_option_printer_error(const cag_option_context *context,
  cag_printer printer, void *printer_ctx);

/**
 * @brief Prints all options to the terminal.
 *
 * This function prints all options to the terminal. This can be used to
 * generate the output for a "--help" option.
 *
 * @param options The options which will be printed.
 * @param option_count The option count which will be printed.
 * @param destination The destination where the output will be printed.
 */
#ifndef CAG_NO_FILE
CAG_PUBLIC void cag_option_print(const cag_option *options, size_t option_count,
  FILE *destination);
#endif

/**
 * @brief Prints all options using user callback.
 *
 * This function prints all options using user callback. This can be used to
 * generate the output for a "--help" option.
 * Using user callback is useful in tiny system without FILE support
 *
 * @param options The options which will be printed.
 * @param option_count The option count which will be printed.
 * @param destination The destination where the output will be printed.
 * @param printer The printer callback function. For example fprintf.
 * @param printer_ctx The parameter for printer callback. For example fprintf
 * could use parameter stderr.
 */
CAG_PUBLIC void cag_option_printer(const cag_option *options,
  size_t option_count, cag_printer printer, void *printer_ctx);

CAG_DEPRECATED(
  "cag_option_prepare has been deprecated. Use cag_option_init instead.")
CAG_PUBLIC void cag_option_prepare(cag_option_context *context,
  const cag_option *options, size_t option_count, int argc, char **argv);

CAG_DEPRECATED(
  "cag_option_get has been deprecated. Use cag_option_get_identifier instead.")
CAG_PUBLIC char cag_option_get(const cag_option_context *context);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
