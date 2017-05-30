
# Setup

Execute the following commands, replacing `YOUR_WIFI_SSID` and `YOUR_WIFI_PASSWORD` by your respective values

```bash
sed -i 's/$WIFI_SSID/YOUR_WIFI_SSID/g' hardware/firmware/main.ino
sed -i 's/$WIFI_PASSWORD/YOUR_WIFI_PASSWORD/g' hardware/firmware/main.ino
```
