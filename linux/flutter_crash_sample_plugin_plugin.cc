#include <stdexcept>

#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream, std::stringbuf

#include <string.h>

#include "include/flutter_crash_sample_plugin/flutter_crash_sample_plugin.h"

#include <flutter_linux/flutter_linux.h>
#include <gtk/gtk.h>
#include <sys/utsname.h>

#define FLUTTER_CRASH_SAMPLE_PLUGIN(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), flutter_crash_sample_plugin_get_type(), \
                              FlutterCrashSamplePlugin))

struct _FlutterCrashSamplePlugin {
  GObject parent_instance;
};

G_DEFINE_TYPE(FlutterCrashSamplePlugin, flutter_crash_sample_plugin, g_object_get_type())

// Called when a method call is received from Flutter.
static void flutter_crash_sample_plugin_handle_method_call(
    FlutterCrashSamplePlugin* self,
    FlMethodCall* method_call) {
  g_autoptr(FlMethodResponse) response = nullptr;

  const gchar* method = fl_method_call_get_name(method_call);

  bool err=false;

  if (strcmp(method, "sampleMethod") == 0) {
    try {
        throw std::runtime_error("test_cpp_exception");
        g_autofree gchar *version = g_strdup_printf("%s", "42");
        g_autoptr(FlValue) result = fl_value_new_string(version);
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(result));
    } catch(std::runtime_error & ex) {
        err = true;
    }
  } else {
    response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
  }

  if(err){
      g_autofree gchar *version = g_strdup_printf("exception");
      g_autoptr(FlValue) result = fl_value_new_string(version);
      response = FL_METHOD_RESPONSE(fl_method_success_response_new(result));
  }
  fl_method_call_respond(method_call, response, nullptr);
}

static void flutter_crash_sample_plugin_dispose(GObject* object) {
  G_OBJECT_CLASS(flutter_crash_sample_plugin_parent_class)->dispose(object);
}

static void flutter_crash_sample_plugin_class_init(FlutterCrashSamplePluginClass* klass) {
  G_OBJECT_CLASS(klass)->dispose = flutter_crash_sample_plugin_dispose;
}

static void flutter_crash_sample_plugin_init(FlutterCrashSamplePlugin* self) {}

static void method_call_cb(FlMethodChannel* channel, FlMethodCall* method_call,
                           gpointer user_data) {
  FlutterCrashSamplePlugin* plugin = FLUTTER_CRASH_SAMPLE_PLUGIN(user_data);
  flutter_crash_sample_plugin_handle_method_call(plugin, method_call);
}

void flutter_crash_sample_plugin_register_with_registrar(FlPluginRegistrar* registrar) {
  FlutterCrashSamplePlugin* plugin = FLUTTER_CRASH_SAMPLE_PLUGIN(
      g_object_new(flutter_crash_sample_plugin_get_type(), nullptr));

  g_autoptr(FlStandardMethodCodec) codec = fl_standard_method_codec_new();
  g_autoptr(FlMethodChannel) channel =
      fl_method_channel_new(fl_plugin_registrar_get_messenger(registrar),
                            "flutter_crash_sample_plugin",
                            FL_METHOD_CODEC(codec));
  fl_method_channel_set_method_call_handler(channel, method_call_cb,
                                            g_object_ref(plugin),
                                            g_object_unref);

  g_object_unref(plugin);
}
