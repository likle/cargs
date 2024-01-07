---
title: cag_option_get_error_index
description: Gets the current error index of the context.
---

_(since v1.1.0)_  
Retrieves the index of an invalid option.

## Description
```c
int cag_option_get_error_index(const cag_option_context *context);
```

This function retrieves the index of an invalid option if the provided option
does not match any of the options specified in the `cag_option` list. This is
particularly useful when detailed information about an invalid option is required.

The result of this function may change after a call to `cag_option_fetch`, and
essentially represents the error of the current iteration.

The error index represents the index within the argv parameter passed during the
[context initialization]({{ site.baseurl }}{% link reference/cag_option_init.md %}).

Since users can pass parameters in the short form `-abc`, it won't be entirely clear
which of the option letter was invalid. Please use 
[cag_option_get_error_letter]({{ site.baseurl }}{% link reference/cag_option_get_error_letter.md %})
for this purpose.

 
## Parameters
 * **context**: Pointer to the context from which the option was fetched.

## Return Value
Returns the index of the invalid option, or -1 if it is not invalid.

## Changelog

| Version    | Description                     |
|------------|---------------------------------|
| **v1.1.0** | The function is introduced.     |