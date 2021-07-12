import { Packet } from "./types/Packet";

export class UnrecognisedMessageFormatError extends Error {}

export interface MessageMeta {
  sentTimestamp: number;
}

/**
 * Extract packet data and timestamp from a string message.
 */
export function extractPackets(message: string, meta: MessageMeta): Packet[] {
  // Message format (hex encoded) [1 byte 0x01, 4 byte timestamp, 2 byte reserved, 99 byte loop packet, 2 byte usb voltage, 2 byte battery voltage]
  const matches = message.match(
    /01([a-fA-F0-9]{8}).{4}([a-fA-F0-9]{198})([a-fA-F0-9]{8})?/
  );
  if (matches) {
    const [_, timestampHex, packetData, statusData] = matches;
    return [
      {
        data: Buffer.from(packetData, "hex"),
        statusData: statusData ? Buffer.from(statusData, "hex") : undefined,
        timestamp: parseInt(timestampHex, 16) * 1000,
      },
    ];
  }
  throw new UnrecognisedMessageFormatError();
}
