#include "pebble.h"
#include "resources.h"

///////////////////////////////////////////////////////////////////////////////
/*                                UI VARIABLES                               */
///////////////////////////////////////////////////////////////////////////////
// Windows.  
static Window *main_window; // Main.
static Window *program_window; // Training program.
static Window *timer_window; // Timer.

// Three lines of text for the timer window.
static TextLayer *tw_tl_time; // Time.
static TextLayer *tw_tl_type; // Type of period.
static TextLayer *tw_tl_interval; // Period count.

// Stores string for the timer screen.
char interval_value[20];
char timer_value[10];

// Program menu currently visible.
static struct menu_item *selected_program_menu;

// Menu layers.
static SimpleMenuLayer *main_menu_layer;
static SimpleMenuLayer *program_menu_layer;

// A simple menu layer can have multiple sections.
static SimpleMenuSection main_menu_sections[1];
static SimpleMenuSection program_menu_sections[1];

// Main menu: shows C25K and B210K options.
static SimpleMenuItem main_menu_items[MAX_MENU_ITEMS];
static SimpleMenuItem program_menu_items[MAX_MENU_ITEMS];

///////////////////////////////////////////////////////////////////////////////
/*                                GLOBALS                                    */
///////////////////////////////////////////////////////////////////////////////
bool timer_paused = false; // Is timer paused?

// Control block is a structure that keeps track of all variables related to
// the operation of the timer screen. There is a set of functions that is
// used to abstract apps interaction with it and encapsulate the logic.
struct control_block cb = {.loop_first = false}; // Empty control block.

///////////////////////////////////////////////////////////////////////////////
/*                          FUNCTION DECLARATIONS                            */
///////////////////////////////////////////////////////////////////////////////
static void handle_tick(struct tm *tick_time, TimeUnits units_changed);

///////////////////////////////////////////////////////////////////////////////
/*                            CONTROL BLOCK MANAGEMENT                       */
///////////////////////////////////////////////////////////////////////////////

// Initialize control block.
static void control_block_init(struct interval *current_interval, char *program_title, int intervals_total, bool loop_first){
  cb.current_interval = current_interval;
  cb.program_title = program_title;
  cb.current_interval_index = 0;
  cb.current_interval_seconds_left = cb.current_interval->duration;
  cb.loop_first = loop_first;

  // If we loop the first interval (for intervals mode), we make
  // the total number of itervals to be a 100.
  if (!loop_first){
    cb.intervals_total = intervals_total;
    cb.intervals_left = intervals_total;
  } else {
    cb.intervals_total = 100;
    cb.intervals_left = 100;
  }

  cb.is_interval_over = false;
  cb.is_program_over = false;
}

// Prints the status of the control block.
static void control_block_log_status(){
  APP_LOG(APP_LOG_LEVEL_DEBUG, "======================== CB STR ========================");
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Intervals: \tTotal\tCurrent\tLeft");
  APP_LOG(APP_LOG_LEVEL_DEBUG, "\t\t%d\t%d\t%d", cb.intervals_total, cb.current_interval_index, cb.intervals_left);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Seconds left: %d", cb.current_interval_seconds_left);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Is first interval looped: %d", cb.loop_first);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Is interval over: %d", cb.is_interval_over);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Is program over: %d", cb.is_interval_over);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "======================== CB END ========================");
}

// Get Program's Title.
static char* control_block_get_program_title(){
  return cb.program_title;
}

// Get number of seconds left in the current interval.
static int control_block_get_interval_seconds_left(){
  return cb.current_interval_seconds_left;
}

// Get type of the interval (walk, run).
static int control_block_get_interval_type(){
  return cb.current_interval -> type;
}

// Get the number of intervals legft in the program.
static int control_block_get_intervals_left(){
  return cb.intervals_left;
}

// Get the total number of intervals in the program.
static int control_block_get_intervals_total(){
  return cb.intervals_total;
}

// Advance to the next period.
static void control_block_next_period(){
  if (cb.intervals_left > 1){

    cb.intervals_left--;
    
    // If we are looping through the first
    // interval in the program, we don't need
    // to advance to the next interval in
    // the structure.
    if (!cb.loop_first){
      cb.current_interval++;
    }

    cb.current_interval_index++;

    // Adding one second because timer_callback starts by subtracting one.
    cb.current_interval_seconds_left = cb.current_interval->duration + 1;
  }
}

// Go back to the previos period.
static void control_block_previous_period(){
  if (cb.intervals_left < cb.intervals_total){
    cb.intervals_left++;
    
    // If we are looping through the first
    // interval in the program, we don't need
    // to go back to the previous interval in
    // the structure.
    if (!cb.loop_first){
      cb.current_interval--;
    }
    cb.current_interval_index--;
  } 

  // Adding one second because timer_callback starts by subtracting one.
  cb.current_interval_seconds_left = cb.current_interval->duration + 1;
}

// Updated the control block to handle a "tick".
static void control_block_tick(){
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Timer ticked, control state BEFORE:");
  control_block_log_status();

  cb.current_interval_seconds_left--;

  if (cb.current_interval_seconds_left == 0){
    cb.is_interval_over = true;

    // Check if the program has more periods.
    if (--cb.intervals_left > 0){
        
        // Unsubscrive as we are switching.
        tick_timer_service_unsubscribe();

        // Move to the next period.
        if (!cb.loop_first){
          cb.current_interval++;
          cb.current_interval_index++;
        }

        // Get seconds in the current period.
        cb.current_interval_seconds_left = cb.current_interval->duration;

        APP_LOG(APP_LOG_LEVEL_DEBUG, "Next interval of the program, duration: %d", cb.current_interval_seconds_left);  
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Intervals left %d", cb.intervals_left);
        
        // Resubscribe.
        tick_timer_service_subscribe(SECOND_UNIT, handle_tick);

    } else {
      cb.is_program_over = true;
      tick_timer_service_unsubscribe();
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Completed the program!");  
    }
  } else {
    cb.is_interval_over = false;
  }

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Timer ticked, control state AFTER:");
  control_block_log_status();
}

// Returns true if the interval is over.
static bool control_block_is_interval_over(){
  return cb.is_interval_over;
}

// Returns true if the program is over.
static bool control_block_is_program_over(){
  return cb.is_program_over;
}


///////////////////////////////////////////////////////////////////////////////
/*                              UTILITY FUNCTIONS                            */
///////////////////////////////////////////////////////////////////////////////
// Builds menu sections and items from a structure in resources.h
static void build_menu_from_struc(struct menu_item *menu, int menu_size, SimpleMenuSection *menu_sections, SimpleMenuItem *menu_items, void *callback){
  APP_LOG(APP_LOG_LEVEL_DEBUG, "build_menu_from_struc: drawing menu layer. Menu size = %d", menu_size);  

  for(int i=0; i < menu_size; i++){
    APP_LOG(APP_LOG_LEVEL_DEBUG, "build_menu_from_struc: adding an item %d: '%s'", i, menu[i].title);

    menu_items[i] = (SimpleMenuItem){
      .title = menu[i].title,
      .subtitle = menu[i].subtitle,
      .callback = callback,
    };
  }

  menu_sections[0] = (SimpleMenuSection){
    .num_items = menu_size,
    .items = menu_items,
  };  

  APP_LOG(APP_LOG_LEVEL_DEBUG, "build_menu_from_struc: finished drawing a menu.");
}


static void draw_timer(){
  // Get seconds and minutes.  
  int m = control_block_get_interval_seconds_left() / 60;
  int s = control_block_get_interval_seconds_left() - (m * 60);

  // Update timer.
  snprintf(timer_value, sizeof(timer_value) + 1, "%02d:%02d", m, s);
  text_layer_set_text(tw_tl_time, timer_value); 
  layer_mark_dirty((Layer *)tw_tl_time);

  // Update type.
  if (control_block_get_interval_type() == INTERVAL_TYPE_RUN){
    text_layer_set_text(tw_tl_type, MESSAGE_RUN); 
  } else if (control_block_get_interval_type() == INTERVAL_TYPE_WALK) {
    text_layer_set_text(tw_tl_type, MESSAGE_WALK); 
  } else if (control_block_get_interval_type() == INTERVAL_TYPE_WARMUP) {
    text_layer_set_text(tw_tl_type, MESSAGE_WARMUP);
  } else if (control_block_get_interval_type() == INTERVAL_TYPE_COOLDOWN) {
    text_layer_set_text(tw_tl_type, MESSAGE_COOLDOWN);
  } else {
    text_layer_set_text(tw_tl_type, control_block_get_program_title()); 
  }
  layer_mark_dirty((Layer *)tw_tl_type);

  // Update period.
  snprintf(interval_value, sizeof(interval_value) + 1, MESSAGE_PERIOD, (control_block_get_intervals_total()-control_block_get_intervals_left()) + 1, control_block_get_intervals_total());
  text_layer_set_text(tw_tl_interval, interval_value); 
  layer_mark_dirty((Layer *)tw_tl_interval);
}

// Handles tick of the system clock.
static void handle_tick(struct tm *tick_time, TimeUnits units_changed) {
    if(units_changed & SECOND_UNIT) {

      // Update control block.
      control_block_tick();
    
      // Redraw the timer.
      draw_timer();
    
      // Update UI.
      if (control_block_is_program_over()){
          // Vibrate with different pattern at the end of the program.
          vibes_double_pulse();
    
          // And update the screen with a final message.
          layer_set_hidden((Layer *)tw_tl_time, true);
          layer_set_hidden((Layer *)tw_tl_interval, true);
    
          text_layer_set_text(tw_tl_type, "Done!");
          layer_mark_dirty((Layer *)tw_tl_type);
    
          return;
      } else if (control_block_is_interval_over()){
        vibes_long_pulse();
      }
    }  
}

///////////////////////////////////////////////////////////////////////////////
/*                              BUTTON CLICK HANDLERS                        */
///////////////////////////////////////////////////////////////////////////////
// Down click on timer window.
void down_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
  // Go to the next period, if this is the last period, don't do anything.
  if (control_block_get_intervals_left() > 1){
    // Stop timer.
    tick_timer_service_unsubscribe();
    
    control_block_next_period();
    
    // Start the timer.
    tick_timer_service_subscribe(SECOND_UNIT, handle_tick);
  }
}

// Up click on timer window.
void up_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
  // Stop timer.
  tick_timer_service_unsubscribe();
  
  control_block_previous_period();

  // Start the timer.
  tick_timer_service_subscribe(SECOND_UNIT, handle_tick);
}

// Select click on timer window.
void select_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
  if (timer_paused){
    // Start the timer.
    tick_timer_service_subscribe(SECOND_UNIT, handle_tick);
  } else {
    // Stop timer, to pause.
    tick_timer_service_unsubscribe();
  }
  timer_paused = !timer_paused;
}

void click_config_provider(Window *window) {
  window_single_click_subscribe(BUTTON_ID_UP, (ClickHandler) up_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, (ClickHandler) down_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, (ClickHandler) select_single_click_handler);
}

///////////////////////////////////////////////////////////////////////////////
/*                                        UI                                 */
///////////////////////////////////////////////////////////////////////////////

// This callback will initialize the timer and starts the count down.
static void program_menu_callback(int index, void *ctx) {
  
  if (selected_program_menu -> program_menu == interval_menu){
    control_block_init(((selected_program_menu -> program_menu[index])).intervals, ((selected_program_menu -> program_menu[index])).title, ((selected_program_menu -> program_menu[index])).intervals_cnt, true);
  } else {
    control_block_init(((selected_program_menu -> program_menu[index])).intervals, ((selected_program_menu -> program_menu[index])).title, ((selected_program_menu -> program_menu[index])).intervals_cnt, false);
  }

  // Add the timer window to the stack.
  window_stack_push(timer_window, true);

  // Log the status of the control block.
  control_block_log_status();

  // Set up timer screen.  
  draw_timer();

  // Start ticking.
  tick_timer_service_subscribe(SECOND_UNIT, handle_tick);
 
}
// This callback will remove the main menu layer and draw the program menu instead.
static void main_menu_callback(int index, void *ctx) {
  selected_program_menu = &main_menu[index];
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Selected '%s' program menu.", selected_program_menu -> title);  

  Layer *window_layer = window_get_root_layer(program_window);

  build_menu_from_struc(selected_program_menu->program_menu, selected_program_menu->program_menu_size, program_menu_sections, program_menu_items, program_menu_callback);

  // The order of the next two statements are important.
  window_stack_push(program_window, true);

  layer_add_child(window_layer, simple_menu_layer_get_layer(program_menu_layer)); 
}

// Main window is a menu window.
static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);
  main_menu_layer = simple_menu_layer_create(bounds, window, main_menu_sections, 1, NULL);

  // Load main menu.
  build_menu_from_struc(main_menu, sizeof(main_menu) / sizeof(struct menu_item), main_menu_sections, main_menu_items, main_menu_callback);

  // Add the prepared layer to the screen.
  layer_add_child(window_layer, (Layer *)main_menu_layer);

  // Note: It's beyond me at the moment why I need to do that, considering
  // I don't need to call menu_layer_reload_date for other menus and
  // I am drawing this menu only once, but it fixes the scrolling issue.
  menu_layer_reload_data(simple_menu_layer_get_menu_layer(main_menu_layer));
}

// Deinitialize resources on window unload that were initialized on window load
void main_window_unload(Window *window) {
  simple_menu_layer_destroy(main_menu_layer);
}

// Program window is another menu window.
void program_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);
  program_menu_layer = simple_menu_layer_create(bounds, window, program_menu_sections, 1, NULL);
}

void program_window_unload(Window *window) {
  simple_menu_layer_destroy(program_menu_layer);
}

// Timer window contains three text layers that are setup here.
void timer_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(timer_window);
  GRect bounds = layer_get_frame(window_layer);

  // Setup time text.
  tw_tl_time = text_layer_create((GRect){ .origin = { 0, 10 }, .size = bounds.size });
  text_layer_set_text(tw_tl_time, "00:00");
  text_layer_set_font(tw_tl_time, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
  text_layer_set_text_alignment(tw_tl_time, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(tw_tl_time));

  // Setup action type text.
  tw_tl_type = text_layer_create((GRect){ .origin = { 0, 54 }, .size = bounds.size });
  text_layer_set_text(tw_tl_type, "Walk");
  text_layer_set_font(tw_tl_type, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(tw_tl_type, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(tw_tl_type));

  // Setup period count text.
  tw_tl_interval = text_layer_create((GRect){ .origin = { 0, 106 }, .size = bounds.size });
  text_layer_set_text(tw_tl_interval, "period 1 of 6");
  text_layer_set_font(tw_tl_interval, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text_alignment(tw_tl_interval, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(tw_tl_interval));
}

void timer_window_unload(Window *window) {
  tick_timer_service_unsubscribe();

  text_layer_destroy(tw_tl_time);
  text_layer_destroy(tw_tl_type);
  text_layer_destroy(tw_tl_interval);
}

int main(void) {

  // Create windows and setup handlers.
  main_window = window_create();
  program_window = window_create();
  timer_window = window_create();


  window_set_window_handlers(main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });

  window_set_window_handlers(program_window, (WindowHandlers) {
    .load = program_window_load,
    .unload = program_window_unload,
  });

  window_set_window_handlers(timer_window, (WindowHandlers) {
    .load = timer_window_load,
    .unload = timer_window_unload,
  });
  window_set_click_config_provider(timer_window, (ClickConfigProvider) click_config_provider);

  // Push main window.
  window_stack_push(main_window, true);

  app_event_loop();

  // Destroy Windows.
  window_destroy(main_window);
  window_destroy(program_window);
  window_destroy(timer_window);
}