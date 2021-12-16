#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define ARRAY_PROTOTYPES_(ArrayType, ValueType)                                                                        \
    void ArrayType##_init(ArrayType* array);                                                                           \
    void ArrayType##_deinit(ArrayType* array)

#define ARRAY_DECLARATION(ArrayType, ValueType)                                                                        \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        ValueType* data;                                                                                               \
        size_t size;                                                                                                   \
    } ArrayType;                                                                                                       \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        bool is_valid;                                                                                                 \
        void (*f)(ValueType*);                                                                                         \
    } ArrayType##Destructor_;                                                                                          \
    ARRAY_PROTOTYPES_(ArrayType, ValueType)

#define ARRAY_DESTRUCTOR(ArrayType, Function)                                                                          \
    ((ArrayType##Destructor_){                                                                                         \
        .is_valid = true,                                                                                              \
        .f = Function,                                                                                                 \
    })

#define ARRAY_DESTRUCTOR_NONE(ArrayType)                                                                               \
    ((ArrayType##Destructor_){                                                                                         \
        .is_valid = false,                                                                                             \
    })

#define ARRAY_IMPLEMENTATION(ArrayType, ValueType, Destructor)                                                         \
    void ArrayType##_init(ArrayType* array)                                                                            \
    {                                                                                                                  \
        array->data = NULL;                                                                                            \
        array->size = 0;                                                                                               \
    }                                                                                                                  \
    void ArrayType##_deinit(ArrayType* array)                                                                          \
    {                                                                                                                  \
        static const ArrayType##Destructor_ dtor = Destructor;                                                         \
        if (array->data)                                                                                               \
        {                                                                                                              \
            if (dtor.is_valid)                                                                                         \
            {                                                                                                          \
                for (size_t i = 0; i < array->size; ++i)                                                               \
                {                                                                                                      \
                    dtor.f(&array->data[i]);                                                                           \
                }                                                                                                      \
            }                                                                                                          \
            free(array->data);                                                                                         \
        }                                                                                                              \
    }
