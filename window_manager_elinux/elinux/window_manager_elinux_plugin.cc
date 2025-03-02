#include "include/window_manager_elinux/window_manager_e_linux_plugin.h"

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar.h>
#include <flutter/standard_method_codec.h>

#include <map>
#include <memory>
#include <sstream>
#include "window_manager.h"

namespace
{

std::unique_ptr<
    flutter::MethodChannel<flutter::EncodableValue>,
    std::default_delete<flutter::MethodChannel<flutter::EncodableValue>>>
    channel = nullptr;

  class WindowManagerELinuxPlugin : public flutter::Plugin
  {
  public:
    static void RegisterWithRegistrar(flutter::PluginRegistrar *registrar);

    WindowManagerELinuxPlugin();

    virtual ~WindowManagerELinuxPlugin();

  private:
	  WindowManager* window_manager;
    // Called when a method is called on this plugin's channel from Dart.
    void HandleMethodCall(
        const flutter::MethodCall<flutter::EncodableValue> &method_call,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
	void _EmitEvent(std::string eventName);
  };

  // static
  void WindowManagerELinuxPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrar *registrar)
  {

     channel =
        std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
            registrar->messenger(), "window_manager",
            &flutter::StandardMethodCodec::GetInstance());

    auto plugin = std::make_unique<WindowManagerELinuxPlugin>();

    channel->SetMethodCallHandler(
        [plugin_pointer = plugin.get()](const auto &call, auto result)
        {
          plugin_pointer->HandleMethodCall(call, std::move(result));
        });

    registrar->AddPlugin(std::move(plugin));
  }

  WindowManagerELinuxPlugin::WindowManagerELinuxPlugin() {
	window_manager = new WindowManager();
  }


void WindowManagerELinuxPlugin::_EmitEvent(std::string eventName) {
  if (channel == nullptr)
    return;
  flutter::EncodableMap args = flutter::EncodableMap();
  args[flutter::EncodableValue("eventName")] =
      flutter::EncodableValue(eventName);
  channel->InvokeMethod("onEvent",
                        std::make_unique<flutter::EncodableValue>(args));
}

  WindowManagerELinuxPlugin::~WindowManagerELinuxPlugin() {}

void WindowManagerELinuxPlugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue>& method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  std::string method_name = method_call.method_name();

  printf("WindowManager method_name %s\n", method_name.c_str());

  if (method_name.compare("ensureInitialized") == 0) {
    //window_manager->native_window =
    //    ::GetAncestor(registrar->GetView()->GetNativeWindow(), GA_ROOT);
    result->Success(flutter::EncodableValue(true));
  } else if (method_name.compare("waitUntilReadyToShow") == 0) {
    window_manager->WaitUntilReadyToShow();
    result->Success(flutter::EncodableValue(true));
  } else if (method_name.compare("setAsFrameless") == 0) {
    window_manager->SetAsFrameless();
    result->Success(flutter::EncodableValue(true));
  } else if (method_name.compare("destroy") == 0) {
    window_manager->Destroy();
    result->Success(flutter::EncodableValue(true));
  } else if (method_name.compare("close") == 0) {
    window_manager->Close();
    result->Success(flutter::EncodableValue(true));
  } else if (method_name.compare("isPreventClose") == 0) {
    auto value = window_manager->IsPreventClose();
    result->Success(flutter::EncodableValue(value));
  } else if (method_name.compare("setPreventClose") == 0) {
    const flutter::EncodableMap& args =
        std::get<flutter::EncodableMap>(*method_call.arguments());
    window_manager->SetPreventClose(args);
    result->Success(flutter::EncodableValue(true));
  } else if (method_name.compare("focus") == 0) {
    window_manager->Focus();
    result->Success(flutter::EncodableValue(true));
  } else if (method_name.compare("blur") == 0) {
    window_manager->Blur();
    result->Success(flutter::EncodableValue(true));
  } else if (method_name.compare("isFocused") == 0) {
    bool value = window_manager->IsFocused();
    result->Success(flutter::EncodableValue(value));
  } else if (method_name.compare("show") == 0) {
    window_manager->Show();
    result->Success(flutter::EncodableValue(true));
  } else if (method_name.compare("hide") == 0) {
    window_manager->Hide();
    result->Success(flutter::EncodableValue(true));
  } else if (method_name.compare("isVisible") == 0) {
    bool value = window_manager->IsVisible();
    result->Success(flutter::EncodableValue(value));
  } else if (method_name.compare("isMaximized") == 0) {
    bool value = window_manager->IsMaximized();
    result->Success(flutter::EncodableValue(value));
  } else if (method_name.compare("maximize") == 0) {
    const flutter::EncodableMap& args =
        std::get<flutter::EncodableMap>(*method_call.arguments());
    window_manager->Maximize(args);
    result->Success(flutter::EncodableValue(true));
  } else if (method_name.compare("unmaximize") == 0) {
    window_manager->Unmaximize();
    result->Success(flutter::EncodableValue(true));
  } else if (method_name.compare("isMinimized") == 0) {
    bool value = window_manager->IsMinimized();
    result->Success(flutter::EncodableValue(value));
  } else if (method_name.compare("minimize") == 0) {
    window_manager->Minimize();
    result->Success(flutter::EncodableValue(true));
  } else if (method_name.compare("restore") == 0) {
    window_manager->Restore();
    result->Success(flutter::EncodableValue(true));
  } else if (method_name.compare("isDockable") == 0) {
    bool value = window_manager->IsDockable();
    result->Success(flutter::EncodableValue(value));
  } else if (method_name.compare("isDocked") == 0) {
    int value = window_manager->IsDocked();
    result->Success(flutter::EncodableValue(value));
  } else if (method_name.compare("dock") == 0) {
    const flutter::EncodableMap& args =
        std::get<flutter::EncodableMap>(*method_call.arguments());
    window_manager->Dock(args);
    result->Success(flutter::EncodableValue(true));
  } else if (method_name.compare("undock") == 0) {
    bool value = window_manager->Undock();
    result->Success(flutter::EncodableValue(value));
  } else if (method_name.compare("isFullScreen") == 0) {
    bool value = window_manager->IsFullScreen();
    result->Success(flutter::EncodableValue(value));
  } else if (method_name.compare("setFullScreen") == 0) {
    const flutter::EncodableMap& args =
        std::get<flutter::EncodableMap>(*method_call.arguments());
    window_manager->SetFullScreen(args);
    result->Success(flutter::EncodableValue(true));
  } else if (method_name.compare("setAspectRatio") == 0) {
    const flutter::EncodableMap& args =
        std::get<flutter::EncodableMap>(*method_call.arguments());
    window_manager->SetAspectRatio(args);
    result->Success(flutter::EncodableValue(true));
  } else if (method_name.compare("setBackgroundColor") == 0) {
    const flutter::EncodableMap& args =
        std::get<flutter::EncodableMap>(*method_call.arguments());
    window_manager->SetBackgroundColor(args);
    result->Success(flutter::EncodableValue(true));
  } else if (method_name.compare("getBounds") == 0) {
    const flutter::EncodableMap& args =
        std::get<flutter::EncodableMap>(*method_call.arguments());
    flutter::EncodableMap value = window_manager->GetBounds(args);
    result->Success(flutter::EncodableValue(value));
  } else if (method_name.compare("setBounds") == 0) {
    const flutter::EncodableMap& args =
        std::get<flutter::EncodableMap>(*method_call.arguments());
    window_manager->SetBounds(args);
    result->Success(flutter::EncodableValue(true));
  } else if (method_name.compare("setMinimumSize") == 0) {
    const flutter::EncodableMap& args =
        std::get<flutter::EncodableMap>(*method_call.arguments());
    window_manager->SetMinimumSize(args);
    result->Success(flutter::EncodableValue(true));
  } else if (method_name.compare("setMaximumSize") == 0) {
    const flutter::EncodableMap& args =
        std::get<flutter::EncodableMap>(*method_call.arguments());
    window_manager->SetMaximumSize(args);
    result->Success(flutter::EncodableValue(true));
  } else if (method_name.compare("isResizable") == 0) {
    bool value = window_manager->IsResizable();
    result->Success(flutter::EncodableValue(value));
  } else if (method_name.compare("setResizable") == 0) {
    const flutter::EncodableMap& args =
        std::get<flutter::EncodableMap>(*method_call.arguments());
    window_manager->SetResizable(args);
    result->Success(flutter::EncodableValue(true));
  } else if (method_name.compare("isMinimizable") == 0) {
    bool value = window_manager->IsMinimizable();
    result->Success(flutter::EncodableValue(value));
  } else if (method_name.compare("setMinimizable") == 0) {
    const flutter::EncodableMap& args =
        std::get<flutter::EncodableMap>(*method_call.arguments());
    window_manager->SetMinimizable(args);
    result->Success(flutter::EncodableValue(true));
  } else if (method_name.compare("isMaximizable") == 0) {
    bool value = window_manager->IsMaximizable();
    result->Success(flutter::EncodableValue(value));
  } else if (method_name.compare("setMaximizable") == 0) {
    const flutter::EncodableMap& args =
        std::get<flutter::EncodableMap>(*method_call.arguments());
    window_manager->SetMaximizable(args);
    result->Success(flutter::EncodableValue(true));
  } else if (method_name.compare("isClosable") == 0) {
    bool value = window_manager->IsClosable();
    result->Success(flutter::EncodableValue(value));
  } else if (method_name.compare("setClosable") == 0) {
    const flutter::EncodableMap& args =
        std::get<flutter::EncodableMap>(*method_call.arguments());
    window_manager->SetClosable(args);
    result->Success(flutter::EncodableValue(true));
  } else if (method_name.compare("isAlwaysOnTop") == 0) {
    bool value = window_manager->IsAlwaysOnTop();
    result->Success(flutter::EncodableValue(value));
  } else if (method_name.compare("setAlwaysOnTop") == 0) {
    const flutter::EncodableMap& args =
        std::get<flutter::EncodableMap>(*method_call.arguments());
    window_manager->SetAlwaysOnTop(args);
    result->Success(flutter::EncodableValue(true));
  } else if (method_name.compare("isAlwaysOnBottom") == 0) {
    bool value = window_manager->IsAlwaysOnBottom();
    result->Success(flutter::EncodableValue(value));
  } else if (method_name.compare("setAlwaysOnBottom") == 0) {
    const flutter::EncodableMap& args =
        std::get<flutter::EncodableMap>(*method_call.arguments());
    window_manager->SetAlwaysOnBottom(args);
    result->Success(flutter::EncodableValue(true));
  } else if (method_name.compare("getTitle") == 0) {
    std::string value = window_manager->GetTitle();
    result->Success(flutter::EncodableValue(value));
  } else if (method_name.compare("setTitle") == 0) {
    const flutter::EncodableMap& args =
        std::get<flutter::EncodableMap>(*method_call.arguments());
    window_manager->SetTitle(args);
    result->Success(flutter::EncodableValue(true));
  } else if (method_name.compare("setTitleBarStyle") == 0) {
    const flutter::EncodableMap& args =
        std::get<flutter::EncodableMap>(*method_call.arguments());
    window_manager->SetTitleBarStyle(args);
    result->Success(flutter::EncodableValue(true));
  } else if (method_name.compare("getTitleBarHeight") == 0) {
    int value = window_manager->GetTitleBarHeight();
    result->Success(flutter::EncodableValue(value));
  } else if (method_name.compare("isSkipTaskbar") == 0) {
    bool value = window_manager->IsSkipTaskbar();
    result->Success(flutter::EncodableValue(value));
  } else if (method_name.compare("setSkipTaskbar") == 0) {
    const flutter::EncodableMap& args =
        std::get<flutter::EncodableMap>(*method_call.arguments());
    window_manager->SetSkipTaskbar(args);
    result->Success(flutter::EncodableValue(true));
  } else if (method_name.compare("setProgressBar") == 0) {
    const flutter::EncodableMap& args =
        std::get<flutter::EncodableMap>(*method_call.arguments());
    window_manager->SetProgressBar(args);
    result->Success(flutter::EncodableValue(true));
  } else if (method_name.compare("setIcon") == 0) {
    const flutter::EncodableMap& args =
        std::get<flutter::EncodableMap>(*method_call.arguments());
    window_manager->SetIcon(args);
    result->Success(flutter::EncodableValue(true));
  } else if (method_name.compare("hasShadow") == 0) {
    bool value = window_manager->HasShadow();
    result->Success(flutter::EncodableValue(value));
  } else if (method_name.compare("setHasShadow") == 0) {
    const flutter::EncodableMap& args =
        std::get<flutter::EncodableMap>(*method_call.arguments());
    window_manager->SetHasShadow(args);
    result->Success(flutter::EncodableValue(true));
  } else if (method_name.compare("getOpacity") == 0) {
    double value = window_manager->GetOpacity();
    result->Success(flutter::EncodableValue(value));
  } else if (method_name.compare("setOpacity") == 0) {
    const flutter::EncodableMap& args =
        std::get<flutter::EncodableMap>(*method_call.arguments());
    window_manager->SetOpacity(args);
    result->Success(flutter::EncodableValue(true));
  } else if (method_name.compare("setBrightness") == 0) {
    const flutter::EncodableMap& args =
        std::get<flutter::EncodableMap>(*method_call.arguments());
    window_manager->SetBrightness(args);
    result->Success(flutter::EncodableValue(true));
  } else if (method_name.compare("setIgnoreMouseEvents") == 0) {
    const flutter::EncodableMap& args =
        std::get<flutter::EncodableMap>(*method_call.arguments());
    window_manager->SetIgnoreMouseEvents(args);
    result->Success(flutter::EncodableValue(true));
  } else if (method_name.compare("popUpWindowMenu") == 0) {
    const flutter::EncodableMap& args =
        std::get<flutter::EncodableMap>(*method_call.arguments());
    window_manager->PopUpWindowMenu(args);
    result->Success(flutter::EncodableValue(true));
  } else if (method_name.compare("startDragging") == 0) {
    window_manager->StartDragging();
    result->Success(flutter::EncodableValue(true));
  } else if (method_name.compare("startResizing") == 0) {
    const flutter::EncodableMap& args =
        std::get<flutter::EncodableMap>(*method_call.arguments());
    window_manager->StartResizing(args);
    result->Success(flutter::EncodableValue(true));
  } else {
    result->NotImplemented();
  }
}


} // namespace

void WindowManagerELinuxPluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar)
{
	FlutterDesktopViewRef view = FlutterDesktopPluginRegistrarGetView(registrar);
	int32_t rate = FlutterDesktopViewGetFrameRate(view);

	printf("view rate %d\n", rate);

	flutter::PluginRegistrar *plug_registrar = 
	flutter::PluginRegistrarManager::GetInstance()->GetRegistrar<flutter::PluginRegistrar>(registrar);

	WindowManagerELinuxPlugin::RegisterWithRegistrar(plug_registrar);
}
