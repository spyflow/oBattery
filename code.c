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

/* Function to display help message */
void show_help() {
    printf("oBattery Calculator\n");
    printf("Usage: obattery [options]\n");
    printf("Options:\n");
    printf("  -p, --precision <num>  Set the number of decimals (default is 2)\n");
    printf("  -n, --number           Return only the numeric value (no text or %% symbol)\n");
    printf("  -h, --help             Show this help message\n");
    printf("  -v, --version          Show program version\n");
    printf("  -a, --author           Show program author\n");
}

int main(int argc, char *argv[]) {
    int precision = 2;  /* Default precision */
    int number_only = 0;  /* Flag for numeric output only */
    FILE *file;
    long energy_now, energy_full;

    /* Parse command-line arguments */
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            show_help();
            return 0;
        }
        else if (strcmp(argv[i], "--precision") == 0 || strcmp(argv[i], "-p") == 0) {
            if (i + 1 < argc) {
                precision = atoi(argv[i + 1]);
                i++;  /* Skip next argument since it's the precision value */
            } else {
                fprintf(stderr, "Error: --precision requires a number argument.\n");
                return 1;
            }
        }
        else if (strcmp(argv[i], "--number") == 0 || strcmp(argv[i], "-n") == 0) {
            number_only = 1;
        }
        else if (strcmp(argv[i], "--version") == 0 || strcmp(argv[i], "-v") == 0) {
            printf("oBattery version 0.0.3\n");
            return 0;
        }
        else if (strcmp(argv[i], "--author") == 0 || strcmp(argv[i], "-a") == 0) {
            printf("Program author: spyflow\n");
            return 0;
        }
        else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            return 1;
        }
    }
    
    /* Read energy_now */
    file = fopen("/sys/class/power_supply/BAT0/energy_now", "r");
    if (file == NULL) {
        perror("Error reading energy_now");
        return 1;
    }
    fscanf(file, "%ld", &energy_now);
    fclose(file);

    /* Read energy_full */
    file = fopen("/sys/class/power_supply/BAT0/energy_full", "r");
    if (file == NULL) {
        perror("Error reading energy_full");
        return 1;
    }
    fscanf(file, "%ld", &energy_full);
    fclose(file);

    /* Avoid division by zero */
    if (energy_full == 0) {
        printf("Error: energy_full is 0.\n");
        return 1;
    }

    /* Calculate battery percentage */
    double battery = (double)energy_now / energy_full * 100;

    /* Print the result based on output mode */
    if (number_only) {
        printf("%.*f\n", precision, battery);
    } else {
        printf("Battery: %.*f%%\n", precision, battery);
    }

    return 0;
}
