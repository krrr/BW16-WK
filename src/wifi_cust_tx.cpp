#include "wifi_cust_tx.h"

// 安全的MAC地址拷贝函数，避免编译器优化出非对齐的32位内存访问(导致UNALIGN_TRP报错)
static inline void safe_mac_cpy(void* dest, const void* src) {
  volatile uint8_t* d = (volatile uint8_t*)dest;
  volatile uint8_t* s = (volatile uint8_t*)src;
  for (int i = 0; i < 6; i++) {
    d[i] = s[i];
  }
}

#include "wifi_util.h"

/*
 * 发送一个指定长度的原始802.11帧
 * 该帧必须是有效的，且序列号为0（将会自动设置）
 * 帧校验序列会自动添加，不需要包含在长度中
 * @param frame 指向原始帧的指针
 * @param size 帧的大小
*/
void wifi_tx_raw_frame(void* frame, size_t length) {
  // 调用 AmebaD 官方 SDK 提供的管理帧发送函数
  // 似乎官方新版SDK系新增了，不用再采取逆向工程手段了。 https://tesa-klebeband.github.io/making-raw-802-11-frame-injection-possible-on-an-rtl8720dn
  wext_send_mgnt(WLAN0_NAME, (char*)frame, length, 0);
}


/*
 * 在当前信道发送802.11解除认证帧
 * @param src_mac 包含发送者MAC地址的字节数组，必须为6字节
 * @param dst_mac 包含目标MAC地址的字节数组，或使用FF:FF:FF:FF:FF:FF进行广播
 * @param reason 符合802.11规范的原因码（可选）
*/
void wifi_tx_deauth_frame_ext(void* src_mac, void* dst_mac, void* ap_mac, uint16_t reason) {
  DeauthFrame frame;
  // 设置源MAC地址
  safe_mac_cpy(&frame.source, src_mac);
  // 设置目标MAC地址
  safe_mac_cpy(&frame.destination, dst_mac);
  // 设置接入点MAC地址
  safe_mac_cpy(&frame.access_point, ap_mac);
  frame.reason = reason;
  wifi_tx_raw_frame(&frame, sizeof(DeauthFrame));
}

void wifi_tx_deauth_frame(void* src_mac, void* dst_mac, uint16_t reason) {
  wifi_tx_deauth_frame_ext(src_mac, dst_mac, src_mac, reason);
}

/*
 * 在当前信道发送一个基本的802.11信标帧
 * @param src_mac 包含发送者MAC地址的字节数组，必须为6字节
 * @param dst_mac 包含目标MAC地址的字节数组，或使用FF:FF:FF:FF:FF:FF进行广播
 * @param ssid 以'\0'结尾的字符数组，表示SSID
*/
void wifi_tx_beacon_frame(void* src_mac, void* dst_mac, const char *ssid) {
  BeaconFrame frame;
  // 设置源MAC地址
  safe_mac_cpy(&frame.source, src_mac);
  // 设置接入点MAC地址
  safe_mac_cpy(&frame.access_point, src_mac);
  // 设置目标MAC地址
  safe_mac_cpy(&frame.destination, dst_mac);
  // 复制SSID并计算长度
  for (int i = 0; ssid[i] != '\0'; i++) {
    frame.ssid[i] = ssid[i];
    frame.ssid_length++;
  }
  // 发送帧（帧大小为基础大小38字节加上SSID长度）
  wifi_tx_raw_frame(&frame, 38 + frame.ssid_length);
}

size_t wifi_build_beacon_frame(void* src_mac, void* dst_mac, const char *ssid, BeaconFrame &out) {
  // 预构建用于复用的信标帧缓冲
  safe_mac_cpy(&out.source, src_mac);
  safe_mac_cpy(&out.access_point, src_mac);
  safe_mac_cpy(&out.destination, dst_mac);
  out.ssid_length = 0;
  for (int i = 0; ssid[i] != '\0'; i++) {
    out.ssid[i] = ssid[i];
    out.ssid_length++;
  }
  return 38 + out.ssid_length;
}

void wifi_tx_probe_resp_frame(void* src_mac, void* dst_mac, const char *ssid) {
  ProbeRespFrame frame;
  safe_mac_cpy(&frame.source, src_mac);
  safe_mac_cpy(&frame.access_point, src_mac);
  safe_mac_cpy(&frame.destination, dst_mac);
  frame.ssid_length = 0;
  for (int i = 0; ssid[i] != '\0'; i++) {
    frame.ssid[i] = ssid[i];
    frame.ssid_length++;
  }
  wifi_tx_raw_frame(&frame, 38 + frame.ssid_length);
}

size_t wifi_build_probe_resp_frame(void* src_mac, void* dst_mac, const char *ssid, ProbeRespFrame &out) {
  safe_mac_cpy(&out.source, src_mac);
  safe_mac_cpy(&out.access_point, src_mac);
  safe_mac_cpy(&out.destination, dst_mac);
  out.ssid_length = 0;
  for (int i = 0; ssid[i] != '\0'; i++) {
    out.ssid[i] = ssid[i];
    out.ssid_length++;
  }
  return 38 + out.ssid_length;
}

size_t wifi_build_auth_req(void* sta_mac, void* bssid, AuthReqFrame &out) {
  safe_mac_cpy(&out.source, sta_mac);
  safe_mac_cpy(&out.destination, bssid);
  safe_mac_cpy(&out.bssid, bssid);
  out.auth_algorithm = 0x0000; // Open System
  out.auth_sequence = 0x0001;
  out.status_code = 0x0000;
  return sizeof(AuthReqFrame);
}

void wifi_tx_auth_req(void* sta_mac, void* bssid) {
  AuthReqFrame frame;
  size_t len = wifi_build_auth_req(sta_mac, bssid, frame);
  wifi_tx_raw_frame(&frame, len);
}

size_t wifi_build_assoc_req(void* sta_mac, void* bssid, const char* ssid, AssocReqFrame &out) {
  safe_mac_cpy(&out.source, sta_mac);
  safe_mac_cpy(&out.destination, bssid);
  safe_mac_cpy(&out.bssid, bssid);
  out.ssid_length = 0;
  for (int i = 0; ssid && ssid[i] != '\0' && i < 32; i++) {
    out.ssid[i] = ssid[i];
    out.ssid_length++;
  }
  // 固定能力与监听间隔已在结构体中给定
  // 关联请求帧长度 = 24字节MAC头(此处由结构体前部近似) + 固定字段(4) + IE(2+len)
  // 我们结构体已线性布局，直接返回有效占用长度
  return sizeof(AssocReqFrame) - (32 - out.ssid_length);
}

void wifi_tx_assoc_req(void* sta_mac, void* bssid, const char* ssid) {
  AssocReqFrame frame;
  size_t len = wifi_build_assoc_req(sta_mac, bssid, ssid, frame);
  wifi_tx_raw_frame(&frame, len);
}


void wifi_tx_broadcast_deauth(void* bssid, uint16_t reason, int burstCount, int interDelayUs) {
  uint8_t broadcast[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
  DeauthFrame frame;
  safe_mac_cpy(&frame.source, bssid);
  safe_mac_cpy(&frame.access_point, bssid);
  safe_mac_cpy(&frame.destination, broadcast);
  frame.reason = reason;
  for (int i = 0; i < burstCount; i++) {
    wifi_tx_raw_frame(&frame, sizeof(DeauthFrame));
    if (interDelayUs > 0) delayMicroseconds(interDelayUs);
  }
}

typedef struct __attribute__((packed, aligned(4))) {
  uint16_t frame_control = 0xA0;      // Disassociation
  uint16_t duration = 0x0000;
  uint8_t destination[6];
  uint8_t source[6];
  uint8_t bssid[6];
  const uint16_t sequence_number = 0;
  uint16_t reason = 0x0008;           // Disassoc due to inactivity by default
} DisassocFrame;

void wifi_tx_broadcast_disassoc(void* bssid, uint16_t reason, int burstCount, int interDelayUs) {
  uint8_t broadcast[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
  DisassocFrame frame = {};
  safe_mac_cpy(&frame.source, bssid);
  safe_mac_cpy(&frame.bssid, bssid);
  safe_mac_cpy(&frame.destination, broadcast);
  frame.reason = reason;
  for (int i = 0; i < burstCount; i++) {
    wifi_tx_raw_frame(&frame, sizeof(DisassocFrame));
    if (interDelayUs > 0) delayMicroseconds(interDelayUs);
  }
}

