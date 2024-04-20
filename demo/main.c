#include <cargs.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static struct cag_option options[] = {
  {.identifier = 's',
    .access_letters = "s",
    .access_name = NULL,
    .value_name = NULL,
    .description = "Simple flag"},

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
    .description = "Parameter value"},

  {.identifier = 'h',
    .access_letters = "h",
    .access_name = "help",
    .description = "Shows the command help"},
};

int main(int argc, char *argv[])
{
  bool simple_flag = false, multiple_flag = false, long_flag = false;
  const char *value = NULL;
  int param_index;

  cag_option_context context;
  cag_option_init(&context, options, CAG_ARRAY_SIZE(options), argc, argv);
  while (cag_option_fetch(&context)) {
    switch (cag_option_get_identifier(&context)) {
    case 's':
      simple_flag = true;
      break;
    case 'm':
      multiple_flag = true;
      break;
    case 'l':
      long_flag = true;
      break;
    case 'k':
      value = cag_option_get_value(&context);
      break;
    case 'h':
      printf("Usage: cargsdemo [OPTION]...\n");
      printf("Demonstrates the cargs library.\n\n");
      cag_option_print(options, CAG_ARRAY_SIZE(options), stdout);
      printf("\nNote that all formatting is done by cargs.\n");
      return EXIT_SUCCESS;
    case '?':
      cag_option_print_error(&context, stdout);
      break;
    }
  }

  printf("simple_flag: %i, multiple_flag: %i, long_flag: %i, key: %s\n",
    simple_flag, multiple_flag, long_flag, value ? value : "-");

  for (param_index = cag_option_get_index(&context); param_index < argc;
       ++param_index) {
    printf("additional parameter: %s\n", argv[param_index]);
  }

  return EXIT_SUCCESS;
}
