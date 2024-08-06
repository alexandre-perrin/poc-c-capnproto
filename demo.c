#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "demo.capnp.h"

#include <capnp_c.h>

#define BUF_LEN (256)
#define KEY "zirigdum1"
#define VALUE "my string value"

static capn_text chars_to_text(const char *chars) {
  return (capn_text){
      .len = (int)strlen(chars),
      .str = chars,
      .seg = NULL,
  };
}

void hexdump(uint8_t *buf, size_t len) {
  size_t i;
  for (i = 0; i < len; i++) {
    if (!(i % 16)) {
      printf("%04jx: ", i);
    }
    printf("%02x ", buf[i]);
    if (i % 16 == 15 || i == len) {
      printf("\n");
    }
  }
  printf("\n");
}

int main(void) {
  uint8_t buffer[BUF_LEN] = {};
  int ret = 0;
  ssize_t sz;

  /* Serialize */
  {
    struct capn c;
    capn_init_malloc(&c);
    capn_ptr cr = capn_root(&c);
    struct capn_segment *cs = cr.seg;

    /* Initial data */
    struct TelemetryRequest req = {
        .key = chars_to_text(KEY),
        .value = chars_to_text(VALUE),
    };

    TelemetryRequest_ptr preq = new_TelemetryRequest(cs);
    write_TelemetryRequest(&req, preq);
    ret = capn_setp(cr, 0, preq.p);
    if (ret) {
      printf("capn_setp failed with %d\n", ret);
      return 1;
    }

    sz = capn_write_mem(&c, buffer, sizeof(buffer), 0);
    if (sz < 0) {
      printf("capn_write_mem failed with %zi\n", sz);
      return 1;
    }
    capn_free(&c);
    printf("Serialized output:\n");
  }

  hexdump(buffer, sz);

  /* Deserialize */
  {
    struct capn c;
    ret = capn_init_mem(&c, buffer, sz, 0);
    if (ret) {
      printf("capn_init_mem failed with %d\n", ret);
      return 1;
    }

    TelemetryRequest_ptr preq;
    struct TelemetryRequest req;
    preq.p = capn_getp(capn_root(&c), 0 /* off */, 1 /* resolve */);

    read_TelemetryRequest(&req, preq);

    if (memcmp(req.key.str, KEY, req.key.len)) {
      printf("Invalid key: %s\n", req.key.str);
      ret = 1;
    }

    if (memcmp(req.value.str, VALUE, req.value.len)) {
      printf("Invalid value: %s\n", req.value.str);
      ret = 1;
    }

    printf("Deserialized: key: '%s' value: '%s'\n", req.key.str, req.value.str);

    capn_free(&c);
  }
  return ret;
}