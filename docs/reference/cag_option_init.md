---
title: cag_option_prepare
description: Prepare argument options context for parsing.
---

_(since v1.0.0)_  
Prepare argument options context for parsing.

## Description
```c
void cag_option_init(cag_option_context *context, const cag_option *options,
  size_t option_count, int argc, char **argv);
```

This function prepares the context for iteration and initializes the context
with the supplied options and arguments. After the context has been prepared,
it can be used to fetch arguments from it.

Before version **v1.1.0** this function was called `cag_option_prepare`.
 
## Parameters
 * **context**: The context which will be initialized.
 * **options**: The registered options which are available for the program.
 * **option_count**: The amount of options which are available for the program.
 * **argc**: The amount of arguments the user supplied in the main function.
 * **argv**: A pointer to the arguments of the main function.

## Changelog

| Version    | Description                                 |
|------------|---------------------------------------------|
| **v1.1.0** | The function is renamed to cag_option_init. |
| **v1.0.3** | Added shared library exports.               |
| **v1.0.1** | Added support for C++ projects.             |
| **v1.0.0** | The function is introduced.                 |
