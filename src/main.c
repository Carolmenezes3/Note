#include <pebble.h>

Window *my_window;
TextLayer *text_layer;
static char note_text[255] = "";

#define NOTE_INDEX 55

void in_received_handler(DictionaryIterator* received, void* context){
  Tuple* text_tuple = dict_find(received, 0);
  if(text_tuple){
    strcpy(note_text, text_tuple->value->cstring);
    persist_write_string(NOTE_INDEX, note_text);
    text_layer_set_text(text_layer, note_text); 
  } else {
    text_layer_set_text(text_layer, "empty"); 
  }
}

void handle_init(void) {
  my_window = window_create();

  window_stack_push(my_window, true);
  Layer *window_layer = window_get_root_layer(my_window);
  GRect bounds = layer_get_frame(window_layer);
  text_layer = text_layer_create((GRect){ .origin = { 0, 30 }, .size = bounds.size });
  if(persist_exists(NOTE_INDEX)){
    int x = persist_read_string(NOTE_INDEX, note_text, 255);
    text_layer_set_text(text_layer, note_text);
  } else {
    text_layer_set_text(text_layer, "<none>");
  }
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

void handle_deinit(void) {
  persist_write_string(NOTE_INDEX, note_text);
  text_layer_destroy(text_layer);
  window_destroy(my_window);
}

int main(void) {
  handle_init();

  app_message_register_inbox_received(in_received_handler);
  const uint32_t inbound_size = 64;
  const uint32_t outbound_size = 64;
  app_message_open(inbound_size, outbound_size);

  app_event_loop();
  handle_deinit();
}
