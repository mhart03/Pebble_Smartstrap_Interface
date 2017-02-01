#include "pebble.h"

#define NUM_MENU_SECTIONS 2
#define NUM_FIRST_MENU_ITEMS 1
#define NUM_SECOND_MENU_ITEMS 1

static Window *s_main_window;
static SimpleMenuLayer *s_simple_menu_layer;
static SimpleMenuSection s_menu_sections[NUM_MENU_SECTIONS];
static SimpleMenuItem s_first_menu_items[NUM_FIRST_MENU_ITEMS];
static SimpleMenuItem s_second_menu_items[NUM_SECOND_MENU_ITEMS];
static GBitmap *s_menu_icon_image;

static bool door_lock_status = false;

static void menu_select_callback(int index, void *ctx) {
  s_second_menu_items[index].subtitle = "You've hit settings!";
  layer_mark_dirty(simple_menu_layer_get_layer(s_simple_menu_layer));

}

static void special_select_callback(int index, void *ctx) {
  // Of course, you can do more complicated things in a menu item select callback
  // Here, we have a simple toggle
  door_lock_status = !door_lock_status;

  SimpleMenuItem *menu_item = &s_first_menu_items[index];

  if (door_lock_status) {
    menu_item->subtitle = "Door Locked.";
  } else {
    menu_item->subtitle = "Door Unlocked.";
  }

  
  layer_mark_dirty(simple_menu_layer_get_layer(s_simple_menu_layer));
}

static void main_window_load(Window *window) {
  s_menu_icon_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MENU_ICON_1);

  // Although we already defined NUM_FIRST_MENU_ITEMS, you can define
  // an int as such to easily change the order of menu items later
  int num_a_items = 0;

  s_menu_sections[0] = (SimpleMenuSection) {
    .title = PBL_IF_RECT_ELSE("Commands", NULL),
    .num_items = NUM_FIRST_MENU_ITEMS,
    .items = s_first_menu_items,
  };
  
  s_first_menu_items[0] = (SimpleMenuItem) {
    .title = "Lock/Unlock Door",
    .callback = special_select_callback,
  };

  s_menu_sections[1] = (SimpleMenuSection) {
    .num_items = NUM_SECOND_MENU_ITEMS,
    .items = s_second_menu_items,
  };

  s_second_menu_items[num_a_items++] = (SimpleMenuItem) {
    .title = "Settings",
    .callback = menu_select_callback,
    .icon = PBL_IF_RECT_ELSE(s_menu_icon_image, NULL),
  };

  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  s_simple_menu_layer = simple_menu_layer_create(bounds, window, s_menu_sections, NUM_MENU_SECTIONS, NULL);

  layer_add_child(window_layer, simple_menu_layer_get_layer(s_simple_menu_layer));
}

void main_window_unload(Window *window) {
  simple_menu_layer_destroy(s_simple_menu_layer);
  gbitmap_destroy(s_menu_icon_image);
}

static void init() {
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_stack_push(s_main_window, true);
}

static void deinit() {
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
