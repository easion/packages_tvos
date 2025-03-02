#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#include <flutter/method_channel.h>
#include <flutter/plugin_registrar.h>
#include <flutter/standard_method_codec.h>
#include "window_manager.h"


const flutter::EncodableValue* ValueOrNull(const flutter::EncodableMap& map,
                                           const char* key) {
  auto it = map.find(flutter::EncodableValue(key));
  if (it == map.end()) {
    return nullptr;
  }
  return &(it->second);
}

WindowManager::WindowManager() {}

WindowManager::~WindowManager() {}

/*
HWND WindowManager::GetMainWindow() {
  return native_window;
}
*/


void WindowManager::SetAsFrameless() {
  
}

void WindowManager::WaitUntilReadyToShow() {
  
}

void WindowManager::Destroy() {
  //PostQuitMessage(0);
}

void WindowManager::Close() {
  
  //PostMessage(hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
}

void WindowManager::SetPreventClose(const flutter::EncodableMap& args) {
  is_prevent_close_ =
      std::get<bool>(args.at(flutter::EncodableValue("isPreventClose")));
}

bool WindowManager::IsPreventClose() {
  return is_prevent_close_;
}

void WindowManager::Focus() {
  //HWND hWnd = GetMainWindow();
  if (IsMinimized()) {
    Restore();
  }

  //::SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
  //SetForegroundWindow(hWnd);
}

void WindowManager::Blur() {
  
}

bool WindowManager::IsFocused() {
  return true;//GetMainWindow() == GetForegroundWindow();
}

void WindowManager::Show() {
  
}

void WindowManager::Hide() {
  //ShowWindow(GetMainWindow(), SW_HIDE);
}

bool WindowManager::IsVisible() {
  bool isVisible = true;// IsWindowVisible(GetMainWindow());
  return isVisible;
}

bool WindowManager::IsMaximized() {
  //HWND mainWindow = GetMainWindow();
  //WINDOWPLACEMENT windowPlacement;
  //GetWindowPlacement(mainWindow, &windowPlacement);

  //return windowPlacement.showCmd == SW_MAXIMIZE;
  return false;
}

void WindowManager::Maximize(const flutter::EncodableMap& args) {
  bool vertically =
      std::get<bool>(args.at(flutter::EncodableValue("vertically")));

  //HWND hwnd = GetMainWindow();
  //WINDOWPLACEMENT windowPlacement;
  //GetWindowPlacement(hwnd, &windowPlacement);

  if (vertically) {
    //POINT cursorPos;
    //GetCursorPos(&cursorPos);
    //PostMessage(hwnd, WM_NCLBUTTONDBLCLK, HTTOP,
    //            MAKELPARAM(cursorPos.x, cursorPos.y));
  } else {
    //if (windowPlacement.showCmd != SW_MAXIMIZE) {
    //  PostMessage(hwnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
    //}
  }
}

void WindowManager::Unmaximize() {
 
}

bool WindowManager::IsMinimized() {
  return false;
}

void WindowManager::Minimize() {
  if (IsFullScreen()) {  // Like chromium, we don't want to minimize fullscreen
                         // windows
    return;
  }
  //
}

void WindowManager::Restore() {
  
}

bool WindowManager::IsDockable() {
  return true;
}

int WindowManager::IsDocked() {
  return is_docked_;
}



void WindowManager::Dock(const flutter::EncodableMap& args) {
}

bool WindowManager::Undock() {
  //HWND mainWindow = GetMainWindow();
  bool result = false;// RegisterAccessBar(mainWindow, false);
  is_docked_ = 0;
  return result;
}



bool WindowManager::IsFullScreen() {
  return g_is_window_fullscreen;
}

void WindowManager::SetFullScreen(const flutter::EncodableMap& args) {
  bool isFullScreen =
      std::get<bool>(args.at(flutter::EncodableValue("isFullScreen")));


  // Save current window state if not already fullscreen.
  if (!g_is_window_fullscreen) {
    // Save current window information.
    //g_maximized_before_fullscreen = ::IsZoomed(mainWindow);
    //g_style_before_fullscreen = GetWindowLong(mainWindow, GWL_STYLE);
    //::GetWindowRect(mainWindow, &g_frame_before_fullscreen);
    g_title_bar_style_before_fullscreen = title_bar_style_;
  }

  g_is_window_fullscreen = isFullScreen;

  if (isFullScreen) {  // Set to fullscreen
  } else {  // Restore from fullscreen
  }
}

void WindowManager::SetAspectRatio(const flutter::EncodableMap& args) {
  aspect_ratio_ =
      std::get<double>(args.at(flutter::EncodableValue("aspectRatio")));
}

void WindowManager::SetBackgroundColor(const flutter::EncodableMap& args) {
  int backgroundColorA =
      std::get<int>(args.at(flutter::EncodableValue("backgroundColorA")));
  int backgroundColorR =
      std::get<int>(args.at(flutter::EncodableValue("backgroundColorR")));
  int backgroundColorG =
      std::get<int>(args.at(flutter::EncodableValue("backgroundColorG")));
  int backgroundColorB =
      std::get<int>(args.at(flutter::EncodableValue("backgroundColorB")));

  UNUSED bool isTransparent = backgroundColorA == 0 && backgroundColorR == 0 &&
                       backgroundColorG == 0 && backgroundColorB == 0;

  
}

flutter::EncodableMap WindowManager::GetBounds(
    const flutter::EncodableMap& args) {
  //HWND hwnd = GetMainWindow();
  UNUSED double devicePixelRatio =
      std::get<double>(args.at(flutter::EncodableValue("devicePixelRatio")));

  flutter::EncodableMap resultMap = flutter::EncodableMap();
  if (devicePixelRatio > 0)
  {
  }

  int x_ = 0;
  int y_ = 0;
  int width_ = 960;
  int height_ = 540;

  double x = x_ / devicePixelRatio * 1.0f;
  double y = y_ / devicePixelRatio * 1.0f;
  double width = (width_) / devicePixelRatio * 1.0f;
  double height = (height_) / devicePixelRatio * 1.0f;

  resultMap[flutter::EncodableValue("x")] = flutter::EncodableValue(x);
  resultMap[flutter::EncodableValue("y")] = flutter::EncodableValue(y);
  resultMap[flutter::EncodableValue("width")] =
    flutter::EncodableValue(width);
  resultMap[flutter::EncodableValue("height")] =
    flutter::EncodableValue(height);
  
  return resultMap;
}

void WindowManager::SetBounds(const flutter::EncodableMap& args) {
  //HWND hwnd = GetMainWindow();

  double devicePixelRatio =
      std::get<double>(args.at(flutter::EncodableValue("devicePixelRatio")));

  auto* null_or_x = std::get_if<double>(ValueOrNull(args, "x"));
  auto* null_or_y = std::get_if<double>(ValueOrNull(args, "y"));
  auto* null_or_width = std::get_if<double>(ValueOrNull(args, "width"));
  auto* null_or_height = std::get_if<double>(ValueOrNull(args, "height"));

  int x = 0;
  int y = 0;
  int width = 0;
  int height = 0;
  //UINT uFlags = NULL;

  if (null_or_x != nullptr && null_or_y != nullptr) {
    x = static_cast<int>(*null_or_x * devicePixelRatio);
    y = static_cast<int>(*null_or_y * devicePixelRatio);
  }
  if (null_or_width != nullptr && null_or_height != nullptr) {
    width = static_cast<int>(*null_or_width * devicePixelRatio);
    height = static_cast<int>(*null_or_height * devicePixelRatio);
  }

  if (null_or_x == nullptr || null_or_y == nullptr) {
    //uFlags = SWP_NOMOVE;
  }
  if (null_or_width == nullptr || null_or_height == nullptr) {
    //uFlags = SWP_NOSIZE;
  }

  if (x&y&width&height)
  {
  }

  //SetWindowPos(hwnd, HWND_TOP, x, y, width, height, uFlags);
}

void WindowManager::SetMinimumSize(const flutter::EncodableMap& args) {
  double devicePixelRatio =
      std::get<double>(args.at(flutter::EncodableValue("devicePixelRatio")));
  double width = std::get<double>(args.at(flutter::EncodableValue("width")));
  double height = std::get<double>(args.at(flutter::EncodableValue("height")));

  if (width >= 0 && height >= 0) {
    pixel_ratio_ = devicePixelRatio;
    //POINT point = {};
    //point.x = static_cast<LONG>(width);
    //point.y = static_cast<LONG>(height);
    //minimum_size_ = point;
  }
}

void WindowManager::SetMaximumSize(const flutter::EncodableMap& args) {
  double devicePixelRatio =
      std::get<double>(args.at(flutter::EncodableValue("devicePixelRatio")));
  double width = std::get<double>(args.at(flutter::EncodableValue("width")));
  double height = std::get<double>(args.at(flutter::EncodableValue("height")));

  if (width >= 0 && height >= 0) {
    pixel_ratio_ = devicePixelRatio;
    //POINT point = {};
    //point.x = static_cast<LONG>(width);
    //point.y = static_cast<LONG>(height);
    //maximum_size_ = point;
  }
}

bool WindowManager::IsResizable() {
  return is_resizable_;
}

void WindowManager::SetResizable(const flutter::EncodableMap& args) {
  //HWND hWnd = GetMainWindow();
  is_resizable_ =
      std::get<bool>(args.at(flutter::EncodableValue("isResizable")));
  
}

bool WindowManager::IsMinimizable() {
  //HWND hWnd = GetMainWindow();
  //DWORD gwlStyle = GetWindowLong(hWnd, GWL_STYLE);
  return false;
}

void WindowManager::SetMinimizable(const flutter::EncodableMap& args) {
  
}

bool WindowManager::IsMaximizable() {
  
  return false;
}

void WindowManager::SetMaximizable(const flutter::EncodableMap& args) {
 
}

bool WindowManager::IsClosable() {
 
  return true;
}

void WindowManager::SetClosable(const flutter::EncodableMap& args) {
  //HWND hWnd = GetMainWindow();
  bool isClosable =
      std::get<bool>(args.at(flutter::EncodableValue("isClosable")));
  if (isClosable)
  {
  }
  //DWORD gclStyle = GetClassLong(hWnd, GCL_STYLE);
  //gclStyle = isClosable ? gclStyle & ~CS_NOCLOSE : gclStyle | CS_NOCLOSE;
  //SetClassLong(hWnd, GCL_STYLE, gclStyle);
}

bool WindowManager::IsAlwaysOnTop() {
  //DWORD dwExStyle = GetWindowLong(GetMainWindow(), GWL_EXSTYLE);
  return false;
}

void WindowManager::SetAlwaysOnTop(const flutter::EncodableMap& args) {
  bool isAlwaysOnTop =
      std::get<bool>(args.at(flutter::EncodableValue("isAlwaysOnTop")));
  if (isAlwaysOnTop)
  {
  }
}

bool WindowManager::IsAlwaysOnBottom() {
  return is_always_on_bottom_;
}

void WindowManager::SetAlwaysOnBottom(const flutter::EncodableMap& args) {
  is_always_on_bottom_ =
      std::get<bool>(args.at(flutter::EncodableValue("isAlwaysOnBottom")));

  if (is_always_on_bottom_)
  {
  }

}

std::string WindowManager::GetTitle() {
  //int const bufferSize = 1 + GetWindowTextLength(GetMainWindow());
  //std::wstring title(bufferSize, L'\0');
  //GetWindowText(GetMainWindow(), &title[0], bufferSize);

  //std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
  return std::string("");
}

void WindowManager::SetTitle(const flutter::EncodableMap& args) {
  std::string title =
      std::get<std::string>(args.at(flutter::EncodableValue("title")));

  //std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
  //SetWindowText(GetMainWindow(), converter.from_bytes(title).c_str());
}

void WindowManager::SetTitleBarStyle(const flutter::EncodableMap& args) {
  title_bar_style_ =
      std::get<std::string>(args.at(flutter::EncodableValue("titleBarStyle")));
  // Enables the ability to go from setAsFrameless() to
  // TitleBarStyle.normal/hidden
  is_frameless_ = false;

  
}

int WindowManager::GetTitleBarHeight() {
  
  return 0;
}

bool WindowManager::IsSkipTaskbar() {
  return is_skip_taskbar_;
}

void WindowManager::SetSkipTaskbar(const flutter::EncodableMap& args) {
  is_skip_taskbar_ =
      std::get<bool>(args.at(flutter::EncodableValue("isSkipTaskbar")));
  if (is_skip_taskbar_)
  {
  }

}

void WindowManager::SetProgressBar(const flutter::EncodableMap& args) {
  double progress =
      std::get<double>(args.at(flutter::EncodableValue("progress")));

  

  if (progress < 0) {
    
  } else if (progress > 1) {
    
  } else {
  }
}

void WindowManager::SetIcon(const flutter::EncodableMap& args) {
  std::string iconPath =
      std::get<std::string>(args.at(flutter::EncodableValue("iconPath")));

}

bool WindowManager::HasShadow() {
  if (is_frameless_)
    return has_shadow_;
  return true;
}

void WindowManager::SetHasShadow(const flutter::EncodableMap& args) {
  if (is_frameless_) {
    has_shadow_ = std::get<bool>(args.at(flutter::EncodableValue("hasShadow")));

	if (has_shadow_)
	{
	}

  }
}

double WindowManager::GetOpacity() {
  return opacity_;
}

void WindowManager::SetOpacity(const flutter::EncodableMap& args) {
  opacity_ = std::get<double>(args.at(flutter::EncodableValue("opacity")));
  
}

void WindowManager::SetBrightness(const flutter::EncodableMap& args) {
  

  //if (result == ERROR_SUCCESS) {
    std::string brightness =
        std::get<std::string>(args.at(flutter::EncodableValue("brightness")));
    //HWND hWnd = GetMainWindow();
    //BOOL enable_dark_mode = light_mode == 0 && brightness == "dark";
    //DwmSetWindowAttribute(hWnd, DWMWA_USE_IMMERSIVE_DARK_MODE,
    //                      &enable_dark_mode, sizeof(enable_dark_mode));
  //}
}

void WindowManager::SetIgnoreMouseEvents(const flutter::EncodableMap& args) {
  bool ignore = std::get<bool>(args.at(flutter::EncodableValue("ignore")));
  if (ignore)
  {
  }

}

void WindowManager::PopUpWindowMenu(const flutter::EncodableMap& args) {
  
}

void WindowManager::StartDragging() {
  //ReleaseCapture();
  //Undock();
  //SendMessage(GetMainWindow(), WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
}

void WindowManager::StartResizing(const flutter::EncodableMap& args) {
  bool top = std::get<bool>(args.at(flutter::EncodableValue("top")));
  bool bottom = std::get<bool>(args.at(flutter::EncodableValue("bottom")));
  bool left = std::get<bool>(args.at(flutter::EncodableValue("left")));
  bool right = std::get<bool>(args.at(flutter::EncodableValue("right")));

  //HWND hWnd = GetMainWindow();
  //Undock();
  //ReleaseCapture();
  //LONG command;
  if (top && !bottom && !right && !left) {
    //command = HTTOP;
  } else if (top && left && !bottom && !right) {
    //command = HTTOPLEFT;
  } else if (left && !top && !bottom && !right) {
    //command = HTLEFT;
  } else if (right && !top && !left && !bottom) {
    //command = HTRIGHT;
  } else if (top && right && !left && !bottom) {
    //command = HTTOPRIGHT;
  } else if (bottom && !top && !right && !left) {
    //command = HTBOTTOM;
  } else if (bottom && left && !top && !right) {
    //command = HTBOTTOMLEFT;
  } else{
    //command = HTBOTTOMRIGHT;
  }
  
}