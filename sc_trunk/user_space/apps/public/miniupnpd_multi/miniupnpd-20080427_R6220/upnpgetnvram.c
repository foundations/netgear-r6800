#ifdef UPNP_DEBUG
#include <stdio.h>
#include <stdarg.h>
void debug_msg(char *format, ...)
{
    va_list args;
    FILE *fp;

    fp = fopen("/tmp/upnp_debug", "a+");
    if(!fp) return;

    va_start(args, format);
    vfprintf(fp, format, args);
    va_end(args);
    fprintf(fp, "\n");
    fflush(fp);
    fclose(fp);
}
#endif
