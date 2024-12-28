#include "homm3lua_h3m.h"

void h3m_difficulty_set(h3mlib_ctx_t ctx, uint8_t difficulty) {
  ctx->h3m.bi.any.difficulty = difficulty;
}

int h3m_object_set_does_not_grow (h3mlib_ctx_t ctx, int od_index, int does_not_grow) {
  struct  H3M_OD_ENTRY  *h3m_od_entry = &ctx-> h3m.od.entries[od_index];
  struct META_OD_ENTRY *meta_od_entry = &ctx->meta.od_entries[od_index];

  struct H3M_OD_BODY_STATIC_MONSTER *body = (struct H3M_OD_BODY_STATIC_MONSTER *)h3m_od_entry->body;

  if (META_OBJECT_MONSTER != meta_od_entry->oa_type) {
      return 1;
  }

  body->does_not_grow = does_not_grow;

  return 0;
}

int h3m_object_set_message (h3mlib_ctx_t ctx, int od_index, const char *message, uint32_t size) {
  struct  H3M_OD_ENTRY  *h3m_od_entry = &ctx-> h3m.od.entries[od_index];
  struct META_OD_ENTRY *meta_od_entry = &ctx->meta.od_entries[od_index];

  if (META_OBJECT_SIGN != meta_od_entry->oa_type) {
      return 1;
  }

  struct H3M_OD_BODY_DYNAMIC_MESSAGE_BEARER *body = (struct H3M_OD_BODY_DYNAMIC_MESSAGE_BEARER *)h3m_od_entry->body;

  body->mesg_size = size + 1;
  body->mesg = (uint8_t *)strdup(message);
  body->unknown1[0] = 0;
  body->unknown1[1] = 0;
  body->unknown1[2] = 0;
  body->unknown1[3] = 0;

  meta_od_entry->binary_compatible = 0;
  meta_od_entry->body_size = sizeof(*body);

  META_OBJECT_PUSH_PTR(meta_od_entry->dyn_pointers, body, body->mesg, body->mesg_size, 0)

  return 0;
}

int h3m_object_set_never_flees (h3mlib_ctx_t ctx, int od_index, int never_flees) {
  struct  H3M_OD_ENTRY  *h3m_od_entry = &ctx-> h3m.od.entries[od_index];
  struct META_OD_ENTRY *meta_od_entry = &ctx->meta.od_entries[od_index];

  struct H3M_OD_BODY_STATIC_MONSTER *body = (struct H3M_OD_BODY_STATIC_MONSTER *)h3m_od_entry->body;

  if (META_OBJECT_MONSTER != meta_od_entry->oa_type) {
      return 1;
  }

  body->never_flees = never_flees;

  return 0;
}
