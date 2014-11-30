package com.MAVLink.Messages;

import com.MAVLink.MAVLinkPacket;

/**
 * Storage for MAVLink Packet and Error statistics
 * 
 */
public class MAVLinkStats /* implements Serializable */{

	public int receivedPacketCount;

	public int crcErrorCount;

	public int lostPacketCount;

	private int lastPacketSeq;

	/**
	 * Check the new received packet to see if has lost someone between this and
	 * the last packet
	 * 
	 * @param packet
	 *            Packet that should be checked
	 */
	public void newPacket(MAVLinkPacket packet) {
		advanceLastPacketSequence();
		if (hasLostPackets(packet)) {
			updateLostPacketCount(packet);
		}
		lastPacketSeq = packet.seq;
		receivedPacketCount++;
	}

	private void updateLostPacketCount(MAVLinkPacket packet) {
		int lostPackets;
		if (packet.seq - lastPacketSeq < 0) {
			lostPackets = (packet.seq - lastPacketSeq) + 255;
		} else {
			lostPackets = (packet.seq - lastPacketSeq);
		}
		lostPacketCount += lostPackets;
	}

	private boolean hasLostPackets(MAVLinkPacket packet) {
		return lastPacketSeq > 0 && packet.seq != lastPacketSeq;
	}

	private void advanceLastPacketSequence() {
		// wrap from 255 to 0 if necessary
		lastPacketSeq = (lastPacketSeq + 1) & 0xFF;
	}

	/**
	 * Called when a CRC error happens on the parser
	 */
	public void crcError() {
		crcErrorCount++;
	}

	/**
	 * Resets statistics for this MAVLink.
	 */
	public void mavlinkResetStats() {
		lastPacketSeq = -1;
		lostPacketCount = 0;
		crcErrorCount = 0;
		receivedPacketCount = 0;
	}

}