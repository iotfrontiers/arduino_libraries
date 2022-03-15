/**
 * Google's Firebase QueryFilter class, QueryFilter.h version 1.0.3
 * 
 * This library supports Espressif ESP8266 and ESP32
 * 
 * Created October 25, 2021
 * 
 * This work is a part of Firebase ESP Client library
 * Copyright (c) 2021 K. Suwatchai (Mobizt)
 * 
 * The MIT License (MIT)
 * Copyright (c) 2021 K. Suwatchai (Mobizt)
 * 
 * 
 * Permission is hereby granted, free of charge, to any person returning a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "FirebaseFS.h"

#ifdef ENABLE_RTDB

#ifndef FIREBASE_QUERY_FILTER_H
#define FIREBASE_QUERY_FILTER_H
#include <Arduino.h>
#include "Utils.h"
#include "signer/Signer.h"

class QueryFilter
{
    friend class FirebaseData;
    friend class FB_RTDB;
    friend class FirebaseSession;

public:
    QueryFilter();
    ~QueryFilter();

    template <typename T = const char *>
    QueryFilter &orderBy(T val) { return mOrderBy(toString(val)); }

    template <typename T = int>
    QueryFilter &limitToFirst(T val) { return mLimitToFirst(NUM2S(val).get()); }

    template <typename T= int>
    QueryFilter &limitToLast(T val) { return mLimitToLast(NUM2S(val).get()); }

    template <typename T = int>
    auto startAt(T val) -> typename FB_JS::enable_if<FB_JS::is_same<T, float>::value || FB_JS::is_same<T, double>::value || FB_JS::is_num_int<T>::value, QueryFilter &>::type { return mStartAt(NUM2S(val).get(), false); }

    template <typename T = int>
    auto endAt(T val) -> typename FB_JS::enable_if<FB_JS::is_same<T, float>::value || FB_JS::is_same<T, double>::value || FB_JS::is_num_int<T>::value, QueryFilter &>::type { return mEndAt(NUM2S(val).get(), false); }

    template <typename T = const char *>
    auto startAt(T val) -> typename FB_JS::enable_if<FB_JS::is_string<T>::value, QueryFilter &>::type { return mStartAt(toString(val), true); }

    template <typename T = const char *>
    auto endAt(T val) -> typename FB_JS::enable_if<FB_JS::is_string<T>::value, QueryFilter &>::type { return mEndAt(toString(val), true); }

    template <typename T = int>
    auto equalTo(T val) -> typename FB_JS::enable_if<FB_JS::is_num_int<T>::value, QueryFilter &>::type { return mEqualTo(NUM2S(val).get(), false); }

    template <typename T = const char *>
    auto equalTo(T val) -> typename FB_JS::enable_if<FB_JS::is_string<T>::value, QueryFilter &>::type { return mEqualTo(toString(val), true); }

    QueryFilter &clear();

private:
    MBSTRING _orderBy;
    MBSTRING _limitToFirst;
    MBSTRING _limitToLast;
    MBSTRING _startAt;
    MBSTRING _endAt;
    MBSTRING _equalTo;

    QueryFilter &mOrderBy(const char *val);
    QueryFilter &mLimitToFirst(const char *val);
    QueryFilter &mLimitToLast(const char *val);
    QueryFilter &mStartAt(const char *val, bool isString);
    QueryFilter &mEndAt(const char *val, bool isString);
    QueryFilter &mEqualTo(const char *val, bool isString);

    char *strP(PGM_P pgm);
    char *newS(size_t len);
    void appendP(MBSTRING &buf, PGM_P p, bool empty = false);
    void delS(char *p);
    char *floatStr(float value);
    char *intStr(int value);

protected:
    template <typename T>
    auto toString(const T &val) -> typename FB_JS::enable_if<FB_JS::is_std_string<T>::value || FB_JS::is_arduino_string<T>::value || FB_JS::is_mb_string<T>::value, const char *>::type { return val.c_str(); }

    template <typename T>
    auto toString(T val) -> typename FB_JS::enable_if<FB_JS::is_const_chars<T>::value, const char *>::type { return val; }

    template <typename T>
    auto toString(T val) -> typename FB_JS::enable_if<FB_JS::fs_t<T>::value, const char *>::type { return (const char *)val; }

    template <typename T>
    auto toString(T val) -> typename FB_JS::enable_if<FB_JS::is_same<T, std::nullptr_t>::value, const char *>::type { return ""; }
};

#endif

#endif //ENABLE