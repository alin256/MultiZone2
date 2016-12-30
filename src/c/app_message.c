#include <pebble.h>

Window *window;	
TextLayer *main_text;
	
// Key values for AppMessage Dictionary
enum {
	STATUS_KEY = 0,	
	MESSAGE_KEY = 1,
  ICON_KEY = 2,
  TEMPERATURE_KEY = 3
};

// Write message to buffer & send
void send_message(void){
	DictionaryIterator *iter;
	
	app_message_outbox_begin(&iter);
	dict_write_uint8(iter, STATUS_KEY, 0x1);
	
	dict_write_end(iter);
  text_layer_set_text(main_text, "MSG composed");
  app_message_outbox_send();
}

// Called when a message is received from PebbleKitJS
static void in_received_handler(DictionaryIterator *received, void *context) {
	Tuple *tuple;
	text_layer_set_text(main_text, "recieved");
  //layer_mark_dirty(text_layer_get_layer(main_text));
  
	tuple = dict_find(received, STATUS_KEY);
	if(tuple) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Status: %d", (int)tuple->value->uint32); 
	}
	
	tuple = dict_find(received, MESSAGE_KEY);
	if(tuple) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Message: %s", tuple->value->cstring);
    text_layer_set_text(main_text, tuple->value->cstring);
    //layer_mark_dirty(text_layer_get_layer(main_text));
	}

  //tuple = dict_find(received, ICON_KEY);
  //if(tuple)
  //{
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Status: %s", tuple->value->cstring); 
  //}
  
  //tuple = dict_find(received, TEMPERATURE_KEY);
  //if(tuple)
  //{
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Status: %s", tuple->value->cstring); 
    //text_layer_set_text(main_text, tuple->value->cstring);
  //}
  
}

// Called when an incoming message from PebbleKitJS is dropped
static void in_dropped_handler(AppMessageResult reason, void *context) {	
}

// Called when PebbleKitJS does not acknowledge receipt of a message
static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
}


static void main_window_load(Window *window) {
  // Create Window's child Layers here
  // Get the root layer
  Layer *window_layer = window_get_root_layer(window);

  // Get the bounds of the window for sizing the text layer
  GRect bounds = layer_get_bounds(window_layer);

  // Create and Add to layer hierarchy:
  main_text = text_layer_create(GRect(5, 5, bounds.size.w, 30));
  text_layer_set_text(main_text, "This is TextLayer");
  layer_add_child(window_layer, text_layer_get_layer(main_text));
}

static void main_window_unload(Window *window) {
  // Destroy Window's child Layers here
  text_layer_destroy(main_text);
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(main_text, "Down pressed!");
  send_message();
}

static void click_config_provider(void *context) {
  // Register the ClickHandlers
  //window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  //window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}


void init(void) {
	window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_set_click_config_provider(window, click_config_provider);
	window_stack_push(window, true);
	
  
	// Register AppMessage handlers
	app_message_register_inbox_received(in_received_handler); 
	app_message_register_inbox_dropped(in_dropped_handler); 
	app_message_register_outbox_failed(out_failed_handler);
		
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  send_message();
}

void deinit(void) {
	app_message_deregister_callbacks();
	window_destroy(window);
}

int main( void ) {
	init();
	app_event_loop();
	deinit();
}