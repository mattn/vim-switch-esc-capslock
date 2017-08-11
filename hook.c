#include <windows.h>
#include <stdio.h>

HHOOK hook_handle = NULL;

LRESULT CALLBACK
key_handler(int ncode, WPARAM wp, LPARAM lp) {
  static BOOL op = FALSE;
  if (op == FALSE && ncode == HC_ACTION && (
        wp == WM_SYSKEYUP || wp == WM_KEYUP || wp == WM_SYSKEYDOWN ||
        wp == WM_KEYDOWN)) {
    LPKBDLLHOOKSTRUCT kbs = (LPKBDLLHOOKSTRUCT) lp;
    switch (kbs->vkCode) {
      case VK_ESCAPE:
        op = TRUE;
        keybd_event(VK_CAPITAL, 0x14, KEYEVENTF_EXTENDEDKEY, 0);
        keybd_event(VK_CAPITAL, 0x14, KEYEVENTF_EXTENDEDKEY|KEYEVENTF_KEYUP, 0);
        op = FALSE;
        return 1;
      case VK_CAPITAL:
      case VK_OEM_ATTN:
        op = TRUE;
        keybd_event(VK_ESCAPE, 0x1b, KEYEVENTF_EXTENDEDKEY, 0);
        keybd_event(VK_ESCAPE, 0x1b, KEYEVENTF_EXTENDEDKEY|KEYEVENTF_KEYUP, 0);
        op = FALSE;
        return 1;
    };
  }
  return CallNextHookEx(hook_handle, ncode, wp, lp);
}

__declspec(dllexport) int
SetKeyboardHook(int enable) {
  if (enable) {
    hook_handle = SetWindowsHookEx(WH_KEYBOARD_LL, key_handler, GetModuleHandle(NULL), 0);
    if (hook_handle != NULL) {
      return 0;
    }
    return 1;
  } else if (!enable) {
    UnhookWindowsHookEx(hook_handle);
    return 1;
  }
  return 0;
}


