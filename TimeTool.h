#ifndef TOOL_TIME_H
#define TOOL_TIME_H
 
#include <iostream>
#include <chrono>
#include <time.h>
#include <sys/time.h>
#include <ctime>
//#include <conio.h>
#include<iostream>
#include<cstring>

#define  ALT_E          0x01
#define  ALT_O          0x02
//#define LEGAL_ALT(x)       { if (alt_format & ~(x)) return (0); }
#define  LEGAL_ALT(x)       { ; }
#define  TM_YEAR_BASE   (1970)
 
static    int conv_num(const char **, int *, int, int);
static  int strncasecmp(char *s1, char *s2, size_t n);
 
static  const char *day[7] = {
     "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday",
     "Friday", "Saturday"
};
static  const char *abday[7] = {
     "Sun","Mon","Tue","Wed","Thu","Fri","Sat"
};
static  const char *mon[12] = {
     "January", "February", "March", "April", "May", "June", "July",
     "August", "September", "October", "November", "December"
};
static  const char *abmon[12] = {
     "Jan", "Feb", "Mar", "Apr", "May", "Jun",
     "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};
static  const char *am_pm[2] = {
     "AM", "PM"
};
char  *
strptime(const char *buf, const char *fmt, struct tm *tm)
{
     char c;
     const char *bp;
     size_t len = 0;
     int alt_format, i, split_year = 0;
 
     bp = buf;
 
     while ((c = *fmt) != '\0') {
         /* Clear `alternate' modifier prior to new conversion. */
         alt_format = 0;
 
         /* Eat up white-space. */
         if (isspace(c)) {
              while (isspace(*bp))
                   bp++;
 
              fmt++;
              continue;
         }
                  
         if ((c = *fmt++) != '%')
              goto literal;
 
 
again:        switch (c = *fmt++) {
         case '%': /* "%%" is converted to "%". */
literal :
              if (c != *bp++)
                   return (0);
              break;
 
         /*
          * "Alternative" modifiers. Just set the appropriate flag
          * and start over again.
          */
         case 'E': /* "%E?" alternative conversion modifier. */
              LEGAL_ALT(0);
              alt_format |= ALT_E;
              goto again;
 
         case 'O': /* "%O?" alternative conversion modifier. */
              LEGAL_ALT(0);
              alt_format |= ALT_O;
              goto again;
             
         /*
          * "Complex" conversion rules, implemented through recursion.
          */
         case 'c': /* Date and time, using the locale's format. */
              LEGAL_ALT(ALT_E);
              if (!(bp = strptime(bp, "%x %X", tm)))
                   return (0);
              break;
 
         case 'D': /* The date as "%m/%d/%y". */
              LEGAL_ALT(0);
              if (!(bp = strptime(bp, "%m/%d/%y", tm)))
                   return (0);
              break;
 
         case 'R': /* The time as "%H:%M". */
              LEGAL_ALT(0);
              if (!(bp = strptime(bp, "%H:%M", tm)))
                   return (0);
              break;
 
         case 'r': /* The time in 12-hour clock representation. */
              LEGAL_ALT(0);
              if (!(bp = strptime(bp, "%I:%M:%S %p", tm)))
                   return (0);
              break;
 
         case 'T': /* The time as "%H:%M:%S". */
              LEGAL_ALT(0);
              if (!(bp = strptime(bp, "%H:%M:%S", tm)))
                   return (0);
              break;
 
         case 'X': /* The time, using the locale's format. */
              LEGAL_ALT(ALT_E);
              if (!(bp = strptime(bp, "%H:%M:%S", tm)))
                   return (0);
              break;
 
         case 'x': /* The date, using the locale's format. */
              LEGAL_ALT(ALT_E);
              if (!(bp = strptime(bp, "%m/%d/%y", tm)))
                   return (0);
              break;
 
         /*
          * "Elementary" conversion rules.
          */
         case 'A': /* The day of week, using the locale's form. */
         case 'a':
              LEGAL_ALT(0);
              for (i = 0; i < 7; i++) {
                   /* Full name. */
                   len = strlen(day[i]);
                   if (strncasecmp((char *)(day[i]), (char *)bp, len) == 0)
                       break;
 
                   /* Abbreviated name. */
                   len = strlen(abday[i]);
                   if (strncasecmp((char *)(abday[i]), (char *)bp, len) == 0)
                       break;
              }
 
              /* Nothing matched. */
              if (i == 7)
                   return (0);
 
              tm->tm_wday = i;
              bp += len;
              break;
 
         case 'B': /* The month, using the locale's form. */
         case 'b':
         case 'h':
              LEGAL_ALT(0);
              for (i = 0; i < 12; i++) {
                   /* Full name. */
                   len = strlen(mon[i]);
                   if (strncasecmp((char *)(mon[i]), (char *)bp, len) == 0)
                       break;
 
                   /* Abbreviated name. */
                   len = strlen(abmon[i]);
                   if (strncasecmp((char *)(abmon[i]),(char *) bp, len) == 0)
                       break;
              }
 
              /* Nothing matched. */
              if (i == 12)
                   return (0);
 
              tm->tm_mon = i;
              bp += len;
              break;
 
         case 'C': /* The century number. */
              LEGAL_ALT(ALT_E);
              if (!(conv_num(&bp, &i, 0, 99)))
                   return (0);
 
              if (split_year) {
                   tm->tm_year = (tm->tm_year % 100) + (i * 100);
              } else {
                   tm->tm_year = i * 100;
                   split_year = 1;
              }
              break;
 
         case 'd': /* The day of month. */
         case 'e':
              LEGAL_ALT(ALT_O);
              if (!(conv_num(&bp, &tm->tm_mday, 1, 31)))
                   return (0);
              break;
 
         case 'k': /* The hour (24-hour clock representation). */
              LEGAL_ALT(0);
              /* FALLTHROUGH */
         case 'H':
              LEGAL_ALT(ALT_O);
              if (!(conv_num(&bp, &tm->tm_hour, 0, 23)))
                   return (0);
              break;
 
         case 'l': /* The hour (12-hour clock representation). */
              LEGAL_ALT(0);
              /* FALLTHROUGH */
         case 'I':
              LEGAL_ALT(ALT_O);
              if (!(conv_num(&bp, &tm->tm_hour, 1, 12)))
                   return (0);
              if (tm->tm_hour == 12)
                   tm->tm_hour = 0;
              break;
 
         case 'j': /* The day of year. */
              LEGAL_ALT(0);
              if (!(conv_num(&bp, &i, 1, 366)))
                   return (0);
              tm->tm_yday = i - 1;
              break;
 
          case 'M': /* The minute. */
              LEGAL_ALT(ALT_O);
              if (!(conv_num(&bp, &tm->tm_min, 0, 59)))
                   return (0);
              break;
 
         case 'm': /* The month. */
              LEGAL_ALT(ALT_O);
              if (!(conv_num(&bp, &i, 1, 12)))
                   return (0);
              tm->tm_mon = i - 1;
              break;
 
//       case 'p': /* The locale's equivalent of AM/PM. */
//            LEGAL_ALT(0);
//            /* AM? */
//            if (strcasecmp(am_pm[0], bp) == 0) {
//                 if (tm->tm_hour > 11)
//                     return (0);
//
//                 bp += strlen(am_pm[0]);
//                 break;
//            }
//            /* PM? */
//            else if (strcasecmp(am_pm[1], bp) == 0) {
//                 if (tm->tm_hour > 11)
//                     return (0);
//
//                 tm->tm_hour += 12;
//                 bp += strlen(am_pm[1]);
//                 break;
//            }
//
//            /* Nothing matched. */
//            return (0);
 
         case 'S': /* The seconds. */
              LEGAL_ALT(ALT_O);
              if (!(conv_num(&bp, &tm->tm_sec, 0, 61)))
                   return (0);
              break;
 
         case 'U': /* The week of year, beginning on sunday. */
         case 'W': /* The week of year, beginning on monday. */
              LEGAL_ALT(ALT_O);
              /*
               * XXX This is bogus, as we can not assume any valid
               * information present in the tm structure at this
               * point to calculate a real value, so just check the
               * range for now.
               */
               if (!(conv_num(&bp, &i, 0, 53)))
                   return (0);
               break;
 
         case 'w': /* The day of week, beginning on sunday. */
              LEGAL_ALT(ALT_O);
              if (!(conv_num(&bp, &tm->tm_wday, 0, 6)))
                   return (0);
              break;
 
         case 'Y': /* The year. */
              LEGAL_ALT(ALT_E);
              if (!(conv_num(&bp, &i, 0, 9999)))
                   return (0);
 
              tm->tm_year = i - TM_YEAR_BASE;
              break;
 
         case 'y': /* The year within 100 years of the epoch. */
              LEGAL_ALT(ALT_E | ALT_O);
              if (!(conv_num(&bp, &i, 0, 99)))
                   return (0);
 
              if (split_year) {
                   tm->tm_year = ((tm->tm_year / 100) * 100) + i;
                   break;
              }
              split_year = 1;
              if (i <= 68)
                   tm->tm_year = i + 2000 - TM_YEAR_BASE;
              else
                   tm->tm_year = i + 1900 - TM_YEAR_BASE;
              break;
 
         /*
          * Miscellaneous conversions.
          */
         case 'n': /* Any kind of white-space. */
         case 't':
              LEGAL_ALT(0);
              while (isspace(*bp))
                   bp++;
              break;
 
 
         default: /* Unknown/unsupported conversion. */
              return (0);
         }
 
 
     }
 
     /* LINTED functional specification */
     return ((char *)bp);
}
 
 
static  int
conv_num(const char **buf, int *dest, int llim, int ulim)
{
     int result = 0;
 
     /* The limit also determines the number of valid digits. */
     int rulim = ulim;
 
     if (**buf < '0' || **buf > '9')
         return (0);
 
     do {
         result *= 10;
         result += *(*buf)++ - '0';
         rulim /= 10;
     } while ((result * 10 <= ulim) && rulim && **buf >= '0' && **buf <= '9');
 
     if (result < llim || result > ulim)
         return (0);
 
     *dest = result;
     return (1);
}
 
int  strncasecmp(char *s1, char *s2, size_t n)
{
 if (n == 0)
    return 0;
 
 while (n-- != 0 && tolower(*s1) == tolower(*s2))
    {
      if (n == 0 || *s1 == '\0' || *s2 == '\0')
     break;
      s1++;
      s2++;
    }
 
 return tolower(*(unsigned char *) s1) - tolower(*(unsigned char *) s2);
}

class TimeTool
{
public:
    static uint32_t elapse_seconds(std::chrono::system_clock::time_point &start)
    {
        return std::chrono::duration_cast<std::chrono::seconds>
                (std::chrono::system_clock::now() - start).count();
    }
 
    static uint64_t elapse_ms(std::chrono::system_clock::time_point &start)
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>
                (std::chrono::system_clock::now() - start).count();
    }
 
    static uint32_t now_to_seconds()
    {
        return std::chrono::duration_cast<std::chrono::seconds>
                (std::chrono::system_clock::now().time_since_epoch()).count();
    }
 
    static uint64_t now_to_ms()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>
                (std::chrono::system_clock::now().time_since_epoch()).count();
    }
 
    static uint64_t now_to_us()
    {
        return std::chrono::duration_cast<std::chrono::microseconds>
                (std::chrono::system_clock::now().time_since_epoch()).count();
    }
 
    static uint64_t to_ms(const std::chrono::system_clock::time_point &time)
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>
                (time.time_since_epoch()).count();
    }
 
    //"%Y-%m-%d %H:%M:%S"
    static std::string to_str(const std::chrono::system_clock::time_point &time)
    {
        char _time[25] = {0};
        time_t tt = std::chrono::system_clock::to_time_t(time);
        struct tm local_time;
        localtime_r(&tt, &local_time);
        strftime(_time, 22, "%Y-%m-%d %H:%M:%S", &local_time);
 
        return std::string(_time);
    }
 
    //"%d-%02d-%02d %02d:%02d:%02d.%03d"
    static std::string to_str_ex(const std::chrono::system_clock::time_point &time)
    {
        uint64_t mill = std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch()).count()
                -std::chrono::duration_cast<std::chrono::seconds>(time.time_since_epoch()).count()*1000;
 
        char _time[25] = {0};
        time_t tt = std::chrono::system_clock::to_time_t(time);
        struct tm local_time;
        localtime_r(&tt, &local_time);
        //strftime(_time, 22, "%Y-%m-%d %H:%M:%S", local_time);
        sprintf(_time, "%d-%02d-%02d %02d:%02d:%02d.%03d", local_time.tm_year+1900,
                local_time.tm_mon+1, local_time.tm_mday, local_time.tm_hour,
                local_time.tm_min, local_time.tm_sec, mill);
 
        return std::string(_time);
    }
 
    static uint64_t now_to_ms_ex()
    {
        struct timeval tv;
        ::gettimeofday(&tv, 0);
        return tv.tv_sec*1000+tv.tv_usec/1000;
    }
 
    //"%Y-%m-%d %H:%M:%S"
    static time_t to_time(std::string str)
    {
        time_t t_;
        tm tm_;
        strptime(str.c_str(), "%Y-%m-%d %H:%M:%S", &tm_); //将字符串转换为tm时间
       // t_ = mktime(&tm_); //将tm时间转换为秒时间
 
        return t_;
    }
 
    //"%d-%02d-%02d %02d:%02d:%02d.%03d"
    static std::chrono::system_clock::time_point to_time_ex(std::string str)
    {
        int pos = str.length()-3;
 
        time_t t_;
        tm tm_;
        strptime(str.substr(0,pos).c_str(), "%Y-%m-%d %H:%M:%S", &tm_); //将字符串转换为tm时间
        t_ = mktime(&tm_); //将tm时间转换为秒时间
 
        int milli = std::stoi(str.substr(pos));
 
        return std::chrono::system_clock::time_point(std::chrono::milliseconds(t_*1000 + milli));
    }
 
    static std::string to_str_ex(uint64_t ms)
    {
        uint64_t mill = ms%1000;
 
        char _time[25] = {0};
        time_t tt = ms/1000;
        struct tm *local_time=localtime(&tt);
        //strftime(_time, 22, "%Y-%m-%d %H:%M:%S", local_time);
        sprintf(_time, "%d-%02d-%02d %02d:%02d:%02d.%03d", local_time->tm_year+1900,
                local_time->tm_mon+1, local_time->tm_mday, local_time->tm_hour,
                local_time->tm_min, local_time->tm_sec, mill);
 
        return std::string(_time);
    }
 
    static uint32_t elapse_seconds(time_t &start)
    {
        time_t now;
        time(&now);
        return static_cast<uint32_t>(std::difftime(now, start));
    }
 
    //"%Y-%m-%d %H:%M:%S"
    static std::string to_str(const std::time_t &time)
    {
        char _time[25] = {0};
        struct tm local_time;
        localtime_r(&time, &local_time);
        strftime(_time, 22, "%Y-%m-%d %H:%M:%S", &local_time);
 
        return std::string(_time);
    }
};
 
#endif // TOOL_TIME_H

/*
int main()
{
    auto now = std::chrono::system_clock::now();
    std::string tt=tool_time::to_str_ex(tool_time::now_to_ms());
    std::string tt2=tool_time::to_str_ex(tool_time::to_ms(now));
 
    std::string tt3=tool_time::to_str(std::chrono::system_clock::to_time_t(now));
    std::string tt4=tool_time::to_str(now);
 
    printf("%s\n", tt.c_str());
    printf("%s\n", tt2.c_str());
 
    printf("%s\n", tt3.c_str());
    printf("%s\n", tt4.c_str());
 
    printf("%ld\n", tool_time::to_time(tt));
    printf("%ld\n", tool_time::now_to_seconds());
    printf("%ld\n", tool_time::now_to_ms()/1000);
    printf("%ld\n", tool_time::now_to_ms_ex()/1000);
 
    printf("%ld\n", tool_time::now_to_ms());
    printf("%ld\n", tool_time::now_to_ms_ex());
 
    return 0;

}
*/