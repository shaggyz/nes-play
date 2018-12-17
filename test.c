#include <stdio.h>

#define BUFSZ 16

int main (int argc, char **argv) {

    unsigned char buf[BUFSZ] = {0};
    size_t bytes = 0, i, readsz = sizeof buf;
    FILE *fp = argc > 1 ? fopen (argv[1], "rb") : stdin;

    if (!fp) {
        fprintf (stderr, "error: file open failed '%s'.\n", argv[1]);
        return 1;
    }

    /* read/output BUFSZ bytes at a time */
    while ((bytes = fread (buf, sizeof *buf, readsz, fp)) == readsz) {
        for (i = 0; i < readsz; i++)
            printf (" 0x%02x", buf[i]);
        putchar ('\n');
    }
    for (i = 0; i < bytes; i++) /* output final partial buf */
        printf (" 0x%02x", buf[i]);
    putchar ('\n');

    if (fp != stdin)
        fclose (fp);

    return 0;
}
