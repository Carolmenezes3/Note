#include <pebble.h>

Window *my_window;
TextLayer *text_layer;
static char note_text[255] = "";
static int note_to_display=0;

#define START_NOTE_IDX 1
#define NUM_NOTES 5

void in_received_handler(DictionaryIterator* received, void* context){
  for(int i = 0; i < NUM_NOTES; i++){
    // get each note
    Tuple* text_tuple = dict_find(received, i);
    if(text_tuple){
      // do not update the note if the value is empty string
      if(strcmp(text_tuple->value->cstring, "") != 0){
        persist_write_string(START_NOTE_IDX+i, text_tuple->value->cstring);
        if(i == note_to_display){
          strcpy(note_text, text_tuple->value->cstring);
          text_layer_set_text(text_layer, note_text); 
        }
      }
    }   
  }
}

void show_note(int i){
  if(i<0 || i>=NUM_NOTES){
    return;
  }
  note_to_display = i;
  if(persist_exists(START_NOTE_IDX+i)){
    persist_read_string(START_NOTE_IDX+i, note_text, 255);
  } else {
    strcpy(note_text, "<empty>");
  }
  text_layer_set_text(text_layer, note_text);
}

void next_note(){
  show_note((note_to_display+1)%NUM_NOTES);
}

void prev_note(){
  show_note((NUM_NOTES+note_to_display-1)%NUM_NOTES);
}

void show_next_note_click(ClickRecognizerRef recognizer, void* context){
  next_note();
}

void show_prev_note_click(ClickRecognizerRef recognizer, void* context){
  prev_note();
}

void config_provider(void* context){
  window_single_click_subscribe(BUTTON_ID_DOWN, show_next_note_click);
  window_single_click_subscribe(BUTTON_ID_UP, show_prev_note_click);
}

void handle_init(void) {
  my_window = window_create();

  window_stack_push(my_window, true);
  window_set_click_config_provider(my_window, config_provider);
  Layer *window_layer = window_get_root_layer(my_window);
  GRect bounds = layer_get_frame(window_layer);
  text_layer = text_layer_create((GRect){ .origin = { 0, 30 }, .size = bounds.size });
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
  
  show_note(0);
}

void handle_deinit(void) {
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
