# Raspberry Pi OpenVPN Client Setup Guide

Works on Raspberry Pi OS (Bookworm/Bullseye), Debian-based systems, and Pi Zero/2/3/4/5.

---

## 1. Install OpenVPN

Update packages:
```bash
sudo apt update && sudo apt upgrade -y
```

Install OpenVPN:
```bash
sudo apt install openvpn -y
```

---

## 2. Get your `.ovpn` configuration file

Obtain the VPN provider’s `.ovpn` file (e.g., NordVPN, ProtonVPN, or your own OpenVPN server). Copy it to the Pi:
```bash
scp myvpn.ovpn pi@YOUR_PI_IP:/home/pi/
```
Or download it directly onto the Pi.

---

## 3. Move the config to OpenVPN’s directory

```bash
sudo mv ~/myvpn.ovpn /etc/openvpn/client/
```
Rename it (optional but recommended):
```bash
sudo mv /etc/openvpn/client/myvpn.ovpn /etc/openvpn/client/client.conf
```
(OpenVPN requires `.conf` when used as a service.)

---

## 4. Test the VPN manually

Run:
```bash
sudo openvpn --config /etc/openvpn/client/client.conf
```
You should see messages ending with:
```
Initialization Sequence Completed
```
Press **CTRL+C** to stop it and continue.

---

## 5. Enable the VPN to start at boot

```bash
sudo systemctl enable openvpn-client@client.service
sudo systemctl start openvpn-client@client.service
```
Check status:
```bash
systemctl status openvpn-client@client
```

---

## 6. Confirm the VPN tunnel is active

Check your external IP:
```bash
curl ifconfig.me
```
Should show your VPN provider’s IP, **not** your ISP.

Check the tunnel interface:
```bash
ifconfig tun0
```

---

## 7. Optional: Auto-reconnect

Create a systemd override:
```bash
sudo systemctl edit openvpn-client@client.service
```
Add:
```
[Service]
Restart=always
RestartSec=5
```
Save, then:
```bash
sudo systemctl daemon-reload
sudo systemctl restart openvpn-client@client.service
```

---

## 8. If your VPN requires username/password

Create a credentials file:
```bash
sudo nano /etc/openvpn/client/credentials.txt
```
Add:
```
YOUR_USERNAME
YOUR_PASSWORD
```
Secure it:
```bash
sudo chmod 600 /etc/openvpn/client/credentials.txt
```
Modify your `.conf` file:
```
auth-user-pass /etc/openvpn/client/credentials.txt
```

---

## Optional: Route Pi traffic vs LAN traffic

If you want all Pi traffic through VPN:
```
redirect-gateway def1
```
For specific IP ranges, route rules can be added.

---

## VPN Client Router (Optional)

Instructions can be provided to turn the Pi into a **VPN gateway router**, allowing:
- Smart TVs/consoles to connect through Pi’s VPN
- Automatic kill switches and firewall rules

