---
title: Reference
description: A complete reference of the cargs command line arguments parser library for C/C++.
---

## Basic
The basic functions available in cargs, which can be used to do basic command line argument parsing.

### Functions
* **[cag_option_init]({{ site.baseurl }}{% link reference/cag_option_init.md %})**  
Prepare argument options context for parsing.

* **[cag_option_fetch]({{ site.baseurl }}{% link reference/cag_option_fetch.md %})**  
Fetches an option from the argument list.

* **[cag_option_get_identifier]({{ site.baseurl }}{% link reference/cag_option_get_identifier.md %})**  
 Gets the identifier of the option.

* **[cag_option_get_value]({{ site.baseurl }}{% link reference/cag_option_get_value.md %})**  
Gets the value from the option.

* **[cag_option_get_index]({{ site.baseurl }}{% link reference/cag_option_get_index.md %})**  
Gets the current index of the context.

* **[cag_option_get_error_index]({{ site.baseurl }}{% link reference/cag_option_get_error_index.md %})**  
Retrieves the index of an invalid option.

* **[cag_option_get_error_letter]({{ site.baseurl }}{% link reference/cag_option_get_error_letter.md %})**  
Retrieves the letter character of the invalid option.

## Output
This section describes functions which are used to output argument information.

### Functions
* **[cag_option_print_error]({{ site.baseurl }}{% link reference/cag_option_print_error.md %})**  
  Prints the error associated with the invalid option to the specified destination.

* **[cag_option_print]({{ site.baseurl }}{% link reference/cag_option_print.md %})**  
  Prints all options to the terminal.
