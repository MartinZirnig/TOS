#include "stringConversions.h"
#include "./../Memory/memory.h"

#define MAX_STR_LEN 22

static inline void _strrev(char* str, uint32 len) {
    uint32 start = 0;
    uint32 end = len - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

static inline char* _uint32ToStr(uint32 value) {
    char* buf = (char*)malloc(MAX_STR_LEN);
    if (!buf) return null;

    uint32 i = 0;

    if (value == 0) {
        buf[i++] = '0';
    }
    else {
        while (value > 0) {
            buf[i++] = (value % 10) + '0';
            value /= 10;
        }
    }

    buf[i] = '\0';
    _strrev(buf, i);

    return buf;
}

static inline char* _int32ToStr(int32 value) {
    char* buf = (char*)malloc(MAX_STR_LEN);
    if (!buf) return null;

    uint32 i = 0;
    bool isNegative = false;

    if (value == 0) {
        buf[i++] = '0';
        buf[i] = '\0';
        return buf;
    }

    if (value < 0) {
        isNegative = true;

        uint32 uvalue = (uint32)(-(value + 1)) + 1;

        while (uvalue > 0) {
            buf[i++] = (uvalue % 10) + '0';
            uvalue /= 10;
        }
    }
    else {
        uint32 uvalue = (uint32)value;

        while (uvalue > 0) {
            buf[i++] = (uvalue % 10) + '0';
            uvalue /= 10;
        }
    }

    if (isNegative) {
        buf[i++] = '-';
    }

    buf[i] = '\0';
    _strrev(buf, i);

    return buf;
}

static inline uint32 _strToUint32(char* ptr) {
    if (!ptr || *ptr == '\0')
        return 0;

    uint32 result = 0;

    while (*ptr != '\0') {
        if (*ptr < '0' || *ptr > '9')
            return 0;

        result = (result * 10) + (*ptr - '0');
        ptr++;
    }

    return result;
}

static inline int32 _strToInt32(char* ptr) {
    if (!ptr || *ptr == '\0')
        return 0;

    uint32 i = 0;
    bool isNegative = false;

    if (ptr[0] == '-') {
        isNegative = true;
        i++;
    }
    else if (ptr[0] == '+') {
        i++;
    }

    if (ptr[i] == '\0')
        return 0;

    uint32 result = 0;

    while (ptr[i] != '\0') {
        if (ptr[i] < '0' || ptr[i] > '9')
            return 0;

        result = (result * 10) + (ptr[i] - '0');
        i++;
    }

    if (isNegative)
        return -(int32)result;

    return (int32)result;
}



char* utos8(uint8 value)   { return _uint32ToStr((uint32)value); }
char* utos16(uint16 value) { return _uint32ToStr((uint32)value); }
char* utos32(uint32 value) { return _uint32ToStr(value); }

char* itos8(int8 value)   { return _int32ToStr((int32)value); }
char* itos16(int16 value) { return _int32ToStr((int32)value); }
char* itos32(int32 value) { return _int32ToStr(value); }


uint8  stou8(char* ptr)  { return (uint8)_strToUint32(ptr); }
uint16 stou16(char* ptr) { return (uint16)_strToUint32(ptr); }
uint32 stou32(char* ptr) { return _strToUint32(ptr); }

int8  stoi8(char* ptr)  { return (int8)_strToInt32(ptr); }
int16 stoi16(char* ptr) { return (int16)_strToInt32(ptr); }
int32 stoi32(char* ptr) { return _strToInt32(ptr); }

#if defined(__x86_64__)

static inline char* _uint64ToString(uint64 value) {
    char* buf = (char*)malloc(MAX_STR_LEN);
    if (!buf) return null;

    uint32 i = 0;
    if (value == 0) {
        buf[i++] = '0';
    } else {
        while (value > 0) {
            buf[i++] = (value % 10) + '0';
            value /= 10;
        }
    }
    buf[i] = '\0';
    _strrev(buf, i);
    return buf;
}

static inline char* _int64ToString(int64 value) {
    char* buf = (char*)malloc(MAX_STR_LEN);
    if (!buf) return null;

    uint32 i = 0;
    bool isNegative = false;

    if (value == 0) {
        buf[i++] = '0';
        buf[i] = '\0';
        return buf;
    }

    if (value < 0) {
        isNegative = true;
        uint64 uvalue = (uint64)(-(value + 1)) + 1; 
        while (uvalue > 0) {
            buf[i++] = (uvalue % 10) + '0';
            uvalue /= 10;
        }
    } else {
        uint64 uvalue = (uint64)value;
        while (uvalue > 0) {
            buf[i++] = (uvalue % 10) + '0';
            uvalue /= 10;
        }
    }

    if (isNegative) {
        buf[i++] = '-';
    }

    buf[i] = '\0';
    _strrev(buf, i);
    return buf;
}


static inline uint64 _stringToUint64(char* ptr) {
    if (!ptr || *ptr == '\0') return 0;

    uint64 result = 0;
    while (*ptr != '\0') {
        if (*ptr < '0' || *ptr > '9') {
            return 0;
        }
        
        result = (result * 10) + (*ptr - '0');
        ptr++;
    }
    return result;
}

static inline int64 _stringToInt64(char* ptr) {
    if (!ptr || *ptr == '\0') return 0;

    uint32 i = 0;
    bool isNegative = false;

    if (ptr[0] == '-') {
        isNegative = true;
        i++;
    } else if (ptr[0] == '+') {
        i++;
    }

    if (ptr[i] == '\0') return 0;

    uint64 result = 0;
    while (ptr[i] != '\0') {
        if (ptr[i] < '0' || ptr[i] > '9') {
            return 0;
        }

        result = (result * 10) + (ptr[i] - '0');
        i++;
    }

    if (isNegative) {
        return -(int64)result;
    }
    return (int64)result;
}

char* utos64(uint64 value) { return _uint64ToString(value); }
char* itos64(int64 value) { return _int64ToString(value); }

uint64 stou64(char* ptr) { return _stringToUint64(ptr); }
int64 stoi64(char* ptr) { return _stringToInt64(ptr); }

#endif