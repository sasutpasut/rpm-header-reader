#include <stdio.h>
#include <stdlib.h>

#include <rpm/rpmlib.h>
#include <rpm/header.h>
#include <rpm/rpmts.h>
#include <rpm/rpmio.h>

#include "rpm_reader.h"

// this will create a conflict - 3

int main (int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <rpm_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *rpm_file = argv[1];
    Header header=NULL;

    if (read_rpm(rpm_file, &header) == 0) {
        fprintf(stderr, "Failed to read RPM file: %s\n", rpm_file);
        return EXIT_FAILURE;
    }

    if (print_rpm_info(&header) == 0) {
        fprintf(stderr, "Failed to print RPM info for file: %s\n", rpm_file);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}