//   Copyright 2018 Fonwin.
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

static int ctrlExitApp = 0;
static BOOL __stdcall windowsCtrlHandler(DWORD fdwCtrlType) {
   ctrlExitApp = 3;
   switch (fdwCtrlType) {
   case CTRL_C_EVENT:// Handle the CTRL-C signal. 
   case CTRL_CLOSE_EVENT:// CTRL-CLOSE: confirm that the user wants to exit. 
   case CTRL_BREAK_EVENT:// Pass other signals to the next handler. 
   case CTRL_LOGOFF_EVENT:
   case CTRL_SHUTDOWN_EVENT:
   default:
      printf("\n\n" "ExitApp, Windows Ctrl Handler: %lu\n\n", fdwCtrlType);
      return TRUE;
   }
}

int main(int argc, char** argv) {
   int sleepms = 500;
   if(argc > 1)
      sleepms = atoi(argv[1]);
   printf("SSD read interval: %d ms\n", sleepms);
   SetConsoleCtrlHandler(windowsCtrlHandler, TRUE);

   DWORD DesiredAccess = GENERIC_READ|GENERIC_WRITE;
   DWORD ShareMode = FILE_SHARE_READ;
   DWORD CreationDistribution = OPEN_ALWAYS;
   DWORD FlagsAndAttributes = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING;
   HANDLE FD_ = CreateFileA("./dummy", DesiredAccess, ShareMode, NULL, CreationDistribution, FlagsAndAttributes, 0);

   DWORD rsz = 0, wsz = 0;
   char  buf[1024] = { 0 };
   if(::GetFileSize(FD_, NULL) < sizeof(buf))
      WriteFile(FD_, buf, sizeof(buf), &wsz, 0);
   __int64 tms = 0;
   __int64 totalrx = 0;
   while(ctrlExitApp == 0) {
      SetFilePointer(FD_, 0, 0, FILE_BEGIN);
      ReadFile(FD_, buf, 512, &rsz, 0);
      if(rsz == 0)
         printf("[fd=%p, r=%d,w=%d]", FD_, rsz, wsz);
      totalrx += rsz;
      SleepEx(sleepms, TRUE);
      if((tms += sleepms) % (1000*10) == 0) {
         __int64 secs = tms / 1000;
         printf("\r[Spent time: %lld:%02d:%02d][Read %lld bytes]"
                , secs/60/60, (int)(secs/60)%60, (int)(secs%60)
                , totalrx);
      }
   }
   CloseHandle(FD_);
   return 0;
}
