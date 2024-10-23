# rpi_disconnection_test

## service
```
sudo cp [path of service file] /etc/systemd/system/send_serial.service
```
```
sudo systemctl enable send_serial.service
sudo systemctl start send_serial.service
```