# issd-rx
Intel SSD 535 (240G) 寫入放大 10倍 變成 1.5倍 的方法

請參考底下說明:
---------------
http://fonwin.blogspot.tw/2016/04/intel-ssd-535-10-15.html

由於 Windows XP 不支援 FILE_FLAG_NO_BUFFERING，所以在 **XP下沒有效果!**

## 安裝
  * 把 issd-rx.exe 放在 SSD硬碟的路徑下，執行時路徑必須與 issd-rx.exe 相同路徑
  * 設定成開機後自動執行:
    * 底下例子的環境 `C:\Tools\issd-rx\issd-rx.exe`:
    * 以系統管理員執行:
      * `schtasks.exe /create /v1 /sc onstart /np /tn "Fix Intel SSD WA" /tr "C:\Tools\issd-rx\issd-rx.exe"`
    * 電腦重新啟動後可透過 `taskschd.msc` 或「工作管理員」觀察是否有執行
    * 可嘗試刪除 `C:\Tools\issd-rx\dummy` 如果「**刪除失敗** 就 **沒問題**」，表示程式正常運作中

## 執行
  * 手動執行時的訊息:
    * 讀取間隔應看 `SSD read interval: 500 ms` 這個訊息
    * 大約10秒顯示一次 `[Spent time: HH:MM:SS][Read nnnn bytes]`，只是為了確定程式還在運作
  * 第一次執行會建立 dummy 檔案(1024 bytes)作為定時讀取使用
