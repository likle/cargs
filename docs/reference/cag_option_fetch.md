---
title: cag_option_fetch
description: Fetches an option from the argument list.
---

_(since v1.0.0)_  
Fetches an option from the argument list.

## Description
```c
bool cag_option_fetch(cag_option_context *context);
```

This function fetches a single option from the argument list. The context
will be moved to that item. Information can be extracted from the context
after the item has been fetched.
The arguments will be re-ordered, which means that non-option arguments will
be moved to the end of the argument list. After all options have been
fetched, all non-option arguments will be positioned after the index of
the context.
 
## Parameters
 * **context**: The context from which we will fetch the option.

## Return Value
Returns true if there was another option or false if the end is reached.

## Changelog

| Version    | Description                                            |
|------------|--------------------------------------------------------|
| **v1.0.0** | The function is introduced.                            |
