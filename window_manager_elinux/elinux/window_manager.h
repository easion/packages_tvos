#include <wayland-client.h>

#define UNUSED __attribute__((unused))

class WindowManager {
 public:
  WindowManager();

  virtual ~WindowManager();

  //HWND GetMainWindow();
  void ForceRefresh();
  void SetAsFrameless();
  void WaitUntilReadyToShow();
  void Destroy();
  void Close();
  bool IsPreventClose();
  void SetPreventClose(const flutter::EncodableMap& args);
  void Focus();
  void Blur();
  bool IsFocused();
  void Show();
  void Hide();
  bool IsVisible();
  bool IsMaximized();
  void Maximize(const flutter::EncodableMap& args);
  void Unmaximize();
  bool IsMinimized();
  void Minimize();
  void Restore();
  bool IsDockable();
  int IsDocked();
  void Dock(const flutter::EncodableMap& args);
  bool Undock();
  bool IsFullScreen();
  void SetFullScreen(const flutter::EncodableMap& args);
  void SetAspectRatio(const flutter::EncodableMap& args);
  void SetBackgroundColor(const flutter::EncodableMap& args);
  flutter::EncodableMap GetBounds(
      const flutter::EncodableMap& args);
  void SetBounds(const flutter::EncodableMap& args);
  void SetMinimumSize(const flutter::EncodableMap& args);
  void SetMaximumSize(const flutter::EncodableMap& args);
  bool IsResizable();
  void SetResizable(const flutter::EncodableMap& args);
  bool IsMinimizable();
  void SetMinimizable(const flutter::EncodableMap& args);
  bool IsMaximizable();
  void SetMaximizable(const flutter::EncodableMap& args);
  bool IsClosable();
  void SetClosable(const flutter::EncodableMap& args);
  bool IsAlwaysOnTop();
  void SetAlwaysOnTop(const flutter::EncodableMap& args);
  bool IsAlwaysOnBottom();
  void SetAlwaysOnBottom(const flutter::EncodableMap& args);
  std::string GetTitle();
  void SetTitle(const flutter::EncodableMap& args);
  void SetTitleBarStyle(const flutter::EncodableMap& args);
  int GetTitleBarHeight();
  bool IsSkipTaskbar();
  void SetSkipTaskbar(const flutter::EncodableMap& args);
  void SetProgressBar(const flutter::EncodableMap& args);
  void SetIcon(const flutter::EncodableMap& args);
  bool HasShadow();
  void SetHasShadow(const flutter::EncodableMap& args);
  double GetOpacity();
  void SetOpacity(const flutter::EncodableMap& args);
  void SetBrightness(const flutter::EncodableMap& args);
  void SetIgnoreMouseEvents(const flutter::EncodableMap& args);
  void PopUpWindowMenu(const flutter::EncodableMap& args);
  void StartDragging();
  void StartResizing(const flutter::EncodableMap& args);


  //HWND native_window;

  //int last_state = STATE_NORMAL;

  bool has_shadow_ = false;
  bool is_always_on_bottom_ = false;
  bool is_frameless_ = false;
  bool is_prevent_close_ = false;
  double aspect_ratio_ = 0;
  //POINT minimum_size_ = {0, 0};
  //POINT maximum_size_ = {-1, -1};
  double pixel_ratio_ = 1;
  bool is_resizable_ = true;
  int is_docked_ = 0;
  bool is_registered_for_docking_ = false;
  bool is_skip_taskbar_ = true;
  std::string title_bar_style_ = "normal";
  double opacity_ = 1;

  bool is_resizing_ = false;
  bool is_moving_ = false;

  wl_display* display;
  wl_surface* surface;

 private:
  //static constexpr auto kFlutterViewWindowClassName = L"FLUTTERVIEW";
  bool g_is_window_fullscreen = false;
  std::string g_title_bar_style_before_fullscreen;
  //RECT g_frame_before_fullscreen;
  bool g_maximized_before_fullscreen;
  
};

/*
void window_manager_init(FlutterDesktopMessengerRef messenger);

void window_manager_destroy() ;

bool window_manager_set_title(const char* title);
bool window_manager_set_size(int width, int height) ;
bool window_manager_set_position(int x, int y);
bool window_manager_set_minimizable(bool minimizable);
bool window_manager_set_maximizable(bool maximizable);
bool window_manager_set_closable(bool closable);

*/

