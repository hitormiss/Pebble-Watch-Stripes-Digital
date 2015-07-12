#include "stripe.h"
#include "pebble.h"
#include <ctype.h>

static Window *window;
static Layer *background_layer, *time_layer;
static GFont time_font;

static struct GPathInfo STRIPE_PATHINFO = {
		4,
		(GPoint []){ {0, 0}, {0, 0}, {0, 0}, {0, 0} }
	};

static GPath *STRIPE_PATH;

static void bg_update_proc(Layer *layer, GContext *ctx) {

  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);
	
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
	
	int INDEX = 0;
	if (ROTATE_FREQUENCY == 2) {
		int HOUR = (int)(t->tm_hour);
		if (HOUR > 12) HOUR = HOUR - 12;
		INDEX = HOUR * 5;
	}
	else if (ROTATE_FREQUENCY == 1)	INDEX = (int)(t->tm_min);
	else INDEX = (int)(t->tm_sec);
	
	int STRIPE_angle = 0;
	if (ROTATE == 1) STRIPE_angle = (TRIG_MAX_ANGLE * INDEX / 60);
	else if (ROTATE == 2) STRIPE_angle = TRIG_MAX_ANGLE - (TRIG_MAX_ANGLE * INDEX / 60);
	else {
		STRIPE_angle = (TRIG_MAX_ANGLE * STATIC_ANGLE / 360);
		INDEX = STATIC_ANGLE / 6;
	}
	
  GRect bounds = layer_get_bounds(layer);
  GPoint center = grect_center_point(&bounds);

	//graphics_context_set_antialiased(ctx, false);
	
	unsigned short Width = (unsigned short)(WINDOW_WIDTH[INDEX] / NUM_BARS);
	unsigned short OffSet = (unsigned short)(WINDOW_WIDTH[INDEX] / 2);
	int Start = 0;
	int End = 0;
	
	//struct GPathInfo STRIPE_PATHINFO = {
	//	4,
	//	(GPoint []){ {0, 0}, {0, 0}, {0, 0}, {0, 0} }
	//};
	
	char color_string[6]; 
	GColor8 stripe_color; 

  for (unsigned short i = 0; i < NUM_BARS; ++i) {
		Start = (i * Width) - OffSet;
		if(i==0) Start = Start-10;		
		End = ( (i+1) * Width) + 20 - OffSet;
		
		STRIPE_PATHINFO.points[0].y = Start;
		STRIPE_PATHINFO.points[0].x = 112;
		STRIPE_PATHINFO.points[1].y = Start;
		STRIPE_PATHINFO.points[1].x = -112;
		STRIPE_PATHINFO.points[2].y = End;
		STRIPE_PATHINFO.points[2].x = -112;
		STRIPE_PATHINFO.points[3].y = End;
		STRIPE_PATHINFO.points[3].x = 112;
	
		//color_string = 	HEXCOLOR_ARRAY[i];
		memcpy(color_string, HEXCOLOR_ARRAY[i], sizeof("000000"));
		stripe_color = GColorFromHEX(HexStringToUInt(color_string));
		
		graphics_context_set_stroke_color(ctx, stripe_color);
		graphics_context_set_fill_color(ctx, stripe_color);
		
		//GPath *STRIPE_PATH = gpath_create(&STRIPE_PATHINFO);
		STRIPE_PATH = gpath_create(&STRIPE_PATHINFO);
    gpath_move_to(STRIPE_PATH, center);		
		gpath_rotate_to(STRIPE_PATH, STRIPE_angle);
		gpath_draw_filled(ctx, STRIPE_PATH); 
		gpath_destroy(STRIPE_PATH);
  }	
	//gpath_destroy(STRIPE_PATH);		
	
	/* graphics_context_set_text_color(ctx, GColorBlack);
	graphics_draw_text(
		ctx, itoa(WINDOW_WIDTH[MIN]), 
		fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), 
		GRect(65, 140, 50, 50), 
		GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);	*/
}

static void hour_update_proc(Layer *layer, GContext *ctx) {
  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  char hour_buffer[] = "0000000";
	if(clock_is_24h_style() == true) { strftime(hour_buffer, sizeof("00"), "%H", t); }
	else { strftime(hour_buffer, sizeof("00"), "%I", t); }

//app_log(APP_LOG_LEVEL_INFO, __FILE__, __LINE__, "hour_buffer = '%s', min_buffer = '%s'", hour_buffer, min_buffer);	
//app_log(APP_LOG_LEVEL_INFO, __FILE__, __LINE__, "min_buffer = '%s', sec = '%d'", min_buffer, t->tm_sec);	
//app_log(APP_LOG_LEVEL_INFO, __FILE__, __LINE__, "hour_buffer = '%s', sec = '%d'", hour_buffer, t->tm_sec);	

	unsigned short TIME_X;
	unsigned short TIME_Y;
	unsigned short TIME_X_SIZE;
	unsigned short TIME_Y_SIZE;
	
	// DRAW HOUR
	TIME_X = 2;
	TIME_Y = 10;
	TIME_X_SIZE = 140;
	TIME_Y_SIZE = 350;

	graphics_context_set_text_color(ctx, GColorFromHEX(HexStringToUInt(TIME_OUTLINE_COLOR)));
	graphics_context_set_text_color(ctx, GColorFromHEX(HexStringToUInt(TIME_OUTLINE_COLOR)));
	if (TIME_OUTLINE_WIDTH) {
		for (short ix = TIME_OUTLINE_WIDTH*-1; ix <= TIME_OUTLINE_WIDTH; ++ix) {
			for (short iy = TIME_OUTLINE_WIDTH*-1; iy <= TIME_OUTLINE_WIDTH; ++iy) {
				graphics_draw_text(
					ctx, hour_buffer, time_font, 
					GRect(TIME_X + ix, TIME_Y + iy, TIME_X_SIZE, TIME_Y_SIZE), 
					GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
			}
		}
	}

	graphics_context_set_text_color(ctx, GColorFromHEX(HexStringToUInt(TIME_COLOR)));
	//graphics_context_set_text_color(ctx, (GColor)TIME_COLOR);
	graphics_draw_text(
		ctx, hour_buffer, time_font, 
		GRect(TIME_X, TIME_Y, TIME_X_SIZE, TIME_Y_SIZE), 
		GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);	

	char min_buffer[] = "00";
	strftime(min_buffer, sizeof("00"), "%M", t);

	// DRAW MINUTE
	TIME_X = 2;
	TIME_Y = 75;
	TIME_X_SIZE = 140;
	TIME_Y_SIZE = 250;

	graphics_context_set_text_color(ctx, GColorFromHEX(HexStringToUInt(TIME_OUTLINE_COLOR)));
	if (TIME_OUTLINE_WIDTH) {
		for (short ix = TIME_OUTLINE_WIDTH*-1; ix <= TIME_OUTLINE_WIDTH; ++ix) {
			for (short iy = TIME_OUTLINE_WIDTH*-1; iy <= TIME_OUTLINE_WIDTH; ++iy) {
				graphics_draw_text(
					ctx, min_buffer, time_font, 
					GRect(TIME_X + ix, TIME_Y + iy, TIME_X_SIZE, TIME_Y_SIZE), 
					GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
			}
		}
	}

	graphics_context_set_text_color(ctx, GColorFromHEX(HexStringToUInt(TIME_COLOR)));
	//graphics_context_set_text_color(ctx, (GColor)TIME_COLOR);
	graphics_draw_text(
		ctx, min_buffer, time_font, 
		GRect(TIME_X, TIME_Y, TIME_X_SIZE, TIME_Y_SIZE), 
		GTextOverflowModeFill, GTextAlignmentCenter, NULL);			
}

static void handle_tick(struct tm *tick_time, TimeUnits units_changed) {
  //layer_mark_dirty(window_get_root_layer(window));
	layer_mark_dirty(time_layer);
}

void LoadColorArray() {
	//APP_LOG(APP_LOG_LEVEL_INFO, "BAR_COLORS %s", SC);
	/* static char *buffer = "00ff00";
	for (int i = 0; i < 12; ++i) {
		buffer = substring(SC, i*6, 6);
		memcpy(HEXCOLOR_ARRAY[i], buffer, sizeof("00ff00"));
		//APP_LOG(APP_LOG_LEVEL_INFO, "#%d, buffer %s, array %s", i, buffer, HEXCOLOR_ARRAY[i]);
	}; */
	//APP_LOG(APP_LOG_LEVEL_INFO, "BAR_COLORS-2 %s", SC);
	//APP_LOG(APP_LOG_LEVEL_INFO, "num_bars #%d", NUM_BARS);

	APP_LOG(APP_LOG_LEVEL_DEBUG, "BAR_COLORS %s", BAR_COLORS);
	static char *buffer = "0055aa";
	for (short i = 0; i < 12; ++i) {
		buffer = substring(BAR_COLORS, i*6, 6);
		memcpy(HEXCOLOR_ARRAY[i], buffer, sizeof("000000"));
		APP_LOG(APP_LOG_LEVEL_DEBUG, "#%d, index %d, buffer %s, array %s", i, (i*6), buffer, HEXCOLOR_ARRAY[i]);
	}
	
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context)
{
  Tuple *t = dict_read_first(iterator);
  //APP_LOG(APP_LOG_LEVEL_INFO, "AppMessage Received");

  while(t != NULL) {
    switch(t->key){

			case KEY_TIME_COLOR:
				snprintf(TIME_COLOR, sizeof(TIME_COLOR), "%s", t->value->cstring);
	      break;

			case KEY_TIME_OUTLINE_COLOR:
				snprintf(TIME_OUTLINE_COLOR, sizeof(TIME_OUTLINE_COLOR), "%s", t->value->cstring);
	      break;

			case KEY_TIME_OUTLINE_WIDTH:
	      TIME_OUTLINE_WIDTH = (int)t->value->int8;
	      break;

			case KEY_NUM_BARS:
	      NUM_BARS = (int)t->value->int8;
				break;

			case KEY_ROTATE:
	      ROTATE = (int)t->value->int8;
				break;

			case KEY_ROTATE_FREQUENCY:
	      ROTATE_FREQUENCY = (int)t->value->int8;
			  tick_timer_service_unsubscribe();
				if (ROTATE_FREQUENCY>0) tick_timer_service_subscribe(MINUTE_UNIT, handle_tick);
				else tick_timer_service_subscribe(SECOND_UNIT, handle_tick);
				break;

			case KEY_STATIC_ANGLE:
	      STATIC_ANGLE = (int)t->value->int8;
				break;

			case KEY_BAR_COLORS:
				snprintf(BAR_COLORS, sizeof("ff0000ff5500ffff0055aa000000ffaa00ffff0000ff5500ffff0055aa000000ffaa00ffQ"), "%s", t->value->cstring);
				//snprintf(BAR_COLORS, sizeof(BAR_COLORS), "%s", t->value->data);
				//APP_LOG(APP_LOG_LEVEL_DEBUG, "BAR_COLORS %s", BAR_COLORS);
			 	LoadColorArray();
			
				/* APP_LOG(APP_LOG_LEVEL_INFO, "BAR_COLORS %s", BAR_COLORS);
				static char *buffer = "0055aa";
				for (int i = 0; i < 12; ++i) {
					buffer = substring(BAR_COLORS, i*6, 6);
					memcpy(HEXCOLOR_ARRAY[i], buffer, sizeof("000000"));
					APP_LOG(APP_LOG_LEVEL_INFO, "#%d, index %d, buffer %s, array %s-", i, (i*6), buffer, HEXCOLOR_ARRAY[i]);
				}; */
				//APP_LOG(APP_LOG_LEVEL_INFO, "BAR_COLORS-2 %s", BAR_COLORS);
			break;			
			
			APP_LOG(APP_LOG_LEVEL_ERROR, "Key%d not recognized", (int)t->key);
      break;
    }
    t = dict_read_next(iterator);
  }

  layer_mark_dirty(window_get_root_layer(window));
}

static void inbox_dropped_callback(AppMessageResult reason, void *context)
{
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context)
{
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed");
	//psleep(100);
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
	app_message_outbox_send();
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context)
{
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send success!");
}

static void init() {

	//persist_delete(KEY_BAR_COLORS);
	/*persist_delete(KEY_NUM_BARS);

	persist_delete(KEY_TIME_COLOR);
	persist_delete(KEY_TIME_OUTLINE_COLOR);
	persist_delete(KEY_TIME_OUTLINE_WIDTH);

	persist_delete(KEY_NUM_BARS);
	persist_delete(KEY_ROTATE);
	persist_delete(KEY_STATIC_ANGLE); */
	
	if (persist_exists(KEY_TIME_COLOR))					{	persist_read_string(KEY_TIME_COLOR, TIME_COLOR, 7); }
	if (persist_exists(KEY_TIME_OUTLINE_COLOR))	{	persist_read_string(KEY_TIME_OUTLINE_COLOR, TIME_OUTLINE_COLOR, 7); }
	if (persist_exists(KEY_TIME_OUTLINE_WIDTH))	{ TIME_OUTLINE_WIDTH = persist_read_int(KEY_TIME_OUTLINE_WIDTH); }

	if (persist_exists(KEY_ROTATE))						{ ROTATE = persist_read_int(KEY_ROTATE); }
	if (persist_exists(KEY_STATIC_ANGLE))	 		{ STATIC_ANGLE = persist_read_int(KEY_STATIC_ANGLE); }
	if (persist_exists(KEY_ROTATE_FREQUENCY)) { ROTATE_FREQUENCY = persist_read_int(KEY_ROTATE_FREQUENCY); }

	if (persist_exists(KEY_NUM_BARS)) { NUM_BARS = persist_read_int(KEY_NUM_BARS); }
	if (persist_exists(KEY_BAR_COLORS))	{
		persist_read_string(KEY_BAR_COLORS, BAR_COLORS, 73); 
		LoadColorArray();
	}
			
	window = window_create();
  window_stack_push(window, true);
	
	GRect bounds = layer_get_bounds(window_get_root_layer(window));
  background_layer = layer_create(bounds);
  layer_set_update_proc(background_layer, bg_update_proc);
  layer_add_child(window_get_root_layer(window), background_layer);

	time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_65));
	
  time_layer = layer_create(bounds);
  layer_set_update_proc(time_layer, hour_update_proc);
	layer_add_child(window_get_root_layer(window), time_layer);
	
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);

  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

	if (ROTATE_FREQUENCY>0) tick_timer_service_subscribe(MINUTE_UNIT, handle_tick);
	else tick_timer_service_subscribe(SECOND_UNIT, handle_tick);
}

static void deinit() {

  persist_write_string(KEY_TIME_COLOR, (TIME_COLOR));
  persist_write_string(KEY_TIME_OUTLINE_COLOR, (TIME_OUTLINE_COLOR));
  persist_write_int(KEY_TIME_OUTLINE_WIDTH, (TIME_OUTLINE_WIDTH));

  persist_write_string(KEY_BAR_COLORS, (BAR_COLORS));
  persist_write_int(KEY_NUM_BARS, (NUM_BARS));

  persist_write_int(KEY_ROTATE, (ROTATE));
  persist_write_int(KEY_STATIC_ANGLE, (STATIC_ANGLE));
  persist_write_int(KEY_ROTATE_FREQUENCY, (ROTATE_FREQUENCY));
	
  layer_destroy(background_layer);
  layer_destroy(time_layer);
	fonts_unload_custom_font(time_font);	
  tick_timer_service_unsubscribe();
  window_destroy(window);
}

int main() {
  init();
  app_event_loop();
  deinit();
}
