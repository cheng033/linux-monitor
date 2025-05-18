# 1. 使用 Ubuntu 24.04 基礎映像檔
FROM ubuntu:24.04

# 2. 安裝 C 編譯器和必要套件
RUN apt-get update && apt-get install -y \
    gcc \
    make \
    libncurses5-dev

# 3. 複製你所有專案檔案到容器 /app 目錄
WORKDIR /app
COPY . .

# 4. 編譯你的主程式
RUN make

# 5. 預設執行主程式（這裡以 async_monitor 為例，如果有 main.c 要換 monitor）
CMD ["./monitor"]

