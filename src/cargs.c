#include <assert.h>
#include <cargs.h>
#include <memory.h>
#include <stdio.h>
#include <string.h>

static void cag_option_print_value(const cag_option *option,
  int *accessor_length, FILE *destination)
{
  if (option->value_name != NULL) {
    *accessor_length += fprintf(destination, "=%s", option->value_name);
  }
}

static void cag_option_print_letters(const cag_option *option, bool *first,
  int *accessor_length, FILE *destination)
{
  const char *access_letter;
  access_letter = option->access_letters;
  if (access_letter != NULL) {
    while (*access_letter) {
      if (*first) {
        *accessor_length += fprintf(destination, "-%c", *access_letter);
        *first = false;
      } else {
        *accessor_length += fprintf(destination, ", -%c", *access_letter);
      }
      ++access_letter;
    }
  }
}

static void cag_option_print_name(const cag_option *option, bool *first,
  int *accessor_length, FILE *destination)
{
  if (option->access_name != NULL) {
    if (*first) {
      *accessor_length += fprintf(destination, "--%s", option->access_name);
    } else {
      *accessor_length += fprintf(destination, ", --%s", option->access_name);
    }
  }
}

void cag_option_print(const cag_option *options, size_t option_count,
  FILE *destination)
{
  size_t option_index;
  const cag_option *option;
  bool first;
  int i, accessor_length;

  for (option_index = 0; option_index < option_count; ++option_index) {
    option = &options[option_index];
    accessor_length = 0;
    first = true;

    fputs("  ", destination);

    cag_option_print_letters(option, &first, &accessor_length, destination);
    cag_option_print_name(option, &first, &accessor_length, destination);
    cag_option_print_value(option, &accessor_length, destination);

    for (i = accessor_length; i < 20; ++i) {
      fputs(" ", destination);
    }

    fputs(" ", destination);
    fputs(option->description, destination);

    fprintf(destination, "\n");
  }
}

void cag_option_prepare(cag_option_context *context, const cag_option *options,
  size_t option_count, int argc, char **argv)
{
  // This just initialized the values to the beginning of all the arguments.
  context->options = options;
  context->option_count = option_count;
  context->argc = argc;
  context->argv = argv;
  context->index = 1;
  context->inner_index = 0;
  context->forced_end = false;
}

static const cag_option *cag_option_find_by_name(cag_option_context *context,
  char *name, size_t name_size)
{
  const cag_option *option;
  size_t i;

  // We loop over all the available options and stop as soon as we have found
  // one. We don't use any hash map table, since there won't be that many
  // arguments anyway.
  for (i = 0; i < context->option_count; ++i) {
    option = &context->options[i];

    // The option might not have an item name, we can just skip those.
    if (option->access_name == NULL) {
      continue;
    }

    // Try to compare the name of the access name. We can use the name_size or
    // this comparison, since we are guaranteed to have null-terminated access
    // names.
    if (strncmp(option->access_name, name, name_size) == 0) {
      return option;
    }
  }

  return NULL;
}

static const cag_option *cag_option_find_by_letter(cag_option_context *context,
  char letter)
{
  const cag_option *option;
  size_t i;

  // We loop over all the available options and stop as soon as we have found
  // one. We don't use any look up table, since there won't be that many
  // arguments anyway.
  for (i = 0; i < context->option_count; ++i) {
    option = &context->options[i];

    // If this option doesn't have any access letters we will skip them.
    if (option->access_letters == NULL) {
      continue;
    }

    // Verify whether this option has the access letter in it's access letter
    // string. If it does, then this is our option.
    if (strchr(option->access_letters, letter) != NULL) {
      return option;
    }
  }

  return NULL;
}

static void cag_option_parse_value(cag_option_context *context,
  const cag_option *option, char **c)
{
  // And now let's check whether this option is supposed to have a value, which
  // is the case if there is a value name set. The value can be either submitted
  // with a '=' sign or a space, which means we would have to jump over to the
  // next argv index. This is somewhat ugly, but we do it to behave the same as
  // the other option parsers.
  if (option->value_name != NULL) {
    if (**c == '=') {
      context->value = ++(*c);
    } else {
      // If the next index is larger or equal to the argument count, then the
      // parameter for this option is missing. The user will know about this,
      // since the value pointer of the context will be NULL because we don't
      // set it here in that case.
      if (context->argc > context->index + 1) {
        // We consider this argv to be the value, no matter what the contents
        // are.
        ++context->index;
        *c = context->argv[context->index];
        context->value = *c;
      }
    }

    // Move c to the end of the value, to not confuse the caller about our
    // position.
    while (**c) {
      ++(*c);
    }
  }
}

static void cag_option_parse_access_name(cag_option_context *context, char **c)
{
  const cag_option *option;
  char *n;

  // Now we need to extract the access name, which is any symbol up to a '=' or
  // a '\0'.
  n = *c;
  while (**c && **c != '=') {
    ++*c;
  }

  // Now this will obviously always be true, but we are paranoid. Sometimes. It
  // doesn't hurt to check.
  assert(*c >= n);

  // Figure out which option this name belongs to. This might return NULL if the
  // name is not registered, which means the user supplied an unknown option. In
  // that case we return true to indicate that we finished with this option. We
  // have to skip the value parsing since we don't know whether the user thinks
  // this option has one or not. Since we don't set any identifier specifically,
  // it will remain '?' within the context.
  option = cag_option_find_by_name(context, n, (size_t)(*c - n));
  if (option == NULL) {
    // Since this option is invalid, we will move on to the next index. There is
    // nothing we can do about this.
    ++context->index;
    return;
  }

  // We found an option and now we can specify the identifier within the
  // context.
  context->identifier = option->identifier;

  // And now we try to parse the value. This function will also check whether
  // this option is actually supposed to have a value.
  cag_option_parse_value(context, option, c);

  // And finally we move on to the next index.
  ++context->index;
}

static void cag_option_parse_access_letter(cag_option_context *context,
  char **c)
{
  const cag_option *option;
  char *n = *c;
  char *v;

  // Figure out which option this letter belongs to. This might return NULL if
  // the letter is not registered, which means the user supplied an unknown
  // option. In that case we return true to indicate that we finished with this
  // option. We have to skip the value parsing since we don't know whether the
  // user thinks this option has one or not. Since we don't set any identifier
  // specifically, it will remain '?' within the context.
  option = cag_option_find_by_letter(context, n[context->inner_index]);
  if (option == NULL) {
    ++context->index;
    context->inner_index = 0;
    return;
  }

  // We found an option and now we can specify the identifier within the
  // context.
  context->identifier = option->identifier;

  // And now we try to parse the value. This function will also check whether
  // this option is actually supposed to have a value.
  v = &n[++context->inner_index];
  cag_option_parse_value(context, option, &v);

  // Check whether we reached the end of this option argument.
  if (*v == '\0') {
    ++context->index;
    context->inner_index = 0;
  }
}

static void cag_option_swap(cag_option_context *context, int index_a,
  int index_b)
{
  char *tmp;

  // If this is the same index, we won't have to do anything. Actually we don't
  // need this check, but it is still here for documentation - since the swap
  // function must support this.
  if (index_a == index_b) {
    return;
  }

  // And now we swap the indexes with the help of the temporary variable.
  tmp = context->argv[index_a];
  context->argv[index_a] = context->argv[index_b];
  context->argv[index_b] = tmp;
}

static bool cag_option_is_argument_string(const char *c)
{
  return *c == '-' && *(c + 1) != '\0';
}

static bool cag_option_prepare_next(cag_option_context *context)
{
  int next_index, next_option_index;
  char *c;

  // Prepare to search the next option at the next index.
  next_index = context->index;
  next_option_index = next_index;

  // Grab a pointer to the string and verify that it is not the end. If it is
  // the end, we have to return false to indicate that we finished.
  c = context->argv[next_option_index];
  if (context->forced_end || c == NULL) {
    return false;
  }

  // Check whether it is a '-'. We need to find the next option - and an option
  // always starts with a '-'. If there is a string "-\0", we don't consider it
  // as an option neither.
  while (!cag_option_is_argument_string(c)) {
    // Find next option and swap.
    c = context->argv[++next_option_index];
    if (c == NULL) {
      // We reached the end and did not find any argument anymore. Let's tell
      // our caller that we couldn't prepare the next item.
      return false;
    }
  }

  // We found the next item, let's swap them around and set the context index
  // to the new index.
  cag_option_swap(context, next_index, next_option_index);
  context->index = next_index;

  // Indicate that we found an option which can be processed. The option is
  // now in the index of the context.
  return true;
}

bool cag_option_fetch(cag_option_context *context)
{
  char *c;

  // Reset our identifier to a question mark, which indicates an "unknown"
  // option. The value is set to NULL, to make sure we are not carrying the
  // parameter from the previous option to this one.
  context->identifier = '?';
  context->value = NULL;

  // Check whether there are any options left to parse
  if (!cag_option_prepare_next(context)) {
    return false;
  }

  // Grab a pointer to the beginning of the option.At this point, the next
  // character must be a '-', since if it was not the prepare function would
  // have returned false. We will skip that symbol and proceed.
  c = context->argv[context->index];
  assert(*c == '-');
  ++c;

  // Check whether this is a long option, starting with a double "--".
  if (*c == '-') {
    ++c;

    // This might be a double "--" which indicates the end of options. If this
    // is the case, we will not move to the next index. That ensures that
    // another call to the fetch function will not skip the "--".
    if (*c == '\0') {
      ++context->index;
      context->forced_end = true;
      return false;
    }

    // We parse now the access name. All information about it will be written
    // to the context.
    cag_option_parse_access_name(context, &c);

    // The long argument has been parsed. Indicate that to the caller, so he
    // can process the option.
    return true;
  }

  cag_option_parse_access_letter(context, &c);
  return true;
}

char cag_option_get(const cag_option_context *context)
{
  // We just return the identifier here.
  return context->identifier;
}

const char *cag_option_get_value(const cag_option_context *context)
{
  // We just return the internal value pointer of the context.
  return context->value;
}

int cag_option_get_index(const cag_option_context *context)
{
  // Either we point to a value item,
  return context->index;
}
