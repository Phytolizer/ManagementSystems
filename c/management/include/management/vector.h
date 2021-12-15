#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define VECTOR_PROTOTYPES_(VectorType, ValueType)                                                                      \
    void VectorType##_init(VectorType* vector);                                                                        \
    void VectorType##_deinit(VectorType* vector);                                                                      \
    void VectorType##_push(VectorType* vector, ValueType value);                                                       \
    ValueType VectorType##_pop(VectorType* vector);

#define VECTOR_DECLARATION(VectorType, ValueType)                                                                      \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        ValueType* data;                                                                                               \
        size_t size;                                                                                                   \
        size_t capacity;                                                                                               \
    } VectorType;                                                                                                      \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        bool is_valid;                                                                                                 \
        void (*f)(ValueType*);                                                                                         \
    } VectorType##Destructor_;                                                                                         \
    VECTOR_PROTOTYPES_(VectorType, ValueType)

#define VECTOR_DESTRUCTOR(VectorType, Function)                                                                        \
    ((VectorType##Destructor_){                                                                                        \
        .is_valid = true,                                                                                              \
        .f = Function,                                                                                                 \
    })

#define VECTOR_DESTRUCTOR_NONE(VectorType)                                                                             \
    ((VectorType##Destructor_){                                                                                        \
        .is_valid = false,                                                                                             \
    })

#define VECTOR_INIT_IMPL_(VectorType, ValueType)                                                                       \
    void VectorType##_init(VectorType* vector)                                                                         \
    {                                                                                                                  \
        vector->data = NULL;                                                                                           \
        vector->size = 0;                                                                                              \
        vector->capacity = 0;                                                                                          \
    }

#define VECTOR_DEINIT_IMPL_(VectorType, ValueType, Destructor)                                                         \
    void VectorType##_deinit(VectorType* vector)                                                                       \
    {                                                                                                                  \
        static const VectorType##Destructor_ dtor = Destructor;                                                        \
        if (vector->data)                                                                                              \
        {                                                                                                              \
            if (dtor.is_valid)                                                                                         \
            {                                                                                                          \
                for (size_t i = 0; i < vector->size; ++i)                                                              \
                {                                                                                                      \
                    dtor.f(&vector->data[i]);                                                                          \
                }                                                                                                      \
            }                                                                                                          \
            free(vector->data);                                                                                        \
        }                                                                                                              \
    }

#define VECTOR_PUSH_IMPL_(VectorType, ValueType)                                                                       \
    void VectorType##_push(VectorType* vector, ValueType value)                                                        \
    {                                                                                                                  \
        if (vector->size == vector->capacity)                                                                          \
        {                                                                                                              \
            size_t new_capacity = vector->capacity ? vector->capacity * 2 : 1;                                         \
            ValueType* new_data = realloc(vector->data, new_capacity * sizeof(ValueType));                             \
            if (!new_data)                                                                                             \
            {                                                                                                          \
                return;                                                                                                \
            }                                                                                                          \
            vector->data = new_data;                                                                                   \
            vector->capacity = new_capacity;                                                                           \
        }                                                                                                              \
        vector->data[vector->size++] = value;                                                                          \
    }

#define VECTOR_POP_IMPL_(VectorType, ValueType)                                                                        \
    ValueType VectorType##_pop(VectorType* vector)                                                                     \
    {                                                                                                                  \
        return vector->data[--vector->size];                                                                           \
    }

#define VECTOR_IMPLEMENTATION(VectorType, ValueType, Destructor)                                                       \
    VECTOR_INIT_IMPL_(VectorType, ValueType)                                                                           \
    VECTOR_DEINIT_IMPL_(VectorType, ValueType, Destructor)                                                             \
    VECTOR_PUSH_IMPL_(VectorType, ValueType)                                                                           \
    VECTOR_POP_IMPL_(VectorType, ValueType)                                                                            \
    static const bool VectorType##_IS_IMPLEMENTED = true
