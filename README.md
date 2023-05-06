<img style="width:100%;" src="banner.png">

[![Travis Build](https://img.shields.io/travis/com/likle/cargs/master?label=Linux%2C%20macOS%20%26%20FreeBSD)](https://app.travis-ci.com/github/likle/cargs)
[![Appveyor Build](https://img.shields.io/appveyor/ci/likle/cargs/master.svg?label=Windows)](https://ci.appveyor.com/project/likle/cargs) 
[![codecov](https://img.shields.io/codecov/c/github/likle/cargs/master.svg?label=Coverage)](https://codecov.io/gh/likle/cargs)

# libcargs - command line argument library for C/C++
This is a lighweight C command line argument library. It is currently compiled and 
tested under **Windows**, **MacOS** and **Linux**. 

## Features
Please have a look at the 
**[reference](https://likle.github.io/cargs/reference/)** for detailed 
information. Some features this library includes:

 * **cross-platform** on windows, linux and macOS
 * **simple interface** - just one header
 * **one simple loop** - to iterate over the arguments
 * **automatic help output** - showing all options available 
 * **long and short options** - giving users alternatives
 * **option values** - for options which are more than just flags 
 
 ## Building
 **[Building](https://likle.github.io/cargs/build.html)**, 
 **[embedding](https://likle.github.io/cargs/embed.html)** and 
 **[testing](https://likle.github.io/cargs/build.html)** instructions are 
 available in the documentation (it's very easy).
 
 ## Docs
 All the documentation is available in the 
 **[the github page](https://likle.github.io/cargs/)** of this repository.
 
 ## Example
 ```c
#include <cargs.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 * This is the main configuration of all options available.
 */
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
    .description = "Shows the command help"}};

/**
 * This is a custom project configuration structure where you can store the
 * parsed information.
 */
struct demo_configuration
{
  bool simple_flag;
  bool multiple_flag;
  bool long_flag;
  const char *key;
};

int main(int argc, char *argv[])
{
  char identifier;
  const char *value;
  cag_option_context context;
  struct demo_configuration config = {false, false, false, NULL};
  int param_index;

  /**
   * Now we just prepare the context and iterate over all options. Simple!
   */
  cag_option_prepare(&context, options, CAG_ARRAY_SIZE(options), argc, argv);
  while (cag_option_fetch(&context)) {
    identifier = cag_option_get(&context);
    switch (identifier) {
    case 's':
      config.simple_flag = true;
      break;
    case 'm':
      config.multiple_flag = true;
      break;
    case 'l':
      config.long_flag = true;
      break;
    case 'k':
      value = cag_option_get_value(&context);
      config.key = value;
      break;
    case 'h':
      printf("Usage: cargsdemo [OPTION]...\n");
      printf("Demonstrates the cargs library.\n\n");
      cag_option_print(options, CAG_ARRAY_SIZE(options), stdout);
      printf("\nNote that all formatting is done by cargs.\n");
      return EXIT_SUCCESS;
    }
  }

  printf("simple_flag: %i, multiple_flag: %i, long_flag: %i, key: %s\n",
    config.simple_flag, config.multiple_flag, config.long_flag,
    config.key ? config.key : "-");
  
  for (param_index = context.index; param_index < argc; ++param_index) {
    printf("additional parameter: %s\n", argv[param_index]);
  }

  return EXIT_SUCCESS;
}

```

### Example output
```console
foo@bar:~$ ./cargsdemo 
simple_flag: 0, multiple_flag: 0, long_flag: 0, key: -
```

```console
foo@bar:~$ ./cargsdemo -k=test -sm --long
simple_flag: 1, multiple_flag: 1, long_flag: 1, key: test
```

```console
foo@bar:~$ ./cargsdemo --help
Usage: cargsdemo [OPTION]...
Demonstrates the cargs library.

  -s                   Simple flag
  -m, -M, -o, -O       Multiple access letters
  --long               Long parameter name
  -k, --key=VALUE      Parameter value
  -h, --help           Shows the command help

Note that all formatting is done by cargs.
```

```console
foo@bar:~$ ./cargsdemo also -k=test some -sm additional --long parameters
simple_flag: 1, multiple_flag: 1, long_flag: 1, key: test
additional parameter: also
additional parameter: some
additional parameter: additional
additional parameter: parameters
```
