#include <pebble.h>
  
static Window *s_main_window;
static TextLayer *s_time_layer;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  static char buffer[] = "Your milkshake will hopefully be coming in a short while.";
		
	int hour = tick_time->tm_hour;
	int mins = tick_time->tm_min;
	
	if (hour >= 12 && hour < 23) {
      int hh_left = 24 - hour;
      char hours_text[6];
      hh_left == 1 ? strncpy(hours_text, "hour", sizeof("hour")) : strncpy(hours_text, "hours", sizeof("hours"));
      snprintf(buffer, 33, "%d %s to midnight milkshakes.", hh_left, hours_text);
	} else if (hour >= 23) {
      int mm_left = 60 - mins;
      char mins_text[5];
      mm_left == 1 ? strncpy(mins_text, "min", sizeof("min")) : strncpy(mins_text, "mins", sizeof("mins"));
      snprintf(buffer, 37, "Only %d %s to midnight milkshakes!", mm_left, mins_text);
	} else if (hour == 0) {
      int mm_left = (120 - ((hour * 60) + mins)) % 60;
      char mins_text[5];
      mm_left == 1 ? strncpy(mins_text, "min", sizeof("min")) : strncpy(mins_text, "mins", sizeof("mins"));
      snprintf(buffer, 50, "You have 1 hour, %d %s to get your milkshake.", mm_left, mins_text);
	} else if (hour == 1) {
      int mm_left = (120 - ((hour * 60) + mins)) % 60;
      char mins_text[5];
      mm_left == 1 ? strncpy(mins_text, "min", sizeof("min")) : strncpy(mins_text, "mins", sizeof("mins"));
      snprintf(buffer, 40, "You have %d %s to get your milkshake!", mm_left, mins_text);
	} else {
      snprintf(buffer, 38, "I hope you got your milkshake by now.");
	}
	
  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
}

static void main_window_load(Window *window) {
  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(10, 55, 124, 80));
  text_layer_set_background_color(s_time_layer, GColorBlack);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_overflow_mode(s_time_layer, GTextOverflowModeWordWrap);

  // Improve the layout to be more like a watchface
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentLeft);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  
  // Make sure the time is displayed from the start
  update_time();
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}
  
static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();
  window_set_background_color(s_main_window, GColorBlack);

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
