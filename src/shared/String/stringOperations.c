#include "stringOperations.h"
#include "string.h"
#include "memory.h"

static inline uint32 _str_len(const str text) {
    uint32 len = 0;
    while (text[len] != '\0') {
        len++;
    }
    return len;
}


uint32 str_len(str text) {
    if (!text) return 0;
    return _str_len(text);
}

str str_copy(str text) {
    if (!text) return null;
    
    uint32 len = _str_len(text);
    str duplicate = (str)malloc(len + 1);
    if (!duplicate) return null;
    
    copy(text, duplicate, len + 1);
    return duplicate;
}

str str_concat(str first, str next) {
    if (!first || !next) return null;
    
    uint32 len1 = _str_len(first);
    uint32 len2 = _str_len(next);
    
    str result = (str)malloc(len1 + len2 + 1);
    if (result == null) return null;
    
    copy(first, result, len1);
    copy(next, result + len1, len2 + 1);
    
    return result;
}

str str_slice(str text, uint32 start, uint32 len) {
    if (!text) return null;
    
    uint32 total_len = _str_len(text);
    if (start >= total_len) return null;
    
    if (start + len > total_len) {
        len = total_len - start;
    }
    
    str slice = (str)malloc(len + 1);
    if (slice == null) return null;
    
    copy(text + start, slice, len);
    slice[len] = '\0';
    
    return slice;
}

str str_replaceC(str text, char value, char replacement) {
    if (!text) return null;
    
    uint32 len = _str_len(text);
    str result = (str)malloc(len + 1);
    if (result == null) return null;
    
    for (uint32 i = 0; i < len; i++) {
        if (text[i] == value) {
            result[i] = replacement;
        } else {
            result[i] = text[i];
        }
    }
    result[len] = '\0';
    
    return result;
}

str str_replaceS(str text, str value, str replacement) {
    if (!text || !value || !replacement) return null;
    
    uint32 text_len = _str_len(text);
    uint32 val_len = _str_len(value);
    uint32 rep_len = _str_len(replacement);
    
    if (val_len == 0) return str_copy(text);
    
    uint32 count = 0;
    for (uint32 i = 0; i <= text_len - val_len; ) {
        bool match = true;
        for (uint32 j = 0; j < val_len; j++) {
            if (text[i + j] != value[j]) {
                match = false;
                break;
            }
        }
        if (match) {
            count++;
            i += val_len;
        } else {
            i++;
        }
    }
    
    if (count == 0) return str_copy(text);
    
    uint32 new_len = text_len + (count * rep_len) - (count * val_len);
    str result = (str)malloc(new_len + 1);
    if (!result) return null;
    
    uint32 text_idx = 0;
    uint32 res_idx = 0;
    
    while (text_idx < text_len) {
        bool match = false;
        if (text_idx <= text_len - val_len) {
            match = true;
            for (uint32 j = 0; j < val_len; j++) {
                if (text[text_idx + j] != value[j]) {
                    match = false;
                    break;
                }
            }
        }
        
        if (match) {
            copy(replacement, result + res_idx, rep_len);
            res_idx += rep_len;
            text_idx += val_len;
        } else {
            result[res_idx] = text[text_idx];
            res_idx++;
            text_idx++;
        }
    }
    result[new_len] = '\0';
    
    return result;
}