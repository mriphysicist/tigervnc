/* Copyright (C) 2002-2005 RealVNC Ltd.  All Rights Reserved.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307,
 * USA.
 */
#ifndef WINVNCCONF_DESKTOP
#define WINVNCCONF_DESKTOP

#include <rfb_win32/Registry.h>
#include <rfb_win32/Dialog.h>
#include <rfb_win32/SDisplay.h>

namespace rfb {

  namespace win32 {

    class DesktopPage : public PropSheetPage {
    public:
      DesktopPage(const RegKey& rk)
        : PropSheetPage(GetModuleHandle(0), MAKEINTRESOURCE(IDD_DESKTOP)), regKey(rk) {}
      void initDialog() {
        CharArray action(rfb::win32::SDisplay::disconnectAction.getData());
        bool disconnectLock = stricmp(action.buf, "Lock") == 0;
        bool disconnectLogoff = stricmp(action.buf, "Logoff") == 0;
        CharArray display(rfb::win32::SDisplay::displayDevice.getData());
        bool displayShareAll = stricmp(display.buf, "") == 0;
        setItemChecked(IDC_DISCONNECT_LOGOFF, disconnectLogoff);
        setItemChecked(IDC_DISCONNECT_LOCK, disconnectLock);
        setItemChecked(IDC_DISCONNECT_NONE, !disconnectLock && !disconnectLogoff);
        setItemChecked(IDC_REMOVE_WALLPAPER, rfb::win32::SDisplay::removeWallpaper);
        setItemChecked(IDC_DISABLE_EFFECTS, rfb::win32::SDisplay::disableEffects);
        setItemChecked(IDC_SCREENS_ALL, displayShareAll);
        setItemChecked(IDC_SCREENS_ONE, !displayShareAll);
        setItemString(IDC_SCREENS_SHARE, display.buf);

        onCommand(IDC_SCREENS_ONE, EN_CHANGE);
      }
      bool onCommand(int id, int cmd) {
        switch (id) {
        case IDC_DISCONNECT_LOGOFF:
        case IDC_DISCONNECT_LOCK:
        case IDC_DISCONNECT_NONE:
        case IDC_REMOVE_WALLPAPER:
        case IDC_DISABLE_EFFECTS:
        {
          CharArray action(rfb::win32::SDisplay::disconnectAction.getData());
          bool disconnectLock = stricmp(action.buf, "Lock") == 0;
          bool disconnectLogoff = stricmp(action.buf, "Logoff") == 0;
          setChanged((disconnectLogoff != isItemChecked(IDC_DISCONNECT_LOGOFF)) ||
                     (disconnectLock != isItemChecked(IDC_DISCONNECT_LOCK)) ||
                     (isItemChecked(IDC_REMOVE_WALLPAPER) != rfb::win32::SDisplay::removeWallpaper) ||
                     (isItemChecked(IDC_DISABLE_EFFECTS) != rfb::win32::SDisplay::disableEffects));
          break;
        }
        case IDC_SCREENS_ALL:
        {
          CharArray display(rfb::win32::SDisplay::displayDevice.getData());
          bool displayShareAll = stricmp(display.buf, "") == 0;
          setChanged(displayShareAll != isItemChecked(IDC_SCREENS_ALL));
          if(isItemChecked(IDC_SCREENS_ALL)) {
            setItemString(IDC_SCREENS_SHARE, "");
          }
          break;
        }
        case IDC_SCREENS_ONE:
        {
          CharArray display(rfb::win32::SDisplay::displayDevice.getData());
          bool displayShareAll = stricmp(display.buf, "") == 0;
          bool displayShareOne = !displayShareAll;
          setChanged(displayShareOne != isItemChecked(IDC_SCREENS_ONE));
          enableItem(IDC_SCREENS_SHARE, isItemChecked(IDC_SCREENS_ONE));
          break;
        }
        case IDC_IDLE_TIMEOUT:
        case IDC_SCREENS_SHARE:
        {
          if(cmd == EN_CHANGE)
          {
            CharArray display(rfb::win32::SDisplay::displayDevice.getData());
            setChanged(stricmp(display.buf, getItemString(IDC_SCREENS_SHARE)) != 0);
          }
          break;
        }
        }
        return false;
      }
      bool onOk() {
        const TCHAR* action = _T("None");
        const TCHAR* display = _T("");
        if (isItemChecked(IDC_DISCONNECT_LOGOFF))
          action = _T("Logoff");
        else if (isItemChecked(IDC_DISCONNECT_LOCK))
          action = _T("Lock");
        if (isItemChecked(IDC_SCREENS_ONE))
          display = getItemString(IDC_SCREENS_SHARE);
        regKey.setString(_T("DisconnectAction"), action);
        regKey.setBool(_T("RemoveWallpaper"), isItemChecked(IDC_REMOVE_WALLPAPER));
        regKey.setBool(_T("DisableEffects"), isItemChecked(IDC_DISABLE_EFFECTS));
        regKey.setString(_T("DisplayDevice"), display);
        return true;
      }
    protected:
      RegKey regKey;
    };

  };

};

#endif
