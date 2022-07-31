#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#ifndef LIST_NAME
#error "Must define LIST_NAME"
#endif

#ifndef LIST_TYPE
#error "Must define LIST_TYPE"
#endif

#ifdef LIST_IS_STATIC
#define LIST_STATIC static
#else
#define LIST_STATIC
#endif

#ifdef LIST_IS_STATIC
#define LIST_IMPL
#elif defined(LIST_DEFINE)
#define LIST_IMPL
#endif

#define LIST_CONCAT(x, y) x##y
#define LIST_MAKE_STR1(x, y) LIST_CONCAT(x, y)
#define LIST_MAKE_STR_END(x) \
  LIST_MAKE_STR1(LIST_NAME, LIST_MAKE_STR1(LIST_SUFFIX, x))
#define LIST_MAKE_STR_BEG(x) \
  LIST_MAKE_STR1(x, LIST_MAKE_STR1(LIST_NAME, LIST_SUFFIX))

#define LIST LIST_MAKE_STR1(LIST_NAME, LIST_SUFFIX)
#define LIST_INIT_NAME LIST_MAKE_STR_BEG(Init)
#define LIST_FREE_NAME LIST_MAKE_STR_BEG(Free)
#define LIST_GET_NAME LIST_MAKE_STR_END(Get)
#define LIST_SIZE_NAME LIST_MAKE_STR_END(Size)
#define LIST_ADD_NAME LIST_MAKE_STR_END(Add)

#ifndef LIST_SUFFIX
#define LIST_SUFFIX List
#endif

#ifndef LIST_IMPL

typedef struct {
  LIST_TYPE* arr;
  size_t size;
  size_t capacity;
} LIST;

LIST_STATIC void LIST_INIT_NAME(LIST* list);
LIST_STATIC void LIST_ADD_NAME(LIST* list, LIST_TYPE value);
LIST_STATIC LIST_TYPE* LIST_GET_NAME(LIST* list, size_t index);
LIST_STATIC size_t LIST_SIZE_NAME(LIST* list);
LIST_STATIC void LIST_FREE_NAME(LIST* list);

#else

#ifndef LIST_MALLOC
#define LIST_MALLOC(Sz) malloc(Sz)
#endif
#ifndef LIST_CALLOC
#define LIST_CALLOC(Cnt, Sz) calloc(Cnt, Sz)
#endif
#ifndef LIST_REALLOC
#define LIST_REALLOC(P, Sz) realloc(P, Sz)
#endif
#ifndef LIST_FREE
#define LIST_FREE(P) free(P)
#endif

LIST_STATIC void LIST_INIT_NAME(LIST* list) {
  list->arr = NULL;
  list->size = 0;
  list->capacity = 0;
}

LIST_STATIC void LIST_ADD_NAME(LIST* list, LIST_TYPE value) {
  if (list->size == list->capacity) {
    list->capacity = list->capacity * 2 + 1;
    LIST_TYPE* newarr = LIST_CALLOC(list->capacity, sizeof(LIST_TYPE));
    for (size_t i = 0; i < list->size; i++) {
      newarr[i] = list->arr[i];
    }
    list->arr = newarr;
  }
  list->arr[list->size++] = value;
}

LIST_STATIC LIST_TYPE* LIST_GET_NAME(LIST* list, size_t index) {
  return &list->arr[index];
}

LIST_STATIC size_t LIST_SIZE_NAME(LIST* list) {
  return list->size;
}

LIST_STATIC void LIST_FREE_NAME(LIST* list) {
  LIST_FREE(list->arr);
}

#endif

#undef LIST_IMPL
#undef LIST_STATIC
#undef LIST_MAKE_STR1
#undef LIST_MAKE_STR_BEG
#undef LIST_MAKE_STR_END
#undef LIST_ADD_NAME
#undef LIST_INIT_NAME
#undef LIST_GET_NAME
#undef LIST_FREE_NAME
#undef LIST_FREE
#undef LIST_MALLOC
#undef LIST_CALLOC
#undef LIST_REALLOC
#undef LIST_CONCAT
#undef LIST_SUFFIX
#undef LIST_NAME
#undef LIST_TYPE
#undef LIST
