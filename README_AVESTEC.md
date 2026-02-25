# Avestec MAVLink Customizations

This document describes all additions Avestec has made to the upstream MAVLink dialect.
All changes live exclusively in `message_definitions/v1.0/SKYRON.xml` — upstream files are never modified.

---

## Custom Dialect: SKYRON.xml

`SKYRON.xml` is the Avestec dialect. It includes `common.xml` and extends it with Avestec-specific enums and messages.

---

## Flight Assist Source Bits (`MAV_SYS_STATUS_SENSOR_EXTENDED`)

Avestec extends the upstream `MAV_SYS_STATUS_SENSOR_EXTENDED` bitmask enum with additional positioning source entries.
These are reported inside the standard `SYS_STATUS` message (id 1) which is broadcast at ~1 Hz.

### SYS_STATUS fields used

| Field | Meaning |
|---|---|
| `onboard_control_sensors_present_extended` | Hardware physically detected |
| `onboard_control_sensors_enabled_extended` | Source actively fused by EKF |
| `onboard_control_sensors_health_extended` | Source data valid / healthy |

### Why extend this enum instead of creating a new message?

- `SYS_STATUS` is already decoded and displayed by all GCS tools (QGC, Mission Planner, etc.)
- Bitmask extensions are wire-safe: adding new bits never changes the message byte layout
- No new message ID required; no GCS plugin required for basic visibility

### Bit allocation

Upstream `common.xml` currently uses only bit 0 (`MAV_SYS_STATUS_RECOVERY_SYSTEM = 1`).
Bits 1–15 are intentionally left free for upstream growth.
Avestec entries start at **bit 16** (value `0x10000`).

| Bit | Value (hex) | Constant name | Description |
|-----|-------------|---------------|-------------|
| 16 | `0x10000` | `MAV_SYS_STATUS_SENSOR_LIDAR_SLAM` | 3D LiDAR-based SLAM (e.g. FAST-LIO, Cartographer, LOAM). Distinct from upstream `MAV_SYS_STATUS_SENSOR_LASER_POSITION` (0x100) which covers simple 1D rangefinders. |
| 17 | `0x20000` | `MAV_SYS_STATUS_SENSOR_VPS` | Vision Positioning System — downward-facing camera hovering aid. |
| 18 | `0x40000` | `MAV_SYS_STATUS_SENSOR_VISUAL_SLAM` | Camera-based SLAM (e.g. ORB-SLAM, RTABMap). |
| 19 | `0x80000` | `MAV_SYS_STATUS_SENSOR_VIO` | Visual-Inertial Odometry — tightly coupled camera + IMU fusion. |
| 20–31 | — | *(Avestec future use)* | |

> **Note:** `MAV_SYS_STATUS_SENSOR_VISION_POSITION` (upstream, value `0x80`) covers generic camera position.
> `VIO` is added separately because it implies tight camera+IMU fusion, which is architecturally distinct.

---

## Compatibility & Versioning Rules

### Backward compatibility guarantee

| Scenario | Result |
|---|---|
| Old QGC + new firmware (new bits set) | Old QGC sees unknown bits → silently ignored. Known bits unaffected. |
| New QGC + old firmware (new bits never set) | New QGC reads 0 for new bits → correctly shown as absent/inactive. |
| Old firmware + old QGC | Completely unaffected. |

### Rules for adding future entries

1. **Always append at the next free bit.** Never reuse or renumber an existing bit, even if it was removed.
2. Keep the bit table in this README up to date.
3. Do not add entries below bit 16 — those positions are reserved for upstream `common.xml` growth.

---

## Reading in QGC (C++)

```cpp
mavlink_sys_status_t s;
mavlink_msg_sys_status_decode(&message, &s);

// Base sensors (upstream MAV_SYS_STATUS_SENSOR bits)
bool gpsHealthy    = (s.onboard_control_sensors_health & MAV_SYS_STATUS_SENSOR_GPS) != 0;
bool baroHealthy   = (s.onboard_control_sensors_health & MAV_SYS_STATUS_SENSOR_ABSOLUTE_PRESSURE) != 0;

// Avestec custom bits (MAV_SYS_STATUS_SENSOR_EXTENDED)
bool lidarSlamOk   = (s.onboard_control_sensors_health_extended & 0x10000) != 0;
bool vpsOk         = (s.onboard_control_sensors_health_extended & 0x20000) != 0;
bool visualSlamOk  = (s.onboard_control_sensors_health_extended & 0x40000) != 0;
bool vioOk         = (s.onboard_control_sensors_health_extended & 0x80000) != 0;

// Three-state check for a single source (present / enabled / healthy)
bool lidarSlamPresent = (s.onboard_control_sensors_present_extended & 0x10000) != 0;
bool lidarSlamEnabled = (s.onboard_control_sensors_enabled_extended & 0x10000) != 0;
```

---

## Related files

| File | Purpose |
|---|---|
| `message_definitions/v1.0/SKYRON.xml` | Avestec dialect — all custom enums and messages |
| `message_definitions/v1.0/common.xml` | Upstream MAVLink common definitions (do not modify) |
