---
title: cag_option_get_error_index
description: Gets the current error index of the context.
---

_(since v1.1.0)_  
Retrieves the letter character of the invalid option.

## Description
```c
char cag_option_get_error_letter(const cag_option_context *context);
```

This function retrieves the character of the invalid option character
if the provided option does not match any of the options specified in the
`cag_option` list.

The result of this function may change after a call to `cag_option_fetch`, and
essentially represents the error letter of the current iteration.

If long options are used it might be necessary to use 
[cag_option_get_error_index]({{ site.baseurl }}{% link reference/cag_option_get_error_index.md %}) as well,
since this function only works with access letters.

## Parameters
 * **context**: Pointer to the context from which the option was fetched.

## Return Value
Returns the letter that was unknown. The error letter is 0 if the current option did not fail
or if the current option is in the long format (e.g. `--long-option`).

## Changelog

| Version    | Description                                            |
|------------|--------------------------------------------------------|
| **v1.1.0** | The function is introduced.                            |
