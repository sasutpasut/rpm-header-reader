#include <stdio.h>
#include <stdlib.h>

#include <rpm/rpmlib.h>
#include <rpm/header.h>
#include <rpm/rpmts.h>
#include <rpm/rpmio.h>
#include <rpm/rpmtd.h>

#include "rpm_reader.h"

/* Function to read RPM file and populate header 
 * @param rpm_file  Path to the RPM file
 * @param header    Pointer to Header variable to populate
 * @return          1 on success, 0 on failure
 */
int read_rpm(const char *rpm_file, Header *header) {
    rpmReadConfigFiles(NULL, NULL);

    rpmts ts = rpmtsCreate();
    if (!ts) {
        fprintf(stderr, "Failed to create RPM transaction set.\n");
        return 0;
    }
    rpmtsSetRootDir(ts, "/");
    rpmtsSetVSFlags(ts, RPMVSF_MASK_NOSIGNATURES | RPMVSF_MASK_NODIGESTS);

    FD_t fd = Fopen(rpm_file, "r.ufdio");
    if (fd == NULL || Ferror(fd)) {
        fprintf(stderr, "Failed to open RPM file: %s\n", rpm_file);
        rpmtsFree(ts);
        return 0;
    }

    rpmRC rc = rpmReadPackageFile(ts, fd, NULL, header);
    if(rc != RPMRC_OK) {
        fprintf(stderr, "Error reading RPM file: %s\n", rpm_file);
        Fclose(fd);
        rpmtsFree(ts);
        return 0;
    }

    return 1;
}

/* Function to print RPM information from header
 * @param header    Pointer to Header variable containing RPM info
 * @return          1 on success, 0 on failure
 */
int print_rpm_info(Header *header) {
    const char *name = headerGetString(*header, RPMTAG_NAME);
    const char *version = headerGetString(*header, RPMTAG_VERSION);
    const char *release = headerGetString(*header, RPMTAG_RELEASE);
    const char *arch = headerGetString(*header, RPMTAG_ARCH);

    if (name) {
        printf("Package Name: %s\n", name);
    } else {
        fprintf(stderr, "Failed to get package name from header.\n");
        return 0;
    }

    if(version) {
        printf("Package Version: %s\n", version);
    } else {
        fprintf(stderr, "Failed to get package version from header.\n");
        return 0;
    }

    if(release) {
        printf("Package Release: %s\n", release);
    } else {
        fprintf(stderr, "Failed to get package release from header.\n");
        return 0;
    }

    if(arch) {
        printf("Package Architecture: %s\n", arch);
    } else {
        fprintf(stderr, "Failed to get package architecture from header.\n");
        return 0;
    }

    struct rpmtd_s td;
    
    if(headerGet(*header, RPMTAG_REQUIRENAME, &td, HEADERGET_DEFAULT)){
        printf("Package Dependencies:\n");
        const char *dep;
        while ((dep = rpmtdNextString(&td)) != NULL) {
            printf("  - %s\n", dep);
        }
        rpmtdFreeData(&td);
    } else {
        fprintf(stderr, "Failed to get package dependencies from header.\n");
        return 0;
    }

    if(headerGet(*header, RPMTAG_BASENAMES, &td, HEADERGET_DEFAULT)){
        printf("Package Installs:\n");
        const char *prov;
        while ((prov = rpmtdNextString(&td)) != NULL) {
            printf("  - %s\n", prov);
        }
        rpmtdFreeData(&td);
    } else {
        fprintf(stderr, "Failed to get package installs from header.\n");
        return 0;
    }

    return 1;
}