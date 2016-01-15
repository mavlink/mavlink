/*
  simple test of mavlink sha256 code. 
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <mavlink_sha256.h>

int main(int argc, const char *argv[])
{
    mavlink_sha256_ctx ctx;
    uint8_t result[6];
    uint8_t i;
    
    mavlink_sha256_init(&ctx);
    mavlink_sha256_update(&ctx, argv[1], strlen(argv[1]));
    mavlink_sha256_final_48(&ctx, result);
    for (i=0; i<6; i++) {
        printf("%02x ", (unsigned)result[i]);
    }
    printf("\n");
    return 0;
}
