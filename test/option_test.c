#include "definitions.h"
#include <assert.h>
#include <cargs.h>
#include <limits.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct cag_option options[] = {

  {.identifier = 's',
    .access_letters = "s",
    .access_name = NULL,
    .value_name = NULL,
    .description = "Simple flag"},

  {.identifier = 'a',
    .access_letters = "a",
    .access_name = NULL,
    .value_name = NULL,
    .description = "Another simple flag"},

  {.identifier = 'm',
    .access_letters = "mMoO",
    .access_name = NULL,
    .value_name = NULL,
    .description = "Multiple access letters"},

  {.identifier = 'l',
    .access_letters = NULL,
    .access_name = "long",
    .value_name = NULL,
    .description = "Long parameter name"},

  {.identifier = 'k',
    .access_letters = "k",
    .access_name = "key",
    .value_name = "VALUE",
    .description = "Parameter value"}};

struct cag_result
{
  bool simple;
  bool another;
  bool multi_access;
  bool long_parameter;
  bool value_parameter;
  bool unknown;
  bool def;
  const char *value;

  int error_index;
  char error_letter;
};

cag_option_context context;
static struct cag_result result;
static char **argv;
static int argc;

static int make_args(const char *str)
{
  const char *c;
  int argIndex, argStart, argLength;

  argc = 0;
  c = str;
  do {
    if (*c == ' ' || *c == '\0') {
      ++argc;
    }
  } while (*(c++));

  argv = malloc(sizeof(char *) * (argc + 1));
  if (argv == NULL) {
    return 1;
  }

  c = str;
  argIndex = 0;
  argStart = 0;
  argLength = 0;
  do {
    if (*c == ' ' || *c == '\0') {
      argv[argIndex] = malloc(argLength + 1);
      memcpy(argv[argIndex], &str[argStart], argLength);
      argv[argIndex][argLength] = '\0';
      ++argIndex;
      argStart += argLength + 1;
      argLength = 0;
    } else {
      ++argLength;
    }

  } while (*(c++));

  argv[argc] = NULL;

  return 0;
}

static void destroy_args(void)
{
  int i;

  for (i = 0; i < argc; ++i) {
    free(argv[i]);
  }

  free(argv);
}

static int option_test_run(int currentArgc, char *currentArgv[])
{
  int index;
  char identifier;

  cag_option_init(&context, options, CAG_ARRAY_SIZE(options), currentArgc,
    currentArgv);

  memset(&result, 0, sizeof(result));

  result.error_index = cag_option_get_error_index(&context);
  result.error_letter = cag_option_get_error_letter(&context);

  while (cag_option_fetch(&context)) {
    identifier = cag_option_get_identifier(&context);
    switch (identifier) {
    case 's':
      result.simple = true;
      break;
    case 'a':
      result.another = true;
      break;
    case 'm':
      result.multi_access = true;
      break;
    case 'l':
      result.long_parameter = true;
      break;
    case 'k':
      result.value_parameter = true;
      result.value = cag_option_get_value(&context);
      break;
    case '?':
      result.error_index = cag_option_get_error_index(&context);
      result.error_letter = cag_option_get_error_letter(&context);
      result.unknown = true;
      break;
    default:
      result.def = true;
      break;
    }
  }

  index = cag_option_get_index(&context);
  if (cag_option_fetch(&context) != false) {
    return -1;
  }

  if (cag_option_get_index(&context) != index) {
    return -1;
  }

  return cag_option_get_index(&context);
}

int option_complex(void)
{
  int status;

  status = make_args("test file1 -s -- -a -- a file");
  if (status != 0) {
    goto err_setup;
  }

  status = option_test_run(argc, argv);
  if (status < 0) {
    goto err_wrong;
  }

  if (!result.simple || result.another || result.multi_access ||
      result.long_parameter || result.value_parameter || result.unknown ||
      result.def || result.value != NULL || result.error_index != -1 ||
      result.error_letter) {
    goto err_wrong;
  }

  destroy_args();

  return EXIT_SUCCESS;

err_wrong:
  destroy_args();
err_setup:
  return EXIT_FAILURE;
}

int option_mixed(void)
{
  int status, i;

  const char *values[] = {"file1", "file2", "mixed", "file3", "--", "-m",
    "parameters", "file4"};

  status = make_args(
    "test -s file1 -k=value file2 -a mixed file3 -- -m parameters file4");
  if (status != 0) {
    goto err_setup;
  }

  status = option_test_run(argc, argv);
  if (status < 0 || argc - status != 8) {
    goto err_wrong;
  }

  for (i = 0; i < (int)CAG_ARRAY_SIZE(values); ++i) {
    if (strcmp(argv[status + i], values[i]) != 0) {
      goto err_wrong;
    }
  }

  if (!result.simple || !result.another || result.multi_access ||
      result.long_parameter || !result.value_parameter || result.unknown ||
      result.def || result.value == NULL || result.error_index != -1 ||
      result.error_letter || strcmp(result.value, "value") != 0) {
    goto err_wrong;
  }

  destroy_args();

  return EXIT_SUCCESS;

err_wrong:
  destroy_args();
err_setup:
  return EXIT_FAILURE;
}

int option_ending(void)
{
  int status;

  status = make_args("test -s -- -a");
  if (status != 0) {
    goto err_setup;
  }

  status = option_test_run(argc, argv);
  if (status < 0) {
    goto err_wrong;
  }

  if (!result.simple || result.another || result.multi_access ||
      result.long_parameter || result.value_parameter || result.unknown ||
      result.def || result.value != NULL || result.error_index != -1 ||
      result.error_letter) {
    goto err_wrong;
  }

  destroy_args();

  return EXIT_SUCCESS;

err_wrong:
  destroy_args();
err_setup:
  return EXIT_FAILURE;
}

int option_long_missing_value(void)
{
  int status;

  status = make_args("test --key");
  if (status != 0) {
    goto err_setup;
  }

  status = option_test_run(argc, argv);
  if (status < 0) {
    goto err_wrong;
  }

  if (result.simple || result.another || result.multi_access ||
      result.long_parameter || !result.value_parameter || result.unknown ||
      result.def || result.value != NULL || result.error_index != -1 ||
      result.error_letter) {
    goto err_wrong;
  }

  destroy_args();

  return EXIT_SUCCESS;

err_wrong:
  destroy_args();
err_setup:
  return EXIT_FAILURE;
}

int option_short_missing_value(void)
{
  int status;

  status = make_args("test -k");
  if (status != 0) {
    goto err_setup;
  }

  status = option_test_run(argc, argv);
  if (status < 0) {
    goto err_wrong;
  }

  if (result.simple || result.another || result.multi_access ||
      result.long_parameter || !result.value_parameter || result.unknown ||
      result.def || result.value != NULL || result.error_index != -1 ||
      result.error_letter) {
    goto err_wrong;
  }

  destroy_args();

  return EXIT_SUCCESS;

err_wrong:
  destroy_args();
err_setup:
  return EXIT_FAILURE;
}

int option_long_space_value(void)
{
  int status;

  status = make_args("test --key super_value");
  if (status != 0) {
    goto err_setup;
  }

  status = option_test_run(argc, argv);
  if (status < 0) {
    goto err_wrong;
  }

  if (result.simple || result.another || result.multi_access ||
      result.long_parameter || !result.value_parameter || result.unknown ||
      result.def || result.value == NULL || result.error_index != -1 ||
      result.error_letter || strcmp(result.value, "super_value") != 0) {
    goto err_wrong;
  }

  destroy_args();

  return EXIT_SUCCESS;

err_wrong:
  destroy_args();
err_setup:
  return EXIT_FAILURE;
}

int option_short_space_value(void)
{
  int status;

  status = make_args("test -k test_value");
  if (status != 0) {
    goto err_setup;
  }

  status = option_test_run(argc, argv);
  if (status < 0) {
    goto err_wrong;
  }

  if (result.simple || result.another || result.multi_access ||
      result.long_parameter || !result.value_parameter || result.unknown ||
      result.def || result.value == NULL || result.error_index != -1 ||
      result.error_letter || strcmp(result.value, "test_value") != 0) {
    goto err_wrong;
  }

  destroy_args();

  return EXIT_SUCCESS;

err_wrong:
  destroy_args();
err_setup:
  return EXIT_FAILURE;
}

int option_long_equal_value(void)
{
  int status;

  status = make_args("test --key=super_value");
  if (status != 0) {
    goto err_setup;
  }

  status = option_test_run(argc, argv);
  if (status < 0) {
    goto err_wrong;
  }

  if (result.simple || result.another || result.multi_access ||
      result.long_parameter || !result.value_parameter || result.unknown ||
      result.def || result.value == NULL || result.error_index != -1 ||
      result.error_letter || strcmp(result.value, "super_value") != 0) {
    goto err_wrong;
  }

  destroy_args();

  return EXIT_SUCCESS;

err_wrong:
  destroy_args();
err_setup:
  return EXIT_FAILURE;
}

int option_short_equal_value(void)
{
  int status;

  status = make_args("test -k=test_value");
  if (status != 0) {
    goto err_setup;
  }

  status = option_test_run(argc, argv);
  if (status < 0) {
    goto err_wrong;
  }

  if (result.simple || result.another || result.multi_access ||
      result.long_parameter || !result.value_parameter || result.unknown ||
      result.def || result.value == NULL || result.error_index != -1 ||
      result.error_letter || strcmp(result.value, "test_value") != 0) {
    goto err_wrong;
  }

  destroy_args();

  return EXIT_SUCCESS;

err_wrong:
  destroy_args();
err_setup:
  return EXIT_FAILURE;
}

int option_combined(void)
{
  int status;

  status = make_args("test -sma");
  if (status != 0) {
    goto err_setup;
  }

  status = option_test_run(argc, argv);
  if (status < 0) {
    goto err_wrong;
  }

  if (!result.simple || !result.another || !result.multi_access ||
      result.long_parameter || result.value_parameter || result.unknown ||
      result.def || result.value != NULL || result.error_index != -1 ||
      result.error_letter) {
    goto err_wrong;
  }

  destroy_args();

  return EXIT_SUCCESS;

err_wrong:
  destroy_args();
err_setup:
  return EXIT_FAILURE;
}

int option_unknown_long(void)
{
  int status;

  status = make_args("test --unknown");
  if (status != 0) {
    goto err_setup;
  }

  status = option_test_run(argc, argv);
  if (status < 0) {
    goto err_wrong;
  }

  if (result.simple || result.another || result.multi_access ||
      result.long_parameter || result.value_parameter || !result.unknown ||
      result.def || result.value != NULL || result.error_index != 1 ||
      result.error_letter) {
    goto err_wrong;
  }

  destroy_args();

  return EXIT_SUCCESS;

err_wrong:
  destroy_args();
err_setup:
  return EXIT_FAILURE;
}

int option_unknown_long_shift(void)
{
  int status;

  status = make_args("test foo bar --unknown param");
  if (status != 0) {
    goto err_setup;
  }

  status = option_test_run(argc, argv);
  if (status < 0) {
    goto err_wrong;
  }

  if (result.simple || result.another || result.multi_access ||
      result.long_parameter || result.value_parameter || !result.unknown ||
      result.def || result.value != NULL || result.error_index != 1 ||
      result.error_letter) {
    goto err_wrong;
  }

  if (strcmp(argv[result.error_index], "--unknown") != 0) {
    goto err_wrong;
  }

  destroy_args();

  return EXIT_SUCCESS;

err_wrong:
  destroy_args();
err_setup:
  return EXIT_FAILURE;
}

int option_unknown_short(void)
{
  int status;

  status = make_args("test -u");
  if (status != 0) {
    goto err_setup;
  }

  status = option_test_run(argc, argv);
  if (status < 0) {
    goto err_wrong;
  }

  if (result.simple || result.another || result.multi_access ||
      result.long_parameter || result.value_parameter || !result.unknown ||
      result.def || result.value != NULL || result.error_index != 1 ||
      result.error_letter != 'u') {
    goto err_wrong;
  }

  if (strcmp(argv[result.error_index], "-u") != 0) {
    goto err_wrong;
  }

  destroy_args();

  return EXIT_SUCCESS;

err_wrong:
  destroy_args();
err_setup:
  return EXIT_FAILURE;
}

int option_unknown_short_shift(void)
{
  int status;

  status = make_args("test foo bar -ug params here");
  if (status != 0) {
    goto err_setup;
  }

  status = option_test_run(argc, argv);
  if (status < 0) {
    goto err_wrong;
  }

  if (result.simple || result.another || result.multi_access ||
      result.long_parameter || result.value_parameter || !result.unknown ||
      result.def || result.value != NULL || result.error_index != 1 ||
      result.error_letter != 'g') {
    goto err_wrong;
  }

  if (strcmp(argv[result.error_index], "-ug") != 0) {
    goto err_wrong;
  }

  destroy_args();

  return EXIT_SUCCESS;

err_wrong:
  destroy_args();
err_setup:
  return EXIT_FAILURE;
}

int option_alias(void)
{
  int status;

  status = make_args("test -O");
  if (status != 0) {
    goto err_setup;
  }

  status = option_test_run(argc, argv);
  if (status < 0) {
    goto err_wrong;
  }

  if (result.simple || result.another || !result.multi_access ||
      result.long_parameter || result.value_parameter || result.unknown ||
      result.def || result.value != NULL || result.error_index != -1 ||
      result.error_letter) {
    goto err_wrong;
  }

  destroy_args();

  return EXIT_SUCCESS;

err_wrong:
  destroy_args();
err_setup:
  return EXIT_FAILURE;
}

int option_simple_long(void)
{
  int status;

  status = make_args("test --long");
  if (status != 0) {
    goto err_setup;
  }

  status = option_test_run(argc, argv);
  if (status < 0) {
    goto err_wrong;
  }

  if (result.simple || result.another || result.multi_access ||
      !result.long_parameter || result.value_parameter || result.unknown ||
      result.def || result.value != NULL || result.error_index != -1 ||
      result.error_letter) {
    goto err_wrong;
  }

  destroy_args();

  return EXIT_SUCCESS;

err_wrong:
  destroy_args();
err_setup:
  return EXIT_FAILURE;
}

int option_simple(void)
{
  int status;

  status = make_args("test -s");
  if (status != 0) {
    goto err_setup;
  }

  status = option_test_run(argc, argv);
  if (status < 0) {
    goto err_wrong;
  }

  if (!result.simple || result.another || result.multi_access ||
      result.long_parameter || result.value_parameter || result.unknown ||
      result.def || result.value != NULL || result.error_index != -1 ||
      result.error_letter) {
    goto err_wrong;
  }

  destroy_args();

  return EXIT_SUCCESS;

err_wrong:
  destroy_args();
err_setup:
  return EXIT_FAILURE;
}

int option_boundaries(void)
{
  int status;
  int tmpargc;

  status = make_args("test -s -a");
  if (status != 0) {
    goto err_setup;
  }

  // Remove the "-a".
  tmpargc = argc;
  argc = 2;

  status = option_test_run(argc, argv);
  if (status < 0) {
    goto err_wrong;
  }

  if (!result.simple || result.another || result.error_index != -1 ||
      result.error_letter) {
    goto err_wrong;
  }

    argc = tmpargc;
  destroy_args();

  return EXIT_SUCCESS;

err_wrong:
  destroy_args();
err_setup:
  return EXIT_FAILURE;
}

int option_boundaries_mix(void)
{
  int status;
  int tmpargc;

  status = make_args("test foobar -s -a ");
  if (status != 0) {
    goto err_setup;
  }

  // Remove the "-s" and "-a".
  tmpargc = argc;
  argc = 2;

  status = option_test_run(argc, argv);
  if (status < 0) {
    goto err_wrong;
  }

  if (result.simple || result.another || result.error_index != -1 ||
      result.error_letter) {
    goto err_wrong;
  }

  argc = tmpargc;
  destroy_args();

  return EXIT_SUCCESS;

err_wrong:
  destroy_args();
err_setup:
  return EXIT_FAILURE;
}

int option_print(void)
{
#ifndef CAG_NO_FILE
  char buf[255];
  const char *expected;
  FILE *test_file;

  expected = "  -s                   Simple flag\n"
             "  -a                   Another simple flag\n"
             "  -m, -M, -o, -O       Multiple access letters\n"
             "  --long               Long parameter name\n"
             "  -k, --key=VALUE      Parameter value\n";

  test_file = tmpfile();
  if (test_file == NULL) {
    goto err_open;
  }

  cag_option_print(options, CAG_ARRAY_SIZE(options), test_file);
  if (fseek(test_file, 0, SEEK_SET) != 0) {
    goto err_seek;
  }

  if (fread(buf, sizeof(buf), 1, test_file) != 1 && feof(test_file) == 0) {
    goto err_read;
  }

  if (memcmp(buf, expected, strlen(expected)) != 0) {
    goto err_test;
  }

  fclose(test_file);
  return EXIT_SUCCESS;

err_test:
err_read:
err_seek:
  fclose(test_file);
err_open:
  return EXIT_FAILURE;

#else // #ifndef CAG_NO_FILE
  return EXIT_SUCCESS;
#endif
}

#ifdef CAG_NO_FILE
typedef void FILE_CTX;
#else
typedef FILE FILE_CTX;
#endif

int option_printer(void)
{
  char buf[255];
  const char *expected;
  FILE_CTX *test_file;

  expected = "  -s                   Simple flag\n"
             "  -a                   Another simple flag\n"
             "  -m, -M, -o, -O       Multiple access letters\n"
             "  --long               Long parameter name\n"
             "  -k, --key=VALUE      Parameter value\n";

  test_file = tmpfile();
  if (test_file == NULL) {
    goto err_open;
  }

  cag_option_printer(options, CAG_ARRAY_SIZE(options), (cag_printer)fprintf,
    test_file);
  if (fseek(test_file, 0, SEEK_SET) != 0) {
    goto err_seek;
  }

  if (fread(buf, sizeof(buf), 1, test_file) != 1 && feof(test_file) == 0) {
    goto err_read;
  }

  if (memcmp(buf, expected, strlen(expected)) != 0) {
    goto err_test;
  }

  fclose(test_file);
  return EXIT_SUCCESS;

err_test:
err_read:
err_seek:
  fclose(test_file);
err_open:
  return EXIT_FAILURE;

}

int option_error_print_short(void)
{
  int status;
  char buf[255];
  const char *expected;
  FILE_CTX *test_file;

  expected = "Unknown option 'u' in '-abu'.\n";

  test_file = tmpfile();
  if (test_file == NULL) {
    goto err_open;
  }

  status = make_args("test -abu");
  if (status != 0) {
    goto err_setup;
  }

  context.argv = argv;
  context.argc = argc;
  context.error_index = 1;
  context.error_letter = 'u';

#ifdef CAG_NO_FILE
  cag_option_printer_error(&context, (cag_printer)fprintf, test_file);
#else
  cag_option_print_error(&context, test_file);
#endif

  if (fseek(test_file, 0, SEEK_SET) != 0) {
    goto err_seek;
  }

  if (fread(buf, sizeof(buf), 1, test_file) != 1 && feof(test_file) == 0) {
    goto err_read;
  }

  if (memcmp(buf, expected, strlen(expected)) != 0) {
    goto err_test;
  }

  destroy_args();
  fclose(test_file);
  return EXIT_SUCCESS;

err_test:
err_read:
err_seek:
  destroy_args();
err_setup:
  fclose(test_file);
err_open:
  return EXIT_FAILURE;
}

int option_error_print_long(void)
{
  int status;
  char buf[255];
  const char *expected;
  FILE_CTX *test_file;

  expected = "Unknown option '--unknown'.\n";

  test_file = tmpfile();
  if (test_file == NULL) {
    goto err_open;
  }

  status = make_args("test --unknown");
  if (status != 0) {
    goto err_setup;
  }

  context.argv = argv;
  context.argc = argc;
  context.error_index = 1;
  context.error_letter = 0;

  cag_option_printer_error(&context, (cag_printer)fprintf, test_file);
  if (fseek(test_file, 0, SEEK_SET) != 0) {
    goto err_seek;
  }

  if (fread(buf, sizeof(buf), 1, test_file) != 1 && feof(test_file) == 0) {
    goto err_read;
  }

  if (memcmp(buf, expected, strlen(expected)) != 0) {
    goto err_test;
  }

  destroy_args();
  fclose(test_file);
  return EXIT_SUCCESS;

err_test:
err_read:
err_seek:
  destroy_args();
err_setup:
  fclose(test_file);
err_open:
  return EXIT_FAILURE;
}
