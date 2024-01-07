---
title: cag_option_print_error
description: Prints the error associated with the invalid option to the specified destination.
---

_(since v1.1.0)_  
Prints the error associated with the invalid option to the specified destination.

## Description
```c
void cag_option_print_error(const cag_option_context *context, FILE *destination);
```

This function prints information about the error associated with the invalid option
to the specified destination (such as a file stream). It helps in displaying the
error of the current context.

This function is meant to be called in a fetch loop if a unknown option occurs.

## Parameters
* **context**: Pointer to the context from which the option was fetched.
* **destination**: Pointer to the file stream where the error information will be printed.

## Changelog

| Version    | Description                                            |
|------------|--------------------------------------------------------|
| **v1.1.0** | The function is introduced.                            |
