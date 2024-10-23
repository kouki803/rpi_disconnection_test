# rpi_disconnection_test

# ESP32
build src/esp32/esp32.ino on Arduino IDE and send binary to ESP32  
Arduino IDEでビルドしてESP32に書き込み

# RasPi
## OS install  
- RasPi Zero 2W  
    - [site]()   
- RasPi CM4  
    - [site]()  


## hardware setup
|RasPi Zero|<->|ESP32|
| - | - | - | 
|TX|->|Pin 16|
|RX|<-|Pin 17|
|GND|<->|Pin xx(GND)|

## software setup 
```
sudo apt update && apt upgrade
sudo apt install build-essentials
sudo apt install python3 git
```

```
pip install pyserial
```
[!Note]  
You can use "pyenv" or "venv" if you want, but you probably don't need it  
since OS dates on SD will corrupt and you have to clean install anyway.
If using them, add "cd" to service file.  

pyenvとかevnvは結局OSのデータが吹っ飛ぶので意味ないかも（笑  
使う場合はserviceファイルのでcdなどする必要アリ

```
git clone [This repository URL]
```

## service
サービスファイルの登録
```
sudo cp [path of service file] /etc/systemd/system/send_serial.service
```
```
sudo systemctl enable send_serial.service
sudo systemctl start send_serial.service
```

## ROM化
手動の設定
```
sudo apt install overlayroot
```
設定ファイルを開いて編集
```
sudo nano /etc/overlayroot.conf
```
以下を追記
```bash
overlayroot="tmpfs"
```

リポジトリのシェルを使う
1. 実行権限付与
```
chmod +x src/rpi/rom.sh
```
2. ROM化の設定
```
sudo src/src/rpi/rom.sh enable
```
3. Reboot

# GAS
1. Google Apps Script (GAS) の作成  
2. スプレッドシートで「拡張機能」→「Apps Script」  
3. ```src/gas/gas.js```のコードをエディタに入力
4. GASエディタの「デプロイ」→「ウェブアプリとしてデプロイ」
5. 「ウェブアプリのURL」をコピーしておく（後でESP32に送る）