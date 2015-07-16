#include <pebble.h>
#include "gbitmap_color_palette_manipulator.h"
  
static Window *window;

bool bt_status = false;

BitmapLayer *btt_layer;
GBitmap *bt100_img;
GBitmap *bt80_img;
GBitmap *bt60_img;
GBitmap *bt40_img;
GBitmap *bt20_img;

static GBitmap *BG_image;
static BitmapLayer *BG_layer;

const int HOUR_IMAGE_RESOURCE_IDS[] = {
  RESOURCE_ID_0h,
  RESOURCE_ID_1h,
  RESOURCE_ID_2h,
  RESOURCE_ID_3h,
  RESOURCE_ID_4h,
  RESOURCE_ID_5h,
  RESOURCE_ID_6h,
  RESOURCE_ID_7h,
  RESOURCE_ID_8h,
  RESOURCE_ID_9h,
  RESOURCE_ID_10h,
  RESOURCE_ID_11h
};
#define TOTAL_HOUR_DIGITS 1
static GBitmap *time_digits_images[TOTAL_HOUR_DIGITS];
static BitmapLayer *time_digits_layers[TOTAL_HOUR_DIGITS];

const int MINa_IMAGE_RESOURCE_IDS[] = {
  RESOURCE_ID_0a,
  RESOURCE_ID_1a,
  RESOURCE_ID_2a,
  RESOURCE_ID_3a,
  RESOURCE_ID_4a,
  RESOURCE_ID_5a
};
#define TOTAL_MINa_DIGITS 1
static GBitmap *mina_digits_images[TOTAL_MINa_DIGITS];
static BitmapLayer *mina_digits_layers[TOTAL_MINa_DIGITS];

const int MINb_IMAGE_RESOURCE_IDS[] = {
  RESOURCE_ID_0b,
  RESOURCE_ID_1b,
  RESOURCE_ID_2b,
  RESOURCE_ID_3b,
  RESOURCE_ID_4b,
  RESOURCE_ID_5b,
  RESOURCE_ID_6b,
  RESOURCE_ID_7b,
  RESOURCE_ID_8b,
  RESOURCE_ID_9b
};
#define TOTAL_MINb_DIGITS 1
static GBitmap *minb_digits_images[TOTAL_MINb_DIGITS];
static BitmapLayer *minb_digits_layers[TOTAL_MINb_DIGITS];

const int DAY_NAME_IMAGE_RESOURCE_IDS[] = {
  RESOURCE_ID_0w,
  RESOURCE_ID_1w,
  RESOURCE_ID_2w,
  RESOURCE_ID_3w,
  RESOURCE_ID_4w,
  RESOURCE_ID_5w,
  RESOURCE_ID_6w
};
static GBitmap *day_name_image;
static BitmapLayer *day_name_layer;

const int DATENUM_IMAGE_RESOURCE_IDS[] = {
  RESOURCE_ID_0d,
  RESOURCE_ID_1d,
  RESOURCE_ID_2d,
  RESOURCE_ID_3d,
  RESOURCE_ID_4d,
  RESOURCE_ID_5d,
  RESOURCE_ID_6d,
  RESOURCE_ID_7d,
  RESOURCE_ID_8d,
  RESOURCE_ID_9d
};
#define TOTAL_DATE_DIGITS 2
static GBitmap *date_digits_images[TOTAL_DATE_DIGITS];
static BitmapLayer *date_digits_layers[TOTAL_DATE_DIGITS];

static void set_container_image(GBitmap **bmp_image, BitmapLayer *bmp_layer, const int resource_id, GPoint origin, GColor *color) {
  GBitmap *old_image = *bmp_image;
  
  *bmp_image = gbitmap_create_with_resource(resource_id);
  GRect frame = (GRect) {
    .origin = origin,
    .size = gbitmap_get_bounds(*bmp_image).size
  };
  bitmap_layer_set_bitmap(bmp_layer, *bmp_image);
  bitmap_layer_set_compositing_mode(bmp_layer, GCompOpSet);
  if(color != NULL) 
    replace_gbitmap_color(GColorBlack, *color, *bmp_image, bmp_layer);
  //bitmap_layer_set_background_color(bmp_layer, GColorJazzberryJam);
  
  layer_set_frame(bitmap_layer_get_layer(bmp_layer), frame);

  if (old_image != NULL) {
  	gbitmap_destroy(old_image);
  }
}

static void update_display(struct tm *current_time){
  set_container_image(&day_name_image, day_name_layer, DAY_NAME_IMAGE_RESOURCE_IDS[current_time->tm_wday], GPoint(97, 90), NULL);
  
  set_container_image(&date_digits_images[0], date_digits_layers[0], DATENUM_IMAGE_RESOURCE_IDS[current_time->tm_mday/10], GPoint(91, 74), NULL);
  set_container_image(&date_digits_images[1], date_digits_layers[1], DATENUM_IMAGE_RESOURCE_IDS[current_time->tm_mday%10], GPoint(106, 74), NULL);
  
  set_container_image(&time_digits_images[0], time_digits_layers[0], HOUR_IMAGE_RESOURCE_IDS[current_time->tm_hour%12], GPoint(11, 0), &GColorFolly);
  set_container_image(&mina_digits_images[0], mina_digits_layers[0], MINa_IMAGE_RESOURCE_IDS[current_time->tm_min/10], GPoint(50, 26), &GColorFolly);
  set_container_image(&minb_digits_images[0], minb_digits_layers[0], MINb_IMAGE_RESOURCE_IDS[current_time->tm_min%10], GPoint(62, 39), &GColorFolly);
}


void handle_battery(BatteryChargeState charge_state)
{
  static char battery_text[] = "100";
  
  if (charge_state.is_charging)
  {
    bitmap_layer_set_bitmap(btt_layer, bt100_img);
    replace_gbitmap_color(GColorBlack, GColorMediumSpringGreen, bt100_img, btt_layer);
  }
  else
  {
    snprintf(battery_text, sizeof(battery_text), "%d", charge_state.charge_percent);
    if (charge_state.charge_percent <= 100){
      bitmap_layer_set_bitmap(btt_layer, bt100_img);
      replace_gbitmap_color(GColorBlack, GColorBrightGreen, bt100_img, btt_layer);
    }
    if (charge_state.charge_percent <= 80){
      bitmap_layer_set_bitmap(btt_layer, bt80_img);
      replace_gbitmap_color(GColorBlack, GColorInchworm, bt80_img, btt_layer);
    }
    if (charge_state.charge_percent <= 60){
      bitmap_layer_set_bitmap(btt_layer, bt60_img);
      replace_gbitmap_color(GColorBlack, GColorIcterine, bt60_img, btt_layer);
    }
    if (charge_state.charge_percent <= 40){
      bitmap_layer_set_bitmap(btt_layer, bt40_img);
      replace_gbitmap_color(GColorBlack, GColorSunsetOrange, bt40_img, btt_layer);
    }
    if (charge_state.charge_percent <= 20){
      bitmap_layer_set_bitmap(btt_layer, bt20_img);
      replace_gbitmap_color(GColorBlack, GColorRed, bt20_img, btt_layer);
    }
  }
  bitmap_layer_set_compositing_mode(btt_layer, GCompOpSet);
}

void handle_bluetooth(bool connected) {
  if (!connected == 1 && bt_status)
  {
    layer_set_hidden(bitmap_layer_get_layer(btt_layer), true);
    vibes_long_pulse();
  } 
  else 
  {
    layer_set_hidden(bitmap_layer_get_layer(btt_layer), false);
  }
  bt_status = connected;
}

static void handle_timechanges(struct tm *tick_time, TimeUnits units_changed) {
  update_display(tick_time);
}

static void init(void){
  window = window_create();
  window_stack_push(window, true);
  
  BG_image = gbitmap_create_with_resource(RESOURCE_ID_BG);
  BG_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(BG_layer, BG_image);
  
  replace_gbitmap_color(GColorBlack, GColorBrightGreen, BG_image, BG_layer);
  bitmap_layer_set_compositing_mode(BG_layer, GCompOpSet);
  
  bitmap_layer_set_background_color(BG_layer, GColorBlack);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(BG_layer));
    
  btt_layer = bitmap_layer_create(GRect(108, 61, 23, 46));
  bt100_img = gbitmap_create_with_resource(RESOURCE_ID_100ba);
  bt80_img = gbitmap_create_with_resource(RESOURCE_ID_80ba);
  bt60_img = gbitmap_create_with_resource(RESOURCE_ID_60ba);
  bt40_img = gbitmap_create_with_resource(RESOURCE_ID_40ba);
  bt20_img = gbitmap_create_with_resource(RESOURCE_ID_20ba);
  bitmap_layer_set_compositing_mode(btt_layer, GCompOpSet);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(btt_layer));
  
  
  GRect dummy_frame = { {0, 0}, {0, 0} };
  
  day_name_layer = bitmap_layer_create(dummy_frame);
  bitmap_layer_set_compositing_mode((day_name_layer), GCompOpSet);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(day_name_layer));
  
  
  for (int i = 0; i < TOTAL_DATE_DIGITS; ++i) {
    date_digits_layers[i] = bitmap_layer_create(dummy_frame);
    layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(date_digits_layers[i]));
  }
  
  for (int i = 0; i < TOTAL_HOUR_DIGITS; ++i) {
    time_digits_layers[i] = bitmap_layer_create(dummy_frame);
    bitmap_layer_set_compositing_mode(time_digits_layers[i], GCompOpSet);
    layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(time_digits_layers[i]));
  }
  
  for (int i = 0; i < TOTAL_MINa_DIGITS; ++i) {
    mina_digits_layers[i] = bitmap_layer_create(dummy_frame);
    bitmap_layer_set_compositing_mode(mina_digits_layers[i], GCompOpSet);
    layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(mina_digits_layers[i]));
  }
  
  for (int i = 0; i < TOTAL_MINb_DIGITS; ++i) {
    minb_digits_layers[i] = bitmap_layer_create(dummy_frame);
    bitmap_layer_set_compositing_mode(minb_digits_layers[i], GCompOpSet);
    layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(minb_digits_layers[i]));
  }
  
  
  time_t now = time(NULL);
  handle_timechanges(localtime(&now), SECOND_UNIT);
  tick_timer_service_subscribe(SECOND_UNIT, handle_timechanges);
  
  handle_battery(battery_state_service_peek());
  battery_state_service_subscribe(&handle_battery);
  
  bool connected = bluetooth_connection_service_peek();
  handle_bluetooth(connected);
  bluetooth_connection_service_subscribe(&handle_bluetooth);
}

static void deinit(void) {
  window_destroy(window);
  bitmap_layer_destroy(BG_layer);
  gbitmap_destroy(BG_image);
  
  bitmap_layer_destroy(btt_layer);
  gbitmap_destroy(bt100_img);
  gbitmap_destroy(bt80_img);
  gbitmap_destroy(bt60_img);
  gbitmap_destroy(bt40_img);
  gbitmap_destroy(bt20_img);
  
  layer_remove_from_parent(bitmap_layer_get_layer(day_name_layer));
  bitmap_layer_destroy(day_name_layer);
  gbitmap_destroy(day_name_image);
  
  //layer_remove_from_parent(bitmap_layer_get_layer(month_layer));
  //bitmap_layer_destroy(month_layer);
  //gbitmap_destroy(month_image);
  
  for (int i = 0; i < TOTAL_DATE_DIGITS; i++) {
    layer_remove_from_parent(bitmap_layer_get_layer(date_digits_layers[i]));
    gbitmap_destroy(date_digits_images[i]);
    bitmap_layer_destroy(date_digits_layers[i]);
  }
  for (int i = 0; i < TOTAL_HOUR_DIGITS; i++) {
    layer_remove_from_parent(bitmap_layer_get_layer(time_digits_layers[i]));
    gbitmap_destroy(time_digits_images[i]);
    bitmap_layer_destroy(time_digits_layers[i]);
  }
  
  for (int i = 0; i < TOTAL_MINa_DIGITS; i++) {
    layer_remove_from_parent(bitmap_layer_get_layer(mina_digits_layers[i]));
    gbitmap_destroy(mina_digits_images[i]);
    bitmap_layer_destroy(mina_digits_layers[i]);
  }
  
  for (int i = 0; i < TOTAL_MINb_DIGITS; i++) {
    layer_remove_from_parent(bitmap_layer_get_layer(minb_digits_layers[i]));
    gbitmap_destroy(minb_digits_images[i]);
    bitmap_layer_destroy(minb_digits_layers[i]);
  }
  
  tick_timer_service_unsubscribe();
  battery_state_service_unsubscribe();
  bluetooth_connection_service_unsubscribe();
}


int main(void) {
  init();
  app_event_loop();
  deinit();
}