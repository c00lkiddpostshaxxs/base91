#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

const char *base91_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!#$%&()*+,./:;<=>?@[]^_`{|}~\"";
unsigned char decode_table[256];

void init_decode_table() {
    for (int i = 0; i < 256; i++) decode_table[i] = 255;
    for (int i = 0; i < 91; i++) decode_table[(unsigned char)base91_alphabet[i]] = i;
}

void encode_base91(FILE *in, FILE *out, int break_count) {
    unsigned long b = 0;
    int n = 0;
    int ch;
    int col = 0;

    while ((ch = fgetc(in)) != EOF) {
        b |= (unsigned long)ch << n;
        n += 8;
        if (n > 13) {
            unsigned long v = b & 8191;
            if (v > 88) {
                b >>= 13;
                n -= 13;
            } else {
                v = b & 16383;
                b >>= 14;
                n -= 14;
            }
            fputc(base91_alphabet[v % 91], out);
            fputc(base91_alphabet[v / 91], out);
            col += 2;
            if (break_count > 0 && col >= break_count) {
                fputc('\n', out);
                col = 0;
            }
        }
    }
    if (n) {
        fputc(base91_alphabet[b % 91], out);
        col++;
        if (n > 7 || b > 90) {
            fputc(base91_alphabet[b / 91], out);
            col++;
        }
    }
    if (col > 0) fputc('\n', out);
}

int decode_base91(FILE *in, FILE *out) {
    unsigned long b = 0;
    int n = 0, v = -1, ch;
    size_t buf_cap = 4096, buf_len = 0;
    unsigned char *buf = malloc(buf_cap);
    
    init_decode_table();
    while ((ch = fgetc(in)) != EOF) {
        if (decode_table[ch] == 255) {
            if (ch == '\n' || ch == '\r') continue;
            free(buf);
            fprintf(stderr, "Invalid character in input stream.\n");
            return 1;
        }
        int c = decode_table[ch];
        if (v < 0) v = c;
        else {
            v += c * 91;
            b |= (unsigned long)v << n;
            n += (v & 8191) > 88 ? 13 : 14;
            while (n > 7) {
                if (buf_len >= buf_cap) buf = realloc(buf, buf_cap *= 2);
                buf[buf_len++] = (unsigned char)(b & 255);
                b >>= 8; n -= 8;
            }
            v = -1;
        }
    }
    
    // --- NEW: Flush the final remaining byte into the buffer ---
    if (v + 1) {
        if (buf_len >= buf_cap) buf = realloc(buf, buf_cap *= 2);
        buf[buf_len++] = (unsigned char)((b | (unsigned long)v << n) & 255);
    }
    // -----------------------------------------------------------

    fwrite(buf, 1, buf_len, out);
    free(buf);
    return 0;
}

void print_help() {
    printf("basE91(1)                    General Commands Manual                   basE91(1)\n\n");
    printf("NAME\n");
    printf("     basE91 - Encode and decode using basE91 representation\n\n");
    printf("SYNOPSIS\n");
    printf("     basE91 [-h | -D | -d] [-b count] [-i input_file] [-o output_file]\n\n");
    printf("DESCRIPTION\n");
    printf("     basE91 encodes and decodes basE91 data. With no options, basE91 reads\n");
    printf("     raw data from stdin and writes encoded data as a continuous block to stdout.\n\n");
    printf("OPTIONS\n");
    printf("     -b count, --break=count\n");
    printf("           Insert line breaks every count characters. Default is 0, which\n");
    printf("           generates an unbroken stream.\n\n");
    printf("     -d, -D, --decode\n");
    printf("           Decode incoming basE91 stream into binary data.\n\n");
    printf("     -h, --help\n");
    printf("           Print usage summary and exit.\n\n");
    printf("     -i input_file, --input=input_file\n");
    printf("           Read input from input_file. Default is stdin; passing - also\n");
    printf("           represents stdin.\n\n");
    printf("     -o output_file, --output=output_file\n");
    printf("           Write output to output_file. Default is stdout; passing - also\n");
    printf("           represents stdout.\n");
}

int main(int argc, char *argv[]) {
    int opt;
    int decode = 0;
    int break_count = 0;
    char *input_file = NULL;
    char *output_file = NULL;

    static struct option long_options[] = {
        {"break",  required_argument, 0, 'b'},
        {"decode", no_argument,       0, 'd'},
        {"help",   no_argument,       0, 'h'},
        {"input",  required_argument, 0, 'i'},
        {"output", required_argument, 0, 'o'},
        {0, 0, 0, 0}
    };

    int option_index = 0;
    while ((opt = getopt_long(argc, argv, "hDdb:i:o:", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'h': print_help(); return 0;
            case 'D':
            case 'd': decode = 1; break;
            case 'b': break_count = atoi(optarg); break;
            case 'i': input_file = optarg; break;
            case 'o': output_file = optarg; break;
            default: print_help(); return 1;
        }
    }

    FILE *in = stdin;
    FILE *out = stdout;

    if (input_file && strcmp(input_file, "-") != 0) {
        in = fopen(input_file, "rb");
        if (!in) { perror("basE91: input file error"); return 1; }
    }

    if (output_file && strcmp(output_file, "-") != 0) {
        out = fopen(output_file, "wb");
        if (!out) { perror("basE91: output file error"); return 1; }
    }

    if (in == stdin && isatty(fileno(stdin))) {
        fprintf(stderr, "basE91: No text detected. Exiting...\n");
        return 1;
    }

    if (decode) {
        if (decode_base91(in, out) != 0) {
            if (in != stdin) fclose(in);
            if (out != stdout && out != NULL) fclose(out);
            return 1;
        }
    } else {
        encode_base91(in, out, break_count);
    }

    if (in != stdin) fclose(in);
    if (out != stdout && out != NULL) fclose(out);

    return 0;
}
