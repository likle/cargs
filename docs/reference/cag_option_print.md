---
title: cag_option_print
description: Prints all options to the terminal.
---

_(since v1.0.0)_  
description: Prints all options to the terminal.

## Description
```c
void cag_option_print(const cag_option *options, size_t option_count,
  FILE *destination);
```

This function prints all options to the terminal. This can be used to generate
the output for a "--help" option.
 
## Parameters
 * **options**: The options which will be printed.
 * **option_count**: The option count which will be printed.
 * **destination**: The destination where the output will be printed.

## Changelog

| Version    | Description                                   |
|------------|-----------------------------------------------|
| **v1.0.3** | Added shared library exports.                 |
| **v1.0.2** | Fixed option indention for long descriptions. |
| **v1.0.1** | Added support for C++ projects.               |
| **v1.0.0** | The function is introduced.                   |
