## 快速開始
### 1️⃣ 下載原始碼
```bash
git clone https://github.com/cheng033/linux-monitor.git
cd linux-monitor
```
### 2️⃣ 建立 Docker 映像檔
```bash
docker build -t linux-monitor .
```
### 3️⃣ 啟動 Container 進行監控
```bash
docker run --rm -it -e TERM=xterm linux-monitor
```
