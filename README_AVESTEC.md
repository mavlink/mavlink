# Avestec MAVLink Customizations

This document describes all additions Avestec has made to the upstream MAVLink dialect.
All changes live exclusively in `message_definitions/v1.0/SKYRON.xml` — upstream files are never modified.

---

## Custom Dialect: SKYRON.xml

`SKYRON.xml` is the Avestec dialect. It includes `common.xml` and extends it with Avestec-specific enums and messages.

---

## Pose Estimation Status (`ONBOARD_POSE_ESTIMATION_STATUS`, id 3367)

### Architecture

```
OBC (avescore/main.py)
  │  has slam_health_state (MAV_STATE) from SLAMHealthMonitor
  │  has vio_health_state  (MAV_STATE) from VIO monitor
  │
  │  publishes FLIGHT_ASSIST_STATUS via /mavlink/to
  ▼
PX4 MAVLink router
  │  forwards transparently (no firmware change needed)
  ▼
QGC receives FLIGHT_ASSIST_STATUS (component 191 = MAV_COMP_ID_ONBOARD_COMPUTER)
```

### Message fields

| Field | Type | Description |
|---|---|---|
| `timestamp` | `uint64_t` (µs) | Time since system boot |
| `lidar_slam_state` | `uint8_t` (`MAV_STATE`) | LiDAR SLAM algorithm health (e.g. FAST-LIO) |
| `visual_slam_state` | `uint8_t` (`MAV_STATE`) | Camera SLAM algorithm health (e.g. ORB-SLAM) |
| `vio_state` | `uint8_t` (`MAV_STATE`) | Visual-Inertial Odometry algorithm health |
| `vps_state` | `uint8_t` (`MAV_STATE`) | Vision Positioning System algorithm health |
| `lidar_hw_state` | `uint8_t` (`MAV_STATE`) | Physical LiDAR hardware health |
| `stereo_cam_hw_state` | `uint8_t` (`MAV_STATE`) | Stereo camera hardware health |
| `active_sources` | `uint32_t` (bitmask, `ONBOARD_POSE_ESTIMATION_SOURCE`) | Sources currently fused by OBC EKF — multiple bits may be set |
| `reserved0/1` | `uint8_t` | Future use (after `<extensions/>`) |

### MAV_STATE values used per source

| Value | Meaning |
|---|---|
| `MAV_STATE_UNINIT (0)` | Not started / no hardware |
| `MAV_STATE_BOOT (1)` | Starting up |
| `MAV_STATE_ACTIVE (4)` | Running and healthy |
| `MAV_STATE_CRITICAL (5)` | Running but degraded |
| `MAV_STATE_EMERGENCY (6)` | Failed |

### `ONBOARD_POSE_ESTIMATION_SOURCE` bitmask (for `active_sources` field)

Multiple bits may be set simultaneously (e.g. LiDAR SLAM fused with VIO = `0x01 | 0x04 = 0x05`).
Bits 5–31 are reserved for future expansion — never reuse or renumber existing bits.

| Bit | Value | Name | Description |
|-----|-------|------|-------------|
| 0 | `0x01` | `ONBOARD_POSE_ESTIMATION_SOURCE_LIDAR_SLAM` | 3D LiDAR SLAM (FAST-LIO, Cartographer) |
| 1 | `0x02` | `ONBOARD_POSE_ESTIMATION_SOURCE_VISUAL_SLAM` | Camera SLAM (ORB-SLAM, RTABMap) |
| 2 | `0x04` | `ONBOARD_POSE_ESTIMATION_SOURCE_VIO` | Visual-Inertial Odometry |
| 3 | `0x08` | `ONBOARD_POSE_ESTIMATION_SOURCE_VPS` | Vision Positioning System (downward camera) |
| 4–31 | — | *(reserved)* | Future sources |

### OBC sending pattern (matches existing main.py style)

```python
from mavros_msgs.msg import Mavlink
import mavros.mavlink as mavros_mavlink
from pymavlink.dialects.v20 import SKYRON as skyron_dialect

class PoseEstimationPublisher:
    def __init__(self):
        self.pub = rospy.Publisher('/mavlink/to', Mavlink, queue_size=10)
        self.mav = skyron_dialect.MAVLink(None, srcSystem=1, srcComponent=191)

    def send(self, lidar_slam_state, visual_slam_state, vio_state, vps_state,
             lidar_hw_state, stereo_cam_hw_state, active_sources):
        msg = self.mav.pose_estimation_status_encode(
            timestamp=rospy.Time.now().to_nsec() // 1000,
            lidar_slam_state=lidar_slam_state,
            visual_slam_state=visual_slam_state,
            vio_state=vio_state,
            vps_state=vps_state,
            lidar_hw_state=lidar_hw_state,
            stereo_cam_hw_state=stereo_cam_hw_state,
            active_sources=active_sources,  # e.g. LIDAR_SLAM | VIO = 0x01 | 0x04
            reserved0=0,
            reserved1=0
        )
        ros_msg = mavros_mavlink.convert_to_rosmsg(msg)
        self.pub.publish(ros_msg)
```

---

## Onboard Computer Modules Status (`ONBOARD_COMPUTER_MODULES_STATUS`, id 3368)

### Purpose

Reports the health of software modules running on the onboard computer. Generic and robot-agnostic — applicable to aerial, ground, and marine platforms. Sent by the OBC (`MAV_COMP_ID_ONBOARD_COMPUTER`) and forwarded transparently by PX4 to QGC.

Complements the upstream `ONBOARD_COMPUTER_STATUS` (id 390), which covers hardware metrics (CPU, RAM, temperature). This message covers what runs *on* the computer.

### Message fields

| Field | Type | Description |
|---|---|---|
| `timestamp` | `uint64_t` (µs) | Time since system boot |
| `ros_state` | `uint8_t` (`MAV_STATE`) | ROS master / middleware core health |
| `autopilot_bridge_state` | `uint8_t` (`MAV_STATE`) | Autopilot communication bridge health (e.g. MAVROS) |
| `primary_camera_state` | `uint8_t` (`MAV_STATE`) | Primary camera process health |
| `video_stream_state` | `uint8_t` (`MAV_STATE`) | Video streaming service health |
| `recording_state` | `uint8_t` (`MAV_STATE`) | Data recording service health |
| `pose_estimation_state` | `uint8_t` (`MAV_STATE`) | Overall pose estimation pipeline health (see `ONBOARD_POSE_ESTIMATION_STATUS` id 3367 for per-source detail) |
| `collision_avoidance_state` | `uint8_t` (`MAV_STATE`) | Collision avoidance module health |
| `navigation_state` | `uint8_t` (`MAV_STATE`) | Path planning / navigation stack health |
| `object_detection_state` | `uint8_t` (`MAV_STATE`) | Object detection / perception pipeline health |

### MAV_STATE values used

| Value | Meaning |
|---|---|
| `MAV_STATE_UNINIT (0)` | Not started / not connected |
| `MAV_STATE_BOOT (1)` | Starting up |
| `MAV_STATE_STANDBY (3)` | Ready but idle (e.g. recording service waiting) |
| `MAV_STATE_ACTIVE (4)` | Running and healthy |
| `MAV_STATE_CRITICAL (5)` | Running but degraded |
| `MAV_STATE_EMERGENCY (6)` | Crashed / failed |

### Design notes

- No `<extensions/>` — message is flat. All fields are always present.
- Fields are generic. `autopilot_bridge_state` covers any autopilot bridge (MAVROS, MAVSDK, etc); `primary_camera_state` covers any primary camera.
- Never reorder or remove existing fields (MAVLink wire compatibility).

---

## Related files

| File | Purpose |
|---|---|
| `message_definitions/v1.0/SKYRON.xml` | Avestec dialect — all custom enums and messages |
| `message_definitions/v1.0/common.xml` | Upstream MAVLink (do not modify) |
| `src/modules/mavlink/streams/AVESAID_STATUS.hpp` | PX4-originated status (attachment, magnets, height source) |
