# web-browser_in_c

# 介紹

製作簡易的劉覽器  

# 目前進度
receiver.c的vth4還沒實作  
目前感覺我參考到的github專案根本不像web-browser，很像是upd socket program  

# 未來實作方向
視窗／繪圖系統 (GUI / Rendering)  

你至少需要一個圖形視窗（Window）來顯示內容。可用的庫例如 SDL2、GTK、SDL + OpenGL、甚至 ncurses（若只做文字為主）等。  

在視窗內，你需要能畫文字 (文字節點)、畫圖片（如果支援 <img>），甚至畫簡單排版（例如標籤換行、粗體、斜體）。  

若只做文字版，視窗可簡化為 Terminal／Console 顯示，但若真正「顯示網頁畫面」通常意味 GUI。  

HTML 下載 + 解析  

專案已有 socket 傳輸部分（HTTP 請求／接收）。但你需要解析 HTTP 回應：讀取標頭、確定內容長度、處理 chunked 傳輸、處理編碼 (如 gzip) 等。  

然後你需要簡易的 HTML 解析器：將 HTML 文字轉成 DOM 樹／節點結構。這部分在目前專案中似乎並不存在。  

排版／佈局  

解析後的 DOM 你要決定如何在視窗中「佈局」。例如：文字流、區塊換行、縮排、圖片定位。即便只支援最簡單的 <p>, <div>, <img>，仍要寫佈局邏輯。  

你可能先做最簡化版本：僅上下順序排版，不支援浮動 (float)、欄位 (column) 等。  

繪製 (Painting / Rendering)  

將排版結果轉換為實際畫面：畫背景、畫文字 (選字型、顏色)、畫圖片 (先載入位圖，再畫)。  

字型選擇與文字排版是比較複雜的部分（字型度量、換行、對齊）。  

互動 (可選)  

支援點擊連結 (<a href="…">) → 再發 HTTP 請求。  

支援滾動(scroll)。  

支援基本表單輸入。這些可以視為進階功能。  

錯誤 &安全  
 
處理不良 HTML、無法閉合標籤、未知標籤。  

處理多種編碼 (UTF-8, ISO-8859-1 等)。  

若支援 HTTPS，要整合 TLS 庫 (例如 OpenSSL)。這對初學者來說比較高難度。  

# 參考
https://github.com/noidontdig/Web-Browser/tree/master