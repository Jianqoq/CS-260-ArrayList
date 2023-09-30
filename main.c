#include "assert.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ExtendIfPossible(array_list, arr)                                      \
  if (array_list->size == array_list->capacity) {                              \
    arr = extend(array_list);                                                  \
    if (!arr) {                                                                \
      return NULL;                                                             \
    }                                                                          \
  } else {                                                                     \
    arr = array_list;                                                          \
  }

#define ShrinkIfPossible(array_list, arr)                                      \
  if (array_list->size < array_list->capacity / 2) {                           \
    arr = shrink(array_list);                                                  \
    if (!arr) {                                                                \
      return NULL;                                                             \
    }                                                                          \
  } else {                                                                     \
    arr = array_list;                                                          \
  }

#define AssertNull(ptr, to_return)                                             \
  if (!ptr) {                                                                  \
    fprintf(stderr, "Err happened when asserting ptr %s which is %p\n", #ptr,  \
            ptr);                                                              \
    return to_return;                                                          \
  }

typedef struct {
  char *name;
  char *lastname;
  float height;
  int age;

} Entry;

typedef struct {
  int size;
  int capacity;
  Entry **entries;
} ArrayList;

ArrayList *init(int capacity) {
  ArrayList *list = (ArrayList *)malloc(sizeof(ArrayList) * 1);
  if (!list) {
    return NULL;
  }
  list->capacity = capacity;
  list->size = 0;
  Entry **entry = (Entry **)malloc(sizeof(Entry *) * capacity);
  if (!entry) {
    free(list);
    return NULL;
  }
  list->entries = entry;
  return list;
}

int delete_list(ArrayList *list) {
  Entry **entry = list->entries;
  if (!entry) {
    return 0;
  }
  int size = list->size;
  for (int i = 0; i < size; i++) {
    if (entry[i]) {
      if (entry[i]->name) {
        free(entry[i]->name);
      }
      if (entry[i]->lastname) {
        free(entry[i]->lastname);
      }
      free(entry[i]);
    }
  }
  list->entries = NULL;
  free(entry);
  return 0;
}

ArrayList *extend(ArrayList *array_list) {
  int size = array_list->size;
  int capacity = array_list->capacity;
  ArrayList *new_arr_list = init(capacity * 2);
  if (!new_arr_list) {
    return NULL;
  }
  new_arr_list->size = size;
  Entry **entry = new_arr_list->entries;
  for (int i = 0; i < size; i++) {
    entry[i] = array_list->entries[i];
  }
  free(array_list->entries);
  free(array_list);
  return new_arr_list;
}

ArrayList *shrink(ArrayList *array_list) {
  int size = array_list->size;
  int capacity = array_list->capacity;
  if (size > capacity / 2) {
    return array_list;
  }
  ArrayList *new_arr_list = init(capacity / 2);
  if (!new_arr_list) {
    return NULL;
  }
  new_arr_list->size = size;
  Entry **entry = new_arr_list->entries;
  for (int i = 0; i < size; i++) {
    entry[i] = array_list->entries[i];
  }
  free(array_list->entries);
  free(array_list);
  return new_arr_list;
}

void move_elements_right(ArrayList **array_list, int index) {
  ArrayList *arr = *array_list;
  int size = arr->size;
  Entry **entrys = arr->entries;
  for (int i = size - 1; i >= index; i--) {
    entrys[i + 1] = entrys[i];
  }
  entrys[index] = NULL;
}

void move_elements_left(ArrayList **array_list, int index) {
  ArrayList *arr = *array_list;
  int size = arr->size;
  Entry **entrys = arr->entries;
  AssertNull(entrys[index], );
  Entry *tmp = entrys[index];
  entrys[index] = NULL;
  if (tmp->name)
    free(tmp->name);
  if (tmp->lastname)
    free(tmp->lastname);
  free(tmp);
  for (int i = index + 1; i < size; i++) {
    entrys[i - 1] = entrys[i];
  }
  entrys[size - 1] = NULL;
}

ArrayList *insert(ArrayList *array_list, Entry *entry, int index) {
  int size = array_list->size;
  if (index < 0 || index > size) {
    fprintf(stderr, "index to insert is not right\n");
    return 0;
  }
  ArrayList *arr = NULL;
  ExtendIfPossible(array_list, arr);
  move_elements_right(&arr, index);
  assert(arr->entries[index] == NULL);
  assert(entry != NULL);
  arr->entries[index] = entry;
  arr->size++;
  return arr;
}

ArrayList *insert_to_tail(ArrayList *array_list, Entry *entry) {
  int size = array_list->size;
  ArrayList *arr = NULL;
  ExtendIfPossible(array_list, arr);
  assert(entry != NULL);
  arr->entries[size] = entry;
  arr->size++;
  return arr;
}

ArrayList *insert_to_head(ArrayList *array_list, Entry *entry) {
  int size = array_list->size;
  ArrayList *arr = NULL;
  ExtendIfPossible(array_list, arr);
  move_elements_right(&arr, 0);
  assert(arr->entries[0] == NULL);
  assert(entry != NULL);
  arr->entries[0] = entry;
  arr->size++;
  return arr;
}

ArrayList *delete_from_tail(ArrayList *array_list) {
  int size = array_list->size;
  if (size == 0) {
    fprintf(stderr, "list is empty cannot remove element from head\n");
    return 0;
  }
  ArrayList *arr = NULL;
  move_elements_left(&array_list, size - 1);
  array_list->size--;
  ShrinkIfPossible(array_list, arr);
  return arr;
}

ArrayList *delete_from_head(ArrayList *array_list) {
  int size = array_list->size;
  if (size == 0) {
    fprintf(stderr, "list is empty cannot remove element from head\n");
    return 0;
  }
  ArrayList *arr = NULL;
  move_elements_left(&array_list, 0);
  array_list->size--;
  ShrinkIfPossible(array_list, arr);
  return arr;
}

ArrayList *_delete(ArrayList *array_list, int index) {
  int size = array_list->size;
  if (index < 0 || index > size) {
    fprintf(stderr, "index to delete is not right\n");
    return 0;
  }
  ArrayList *arr = NULL;
  move_elements_left(&array_list, index);
  array_list->size--;
  ShrinkIfPossible(array_list, arr);
  return arr;
}

int find_pos(ArrayList *array_list, char *name) {
  AssertNull(name, -1);
  int size = array_list->size;
  Entry **entry_list = array_list->entries;
  for (int i = 0; i < size; i++) {
    Entry *entry = entry_list[i];
    if (entry && !strcmp(entry->name, name)) {
      return i;
    }
  }
  return -1;
}

char *strip(char *string, char c) {
  int len = strlen(string);
  char *new_str = (char *)malloc(sizeof(char) * (len + 1));
  if (!new_str) {
    return NULL;
  }
  int tracker = 0, j = 0;
  for (j = 0; j < len; j++) {
    if (string[j] != c) {
      new_str[tracker++] = string[j];
    }
  }
  new_str[tracker] = '\0';
  return new_str;
}

int parse_command(ArrayList **array, char *command) {
  ArrayList *array_list = *array;
  AssertNull(array_list, -1);
  AssertNull(command, -1);
  char *token = strtok(command, " ");
  int index = -1;
  char *new_token = strip(token, '\n');
  if (!strcmp(new_token, "insertToHead")) {
    index = 0;
  } else if (!strcmp(new_token, "insertToTail")) {
    index = array_list->size;
  } else if (!strcmp(new_token, "printList")) {
    int size = array_list->size;
    Entry **entry_list = array_list->entries;
    for (int i = 0; i < size; i++) {
      Entry *entry = entry_list[i];
      printf("[%d]\t%s\t%s\t%0.2f\t%d\n", i, entry->name, entry->lastname,
             entry->height, entry->age);
    }
    free(new_token);
    printf("\n");
    return 0;
  } else if (!strcmp(new_token, "printListInfo")) {
    printf("size:%d, capacity:%d\n", array_list,
           array_list->size, array_list->capacity);
    free(new_token);
    printf("\n");
    return 0;
  } else if (!strcmp(new_token, "insertToPosition")) {
    char *index_str = strtok(NULL, " ");
    index = atoi(index_str);
  } else if (!strcmp(new_token, "findPosition")) {
    char *name = strtok(NULL, " ");
    AssertNull(name, -1);
    char *new_name = strip(name, '\n');
    AssertNull(new_name, -1);
    index = find_pos(array_list, new_name);
    free(new_token);
    free(new_name);
    printf("%d\n\n", index);
    return index;
  } else if (!strcmp(new_token, "deleteFromPosition")) {
    char *index_str = strtok(NULL, " ");
    AssertNull(index_str, -1);
    index = atoi(index_str);
    *array = _delete(array_list, index);
    free(new_token);
    return 0;
  } else if (!strcmp(new_token, "deleteFromHead")) {
    *array = delete_from_head(array_list);
    free(new_token);
    return 0;
  } else if (!strcmp(new_token, "deleteFromTail")) {
    *array = delete_from_tail(array_list);
    free(new_token);
    return 0;
  } else if (!strcmp(new_token, "deleteList")) {
    delete_list(array_list);
    free(new_token);
    return 0;
  } else {
    fprintf(stderr, "Invalid command: <%s>\n\n", new_token);
    free(new_token);
    return -1;
  }
  free(new_token);
  Entry *entry = (Entry *)malloc(sizeof(Entry));
  char *name = strtok(NULL, " ");
  AssertNull(name, -1);
  entry->name = (char *)malloc(strlen(name) + 1);
  AssertNull(entry->name, -1);
  strcpy(entry->name, name);
  char *last_name = strtok(NULL, " ");
  AssertNull(last_name, -1);
  entry->lastname = (char *)malloc(strlen(last_name) + 1);
  if (entry->lastname == NULL) {
    free(entry->name);
    free(entry);
    return -1;
  }
  strcpy(entry->lastname, last_name);
  char *height = strtok(NULL, " ");
  if (!height) {
    free(entry->name);
    free(entry->lastname);
    free(entry);
    return -1;
  }
  float height_f = atof(height);
  entry->height = height_f;
  char *age = strtok(NULL, " ");
  if (!age) {
    free(entry->name);
    free(entry->lastname);
    free(entry);
    return -1;
  }
  float age_f = atoi(age);
  entry->age = age_f;
  *array = insert(array_list, entry, index);
  return 0;
}

int main(int argc, char *argv[]) {
  for (int i = 1; i < argc; i++) {
    ArrayList *list = init(2);
    FILE *fp = fopen(argv[i], "r");
    if (!fp) {
      fprintf(stderr, "Err happened when openning file\n");
      delete_list(list);
      free(list);
      return 1;
    }
    char command[100];
    while (fgets(command, sizeof(command), fp)) {
      parse_command(&list, command);
    }
    fclose(fp);
    if (list) {
      delete_list(list);
      free(list);
    }
  }
  return 0;
}
