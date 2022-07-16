#ifndef RETURN_H_INCLUDED
#define RETURN_H_INCLUDED

// If the function ran correctly:
// 0 success
#define SUCCESS 0

// If the function not ran correctly:
// -1 struct not alloc
#define STRUCT_NOT_ALLOC -1

// -2 out of memory
#define OUT_OF_MEMORY -2

// -3 struct is empty
#define STRUCT_EMPTY -3

// -4 open archive error
#define OPEN_ARCHIVE_ERROR -4

// -5 not segment image
#define NOT_SEGMENT_IMAGE -5

// -6 invalid image
#define INVALID_IMAGE -6

#endif // RETURN_H_INCLUDED