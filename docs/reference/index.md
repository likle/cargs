---
title: Reference
description: A complete reference of the cargs command line arguments parser library for C/C++.
---

## Basic
The basic functions available in cargs, which can be used to do basic command line argument parsing.

### Functions
* **[cag_option_prepare]({{ site.baseurl }}{% link reference/cag_option_prepare.md %})**  
Prepare argument options context for parsing.

* **[cag_option_fetch]({{ site.baseurl }}{% link reference/cag_option_fetch.md %})**  
Fetches an option from the argument list.

* **[cag_option_get]({{ site.baseurl }}{% link reference/cag_option_get.md %})**  
 Gets the identifier of the option.

* **[cag_option_get_value]({{ site.baseurl }}{% link reference/cag_option_get_value.md %})**  
Gets the value from the option.

* **[cag_option_get_index]({{ site.baseurl }}{% link reference/cag_option_get_index.md %})**  
Gets the current index of the context.

## Output
This section describes functions which are used to output argument information.

### Functions
* **[cag_option_print]({{ site.baseurl }}{% link reference/cag_option_print.md %})**  
Prints all options to the terminal.
