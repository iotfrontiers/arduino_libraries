/**
 * Google's Firebase MultiPathStream class, FB_MP_Stream.cpp version 1.1.2
 * 
 * This library supports Espressif ESP8266 and ESP32
 * 
 * Created November 2, 2021
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

#ifndef FIREBASE_MULTIPATH_STREAM_SESSION_CPP
#define FIREBASE_MULTIPATH_STREAM_SESSION_CPP
#include "FB_MP_Stream.h"

FIREBASE_MP_STREAM_CLASS::FIREBASE_MP_STREAM_CLASS()
{
}

FIREBASE_MP_STREAM_CLASS::~FIREBASE_MP_STREAM_CLASS()
{
}

void FIREBASE_MP_STREAM_CLASS::begin(UtilsClass *u, struct fb_esp_stream_info_t *s)
{
    ut = u;
    sif = s;
}

bool FIREBASE_MP_STREAM_CLASS::get(const String &path /* child path */)
{
    value.clear();
    type.clear();
    dataPath.clear();
    bool res = false;
    if (sif->data_type == fb_esp_data_type::d_json)
    {
        char *tmp = ut->strP(fb_esp_pgm_str_1);
        bool r = strcmp(sif->path.c_str(), tmp) == 0;
        ut->delP(&tmp);
        if (r)
        {
            FirebaseJsonData data;
            sif->m_json->get(data, path);
            if (data.success)
            {
                type = data.type;
                char *buf = ut->strP(fb_esp_pgm_str_186);
                if (strcmp(type.c_str(), buf) == 0)
                    type = sif->data_type_str.c_str();
                eventType = sif->event_type_str.c_str();
                value = data.to<const char *>();
                dataPath = path;
                ut->delP(&buf);
                res = true;
            }
        }
        else
        {
            MBSTRING root = path.c_str();
            MBSTRING branch = sif->path;
            //check for the steam data path is matched or under the root (child path)
            if (checkPath(root, branch))
            {
                sif->m_json->toString(value, true);
                type = sif->data_type_str.c_str();
                eventType = sif->event_type_str.c_str();
                dataPath = sif->path.c_str();
                res = true;
            }
        }
    }
    else
    {
        MBSTRING root = path.c_str();
        MBSTRING branch = sif->path;
        //check for the steam data path is matched or under the root (child path)
        if (checkPath(root, branch))
        {
            value = sif->data.c_str();
            dataPath = sif->path.c_str();
            type = sif->data_type_str.c_str();
            eventType = sif->event_type_str.c_str();
            res = true;
        }
    }
    return res;
}

bool FIREBASE_MP_STREAM_CLASS::checkPath(MBSTRING &root, MBSTRING &branch)
{
    if (root[0] != '/')
        root.insert(0, 1, '/');

    if (branch[0] != '/')
        branch.insert(0, 1, '/');

    if (root.length() != branch.length())
    {
        size_t p = branch.find("/", 1);
        if (p != MBSTRING::npos)
            branch = branch.substr(0, p);
    }

    return strcmp(branch.c_str(), root.c_str()) == 0;
}

void FIREBASE_MP_STREAM_CLASS::empty()
{
    dataPath.clear();
    value.clear();
    type.clear();
    sif->m_json = nullptr;
}

int FIREBASE_MP_STREAM_CLASS::payloadLength()
{
    return sif->payload_length;
}

int FIREBASE_MP_STREAM_CLASS::maxPayloadLength()
{
    return sif->max_payload_length;
}

#endif

#endif //ENABLE