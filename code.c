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

// Función para mostrar el mensaje de ayuda
void show_help() {
    printf("oBattery Percentage Calculator\n");
    printf("Usage: obattery [options]\n");
    printf("Options:\n");
    printf("  -p, --precision <num>  Set the number of decimals (default is 2)\n");
    printf("  -h, --help             Show this help message\n");
}

int main(int argc, char *argv[]) {
    int precision = 2;  // Default precision
    FILE *file;
    long energy_now, energy_full;

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            show_help();
            return 0;
        }
        if (strcmp(argv[i], "--precision") == 0 || strcmp(argv[i], "-p") == 0) {
            if (i + 1 < argc) {
                precision = atoi(argv[i + 1]);
                i++;  // Skip next argument since it's the precision value
            } else {
                printf("Error: --precision requires a number argument.\n");
                return 1;
            }
        }else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            //show_help();
            return 1;
        }
    }

    // Read energy_now
    file = fopen("/sys/class/power_supply/BAT0/energy_now", "r");
    if (file == NULL) {
        perror("Error reading energy_now");
        return 1;
    }
    fscanf(file, "%ld", &energy_now);
    fclose(file);

    // Read energy_full
    file = fopen("/sys/class/power_supply/BAT0/energy_full", "r");
    if (file == NULL) {
        perror("Error reading energy_full");
        return 1;
    }
    fscanf(file, "%ld", &energy_full);
    fclose(file);

    // Avoid division by zero
    if (energy_full == 0) {
        printf("Error: energy_full is 0.\n");
        return 1;
    }

    // Calculate battery percentage
    double battery_percentage = (double)energy_now / energy_full * 100;

    // Print the result with the specified precision
    printf("Battery Percentage: %.*f%%\n", precision, battery_percentage);

    return 0;
}
