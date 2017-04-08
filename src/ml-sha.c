#include <string.h>

#include "jerry-api.h"
#include "hal_sha.h"
#include "microlattice.h"

DELCARE_HANDLER(__sha) {
  char str_buffer [50];
  if (args_cnt == 1 && jerry_value_is_string(args_p[0])) {

    jerry_size_t value_req_sz = jerry_get_string_size (args_p[0]);
    jerry_char_t value_buffer[value_req_sz];
    jerry_string_to_char_buffer (args_p[0], value_buffer, value_req_sz);
    value_buffer[value_req_sz] = '\0';

    uint32_t size = (uint32_t)strlen(value_buffer);
    uint8_t digest[64] = {0};

    hal_sha1_context_t sha1_context;
    hal_sha1_init(&sha1_context);
    hal_sha1_append(&sha1_context, value_buffer, size);
    hal_sha1_end(&sha1_context, digest);

    uint8_t i;
    strcpy(str_buffer, "");

    for (i = 0; i < HAL_SHA1_DIGEST_SIZE; i++) {
      if (i % 16 == 0) {
          printf("\r\n");
      }
      char buffer [2];
      sprintf (buffer, "%02x", digest[i]);
      strcat(str_buffer, buffer);
    }
  }
  return jerry_create_string(str_buffer);
}

void ml_sha_init(void) {
  REGISTER_HANDLER(__sha);
}