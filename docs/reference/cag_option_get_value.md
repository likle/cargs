---
title: cag_option_get_value
description: Gets the value from the option.
---

_(since v1.0.0)_  
Gets the value from the option.

## Description
```c
const char *cag_option_get_value(const cag_option_context *context);
```

This function gets the value from the option, if any. If the option does not
contain a value, this function will return NULL.
 
## Parameters
 * **context**: The context from which the option was fetched.

## Return Value
Returns a pointer to the value or NULL if there is no value.

## Changelog

| Version    | Description                                            |
|------------|--------------------------------------------------------|
| **v1.0.0** | The function is introduced.                            |
