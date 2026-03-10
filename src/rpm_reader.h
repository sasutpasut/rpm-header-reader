#include <stdio.h>
#include <stdlib.h>

#include <rpm/rpmlib.h>
#include <rpm/header.h>
#include <rpm/rpmts.h>
#include <rpm/rpmio.h>
#include <rpm/rpmtd.h>

int read_rpm(const char *rpm_file, Header *header);
int print_rpm_info(Header *header);