#include <string.h>

#include "jerry.h"
#include "hal_sha.h"
#include "microlattice.h"

DELCARE_HANDLER(__sha) {
  if (args_cnt == 1 && args_p[0].type == JERRY_API_DATA_TYPE_STRING) {

    // int value_req_sz = jerry_api_string_to_char_buffer(args_p[0].v_string, NULL, 0);
    // value_req_sz *= -1;
    // char value_buffer [value_req_sz+1]; // 不能有*
    // value_req_sz = jerry_api_string_to_char_buffer (args_p[0].v_string, (jerry_api_char_t *) value_buffer, value_req_sz);
    // value_buffer[value_req_sz] = '\0';

    int value_req_sz = -jerry_api_string_to_char_buffer(args_p[0].v_string, NULL, 0);
    char * value_buffer = (char*) malloc (value_req_sz);
    value_req_sz = jerry_api_string_to_char_buffer (args_p[0].v_string, (jerry_api_char_t *) value_buffer, value_req_sz);
    value_buffer[value_req_sz] = '\0';

    uint32_t size = (uint32_t)strlen(value_buffer);
    uint8_t digest[64] = {0};

    hal_sha1_context_t sha1_context;
    hal_sha1_init(&sha1_context);
    hal_sha1_append(&sha1_context, value_buffer, size);
    hal_sha1_end(&sha1_context, digest);

    uint8_t i;
    char str_buffer [50];
    strcpy(str_buffer, "");

    for (i = 0; i < HAL_SHA1_DIGEST_SIZE; i++) {
      if (i % 16 == 0) {
          printf("\r\n");
      }
      char buffer [2];
      sprintf (buffer, "%02x", digest[i]);
      strcat(str_buffer, buffer);
    }

    jerry_api_string_t *result = jerry_api_create_string(str_buffer);
    ret_val_p->type = JERRY_API_DATA_TYPE_STRING;
    ret_val_p->v_string = result;
    free(value_buffer);
    return true;
  }
}

void ml_sha_init(void) {
  REGISTER_HANDLER(__sha);
}