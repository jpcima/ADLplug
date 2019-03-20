#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

static bool parse_version(const char *text, uint8_t &V1, uint8_t &V2, uint8_t &V3)
{
    unsigned v1, v2, v3;
    if (sscanf(text, "%u.%u.%u", &v1, &v2, &v3) != 3 ||
        v1 >= 256 || v2 >= 256 || v3 >= 256)
    {
        return false;
    }
    V1 = v1;
    V2 = v2;
    V3 = v3;
    return true;
}

int main(int argc, char *argv[])
{
    if (argc != 4) {
        fprintf(stderr, "Usage: AUrez <name> <id> <version>\n");
        return (argc < 2) ? 0 : 1;
    }

    // example: ADLplug ADLM 1.0.0
    // example: OPNplug OPNM 1.0.0

    const char *name = argv[1];
    const char *id = argv[2];
    const char *version = argv[3];

    uint8_t V1;
    uint8_t V2;
    uint8_t V3;
    if (!parse_version(version, V1, V2, V3)) {
        fprintf(stderr, "Invalid version number\n");
        return 1;
    }

    if (strlen(id) != 4) {
        fprintf(stderr, "Invalid identifier length\n");
        return 1;
    }
    uint8_t I1 = (uint8_t)id[0];
    uint8_t I2 = (uint8_t)id[1];
    uint8_t I3 = (uint8_t)id[2];
    uint8_t I4 = (uint8_t)id[3];

    if (strlen(name) != 7) {
        fprintf(stderr, "Invalid name length\n");
        return 1;
    }
    uint8_t N1 = (uint8_t)name[0];
    uint8_t N2 = (uint8_t)name[1];
    uint8_t N3 = (uint8_t)name[2];
    uint8_t N4 = (uint8_t)name[3];
    uint8_t N5 = (uint8_t)name[4];
    uint8_t N6 = (uint8_t)name[5];
    uint8_t N7 = (uint8_t)name[6];

    const uint8_t rsrc[] = {
        #include "AUrez.dat"
    };

    FILE *out = stdout;

    if (!isatty(fileno(out))) {
        fwrite(rsrc, 1, sizeof(rsrc), out);
        if (fflush(out) != 0) {
            fprintf(stderr, "File output error\n");
            return 1;
        }
    }
    else {
        out = popen("xxd", "w");
        if (!out) {
            fprintf(stderr, "Cannot run the command `xxd`.\n");
            return 1;
        }
        fwrite(rsrc, 1, sizeof(rsrc), out);
        if (pclose(out) != 0) {
            fprintf(stderr, "Process output error\n");
            return 1;
        }
    }

    return 0;
};
