# Netplan Configuration Guide for Static IP

This guide explains how to configure a static IP address using Netplan on Ubuntu/Debian-based systems, including example configuration for a Raspberry Pi or server. It is written for beginners, so even users who are not familiar with Netplan can follow along.

---

## What is Netplan?
Netplan is a network configuration utility used in modern Ubuntu and Debian-based systems to manage network interfaces. It replaces older methods like `/etc/network/interfaces`. Netplan uses YAML files to define network settings, which are then applied using either `systemd-networkd` or `NetworkManager`.

With Netplan, you can configure:
- Static IP addresses
- DHCP (automatic IP)
- DNS servers
- Routes and gateways
- Multiple network interfaces

---

## Example Netplan Configuration
File: `/etc/netplan/50-cloud-init.yaml`

```yaml
network:
  version: 2
  renderer: networkd
  ethernets:
    eth0:
      addresses: 
        - 192.168.12.20/24
      routes:
        - to: 192.168.12.0/24
          via: 192.168.12.20
          metric: 100
      nameservers:
        addresses:
          - 1.1.1.1
          - 8.8.8.8
      dhcp4: no
```

---

## Explanation of Each Section

- `network:` – The root key for network configuration.
- `version: 2` – Specifies the Netplan configuration version.
- `renderer: networkd` – Uses `systemd-networkd` to manage network interfaces. Alternatively, `NetworkManager` can be used for desktop systems.
- `ethernets:` – Section for Ethernet interface configurations.
  - `eth0:` – The name of the network interface you are configuring. On some systems, it may have a different name like `enx...`.
    - `addresses:` – List of static IP addresses assigned to this interface. `/24` indicates the subnet mask (255.255.255.0).
    - `routes:` – Optional section for custom routes.
      - `to:` – Destination network for the route.
      - `via:` – Gateway IP to reach that network.
      - `metric:` – Priority for choosing this route; lower is preferred.
    - `nameservers:` – DNS servers for resolving domain names.
      - `addresses:` – List of DNS IPs (e.g., 1.1.1.1, 8.8.8.8).
    - `dhcp4: no` – Disables automatic DHCP assignment for IPv4, enforcing the static IP.

---

## Step-by-Step Instructions for Beginners

1. Open a terminal on your Raspberry Pi or server.
2. Edit the Netplan configuration file:
```bash
sudo nano /etc/netplan/50-cloud-init.yaml
```
3. Paste the example configuration above or adjust IPs to match your network.
4. Save the file and exit (CTRL+O, ENTER, CTRL+X).
5. Apply the configuration:
```bash
sudo netplan apply
```
6. Verify the interface:
```bash
ip addr show eth0
```
7. Check routing table and DNS:
```bash
ip route
systemd-resolve --status
```

---

## Tips for Beginners
- YAML files are sensitive to indentation. Make sure to use spaces (not tabs).
- If your network uses a different interface name, replace `eth0` with the correct one.
- Always backup the original file before editing:
```bash
sudo cp /etc/netplan/50-cloud-init.yaml /etc/netplan/50-cloud-init.yaml.bak
```
- For any mistakes, you can revert using the backup and run `sudo netplan apply` again.

---

## Summary
This guide shows how to configure a static IP on Ubuntu/Debian systems using Netplan. By following these steps, even beginners can set up their Raspberry Pi or server to use a fixed IP with DNS and custom routes, ensuring reliable network connectivity.

