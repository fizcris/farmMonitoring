

// connect to wifi – returns true if successful or false if not
bool connectWifi(const char *ssid, const char *password);

bool isWiFiConnected();

void displayConnectionStatus();

void initWiFi(const char *ssid, const char *password);

void checkWiFiStatus();
