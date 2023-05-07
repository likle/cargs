---
title: cag_option_get
description: Gets the identifier of the option.
---

_(since v1.0.0)_  
Gets the identifier of the option.

## Description
```c
char cag_option_get(const cag_option_context *context);
```
This function gets the identifier of the option, which should be unique to
this option and can be used to determine what kind of option this is.
 
## Parameters
 * **context**: The context from which the option was fetched.

## Return Value
Returns the identifier of the option.

## Changelog

| Version    | Description                                            |
|------------|--------------------------------------------------------|
| **v1.0.0** | The function is introduced.                            |
