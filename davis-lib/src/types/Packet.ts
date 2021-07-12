export interface Packet {
  data: Buffer;
  statusData?: Buffer;
  timestamp: number;
}
