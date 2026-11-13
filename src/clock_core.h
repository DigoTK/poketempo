#pragma once
#include <cmath>
#include <cstdio>
#include <ctime>
#include <cstring>

namespace poketime {
constexpr double min_time = 0;
constexpr double max_time = 4102444799.0; // 2099-12-31 23:59:59, calendar seconds (no DST).
inline bool valid(double t) { return std::isfinite(t) && t >= min_time && t <= max_time; }
inline bool decode(double t, int out[6]) {
    if (!valid(t)) return false;
    time_t seconds = static_cast<time_t>(t);
    tm result{};
#ifdef _WIN32
    if (gmtime_s(&result, &seconds)) return false;
#else
    if (!gmtime_r(&seconds, &result)) return false;
#endif
    out[0]=result.tm_year+1900; out[1]=result.tm_mon+1; out[2]=result.tm_mday;
    out[3]=result.tm_hour; out[4]=result.tm_min; out[5]=result.tm_sec;
    return true;
}
inline bool encode(const int in[6], double &out) {
    if(in[0]<1970 || in[0]>2099 || in[1]<1 || in[1]>12 || in[2]<1 || in[2]>31 ||
       in[3]<0 || in[3]>23 || in[4]<0 || in[4]>59 || in[5]<0 || in[5]>59) return false;
    tm v{}; v.tm_year=in[0]-1900; v.tm_mon=in[1]-1; v.tm_mday=in[2];
    v.tm_hour=in[3]; v.tm_min=in[4]; v.tm_sec=in[5];
#ifdef _WIN32
    out=static_cast<double>(_mkgmtime(&v));
#else
    out=static_cast<double>(timegm(&v));
#endif
    int check[6];
    return decode(out,check) && !memcmp(check,in,sizeof check);
}
inline bool parse(const char *text, double &out) {
    if (!text || strlen(text)!=19) return false;
    int v[6]; char tail;
    return sscanf(text,"%d-%d-%d %d:%d:%d%c",v,v+1,v+2,v+3,v+4,v+5,&tail)==6 && encode(v,out);
}
inline double step(double current, double dt, double rate, bool paused) {
    if(paused || !std::isfinite(dt) || dt<0 || dt>2 || !std::isfinite(rate) || rate<0.1 || rate>120) return current;
    return fmin(max_time,current+dt*rate);
}
}
