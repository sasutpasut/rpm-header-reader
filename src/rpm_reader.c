#include <stdio.h>
#include <stdlib.h>
#include <rpm/rpmlib.h>
#include <rpm/header.h>
#include <rpm/rpmts.h>
#include <rpm/rpmte.h>
#include <rpm/rpmio.h>
#include <rpm/rpmtag.h>



int read_rpm(const char *rpm_file, Header *header) {
    rpmReadConfigFiles(NULL, NULL);

    rpmts ts = rpmtsCreate();
    if (!ts) {
        fprintf(stderr, "Failed to create RPM transaction set.\n");
        return -1;
    }
    rpmtsSetRootDir(ts, "/");
    rpmtsSetVSFlags(ts, RPMVSF_MASK_NOSIGNATURES | RPMVSF_MASK_NODIGESTS);

    FD_t fd = Fopen(rpm_file, "r.ufdio");
    if (fd == NULL || Ferror(fd)) {
        fprintf(stderr, "Failed to open RPM file: %s\n", rpm_file);
        rpmtsFree(ts);
        return -1;
    }

    rpmRC rc = rpmReadPackageFile(ts, fd, NULL, header);
    if(rc != RPMRC_OK) {
        fprintf(stderr, "Error reading RPM file: %s\n", rpm_file);
        Fclose(fd);
        rpmtsFree(ts);
        return -1;
    }

    return 0;
}

int print_rpm_info(Header *header) {
    const char *name = headerGetString(*header, RPMTAG_NAME);
    const char *version = headerGetString(*header, RPMTAG_VERSION);

    if (name) {
        printf("Package Name: %s\n", name);
    } else {
        fprintf(stderr, "Failed to get package name from header.\n");
        return -1;
    }

    if(version) {
        printf("Package Version: %s\n", version);
    } else {
        fprintf(stderr, "Failed to get package version from header.\n");
        return -1;
    }
    return 0;
}