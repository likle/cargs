---
title: cag_option_get_identifier
description: Gets the identifier of the option.
---

_(since v1.0.0)_  
Gets the identifier of the option.

## Description
```c
char cag_option_get_identifier(const cag_option_context *context);
```
This function gets the identifier of the option, which should be unique to
this option and can be used to determine what kind of option this is.

Before version **1.1.0** this function was named `cag_option_get`.
 
## Parameters
 * **context**: The context from which the option was fetched.

## Return Value
Returns the identifier of the option.

## Changelog

| Version    | Description                                           |
|------------|-------------------------------------------------------|
| **v1.1.0** | The function is renamed to cag_option_get_identifier. |
| **v1.0.3** | Added shared library exports.                         |
| **v1.0.1** | Added support for C++ projects.                       |
| **v1.0.0** | The function is introduced.                           |
