<img style="width:100%;" src="banner.png">

[![Build Pipeline](https://github.com/likle/cargs/actions/workflows/build-linux.yml/badge.svg)](https://github.com/likle/cargs/actions/workflows/build-linux.yml)
[![Build Pipeline](https://github.com/likle/cargs/actions/workflows/build-freebsd.yml/badge.svg)](https://github.com/likle/cargs/actions/workflows/build-freebsd.yml)
[![Build Pipeline](https://github.com/likle/cargs/actions/workflows/build-macos.yml/badge.svg)](https://github.com/likle/cargs/actions/workflows/build-macos.yml)
[![Build Pipeline](https://github.com/likle/cargs/actions/workflows/build-win.yml/badge.svg)](https://github.com/likle/cargs/actions/workflows/build-win.yml)
[![codecov](https://img.shields.io/codecov/c/github/likle/cargs/master.svg?label=Coverage)](https://codecov.io/gh/likle/cargs)

# libcargs - command line argument library for C/C++

This is a lighweight C command line argument library which does not require any malloc. It is currently
compiled and tested under **Linux**, **FreeBSD**, **macOS** and **Windows**.

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
* **no malloc needed** - for situations where that's not available

## Building

**[Building](https://likle.github.io/cargs/build.html)**,
**[embedding](https://likle.github.io/cargs/embed.html)** and
**[testing](https://likle.github.io/cargs/build.html)** instructions are
available in the documentation (it's very easy).

## Docs

All the documentation is available in the
**[the github page](https://likle.github.io/cargs/)** of this repository.

## Example

First define the options:
 ```c
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
```

And then just iterate `argv`:
```c
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
      return EXIT_SUCCESS;
    case '?':
      cag_option_print_error(&context, stdout);
      break;
    }
  }

  printf("simple_flag: %i, multiple_flag: %i, long_flag: %i, key: %s\n",
    simple_flag, multiple_flag, long_flag, value ? value : "-");

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
```

Note that all formatting is done by cargs.

### Example code
The complete example can be found in the `./demo/` folder of this repository.