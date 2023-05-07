---
title: cag_option_get_index
description: Gets the current index of the context.
---

_(since v1.0.0)_  
Gets the current index of the context.

## Description
```c
int cag_option_get_index(const cag_option_context *context);
```

This function gets the index within the argv arguments of the context. The
context always points to the next item which it will inspect. This is
particularly useful to inspect the original argument array, or to get
non-option arguments after option fetching has finished.
 
## Parameters
 * **context**: The context from which the option was fetched.

## Return Value
Returns the current index of the context.

## Changelog

| Version    | Description                                            |
|------------|--------------------------------------------------------|
| **v1.0.0** | The function is introduced.                            |
