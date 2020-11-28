import { Packet } from "./types/Packet";

export interface MessageMeta {
  sentTimestamp: number;
}

/**
 * Extract packet data and timestamp from a string message.
 */
export function extractPackets(message: string, meta: MessageMeta): Packet[] | undefined {
  // Test format (99 bytes hex encoded, prefixed with "Hello number x")
  const hexData = message.match(/Hello number \d+:([a-fA-F0-9]{198})/)?.[1];
  return hexData
    ? [
        {
          data: Buffer.from(hexData, "hex"),
          timestamp: meta.sentTimestamp,
        },
      ]
    : undefined;
}
