/*
 * oBattery - Simple Linux battery check percentage calculator
 * Copyright (C) 2026 spyflow
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* Function to display help message */
void show_help(void) {
    printf("oBattery Calculator\n");
    printf("Usage: obattery [options]\n");
    printf("Options:\n");
    printf("  -p, --precision <num>  Set the number of decimals (default is 2)\n");
    printf("  -n, --number           Return only the numeric value\n");
    printf("  -h, --help             Show this help message\n");
    printf("  -v, --version          Show program version\n");
    printf("  -a, --author           Show program author\n");
}

/* Parse and validate the precision number */
int parse_precision(const char *text, int *precision) {
    char *end;
    long value;

    errno = 0;
    value = strtol(text, &end, 10);

    if (errno != 0 || *text == '\0' || *end != '\0') {
        return 0;
    }

    if (value < 0 || value > 20) {
        return 0;
    }

    *precision = (int)value;
    return 1;
}

/* Read a long integer from a file */
int read_value(const char *path, long *value) {
    FILE *file = fopen(path, "r");

    if (file == NULL) {
        perror(path);
        return 0;
    }

    if (fscanf(file, "%ld", value) != 1) {
        fprintf(stderr, "Error: could not read a valid number from %s\n", path);
        fclose(file);
        return 0;
    }

    fclose(file);
    return 1;
}

int main(int argc, char *argv[]) {
    int precision = 2;
    int number_only = 0;
    long energy_now;
    long energy_full;

    const char *energy_now_path =
        "/sys/class/power_supply/BAT0/energy_now";

    const char *energy_full_path =
        "/sys/class/power_supply/BAT0/energy_full";

    /*
     * Parse command-line arguments
     */
    for (int i = 1; i < argc; i++) {
        const char *precision_text = NULL;

        if (strcmp(argv[i], "--help") == 0 ||
            strcmp(argv[i], "-h") == 0) {
            show_help();
            return 0;
        }

        else if (strcmp(argv[i], "--version") == 0 ||
                 strcmp(argv[i], "-v") == 0) {
            printf("oBattery version 0.0.3\n");
            return 0;
        }

        else if (strcmp(argv[i], "--author") == 0 ||
                 strcmp(argv[i], "-a") == 0) {
            printf("Program author: spyflow\n");
            return 0;
        }

        else if (strcmp(argv[i], "--number") == 0 ||
                 strcmp(argv[i], "-n") == 0) {
            number_only = 1;
        }

        /*
         * Supports:
         * -p 3
         * --precision 3
         * -p3
         * --precision=3
         */
        else if (strcmp(argv[i], "-p") == 0 ||
                 strcmp(argv[i], "--precision") == 0) {

            if (i + 1 >= argc) {
                fprintf(
                    stderr,
                    "Error: %s requires a number argument.\n",
                    argv[i]
                );
                return 1;
            }

            precision_text = argv[++i];

            if (!parse_precision(precision_text, &precision)) {
                fprintf(
                    stderr,
                    "Error: precision must be an integer between 0 and 20.\n"
                );
                return 1;
            }
        }

        else if (strncmp(argv[i], "--precision=", 12) == 0) {
            precision_text = argv[i] + 12;

            if (!parse_precision(precision_text, &precision)) {
                fprintf(
                    stderr,
                    "Error: precision must be an integer between 0 and 20.\n"
                );
                return 1;
            }
        }

        else if (strncmp(argv[i], "-p", 2) == 0 &&
                 strlen(argv[i]) > 2) {
            precision_text = argv[i] + 2;

            if (!parse_precision(precision_text, &precision)) {
                fprintf(
                    stderr,
                    "Error: precision must be an integer between 0 and 20.\n"
                );
                return 1;
            }
        }

        else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            return 1;
        }
    }

    /*
     * Read the current battery energy.
     */
    if (!read_value(energy_now_path, &energy_now)) {
        fprintf(
            stderr,
            "Make sure that your battery is located at BAT0.\n"
        );
        return 1;
    }

    /*
     * Read the maximum battery energy.
     */
    if (!read_value(energy_full_path, &energy_full)) {
        fprintf(
            stderr,
            "Make sure that your battery is located at BAT0.\n"
        );
        return 1;
    }

    /*
     * Avoid division by zero.
     */
    if (energy_full <= 0) {
        fprintf(stderr, "Error: energy_full must be greater than zero.\n");
        return 1;
    }

    /*
     * Calculate battery percentage.
     */
    double battery = (double)energy_now / (double)energy_full * 100.0;

    /*
     * Print the result.
     */
    if (number_only) {
        printf("%.*f\n", precision, battery);
    } else {
        printf("Battery: %.*f%%\n", precision, battery);
    }

    return 0;
}
